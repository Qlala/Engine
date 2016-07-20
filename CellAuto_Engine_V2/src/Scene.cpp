#include "Header.h"
DWORD WINAPI SceneThreadPasser(void* SceneA){
((Scene*)SceneA)->ThreadRoutine();
}
bool RenderSource::BindToRender(RenderObject* ptr){
    RenderSource* me=static_cast<RenderSource*>(this);
    BoundToRender=ptr->BindRenderSource(me);
   return BoundToRender;
}
bool Scene::BindToRender(RenderObject* ptr){
    ConsoleEcho("Scene en train d'etre Bind");
    RenderSource* me=static_cast<RenderSource*>(this);
    BoundToRender=ptr->BindRenderSource(me);
    SendHGLRC(ptr);
    CallOnBinding(ptr);
   return BoundToRender;
}

Scene::Scene(){

    SceneHung=CreateMutex(NULL,false,GenStrWithUniqueCode("SceneHung",false));
    ThreadStarted=CreateEvent(NULL,true,false,GenStrWithUniqueCode("SceneThreadStarted",false));
    SceneThread=CreateThread(NULL,0,SceneThreadPasser,(void*)this,0,&ThreadId);
    ContextGot=CreateEvent(NULL,true,false,GenStrWithUniqueCode("SceneContextGot"));
}
Scene::~Scene(){
    ConsoleEcho("Fermeture de la Scene");
    TerminateThread(SceneThread,1000);
    CloseHandle(ThreadStarted);
    CloseHandle(SceneThread);
    CloseHandle(SceneHung);
    CloseHandle(ContextGot);
}

APCVoid CallSceneFunction(ULONG_PTR SC){
    Scene * CallerScene=(Scene*)SC;
    CallerScene->Test2();
    ConsoleEcho("Test APC");
}
void Scene::Test2(){
    ConsoleEcho("Test2 APC");
}
void Scene::LockAPC(){
    ConsoleEcho("Odre de Lock des APC");
    WaitForSingleObject(SceneHung,INFINITE);
    ConsoleEcho("APC suposé lock");
};
void Scene::UnlockAPC(){
    ReleaseMutex(SceneHung);
};
void Scene::ReceiveContext(RenderObject* ptr){
    ConsoleEcho("Pre-Tentative d'envoie de Context");
    Context_Received=ptr->AskGlContext();
    if(Context_Received){
        SetEvent(ContextGot);
        ConsoleEcho("Context recu en APC par la Scene");
    }
};
void Scene::WaitContext(DWORD timea){
    WaitForSingleObjectEx(ContextGot,timea,true);
}

void GiveHGLRC(Scene* Sc,void* RenderObj){
    RenderObject* RO=static_cast<RenderObject*>(RenderObj);
    Sc->ReceiveContext(RO);
}
void Scene::SendHGLRC(RenderObject* ptr){
    ConsoleEcho("Envoi du context en APC");
    this->CallSceneAPC(GiveHGLRC,(void*)ptr);
}

void Scene::ThreadRoutine(){
    SetEvent(ThreadStarted);
    while(WaitForSingleObject(ThreadStarted,0)==WAIT_OBJECT_0){
        //ConsoleEcho("Une ittération");
        CallSceneRoutine();
        //ConsoleEcho("Une ittération");
        //SetEvent(ThreadStarted);
        //WaitForSingleObjectEx(StopAPCTreatement,1,true);
        //WaitForSingleObjectEx(StopAPCTreatement,INFINITE,false);
        //Sleep(INFINITE);
        SleepEx(1,true);
        ReleaseMutex(SceneHung);
        //ResetEvent(StopAPCTreatement);
        //SetEvent(AuthorizeNewAPC);
    }
    ResetEvent(ThreadStarted);
}
template<typename Function,typename Param>
    struct Compr{
        Function f_ptr;
        Param Param_ptr;
        unsigned int* CritcalCode;
    };
template<typename Function,typename Param>
    struct ComprSC{
        Scene* SC_Ptr;
        Function* f_ptr;
        Param* Param_ptr;
        unsigned int* CritcalCode;
    };
