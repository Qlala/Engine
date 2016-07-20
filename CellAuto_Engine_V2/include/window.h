#pragma once
#include "Header.h"
#include "RenderObject.h"
extern HINSTANCE GlobalInstanceHandle;
extern int CMDShowNumber;
extern HDC GlobaHDC;
extern HGLRC GLobalHGLRC;

class Window : public RenderObject {
    //bool d'heritage=> défini le comportement de renderObject
    bool RenderThreaded=true;
    //Propriété de fenetre particuliere
    unsigned int FrameRate;
    int width;
    int height;
    //context de fenetre
    HWND WindowHandler;
    //HDC DeviceContext;
    //HGLRC GLRenderingContext;
    DEVMODE DeviceMode;
    DWORD ThreadId;
    HANDLE WNDThread;
    //Propriété superficiel
    const char* Title="Defaul Title\0";
    public :
    //inter thread COmm
    int test;
    volatile HANDLE ThreadStart;
    volatile HANDLE ThreadClosed;
    HANDLE WNDDemandSent;
    HANDLE WNDDemandAnswered;
    HANDLE WNDRenderSuspended;
    int WNDDemandCode;
    private:
    //bool d'état
    bool Created=false;
    bool FullScreen=false;
    bool VSync=false;
    int PerFrameIdleTime=0;
    bool ContextLent=false;


    //Variable des Built-in fonction
        //Texture Diplayer
        GLuint TextureDisplayer_Adress;
        bool TextureDisplayer_Inited=false;
        GLuint TextureDisplayer_Texture=-1;
        //PixelDrawer
        GLuint PixelDrawer_Adress;
        bool PixelDrawer_Inited;
    //ScreenMode
    std::list<int*> ResolutionList;
    std::list<int> FrameRateList;
    float theta = 0.0f;
    public://fonction pour rendre le context avec suspension des ordre de rendu tant que le context est prêtés
        void GetScreenMode();
        void GetDeviceMode();
        void UpdateDeviceMode();

        void CheckDemand();
        Window(int width=0,int height=0,int framerate=0,bool FullScreen_a=true,int Sample=0);
        void Update();
        void WNDCheckDemand();
        void VSyncState(bool st);
        void GetRendererInfo();
        void WindowThread();
        void Create();
        void DisplayFrame();
        void InitPipeline();
        void RemindContext();
        void SetContextAsMain();
        bool BorrowContext();
        bool ReturnContext();
        void DisplayPixel(int x,int y,int z,float* Color);
        //Fonction Hérité des RenderObject
        //void PrepareRenderTarget();
        void FinishRender(GLuint FBuffer);
        //Builtin function
            //Texture Displayer
            void TextureDisplayer_Activation();
            void TextureDisplayer_SetTextureObject(TextureObject Texture);
            void TextureDisplayer_Check();
            void TextureDisplayer_InternReActivation();
        void WaitExit(){WaitForSingleObjectEx(ThreadClosed,INFINITE,true);}//alertable
        virtual ~Window();
    protected:
    private:
};
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
