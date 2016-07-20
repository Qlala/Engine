#pragma once
#include "AssetManager.h"
#include "Header.h"
class AssetManager;

/*Class m�re elle ne doit contenir que es fonction g�n�rale relative � l'interaction avec un domain d'affichage
Pr�voir notament:
-Un fonction qui sera pass� a l'affichage et sera appel� lors du rendu
-Cette fonction recevra en parametre L'objet D'affichage
-Pr�voir un syst�me pour g�rer les adressage du pipeline
-Pr�voir Une Fonction d'init qui sera appel� par l'affichage
Id�e:
Fonction Appel� en Post Process qui pourra etre pass� si diff�rent d'un NULL_ptr

Faire un thread qui ne COmmunique que par APC
qui sert � g�r�
*/
class Scene;
class RenderSource{
protected:
    //Variable de d�finition (� d�finir pour les classe filles)
    bool PostProcess=false;
    //Besoin pour des question de gestion de context
    bool BoundToRender=false;
    public:
    virtual inline bool IsPostProcess(){return PostProcess;}
    virtual  bool RenderInit(RenderObject* ptr){return false;};
    virtual  bool RenderUpdate(RenderObject* ptr){return false;};
    virtual  bool RenderPostProc(RenderObject* ptr){return false;};
    virtual  bool RenderOnQuit(RenderObject* ptr){return false;};
    bool BindToRender(RenderObject* ptr);
    virtual bool CallOnBinding(RenderObject* ptr){return false;};
};

typedef VOID CALLBACK APCVoid;
typedef VOID CALLBACK (APCFunc)(ULONG_PTR);
typedef void (SceneAPCFunc)(Scene*,void*);
struct APCPasserStruct{
    Scene* Sc;
    void* Param;
    SceneAPCFunc* Func;
    unsigned int* CriticalCheck;
};
struct RenderTarget{
    string Name;
    GLuint BufferAdress;//Adress relative au RenderObject
    AmbivalentUniform UniformAdress;//AdressR�el
    GLuint ShaderProgram;//Program avec un num�ro realtif � la scene
    GLuint ColorRes;
    GLuint TextureType;
    GLuint Format;
    float Ratio;
    bool MultiSampleIt;//indique s'il faut apliqu� le multi sample la texture
    bool BufferInitied;
    bool UniformInitied;
    bool Removed;
};

extern void Test4(Scene* me,void* MSG);
struct Shader{
    std::vector<string> Filepath;
    std::vector<GLuint> Type;//type de chaque shader
    GLuint NewBitType;
    GLuint BitType;// concatenation de bit
    GLuint Program;
    bool Initied=false;
    bool Removed=false;
};


class Scene : public RenderSource{
    protected:

        //Gestion du Thread
        HANDLE SceneThread;
        DWORD ThreadId;
        HANDLE SceneHung;
        HANDLE ThreadStarted;
        //Context de rendu HGLRC
        bool Context_Received;
        HANDLE ContextGot;
        bool Ready=false;
        bool PostProcessed_Linked=false;//activ� si le poste process est link� (uniform acessible)
        //nom
        char* Name="not init\0";
        //Render target : buffer � initialis� et dont on doit r�cup�r� les adress dans le PostProcess
        std::vector<RenderTarget> RenderTargets;
        //Shader : bien pens� que l'user croit que les shader commence � 1 alors qu'il commence � 0
        std::vector<Shader> ShaderArray;
        //Asset manager
        bool ManagerJustPassed=false;
        bool ManagerPassed=false;
        AssetManager* AssetLibrary;
    public:
        int Testint;
        Scene();
        //Gestion des THread
        void ThreadRoutine();
        void WaitContext(DWORD timea);
        bool Update(RenderObject* Obj_ptr);
        void Test();
        void Test2();
        //Fonction de load
        virtual void LoadFromIni(char* Filename){};
        //fonction a d�clar� pour g�r� le thread de la scene
        virtual void CallSceneRoutine(){ConsoleEcho("Virtuelle pure");};

        inline void TakeMutex(){WaitForSingleObject(SceneHung,INFINITE);};
        inline void ReleasMutex(){ReleaseMutex(SceneHung);};
        inline void WaitStart(){WaitForSingleObject(ThreadStarted,INFINITE);};

        void LockAPC();
        void NameScene(char*A){Name=A;memory_fence();};