//fonction sans Mutex
//atention je dépointe les parametre donc il faudra bien respecté ca
template<typename Function,typename Param>
    APCVoid SceneAPCCallBack(ULONG_PTR CO){
        Compr<Function,Param>* COMPR=(Compr<Function,Param>*)CO;
        *(COMPR->f)(*(COMPR->Param_ptr));
        delete COMPR;
    };

template<typename Function,typename Param>
     void Scene::CallAPC(Function f,Param &A){
        Compr<Function,Param>* COMPR=new Compr<Function,Param>;
        COMPR->f_ptr=&f;
        COMPR->Param_ptr=&A;
        QueueUserAPC(SceneAPCCallBack<Function,Param>,SceneThread,(ULONG_PTR)COMPR);
};
template<typename Function,typename Param>
    APCVoid SceneCriticalAPCCallBack(ULONG_PTR CO){
        Compr<Function,Param>* COMPR=(Compr<Function,Param>*)CO;
        try{
            *(COMPR->f)(*(COMPR->Param_ptr));
            COMPR->CritcalCode[0]=1;
        }catch(unsigned int A){
            COMPR->CritcalCode[0]=A;
        }catch(...){
            COMPR->CritcalCode[0]=-1;
        }
        delete COMPR;
};

template<typename Function,typename Param>
    unsigned int* Scene::CallCriticalAPC(Function f,Param &A){
        Compr<Function,Param>* COMPR=new Compr<Function,Param>;
        COMPR->f_ptr=&f;
        COMPR->CritcalCode=new unsigned int[1];
        COMPR->CritcalCode[0]=0;
        COMPR->Param_ptr=&A;
        QueueUserAPC(SceneCriticalAPCCallBack<Function,Param>,SceneThread,(ULONG_PTR)COMPR);
        return COMPR->CritcalCode;
};

//fonction avec mutex
template<typename Function,typename Param>
    APCVoid SceneAPCLocalCallBack(ULONG_PTR CO){
        ComprSC<Function,Param>* COMPR=(ComprSC<Function,Param>*)CO;
        COMPR->SC_Ptr->TakeMutex();
        *(COMPR->f)(*(COMPR->Param_ptr));
        COMPR->SC_Ptr->ReleasMutex();
        delete COMPR;

    };

template<typename Function,typename Param>
     void Scene::CallLocalAPC(Function f,Param &A){
        ComprSC<Function,Param>* COMPR=new ComprSC<Function,Param>;
        COMPR->f_ptr=&f;
        COMPR->SC_Ptr=(Scene*)this;
        COMPR->Param_ptr=&A;
        QueueUserAPC(SceneAPCLocalCallBack<Function,Param>,SceneThread,(ULONG_PTR)COMPR);
};
template<typename Function,typename Param>
    APCVoid SceneCriticalAPCLocalCallBack(ULONG_PTR CO){
        ComprSC<Function,Param>* COMPR=(ComprSC<Function,Param>*)CO;
        COMPR->SC_Ptr->TakeMutex();
        try{
            *(COMPR->f)(*(COMPR->Param_ptr));
            COMPR->CritcalCode[0]=1;
        }catch(unsigned int A){
            COMPR->CritcalCode[0]=A;
        }catch(...){
            COMPR->CritcalCode[0]=-1;
        }
        COMPR->SC_Ptr->ReleasMutex();
        delete COMPR;

};

template<typename Function,typename Param>
    unsigned int* Scene::CallLocalCriticalAPC(Function f,Param &A){
        ComprSC<Function,Param>* COMPR=new ComprSC<Function,Param>;
        COMPR->f_ptr=&f;
        COMPR->SC_Ptr=(Scene*)this;
        COMPR->CritcalCode=new unsigned int[1];
        COMPR->CritcalCode[0]=0;
        COMPR->Param_pre=&A;
        QueueUserAPC(SceneCriticalAPCLocalCallBack<Function,Param>,SceneThread,(ULONG_PTR)COMPR);
        return COMPR->CritcalCode;
};


/*
static bool Scene::SceneCallWarper(RenderObject* ptr,RenderFunc Func){
    ConsoleEcho("bricolage");

    return static_cast<Scene*>(ptr->RenderSourcePtr)->TempFunction(ptr);
}
*/

