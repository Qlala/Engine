#pragma once
#include "Header.h"
#include "shader.h"
class RenderObject;
class RenderSource;
typedef bool (*RenderFunc)(RenderObject*);
typedef bool (*RenderFuncWarper)(RenderObject*,RenderFunc);
struct DrawBuffer{
    //Rule
    GLuint ColorRes;//GL_RGBA8...
    GLuint TextureType;//G_TEXTURE_2D
    GLuint BufferFormat;//GL_RGB GL_BGR
    float Ratio;
    //Actual buffer
    bool Initied;
    GLuint Buffer;
};

class RenderObject: public ShaderHandler{
    protected:
        float theta=0;
        //Bool utra imporatnte pour le fonctionement
        bool RenderThreaded;
        //Render in Process
        HANDLE RenderHung;
        //Context
        HGLRC GLRenderingContext;
        bool Context_Initied=false;
        HDC DeviceContext;
        HGLRC SharedRenderContext;//partagé avec le Bound Object
        HGLRC UserSharedRenderContext;//Ce Context est géré par l'utilisateur
        //GLEWContext RenderGlewContext;
        //Propriété
        unsigned int RenderWidth;
        unsigned int RenderHeight;
        //bool d'état
        bool BufferInited=false;
        bool PostBufferInited=false;
        bool B3D_Rendering=false;
        bool Stenciled_Rendering=false;
        bool DepthedPostProcess=false;//Post Process avec de la Profondeur
        int MSAA_Sample=0;

        bool AutoCleanBuffer=true;
        //Programde Rendu
        GLuint Render_Pipeline;
        GLuint* RenderShaderAdress;
        GLuint* RenderShaderType;
        int RenderShaderN;
        bool RenderDefaultProgramInitied=false;
        GLuint PP_Pipeline;
        GLuint* RenderPPShaderAdress;
        GLuint* RenderPPShaderType;
        int RenderPPShaderN;
        bool RenderPPProgramInitied=false;
        //Buffer
        GLuint FrameBuffer;
        GLuint ColorTexture;
        GLuint ZbufferTexture;
        GLuint StencilBuffer;

        GLuint PostProcessBuffer;//farmebuffer pour le PP
        GLuint ColorDisplayTexture;// texture du PP
        GLuint DepthDisplayTexture;//ne gere que la profondeur d'affichage en PostProcess;




        //drawBuffer Rule
        std::vector<DrawBuffer> DrawBuffers;

        //Précision des couleur
        GLuint ColorBitDepth=GL_RGBA8;
        GLuint ZBitDepth=GL_DEPTH_COMPONENT32F;
        GLuint StencilDepth=GL_STENCIL_INDEX8;
        //Objet lié à la scene
        bool SourceBound=false;
        bool SourceInitied=false;
        bool RenderPostProcActivated=false;
        RenderSource* Source;
    public:
        int Test=5;
        //GetFunction
        int GetRenderWidth(){return RenderWidth;}
        int GetRenderHeight(){return RenderHeight;}
        //initialisation
        RenderObject();
        ~RenderObject();
        //Fonction pour la demande HGLRC
        bool AskGlContext();
        //Activationde Fonctionalité
        void SetPostProcessState(bool state);
        //Buffer Init
        void PrepareFramebuffer();
        void PrepareSamplingBuffer(int Sample);
        void PreparePostProcessBuffer();
        void PrepareBuffer();
        void InitZbuffer();//ne vérifie pas la condition B3D mais prend en compte le MSAA
        void InitStencilBuffer();//??? peut etre je sais pas vraiment si ça peut servir
        void InitDepthPPBuffer();//ne Vérifie pas DepthedPostProcess mais le bind automatiquement au framebuffer (pensé à rebind au FB apès l'appelle de la fonction
        //Gestion Des buffer
        void CleanBuffer();
        void DeleteBuffer();
        void DeletePostBuffer();
        void DeleteZbuffer();
        void DeleteStencilBuffer();
        void DeleteDepthPPBuffer();
        void SwapBufferRule(int RW,int RH,int sample,GLuint* TextureType,GLuint* TextureIndex,int N);
        void DefineDrawBuffer(GLuint* TextureType,GLuint* TextureIndex,int N);
        void ClearColor();
        void ClearDepth();
        void ResizeRender(int H,int W);
        void BindFrameToSlot(GLuint Tex);
        void BindAmbivalentFrameToSlot(AmbivalentUniform AmbUni,GLuint slot);//fait le binding de la texture et l'affectation de l'uniform
        void BindAmbivalentStencilToSlot(AmbivalentUniform AmbUni,GLuint slot);
        void BindAmbivalentDepthToSlot(AmbivalentUniform AmbUni,GLuint slot);
        void BindZBufferToSlot(GLuint Tex);
        void BindAmbivalentTextureToSlot(AmbivalentUniform AmbUni,GLuint TexToBind,GLuint slot);
        void SetAmbivalentUniform(AmbivalentUniform AmbUni,GLuint slot);//rien n'oblige les 2 type du multisample à etre 2 version d'un meme type MS et non MS
        //Gestion des buffer Rule
        GLuint AddBufferRule(GLuint Color,GLuint Type,GLuint BufferFormat,float Ratio);
        GLuint AddBufferRule(GLuint Color,GLuint BufferFormat);
        void DeleteAllBufferRule();
        void DeleteBufferRule(GLuint N);
        void CleanDrawBuffer(GLuint N);
        void CleanAllDrawBuffer();
        GLuint AccessBuffer(GLuint N);
        void ActivateBufferRule(bool Force);
        void SetBufferRule(GLuint* Plan,GLuint N);
        void InitDrawBuffer(GLuint N,bool Force);
        void InitDrawBuffer(std::vector<DrawBuffer>::iterator A,bool Force);
        void BindBufferToSlot(GLuint buffer,GLuint slot);
        //changement de 2d->3d
        void Define3dRender(bool state);
        void DefineDepthedPP(bool state);
        //changement de mode de nettoyage
        void DefineAutoClean(bool state);
        //Frame Buffer
        void RenderInFrameBuffer();//Rendu dans le FB de rendu
        bool CheckFBComplete();
        void PrepareRenderTarget();//rendu dans le FB de PP
        //Fonction utilisateur
        void ShareContext();
        void SaveFrameBufferTextureAsset(string Filename);
        void GiveBackContext();
        //Gestion du ProgramPrincipal
        void EnableRenderMethod();//active les methode de rendu particuliere ( msaa et gl depth test)
        void EnablePostProcessMethod();//que GL_Depth_Test pour l'instant

        void DefineMainShaderProgram(GLuint* PrgAdress,GLuint* PrgType,int n);
        void RestoreMainShader();
        void ClearMainShader(bool del);

        void DefinePPShaderProgram(GLuint* PrgAdress,GLuint* PrgType,int n);
        void RestorePPShader();
        void ClearPPShader(bool del);
        //Communication de Donné au Shader
        void ComToShaderFrameResolution(GLuint Location);//ivec2
        void ComToShaderMSAAsample(GLuint Location);//Int
        void ComToShaderDepthActivated(GLuint Location);//int 1/0
        //Notion de Rendu
        void DefineRenderSource(void* ptr){ConsoleEcho("Render Source");memory_fence();};
        bool CallRender();
        bool BindRenderSource(RenderSource*Source);
        void UnbindRenderSource();
        virtual void FinishRender(GLuint FBuffer){};
        //fonction tres dépendant du contexte de rendu
        virtual void Update(){};





};
