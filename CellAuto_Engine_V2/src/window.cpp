#include "Header.h"
DWORD WINAPI ThreadPasser(LPVOID WND){
Window* RealWND=(Window*)WND;
RealWND->test=1;
RealWND->Create();
//SetEvent(RealWND->ThreadStart);
ConsoleEcho("Link Done");

RealWND->WindowThread();
}

Window::Window(int width_a,int height_a,int Framerate_a,bool FullScreen_a,int Sample){
    std::cout<<"CreatingWindow"<<std::endl;
    GetScreenMode();
    if(width_a==0 || height_a==0){

        width=(ResolutionList.back())[0];
        height=(ResolutionList.back())[1];
        ConsoleEcho("No preset");
        ConsoleEcho("Auto Found Width:%i",width);
        ConsoleEcho("Auto Found Height:%i",height);
    }else{
        width=width_a;
        height=height_a;
        std::cout<<"Size preset"<<std::endl;
        std::cout<<width<<std::endl;
        std::cout<<height<<std::endl;
    }
    MSAA_Sample=Sample;
    FrameRate=Framerate_a;
    FullScreen=FullScreen_a;
    ThreadStart=CreateEvent(NULL, true, 0, GenStrWithUniqueCode("WindowThreadStart",false));
    ThreadClosed=CreateEvent(NULL, true, 0, GenStrWithUniqueCode("WindowThreadClosed",false));
    WNDDemandAnswered=CreateEvent(NULL,0,0,GenStrWithUniqueCode("WNDDemandAnswered",false));
    WNDDemandSent=CreateEvent(NULL,0,0,GenStrWithUniqueCode("WNDDemandSent",false));
    WNDRenderSuspended=CreateEvent(NULL,true,0,GenStrWithUniqueCode("WNDRenderSuspended"));
    WNDThread=CreateThread(NULL, 0, ThreadPasser, this,0, &ThreadId);
    std::cout<<WaitForSingleObject(ThreadStart,INFINITE);
    ResetEvent(ThreadStart);
    ConsoleEcho("Link init");
    test=6;

}
void Window::Create(){
    if(true){
    WNDCLASSEX wcex;
    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GlobalInstanceHandle;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "Cell_Engine_Window";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;
    if (!RegisterClassEx(&wcex)){
        //exit(0);
    }
    }

    /* create main window */
    ConsoleEcho("Window Création");
     WindowHandler= CreateWindowEx(0,
                          "Cell_Engine_Window",
                          Title,
                          WS_TILEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          width,
                          height,
                          NULL,
                          NULL,
                          GlobalInstanceHandle,
                          NULL);
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    DeviceContext = GetDC(WindowHandler);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(DeviceContext, &pfd);

    SetPixelFormat(DeviceContext, iFormat, &pfd);

    /* create and enable the render context (RC) */
    GLRenderingContext= wglCreateContext(DeviceContext);
    SharedRenderContext=wglCreateContext(DeviceContext);
    UserSharedRenderContext=wglCreateContext(DeviceContext);
    if(SharedRenderContext==NULL){
        DWORD errorCode=GetLastError();
     LPVOID lpMsgBuf;
     FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf, 0, NULL);
     MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
     LocalFree(lpMsgBuf);exit(125);
    }
    bool Error=true;

    Error=wglShareLists(GLRenderingContext,SharedRenderContext);
    Error=wglShareLists(GLRenderingContext,SharedRenderContext);
    if(Error==false){ConsoleEcho("Error dans Share List");exit(122);}
    ConsoleEcho("ContextCrétaion");
    Error=wglMakeCurrent(DeviceContext,SharedRenderContext);
    if(Error==false){
        NoticeError();
    }
    wglMakeCurrent(DeviceContext,GLRenderingContext);

    Context_Initied=true;
    //wglShareLists(GLobalHGLRC,GLRenderingContext);
    glewInit();
    ConsoleEcho("Device getting");
    GetDeviceMode();
    ConsoleEcho("FrameRate Upd");
    if(FrameRate==0){DeviceMode.dmDisplayFrequency=(FrameRateList.back());
    FrameRate=(FrameRateList.back());
    }else{DeviceMode.dmDisplayFrequency=FrameRate;
    }
    ConsoleEcho("device upd");
    UpdateDeviceMode();


    RenderWidth=width;
    RenderHeight=height;
    ShowWindow(WindowHandler, SW_SHOW);
    Created=true;

    GetRendererInfo();
    ConsoleEcho("Buffer");
    #if defined(DEBUG_MODE)
        DefineGLDebugCallback();
    #endif // defined
    if(MSAA_Sample>0){
        PrepareSamplingBuffer(MSAA_Sample);
    }else{
        MSAA_Sample=0;
        PrepareBuffer();
    }
    glViewport(0,0,RenderWidth,RenderHeight);
    test=5;
    //glEnable(GL_DEPTH_TEST);
    VSyncState(true);
    CreatePipeline();
    SetEvent(ThreadStart);
}
void Window::SetContextAsMain(){
ContextLent=false;
wglMakeCurrent(DeviceContext,GLRenderingContext);
glewInit();
}