VOID CALLBACK CalledFunc(ULONG_PTR ptr){

        ConsoleEcho("Je Suis une Fonction De Passage");
        APCPasserStruct* st=(APCPasserStruct*)ptr;
        st->Sc->TakeMutex();
        st->Func(st->Sc,st->Param);
        st->Sc->ReleasMutex();
        delete st;
};
VOID CALLBACK CalledCriticalFunc(ULONG_PTR ptr){

        ConsoleEcho("Je Suis une Fonction De Passage");
        APCPasserStruct* st=(APCPasserStruct*)ptr;
        st->Sc->TakeMutex();

        try{
            st->Func(st->Sc,st->Param);
            st->CriticalCheck[0]=1;
        }catch(unsigned int A){
            st->CriticalCheck[0]=A;
        }catch(...){
           st->CriticalCheck[0]=-1;
        }
        st->Sc->ReleasMutex();
        //delete st->Param;
        delete st;
        memory_fence();
};

unsigned int* Scene::CallCriticalSceneAPC(SceneAPCFunc Func,void*Param){

        APCPasserStruct* ST=new APCPasserStruct;
        ST->Sc=(Scene*)this;
        ST->Param=Param;
        ST->Func=Func;
        ST->CriticalCheck=new unsigned int[1];

        //WaitStart();
        QueueUserAPC(CalledCriticalFunc,this->SceneThread,(ULONG_PTR)ST);

        return ST->CriticalCheck;

};

void Scene::CallSceneAPC(SceneAPCFunc Func,void*Param){

        APCPasserStruct* ST=new APCPasserStruct;
        ST->Sc=(Scene*)this;
        ST->Param=Param;
        ST->Func=Func;
        //WaitStart();
        QueueUserAPC(CalledFunc,this->SceneThread,(ULONG_PTR)ST);

};
void Scene::PassAssetManager_private(AssetManager* AH){
    AssetLibrary=AH;
    AssetManager::BindManager(AH);
    ManagerPassed=AH->IsInitied();
    ManagerJustPassed=ManagerPassed;
}
void Scene::UpdateAssetManagerBinding(){
    if(ManagerJustPassed){
        AssetManager::BindManager(AssetLibrary);
        ManagerJustPassed=false;
    }
}
void Scene::BindAssetManager(){
    if(ManagerPassed){
        AssetManager::BindManager(AssetLibrary);
    }
};

void PassAssetManager_WARPER(Scene* SC,void* param){
    SC->PassAssetManager_private(static_cast<AssetManager*>(param));
}
void Scene::PassAssetManager(AssetManager* AH){
    CallSceneAPC(PassAssetManager_WARPER,(void*)AH);
}

void Scene::DeleteAssetManager(){
    if(ManagerPassed){
       if(AssetLibrary->IsInitied())delete AssetLibrary;
       ManagerPassed=false;
    }
}

void Scene::WithdrawAssetManager(){
    ManagerPassed=false;
}

void Scene::InitShaders_FromIniDecoder(IniDecoder* ini){
    GLuint LastNbr=0;
    struct ShaderStor{
    string FP;GLuint Type;GLuint Nbr;bool operator<(const ShaderStor St){return Nbr<St.Nbr;}};
    std::vector<ShaderStor> ShadArray;
    ShadArray=ConvertAll<ShaderStor>(ini->SearchProps("ShaderDeclaration"),{"Filepath","Type","ProgramNumber"},{INI_STRING,INI_GL_SHADER,INI_UNSIGNED_INT});
    std::list<ShaderStor> ShadList(ShadArray.begin(),ShadArray.end());
    ShadList.sort();
    for(std::list<ShaderStor>::iterator A =ShadList.begin();A!=ShadList.end();A++){
         GLuint BitType=GLtool::GetDedicated_ShaderBit(A->Type);
        if(LastNbr==A->Nbr){
            MergeShaderWithLast(A->FP,A->Type,BitType);
        }else{
            NewShader(A->FP,A->Type,BitType);
            LastNbr=A->Nbr;
        }
  }
}


