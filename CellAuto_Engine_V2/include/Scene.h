#pragma once
#include "AssetManager.h"
#include "Header.h"
class AssetManager;

/*Class mère elle ne doit contenir que es fonction générale relative à l'interaction avec un domain d'affichage
Prévoir notament:
-Un fonction qui sera passé a l'affichage et sera appelé lors du rendu
-Cette fonction recevra en parametre L'objet D'affichage
-Prévoir un système pour gérer les adressage du pipeline
-Prévoir Une Fonction d'init qui sera appelé par l'affichage
Idée:
Fonction Appelé en Post Process qui pourra etre passé si différent d'un NULL_ptr

Faire un thread qui ne COmmunique que par APC
qui sert à géré
*/
class Scene;
class RenderSource{
protected:
    //Variable de définition (à définir pour les classe filles)
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
    AmbivalentUniform UniformAdress;//AdressRéel
    GLuint ShaderProgram;//Program avec un numéro realtif à la scene
    GLuint ColorRes;
    GLuint TextureType;
    GLuint Format;
    float Ratio;
    bool MultiSampleIt;//indique s'il faut apliqué le multi sample la texture
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
        bool PostProcessed_Linked=false;//activé si le poste process est linké (uniform acessible)
        //nom
        char* Name="not init\0";
        //Render target : buffer à initialisé et dont on doit récupéré les adress dans le PostProcess
        std::vector<RenderTarget> RenderTargets;
        //Shader : bien pensé que l'user croit que les shader commence à 1 alors qu'il commence à 0
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
        //fonction a déclaré pour géré le thread de la scene
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
        //la fonction appelé recoit La Scene en parametre;
        void CallSceneAPC(SceneAPCFunc Func,void* ParamStruct);
        unsigned int* CallCriticalSceneAPC(SceneAPCFunc Func,void* ParamStruct);
        //ca ce sont juste des fonction normal qui prenent un parmetre de type Param

        template<typename Function,typename Param>
            unsigned int* CallCriticalAPC(Function f,Param &A);
        template<typename Function,typename Param>
            void CallAPC(Function f,Param &A);
            //la différence avec celle d'avant cest qu'elle demande le mutex
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
        //Trouvé des Uniform
        //Gestion des Shader
        void InitShaders_FromIniDecoder(IniDecoder* ini);
        void NewShader(string Filename,GLuint Type,GLuint BitType);//Newprogram permet de faire en sorte que le noubveau sahder soit soit déparée soit fusionée avec le program du shader d'avant
        void MergeShader(string Filename,GLuint N,GLuint Type,GLuint BitType);//Rajouté un Shader à un Shader déja initialisé
        void MergeShaderWithLast(string Filename,GLuint Type,GLuint BitType);
        void InitDeclaredShader(RenderObject* RO,bool Force=false);
        void InitShader(GLuint N,RenderObject* RO);
        void RemoveShaderProgram(GLuint N);
        void TakeOutRemovedShader(RenderObject* RO);
        GLuint RealProgram(GLuint N);
        void DeleteAllShader(RenderObject* ptr);
        void UninitializeAllShader(RenderObject* ptr);
        //GLuint RealGLProgram(GLuint N,RenderObject* RO);
        //Gestion des Target on les numérote de 1 à N
        void InitRenderTargets_FromIniDecoder(IniDecoder* ini);
        GLuint NewRenderTarget(RenderTarget tar);//Pensée à avertir l'user des numéros des possible doublon en nombre défini dans l'ini
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
        //les fonction suivantes sont à codé je raplle que les shader sont compté à partir de 1 pour l'user
        void AddShaderAsMainShader(GLuint* List,int N,RenderObject* RO);
        void AddShaderAsPostProcessShader(GLuint* List,int N,RenderObject* RO);
        void ActivateShaderProgram(GLuint* List,int N,RenderObject* RO);
        void ClearMainShader(bool Delete);//pas codé
        void ClearPostProcessShader(bool Delete);//pas codé
        //redefinition de Render Source
        bool BindToRender(RenderObject* ptr);

 };



 //SceneAPCFunc