void Window::GetDeviceMode(){
DeviceMode.dmSize=sizeof(DEVMODE);
EnumDisplaySettings(NULL,ENUM_REGISTRY_SETTINGS,&DeviceMode);

}
void Window::UpdateDeviceMode(){
DWORD flag=0;
if(FullScreen){
    flag=CDS_FULLSCREEN;
    SetWindowLong(WindowHandler,GWL_STYLE,WS_POPUP);
DeviceMode.dmFields= DM_BITSPERPEL | DM_PAPERWIDTH | DM_PELSHEIGHT;
ChangeDisplaySettings(&DeviceMode,flag);
}
}
bool SortResolution(const int* Ra,const int* Rb){
return Ra[0]<Rb[0];
}

void Window::GetScreenMode(){
    std::cout<<"Get Screen INfo"<<std::endl;
      DEVMODE Screen;
      Screen.dmSize=sizeof(DEVMODE);
      for(int i=0;EnumDisplaySettings(NULL,i,&Screen);i++){
          //LPDWORD BitRequired=0;
          //LPDWORD StructNb;
          //EnumMonitorsW(NULL,1,NULL,0,BitRequired,StructNb);
          //LPBYTE Buffer[(long)*BitRequired];
          //EnumMonitorsW(NULL,1,*Buffer,*BitRequired,BitRequired,StructNb);
          char buff[255];
          char buff2[20];
          strcpy(buff,"Device:");
          strcat(buff,(const char*)Screen.dmDeviceName);
          std::cout<<buff<<std::endl;

          WriteToLog(buff,"DEVMOD");
          strcpy(buff,"extra bit driver:");
          sprintf(buff2,"%i",Screen.dmDriverExtra);
          strcat(buff,buff2);
          std::cout<<buff<<std::endl;
          WriteToLog(buff,"DEVMOD");

        //withd et height
          strcpy(buff,"Width/Height:");
          sprintf(buff2,"%u",Screen.dmPelsWidth);
          strcat(buff,buff2);
          strcat(buff,"/");
          sprintf(buff2,"%u",Screen.dmPelsHeight);
          strcat(buff,buff2);
          std::cout<<buff<<std::endl;
        WriteToLog(buff,"DEVMOD");

          //bit par pixel
          strcpy(buff,"Bit per pixel:");
          sprintf(buff2,"%u",Screen.dmBitsPerPel);
          strcat(buff,buff2);
          std::cout<<buff<<std::endl;
          WriteToLog(buff,"DEVMOD");

          strcpy(buff,"FramePerSecond:");
          sprintf(buff2,"%u",Screen.dmDisplayFrequency);
          strcat(buff,buff2);
          std::cout<<buff<<std::endl;
          WriteToLog(buff,"DEVMOD");

          int* Res=new int[2];
          Res[0]=Screen.dmPelsWidth;
          Res[1]=Screen.dmPelsHeight;
          ResolutionList.push_back(Res);
          FrameRateList.push_back(Screen.dmDisplayFrequency);

      }
      ResolutionList.sort(SortResolution);
      FrameRateList.sort();
}
/*void Window::InitPipeline(){
glGenProgramPipelines(1,&WindowPipeline);
glBindProgramPipeline(WindowPipeline);


}*/
void Window::VSyncState(bool st){
    if(st){
        ConsoleEcho("VSync:on\0");
        wglSwapIntervalEXT(1);
        VSync=true;
    }else{
        ConsoleEcho("VSync:off\0");
        wglSwapIntervalEXT(0);
        VSync=false;
    }
}
void Window::GetRendererInfo(){
    char* CG=(char*)glGetString(GL_RENDERER);
    ConsoleEcho("Renderer");
    ConsoleEcho(CG);
    CG=(char*)glGetString(GL_VERSION);
    ConsoleEcho(CG);
    int Size;

    glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS,&Size);

    ConsoleEcho("GLSL version");
    ConsoleEcho((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
//    string buf=std::to_string(Size);
  //  ConsoleEcho((char*)buf.c_str());
    for(int i=0;i<Size;i++){
        char* Exten=(char*)glGetStringi(GL_SHADING_LANGUAGE_VERSION,i);
        ConsoleEcho(Exten);
    }
    glGetIntegerv(GL_NUM_EXTENSIONS,&Size);
    ConsoleEcho("Extension");
    char Total[Size*40];
    for(int i=0;i<Size;i++){
        char* Exten=(char*)glGetStringi(GL_EXTENSIONS,i);
        std::strcat(Total,Exten);
        std::strcat(Total,";");
    }
    ConsoleEcho(Total);
}

void Window::CheckDemand(){
    ShaderCheckDemand();
        if(WaitForSingleObject(WNDDemandSent,1)==WAIT_OBJECT_0){
            switch(WNDDemandCode){
                //ContextSwaping
                case(4):
                    wglMakeCurrent(DeviceContext,GLRenderingContext);
                break;
                case(5):
                    wglMakeCurrent(NULL,NULL);

                break;
            }
        SetEvent(WNDDemandAnswered);
    }
}
void Window::WindowThread(){
    //wglMakeCurrent(DrawingContext,GLRenderingContext);
    //ConsoleEcho("Le Thread Boucle");
    //char buf[2];
    //sprintf(buf,"%d",test);
    //ConsoleEcho(buf);

    BOOL bQuit = FALSE;
    MSG msg;

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg,WindowHandler, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                ConsoleEcho("Quit");
                bQuit = TRUE;
                if(IsShaderReady())
                    InternShaderHandlerDestructor();
                SetEvent(ThreadClosed);
                break;
            }else if(msg.message==WM_CREATE){
                SetEvent(ThreadStart);
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                //ConsoleEcho("et un message");
            }
        }
        else {  CheckDemand();
                if(!WaitForSingleObject(WNDRenderSuspended,0)==WAIT_OBJECT_0){
                    if(ContextLent)
                        RemindContext();
                    Update();
                //ShaderHandler

                }
        }
    }
}