void Scene::NewShader(string Filename,GLuint Type,GLuint BitType){
    Shader NS;
    NS.Filepath.push_back(Filename);
    NS.Initied=false;
    NS.Type.push_back(Type);
    NS.NewBitType=BitType;
    NS.BitType=0;
    NS.Program=-1;
    ShaderArray.push_back(NS);
}
void Scene::MergeShader(string Filename,GLuint N,GLuint Type, GLuint BitType){
    if(N<ShaderArray.size()){
        std::vector<Shader>::iterator ShadA=ShaderArray.begin()+N-1;
        ShadA->Filepath.push_back(Filename);
        ShadA->Type.push_back(Type);
        ShadA->NewBitType|=BitType;
    }
}

void Scene::MergeShaderWithLast(string Filename,GLuint Type, GLuint BitType){
    std::vector<Shader>::iterator ShadA=ShaderArray.end()-1;
    ShadA->Filepath.push_back(Filename);
    ShadA->Type.push_back(Type);
    ShadA->NewBitType|=BitType;
}


void Scene::InitDeclaredShader(RenderObject* RO,bool Force){
    if(!Force){
    ConsoleEcho("Init Shader Scene");
    for(std::vector<Shader>::iterator A=ShaderArray.begin();A!=ShaderArray.end();A++){
        if(!A->Initied){
            ConsoleEcho("Initalisation nouveau Shader");
            std::vector<GLuint>::iterator C=A->Type.begin();
            for(std::vector<string>::iterator B=A->Filepath.begin(); B!=A->Filepath.end();B++){
                ConsoleEcho(B->c_str());
                A->Program=RO->AddShader(*B,*C,A->Program);
                A->BitType=A->NewBitType;
                C++;
            }
            A->Initied=true;
            ConsoleEcho("Shader initialisé completement");
        }
    }

    }else{
        for(std::vector<Shader>::iterator A=ShaderArray.begin();A!=ShaderArray.end();A++){
            if(A->Initied){
                    RO->DeleteProgram(A->Program);
                    A->Program=-1;
                    A->Initied=false;
            }
        }
        InitDeclaredShader(RO);
    }
}
void Scene::RemoveShaderProgram(GLuint N){// à partir de la je deccide que les numéro de program de l'utilisateur commence à 1
    std::vector<Shader>::iterator A=ShaderArray.begin()+N-1;
    A->Removed=true;

}

void Scene::TakeOutRemovedShader(RenderObject* RO){
    for(std::vector<Shader>::iterator A=ShaderArray.begin();A!=ShaderArray.end();A++){
        if(A->Removed){
            if(A->Initied)RO->DeleteProgram(A->Program);
            ShaderArray.erase(A);
        }
    }

}
void Scene::DeleteAllShader(RenderObject* ptr){
    for(std::vector<Shader>::iterator A=ShaderArray.begin();A!=ShaderArray.end();A++){
        if(A->Initied)ptr->DeleteProgram(A->Program);
    }
    ShaderArray.empty();
}
void Scene::UninitializeAllShader(RenderObject* ptr){
    for(std::vector<Shader>::iterator A=ShaderArray.begin();A!=ShaderArray.end();A++){
        if(A->Initied)ptr->DeleteProgram(A->Program);
    }

}


void Scene::InitShader(GLuint N,RenderObject* RO){
    if(N<ShaderArray.size() && 0<N){
        std::vector<Shader>::iterator ShadA=ShaderArray.begin()+(N-1);
        if(!ShadA->Initied){
            ConsoleEcho("Initialisation du Shader de la Scene");
            std::vector<GLuint>::iterator C=ShadA->Type.begin()+(N-1);
            for(std::vector<string>::iterator B=ShadA->Filepath.begin();B!=ShadA->Filepath.end();B++){
                ConsoleEcho(B->c_str());
                ShadA->Program=RO->AddShader(*B,*C,ShadA->Program);
                ShadA->BitType=ShadA->NewBitType;
            }
           ShadA->Initied=true;

        }
    }
};

GLuint Scene::RealProgram(GLuint N){
    ConsoleEcho("RealProgram %u",N);
    if(0<N && 0<ShaderArray.size()+1){
        return ShaderArray[N-1].Program;
    }else return -1;
}