        void UnlockAPC();
        void Say(char* MSG){ConsoleEcho(MSG);};
        //Gestion des context
        void SendHGLRC(RenderObject* ptr);// Une fonction est interne l'autre extern
        void ReceiveContext(RenderObject* ptr);
        void SayName(){ConsoleEcho("Viole");Sleep(200);ConsoleEcho(Name);};
        //la fonction appel� recoit La Scene en parametre;
        void CallSceneAPC(SceneAPCFunc Func,void* ParamStruct);
        unsigned int* CallCriticalSceneAPC(SceneAPCFunc Func,void* ParamStruct);
        //ca ce sont juste des fonction normal qui prenent un parmetre de type Param

        template<typename Function,typename Param>
            unsigned int* CallCriticalAPC(Function f,Param &A);
        template<typename Function,typename Param>
            void CallAPC(Function f,Param &A);
            //la diff�rence avec celle d'avant cest qu'elle demande le mutex
        template<typename Function,typename Param>
            void CallLocalAPC(Function f,Param &A);
        template<typename Function,typename Param>
            unsigned int* CallLocalCriticalAPC(Function f,Param &A);
        ~Scene();
        //AssetManager
        void PassAssetManager(AssetManager* AH);
        void PassAssetManager_private(AssetManager* AH);
        void DeleteAssetManager();
        void WithdrawAssetManager();
        void BindAssetManager();
        void UpdateAssetManagerBinding();
        //Trouv� des Uniform
        //Gestion des Shader
        void InitShaders_FromIniDecoder(IniDecoder* ini);
        void NewShader(string Filename,GLuint Type,GLuint BitType);//Newprogram permet de faire en sorte que le noubveau sahder soit soit d�par�e soit fusion�e avec le program du shader d'avant
        void MergeShader(string Filename,GLuint N,GLuint Type,GLuint BitType);//Rajout� un Shader � un Shader d�ja initialis�
        void MergeShaderWithLast(string Filename,GLuint Type,GLuint BitType);
        void InitDeclaredShader(RenderObject* RO,bool Force=false);
        void InitShader(GLuint N,RenderObject* RO);
        void RemoveShaderProgram(GLuint N);
        void TakeOutRemovedShader(RenderObject* RO);
        GLuint RealProgram(GLuint N);
        void DeleteAllShader(RenderObject* ptr);
        void UninitializeAllShader(RenderObject* ptr);
        //GLuint RealGLProgram(GLuint N,RenderObject* RO);
        //Gestion des Target on les num�rote de 1 � N
        void InitRenderTargets_FromIniDecoder(IniDecoder* ini);
        GLuint NewRenderTarget(RenderTarget tar);//Pens�e � avertir l'user des num�ros des possible doublon en nombre d�fini dans l'ini
        GLuint NewRenderTarget(string Name,GLuint ColorRes,GLuint Format,GLuint Program);
        void InitTarget(GLuint N,RenderObject* RO);
        void InitTargetUniform(std::vector<RenderTarget>::iterator A,RenderObject* RO);
        void InitTargetAmbivalentUniform(std::vector<RenderTarget>::iterator A,RenderObject* RO);
        void InitDeclaredTarget(RenderObject* RO,bool force=false);
        void RemoveTarget(GLuint N);
        void SetTargetPlan(RenderObject* RO);
        void SwapFrameToSource(RenderObject* RO,GLuint FrameUniformAdress,GLuint Tex);
        void SwapZBufferToSource(RenderObject* RO,GLuint ZUniformAdress,GLuint Tex);
        void SwapAllTargetToSource(RenderObject* RO,GLuint FirstTex);// ne fait que pour les render target
        void SwapTargetToSource(RenderObject* RO,GLuint N,GLuint FirstTex);
        void TakeOutRemovedTarget(RenderObject* RO);
        void DeleteAllTarget(RenderObject* RO);
        void UnitializeAllTarget(RenderObject* RO);
        //les fonction suivantes sont � cod� je raplle que les shader sont compt� � partir de 1 pour l'user
        void AddShaderAsMainShader(GLuint* List,int N,RenderObject* RO);
        void AddShaderAsPostProcessShader(GLuint* List,int N,RenderObject* RO);
        void ActivateShaderProgram(GLuint* List,int N,RenderObject* RO);
        void ClearMainShader(bool Delete);//pas cod�
        void ClearPostProcessShader(bool Delete);//pas cod�
        //redefinition de Render Source
        bool BindToRender(RenderObject* ptr);

 };



 //SceneAPCFunc


