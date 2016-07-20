#include "Header.h"


HINSTANCE GlobalInstanceHandle;
int CMDShowNumber;
HDC GlobaHDC;
HGLRC GLobalHGLRC;
char LauchDirectory[MAX_PATH];
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    GetCurrentDirectory(MAX_PATH,LauchDirectory);
    GlobaHDC=wglGetCurrentDC();
    GLobalHGLRC=wglGetCurrentContext();
    wglMakeCurrent(GlobaHDC,GLobalHGLRC);
    HINSTANCE GlobalInstanceHandle=hInstance;
    int CMDShowNumber=nCmdShow;
    Window MainWindow=Window(0,0,0,true,16);
    ConsoleEcho("Ca continue");
    SetCurrentDirectory("Shader");
/*
    int Add=MainWindow.AddShader("2dMain.vert",GL_VERTEX_SHADER,-1);
    MainWindow.AddShader("2dMain.geo",GL_GEOMETRY_SHADER,Add);

    MainWindow.AddShader("2dMain.frag",GL_FRAGMENT_SHADER,Add);

    GLuint ProgramL[1]={0};
    GLuint PrgProp[1]={GL_VERTEX_SHADER_BIT | GL_GEOMETRY_SHADER_BIT|GL_FRAGMENT_SHADER_BIT };
    MainWindow.SetActiveInternProgram(ProgramL,PrgProp,1);

    float Test=0.5f;
    MainWindow.SetUniform(GL_FLOAT,1,&Test,0,"test");
*/
    /*MainWindow.TextureDisplayer_Activation();
    MainWindow.BorrowContext();
    GLuint TexTest;
    TextureObject TestA=TextureObject("Test.bmp");
    int Swa[4]={2,1,0,4};
    BYTE Def[4]={0x00,0x00,0x00,0xFF};
    TestA.ConvertBMP(4,1,4,Swa,Def,GL_RGBA,GL_UNSIGNED_BYTE);
    TestA.Load(0);
    ConsoleEcho("on tente");
    MainWindow.TextureDisplayer_SetTextureObject(TestA);
    MainWindow.ReturnContext();*/
    //MainWindow.DefineAutoClean(false);
    MainWindow.ShaderChangeCheckMode(false);
    MixedScene SceneTest=MixedScene();
    SetCurrentDirectory(LauchDirectory);
//    execl("cd","..");
    //SceneTest.CallAPC<void (MixedScene::*),const char*>(SceneTest.LoadFromIni,"scene.ini")
    SceneTest.LoadFromIni("scene.ini");
    Sleep(20);
    SceneTest.NameScene("connard");
    //SceneTest.Testint=5;

     /*ConsoleEcho("det B=%i",(int)Mat<2>(1).det());
     ConsoleEcho("det B=%i",(int)~((Mat<2>(1)/0)%0));
     ConsoleEcho("det B=%i",(int)~((Mat<2>(0)/0)%1));*/
    //unsigned int* Result=SceneTest.CallCriticalSceneAPC(Test4,(void*)"coucou");
   // SceneTest.CommunicateDrawFuncToRender(&MainWindow);
   SceneTest.BindToRender(&MainWindow);
    /*SceneTest.CallSceneAPC(Test4,(void*)"coucou1");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou2");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou3");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou4");
    SceneTest.LockAPC();
    SceneTest.CallSceneAPC(Test4,(void*)"coucou5");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou6");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou7");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou8");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou9");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou10");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou11");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou12");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou13");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou14");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou15");
    SceneTest.CallSceneAPC(Test4,(void*)"coucou16");*/
    //ConsoleEcho("Fonction Longue");
    //Sleep(2000);
    //SceneTest.UnlockAPC();
    //Sleep(3000);
    //SceneTest.LockAPC();
    //ConsoleEcho("Fonction Longue2");
    //Sleep(2000);
    //SceneTest.UnlockAPC();
    ConsoleEcho("Apres Tout ce quin C'est Passé");
//    std::cout<<Result[0]<<std::endl;
    //SceneTest.UnlockAPC();
      // Window MainWindow2:=Window(255,255,0,false,0);
   /*Window MainWindow3=Window(255,255,0,false,32);
    Window MainWindow4=Window(255,255,0,false,32);
    Window MainWindow5=Window(255,255,0,false,32);
    Window MainWindow6=Window(255,255,0,false,32);*/
    MainWindow.ShareContext();
    Sleep(1000);
    /*TextureAsset Test=TextureAsset("Sans titre.bmp");
    Test.LoadVram();
    Test.EchoTextureInfo(true);
    Test.SaveBMP("Test.bmp");*/
    //PolynomTool::TestPolynom();
    /*
    IniDecoder IniTest;
    IniTest.DirectDecode("scene.ini");
    std::vector<IniProp> Propsfound=IniTest.SearchProps("VBO_Definition.*");
struct Teststruct{
        string Property;
        GLuint Adress;
        float test2;
        bool test;
        char test5;
        bool test6;
};
    Propsfound[0].Filter({"PropertyName","PropertyType","PropertyAdress"});
    Propsfound[0].EchoData();
    Teststruct test5=Propsfound[0].Convert<Teststruct>({INI_STRING,INI_GL_TYPE,INI_FLOAT,INI_EMPTY_BOOL,INI_EMPTY_BYTE,INI_EMPTY_BOOL});
*/
    AssetManager AH;
    AssetManager::BindManager(&AH);
    Asset<TextureAsset>* a5=CreateAsset<TextureAsset,string>("Test.bmp","Test.bmp");
    (*a5)->LoadRam();
    (*a5)->LoadVram();
    (*a5)->UnloadRam();
    (*a5)->LoadRam();
    (*a5)->SaveBMP("Testasset7.bmp");
/*
    TextureAsset* tex= new TextureAsset("Test.bmp");
    AH.CreateNewParentAsset(tex);
    TextureAsset* tex2= new TextureAsset("Test1.bmp");
    Asset<TextureAsset>* Pa1=AH.CreateNewAsset(tex2);
    TextureAsset* tex3= new TextureAsset("Test3.bmp");
    TextureAsset* tex3_clone= new TextureAsset("Test3.bmp");
    TextureAsset* tex4= new TextureAsset("Tesa2.bmp");
    Asset<TextureAsset>* a3 = AH.CreateNewAsset(tex3);
    AH.CreateNewAsset(tex4);
    ParentAsset<TextureAsset>* texsearched;
    ConsoleEcho("Test des Asset");
    if(AH.SearchParentAsset("Aesa2.bmp",texsearched)){
        ConsoleEcho("Trouvé");
        ConsoleEcho(texsearched->AskName());
    }else{
        ConsoleEcho("Rien Trouvé");
    }
    Asset<TextureAsset>* a3_clone=AH.CreateAsset(tex3_clone);
    (*a3_clone)->LoadRam();
    (*a3_clone)->LoadVram();
    (*a3_clone)->UnloadRam();
    (*a3_clone)->LoadRam();
    (*a3_clone)->SaveBMP("Testasset.bmp");
    delete a3;
    delete a3_clone;
    delete Pa1;
*/

   // MainWindow.GetFrameBufferTextureAsset();
    BOOL bQuit = FALSE;
    MSG msg;
    MainWindow.WaitExit();
    ConsoleEcho("hopopo");
}