void Scene::ActivateShaderProgram(GLuint* List,int N,RenderObject* RO){
    GLuint* PrgArray=new GLuint[N];
    GLuint* PrgType=new GLuint[N];
    std::vector<Shader>::iterator ShadA= ShaderArray.begin();
    for(int i=0;i<N;i++){
        InitShader(List[i],RO);
        PrgArray[i]=(ShadA+List[i]-1)->Program;
        PrgType[i]=(ShadA+List[i]-1)->BitType;
    }

    RO->SetActiveInternProgram(PrgArray,PrgType,N);
}
void Scene::AddShaderAsMainShader(GLuint* List,int N,RenderObject* RO){

    GLuint* PrgArray=new GLuint[N];
    GLuint* PrgType=new GLuint[N];
    std::vector<Shader>::iterator ShadA= ShaderArray.begin();
    ConsoleEcho("AddMainShader");
    for(int i=0;i<N;i++){
        InitShader(List[i],RO);
        ConsoleEcho("Listi:%i",List[i]);
        ConsoleEcho("NShader:%i",ShaderArray.size());
        ConsoleEcho((ShadA+(List[i])-1)->Filepath[0].c_str());
        PrgArray[i]=(ShadA+(List[i]-1))->Program;
        PrgType[i]=(ShadA+(List[i]-1))->BitType;
    }

    RO->DefineMainShaderProgram(PrgArray,PrgType,N);
}
void Scene::AddShaderAsPostProcessShader(GLuint* List,int N,RenderObject* RO){
    GLuint* PrgArray=new GLuint[N];
    GLuint* PrgType=new GLuint[N];
    std::vector<Shader>::iterator ShadA= ShaderArray.begin();
    for(int i=0;i<N;i++){
        InitShader(List[i],RO);
        PrgArray[i]=(ShadA+List[i]-1)->Program;
        PrgType[i]=(ShadA+List[i]-1)->BitType;
    }
    PostProcessed_Linked=true;
    RO->DefinePPShaderProgram(PrgArray,PrgType,N);
};

//Fonction pour la gestion des Targets
void Scene::InitRenderTargets_FromIniDecoder(IniDecoder* ini){
    struct RenderTar{
        string N;GLuint Res;GLuint Rank;GLuint Format;float Ratio;GLuint Program;bool operator<(const RenderTar tr){return Rank<tr.Rank;}};
    std::vector<RenderTar> TarArray;
    TarArray=ConvertAll<RenderTar>(ini->SearchProps("RenderTarget"),{"Name","Resolution","Rank","Format","Ratio","Program",},{INI_STRING,INI_GL_TEX,INI_UNSIGNED_INT,INI_GL_TEX_FORMAT,INI_FLOAT,INI_UNSIGNED_INT});
    std::list<RenderTar> TarList(TarArray.begin(),TarArray.end());
    TarList.sort();
    for(std::list<RenderTar>::iterator A =TarList.begin();A!=TarList.end();A++){
        NewRenderTarget(A->N,A->Res,A->Format,A->Program);
        ConsoleEcho(A->N.c_str());
    }
};


GLuint Scene::NewRenderTarget(RenderTarget tar){
    RenderTargets.push_back(tar);
    return RenderTargets.size();
}
GLuint Scene::NewRenderTarget(string Name,GLuint ColorRes,GLuint Format,GLuint Program){
    RenderTarget tar={Name,0,{0,0,0,0},Program,ColorRes,0,Format,1.0,false,false,false,false};
    RenderTargets.push_back(tar);
    return RenderTargets.size();
}

void Scene::InitTargetUniform(std::vector<RenderTarget>::iterator A,RenderObject* RO){
   if(PostProcessed_Linked && 0<A->ShaderProgram<ShaderArray.size()){
        A->UniformAdress=RO->GetLocation(A->Name,RealProgram(A->ShaderProgram));
        A->UniformInitied=true;
   }
}
void Scene::InitTargetAmbivalentUniform(std::vector<RenderTarget>::iterator A,RenderObject* RO){
    if(PostProcessed_Linked && 0<A->ShaderProgram<ShaderArray.size()){
        A->UniformAdress=RO->GetAmbivalentLocation(A->Name,RealProgram(A->ShaderProgram),0,0);
        A->UniformInitied=true;
    }
};