void Window::TextureDisplayer_Activation(){
    SetCurrentDirectory("TextureLoad");
    TextureDisplayer_Adress=AddShader("TexLoad.frag",GL_FRAGMENT_SHADER,-1);
    AddShader("TexLoad.geo",GL_GEOMETRY_SHADER,TextureDisplayer_Adress);
    AddShader("TexLoad.vert",GL_VERTEX_SHADER,TextureDisplayer_Adress);
    ConsoleEcho("12345");
    GLuint Type[1]={GL_GEOMETRY_SHADER_BIT | GL_VERTEX_SHADER_BIT |GL_FRAGMENT_SHADER_BIT};
    SetActiveInternProgram(&TextureDisplayer_Adress,Type,1);
    TextureDisplayer_Inited=true;
    ConsoleEcho("Tes Suposé activé");
}
void Window::TextureDisplayer_InternReActivation(){
    if(TextureDisplayer_Inited){
        ConsoleEcho("Réactivation Tex");
        ActivatePipeline();
        GLuint Type[1]={GL_GEOMETRY_SHADER_BIT | GL_VERTEX_SHADER_BIT |GL_FRAGMENT_SHADER_BIT};
        InternSetActiveInternProgram(&TextureDisplayer_Adress,Type,1);
    }
}
void Window::TextureDisplayer_SetTextureObject(TextureObject Texture){
    if(TextureDisplayer_Inited){
        //BorrowContext();
        ConsoleEcho("on rentre");
        Texture.Load(0);
        ConsoleEcho("Binding");
        Texture.Bind(1);
        ConsoleEcho("Binded");
        int dara=1;
        float dara2=1.0f;
        TextureDisplayer_Texture=Texture.GetTextureIndex();
        ConsoleEcho("Envoie de la Texture");
        SetUniform(GL_INT,1,&dara,TextureDisplayer_Adress,"Tex1");
        memory_fence();
        SetUniform(GL_FLOAT,1,&dara2,TextureDisplayer_Adress,"ColorTest");
    }
}
void Window::TextureDisplayer_Check(){
    if(TextureDisplayer_Inited && !ContextLent && TextureDisplayer_Texture>=0){
        float dara=1.0f;
        glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);
        TextureDisplayer_InternReActivation();
        //SetUniform(GL_INT,1,&dara,TextureDisplayer_Adress,-1,"Tex1");
       // InternUniformSet(GL_FLOAT,1,&dara,TextureDisplayer_Adress,0xffffffff,"ColorTest");
        glBegin(GL_POINTS);
            glVertex2f(1.0,1.0);
        glEnd();
        glBindProgramPipeline(0);
       // ConsoleEcho("j'ai affiché la texture");
    }
}