void Scene::InitTarget(GLuint N,RenderObject* RO){
    if(N<RenderTargets.size()){
        std::vector<RenderTarget>::iterator A=RenderTargets.begin()+N;
        if(A->TextureType==0){
            A->BufferAdress=RO->AddBufferRule(A->ColorRes,A->Format);
        }else{
            A->BufferAdress=RO->AddBufferRule(A->ColorRes,A->TextureType,A->Format,A->Ratio);

        }
        A->BufferInitied=true;
    }

}
void Scene::InitDeclaredTarget(RenderObject* RO,bool force){
    ConsoleEcho("Init des Target");
    for(std::vector<RenderTarget>::iterator A=RenderTargets.begin();A<RenderTargets.end();A++){
        if(A->TextureType==0){
            A->BufferAdress=RO->AddBufferRule(A->ColorRes,A->Format);
        }else{
            A->BufferAdress=RO->AddBufferRule(A->ColorRes,A->TextureType,A->Format,A->Ratio);

        }
        A->BufferInitied=true;
    }
    SetTargetPlan(RO);
    ConsoleEcho("INit des target finis");
}

void Scene::SetTargetPlan(RenderObject* RO){
    GLuint* Plan=new GLuint[RenderTargets.size()];
    GLuint I=0;
    for(std::vector<RenderTarget>::iterator A=RenderTargets.begin();A<RenderTargets.end();A++){
        if(A->BufferInitied){
            Plan[I]=A->BufferAdress;
            I++;
        }
    }
    RO->SetBufferRule(Plan,I);
    delete Plan;
}
void Scene::SwapTargetToSource(RenderObject* RO,GLuint N,GLuint FirstTex){
    std::vector<RenderTarget>::iterator A=RenderTargets.begin()+N;
    if(A->BufferInitied && A->UniformInitied){
        RO->BindBufferToSlot(A->BufferAdress,FirstTex);
        glUniform1i(A->UniformAdress,FirstTex);
    }
}
void Scene::SwapFrameToSource(RenderObject* RO,GLuint FrameUniformAdress,GLuint Tex){
    RO->BindFrameToSlot(Tex);
    glUniform1i(FrameUniformAdress,Tex);
}
void Scene::SwapZBufferToSource(RenderObject* RO,GLuint ZUniformAdress,GLuint Tex){
    RO->BindZBufferToSlot(Tex);
    glUniform1i(ZUniformAdress,Tex);
}


void Scene::SwapAllTargetToSource(RenderObject* RO,GLuint FirstTex){
    GLuint I=FirstTex;
    for(std::vector<RenderTarget>::iterator A=RenderTargets.begin();A<RenderTargets.end();A++){
        if(!A->UniformInitied)InitTargetUniform(A,RO);
        if(A->BufferInitied && A->UniformInitied){
            RO->BindBufferToSlot(A->BufferAdress,I);
            glUniform1i(A->UniformAdress,I);
            I++;
        }
    }
}

void Scene::DeleteAllTarget(RenderObject* RO){
    for(std::vector<RenderTarget>::iterator A=RenderTargets.begin();A<RenderTargets.end();A++){
        if(A->BufferInitied)RO->DeleteBufferRule(A->BufferAdress);
    }
    RenderTargets.empty();
};
void Scene::UnitializeAllTarget(RenderObject* RO){
    for(std::vector<RenderTarget>::iterator A=RenderTargets.begin();A<RenderTargets.end();A++){
        if(A->BufferInitied)RO->DeleteBufferRule(A->BufferAdress);
    }
};

void Scene::Test(){
    ConsoleEcho("Test dans l'APC");
    //QueueUserAPC(CallSceneFunction,SceneThread,(ULONG_PTR)this);
}

//Scene APC Func
 void Test4(Scene* me,void* MSG){
    char* msg=(char*)MSG;
    me->Say(msg);
    Sleep(1000);
 }