bool Window::BorrowContext(){
    SetEvent(WNDRenderSuspended);
    WNDDemandCode=5;
    SetEvent(WNDDemandSent);
    WaitForSingleObject(WNDDemandAnswered,INFINITE);
    bool Res=wglMakeCurrent(DeviceContext,GLRenderingContext);
    glewInit();
    ConsoleEcho("J'essaye");
    if(Res){
        ConsoleEcho("Je te l'emprunte");
        ContextLent=true;
        return true;
    }else{
        wglMakeCurrent(NULL,NULL);
        ResetEvent(WNDRenderSuspended);
        return false;
    }
}
bool Window::ReturnContext(){
    wglMakeCurrent(NULL,NULL);
    //WNDDemandCode=4;
    //SetEvent(WNDDemandSent);
    //WaitForSingleObject(WNDDemandAnswered,INFINITE);
    ResetEvent(WNDRenderSuspended);
    ConsoleEcho("Je te rend ton truc");
    return false;
}
void Window::DisplayFrame(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER,FrameBuffer);
    glBlitFramebuffer(0,0,RenderWidth,RenderHeight,0,0,width,height,GL_COLOR_BUFFER_BIT,GL_NEAREST);
    SwapBuffers(DeviceContext);
    glClearColor(0.5,0.5,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);
    glClearColor(0.5,0.5,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    if(B3D_Rendering)glClear(GL_DEPTH_BUFFER_BIT);
}



void Window::FinishRender(GLuint FBuffer){
    ConsoleEcho("Finsh render : Window");
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER,FBuffer);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlitFramebuffer(0,0,RenderWidth,RenderHeight,0,0,width,height,GL_COLOR_BUFFER_BIT,GL_NEAREST);
    NoticeGlError();
    ConsoleEcho("Swap des buffer de la Window");
    SwapBuffers(DeviceContext);
   // ConsoleEcho("REndu Finit");
}

void Window::DisplayPixel(int x,int y,int z,float* Color){
    RemindContext();
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    int cpl[3]={x,y,z};
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, 3*sizeof(float),cpl);
    glVertexAttribPointer(2,4,GL_FLOAT,GL_FALSE, 3*sizeof(float),Color);


}
void Window::RemindContext(){
    ConsoleEcho("Je recupere le Context");
    ContextLent=false;
    wglMakeCurrent(DeviceContext,GLRenderingContext);
    //glewInit();
    glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);
    ActivatePipeline();
}
void Window::Update(){

          // TextureDisplayer_Check();
            //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            //ConsoleEcho("un rendu");
            CallRender();

           Sleep (PerFrameIdleTime);

}




LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}


Window::~Window()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(GLRenderingContext);
    ReleaseDC(WindowHandler,DeviceContext);
    //SetEvent(ThreadClosed);
    CloseWindow(WindowHandler);
    if(WaitForSingleObject(ThreadStart,0)==WAIT_OBJECT_0)WaitForSingleObject(ThreadClosed,INFINITE);
    CloseHandle(ThreadClosed);
    TerminateThread(WNDThread,1000);
    CloseHandle(WNDThread);
    CloseHandle(WNDDemandSent);
    CloseHandle(WNDDemandAnswered);
    CloseHandle(WNDRenderSuspended);
    CloseHandle(ThreadStart);
}

