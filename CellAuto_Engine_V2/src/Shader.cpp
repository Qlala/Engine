#include "Header.h"
ShaderHandler::ShaderHandler(){
ShaderReady=false;
ShaderEnableCheck=true;
//AsyncMode=false;
ShaderDemandAnswered=CreateEvent(NULL, 0, 0, GenStrWithUniqueCode("ShaderDemandAnswered",false));
ShaderDemandSent=CreateEvent(NULL, 0, 0, GenStrWithUniqueCode("ShaderDemandSent"));
}
void ShaderHandler::CreatePipeline(){
    glGenProgramPipelines(1,&ProgramPipeline);
    GLPipelines.push_back({ProgramPipeline,false,true});

    CurrentPipelineIndex=0;
    ThreadId=GetCurrentThreadId();
    ShaderReady=true;
}
//Synchronous Gestion
int ShaderHandler::ShaderCheckDemand(){
    if(ShaderEnableCheck)
        if(WaitForSingleObject(ShaderDemandSent,ShaderCheckTime)==WAIT_OBJECT_0){
            switch(ShaderDemand){
            //addShader
            case(3):
                IntIndex=InternAddShader(StringArgument,GLuintArgument, IntIndex);
                ConsoleEcho("Un message recu");
                SetEvent(ShaderDemandAnswered);
                return IntIndex;
                break;
            //set active intern program (porgram stage)
            case(5):
                InternSetActiveInternProgram((GLuint*)IntArrayArgument,GLuintArrayArgument,IntIndex);
                SetEvent(ShaderDemandAnswered);
                return 0;
                break;
            case(4):
                InternDeleteProgram(GLuintArgument);
                SetEvent(ShaderDemandAnswered);
                return 0;
                break;
            //Uniform Settings
            case(6):
                memory_fence();
                InternSetUniform(GLuintArrayArgument[0],GLuintArrayArgument[1],PointerArgument,GLuintArrayArgument[2],StringArgument);
                SetEvent(ShaderDemandAnswered);
                return 0;
                break;
            case(7):
                memory_fence();
                InternSetUniform(GLuintArrayArgument[0],GLuintArrayArgument[1],PointerArgument,GLuintArrayArgument[2],GLuintArgument);
                SetEvent(ShaderDemandAnswered);
                return 0;
                break;
            case(8):
                memory_fence();
                GLuintArgument=InternGetLocation(StringArgument,GLuintArgument);
                SetEvent(ShaderDemandAnswered);
                return 0;
                break;
            case(9):
                memory_fence();
                AmbivalentUniform* AmbUni2;
                *AmbUni2=InternGetAmbivalentLocation(StringArgument,GLuintArgument,GLuintArrayArgument[0],GLuintArrayArgument[1]);
                PointerArgument=(void*)AmbUni2;
                SetEvent(ShaderDemandAnswered);
                return 0;
                break;
            case(10):
                memory_fence();
                GLuintArgument=InternCreateNewPipeline();
                return 0;
                break;
            case(11):
                memory_fence();
                GLuintArgument=InternSwapPipeline(GLuintArgument);
                return 0;
                break;
            case(12):
                memory_fence();
                InternDeletePipeline(GLuintArgument);
                return 0;
                break;
            case(2):
                memory_fence();
                InternShaderHandlerDestructor();
                SetEvent(ShaderDemandAnswered);
                return 0;

            }

        }
        Sleep(ShaderIdleTime);
}

void ShaderHandler::ActivatePipeline(){
    if(ShaderReady){
        glUseProgram(0);
        PipelineBound=true;
        glBindProgramPipeline(ProgramPipeline);
    }
}
void ShaderHandler::DesactivatePipeline(){
     if(ShaderReady){
        glUseProgram(0);
        PipelineBound=false;
        glBindProgramPipeline(0);
    }
}

void ShaderHandler::RestorePipelineState(){
    if(ShaderReady){
        if(PipelineBound){
            glUseProgram(0);
            glBindProgramPipeline(ProgramPipeline);
        }else{
            glUseProgram(0);
            glBindProgramPipeline(0);
        }
    }
}
bool ShaderHandler::ValidatePipeline(){
    int Status=GL_TRUE;
    ConsoleEcho("Validation Pipeline");
    glValidateProgramPipeline(ProgramPipeline);
    glGetProgramPipelineiv(ProgramPipeline,GL_VALIDATE_STATUS,&Status);
    if(Status==GL_TRUE){
        ConsoleEcho("Validation réussi");
        return true;
    }else{
        int ErrorLength=0;
        glGetProgramPipelineiv(ProgramPipeline,GL_INFO_LOG_LENGTH,&ErrorLength);
        //glGetProgramInfoLog(ProgramList[PrgAdress[i]],0,&ErrorLength,0);
        char * Error= new char[ErrorLength+30];
        char * FullError=new char[ErrorLength+30+100];
        glGetProgramPipelineInfoLog(ProgramPipeline,ErrorLength+30,0,Error);
        FullError[0]='\0';
        FullError=strcat(FullError,"Error Pipeline: \0");
        FullError=strcat(FullError,Error);
        ConsoleEcho(FullError);
        delete Error;
        delete FullError;
        return false;
    }
}

bool ShaderHandler::ValidateProgram(GLuint programA){
    if(programA<GLPrograms.size()){
        if(!GLPrograms[programA].ValidationState){
                    int Status1=GL_TRUE;
                    int Status2=GL_TRUE;
                    NoticeGlError(true);
                    ConsoleEcho("Validation et Linking Program:%i",programA);

                    glUseProgram(0);
                    glLinkProgram(GLPrograms[programA].Prg);
                    NoticeGlError(true);
                    glGetProgramiv(GLPrograms[programA].Prg,GL_LINK_STATUS,&Status1);
                    NoticeGlError(true);
                    if(Status1==GL_TRUE){
                        //Test du Program
                        glValidateProgram(GLPrograms[programA].Prg);
                        glGetProgramiv(GLPrograms[programA].Prg,GL_VALIDATE_STATUS,&Status2);
                    }
                    if(Status1==GL_FALSE or Status2==GL_FALSE){
                        if(Status1==GL_FALSE)ConsoleEcho("Lin Error");
                        else ConsoleEcho("Validation error");
                        int ErrorLength=0;
                        glGetProgramiv(GLPrograms[programA].Prg,GL_INFO_LOG_LENGTH,&ErrorLength);
                        //glGetProgramInfoLog(ProgramList[PrgAdress[i]],0,&ErrorLength,0);
                        char * Error= new char[ErrorLength+30];
                        char * FullError=new char[ErrorLength+30+50*GLPrograms[programA].ShaderAttachedType.size()];
                        char programdetail[10];
                        FullError[0]='\0';
                        FullError=strcat(FullError,"Error Prg: \0");

                        glGetProgramInfoLog(GLPrograms[programA].Prg,ErrorLength+30,0,Error);
                        FullError=strcat(FullError,Error);

                        FullError=strcat(FullError,"\n detail du program:");
                        FullError=strcat(FullError,"\n numéro:");
                        sprintf(programdetail,"%i",programA);
                        FullError=strcat(FullError,programdetail);
                        FullError=strcat(FullError,"\n Type:");
                        for(std::vector<int>::iterator A=GLPrograms[programA].ShaderAttachedType.begin();A!=GLPrograms[programA].ShaderAttachedType.end();A++){
                        sprintf(programdetail,"%x\n",*A);
                        FullError=strcat(FullError,programdetail);
                        }
                        ConsoleEcho(FullError);
                        NoticeError();
                        delete Error;
                        delete FullError;
                        GLPrograms[programA].ValidationState=false;
                        return false;
                    }else{
                        GLPrograms[programA].ValidationState=true;
                        return true;
                    }
        }else{
                return true;
        }
    }else{
        return false;
    }
}

void ShaderHandler::InternBindMonoProgram(GLuint PrgAdress){
    ShaderProgramSet=true;
    glBindProgramPipeline(0);
    glLinkProgram(GLPrograms[PrgAdress].Prg);
    if(ValidateProgram(PrgAdress))
        glUseProgram(GLPrograms[PrgAdress].Prg);


}
GLuint ShaderHandler::CreateNewProgram(){
    ConsoleEcho("Création du Program");
    if(ShaderReady){
        GLuint NewProgram=glCreateProgram();
        glProgramParameteri(NewProgram,GL_PROGRAM_SEPARABLE,true);
        ConsoleEcho("Tout nouveau prg");
        GLProgram NewPRG=GLProgram(NewProgram);
        NewPRG.Allocated=true;
        GLuint i=0;
        std::vector<GLProgram>::iterator Itt;
        for(Itt=GLPrograms.begin();Itt!=GLPrograms.end();Itt++){
            if(!Itt->Allocated)break;
            i++;
        }
        if(Itt!=GLPrograms.end()){
            *Itt=NewPRG;
            return i;
        }else{
            GLPrograms.push_back(NewPRG);
            return GLPrograms.size()-1;
        }

    }
}
bool ShaderHandler::InternDeleteProgram(GLuint n){
    if(ShaderReady){
        try{
        if(n<GLPrograms.size()){
            std::vector<GLProgram>::iterator Itt=GLPrograms.begin();
            Itt+=n;
            glDeleteProgram(Itt->Prg);
            Itt->DeleteAllShader();
            Itt->Allocated=false;
            if(n==GLPrograms.size()-1){
                while(GLPrograms.back().Allocated==false){
                    GLPrograms.erase(GLPrograms.end()-1);
                }
            }
            //GLPrograms.erase(Itt);

        }else{

        }
        }catch(std::runtime_error Err){
            ConsoleEcho((char*)Err.what());
            return false;
        }
    }
}
bool ShaderHandler::DeleteProgram(GLuint n){
    DWORD Id=GetCurrentThreadId();
    if(Id==ThreadId){
       return InternDeleteProgram(n);
    }else{
        return ExternDeleteProgram(n);
    }
}


void ShaderHandler::ExternSetActiveInternProgram(GLuint * PrgAdress,GLuint* PrgType,int n){
    if(ShaderReady){
        ShaderDemand=5;
        IntArrayArgument=(int*)PrgAdress;
        GLuintArrayArgument=PrgType;
        IntIndex=n;
        SetEvent(ShaderDemandSent);
        WaitForSingleObject(ShaderDemandAnswered,INFINITE);
    }
}
void ShaderHandler::SetActiveInternProgram(GLuint * PrgAdress,GLuint* PrgType,int n){
    DWORD Id=GetCurrentThreadId();
    if(Id==ThreadId){
        InternSetActiveInternProgram(PrgAdress,PrgType,n);
    }else{
        ExternSetActiveInternProgram(PrgAdress,PrgType,n);
    }
}

bool ShaderHandler::ExternDeleteProgram(GLuint n){
        GLuintArgument=n;
        SetEvent(ShaderDemandSent);
        WaitForSingleObject(ShaderDemandAnswered,INFINITE);
}
bool ShaderHandler::ExternSetUniform(GLuint Type,GLuint Count,volatile void* Pointer,GLuint StageAdress,string UniformName){
    GLuintArrayArgument=new GLuint[3];
    GLuintArrayArgument[0]=Type;
    GLuintArrayArgument[1]=Count;
    GLuintArrayArgument[2]=StageAdress;
    PointerArgument=Pointer;
    ShaderDemand=6;
    StringArgument=UniformName;
    SetEvent(ShaderDemandSent);
    memory_fence();
    WaitForSingleObject(ShaderDemandAnswered,INFINITE);
    delete GLuintArrayArgument;
}
bool ShaderHandler::SetUniform(GLuint Type,GLuint Count,volatile void* Pointer,GLuint StageAdress,string UniformName){
    DWORD Id=GetCurrentThreadId();
    if(Id==ThreadId){
        return InternSetUniform(Type,Count,Pointer,StageAdress,UniformName);
    }else{
        return ExternSetUniform(Type,Count,Pointer,StageAdress,UniformName);
    }
}

bool ShaderHandler::ExternSetUniform(GLuint Type,GLuint Count,volatile void* Pointer,GLuint StageAdress,GLuint Location){
    GLuintArrayArgument=new GLuint[3];
    GLuintArrayArgument[0]=Type;
    GLuintArrayArgument[1]=Count;
    GLuintArrayArgument[2]=StageAdress;
    PointerArgument=Pointer;
    ShaderDemand=7;
    GLuintArgument=Location;
    SetEvent(ShaderDemandSent);
    memory_fence();
    WaitForSingleObject(ShaderDemandAnswered,INFINITE);
    delete GLuintArrayArgument;
}
//bool ShaderHandler::InternMonoUniformSet(GLuint Type,GLuint Count,)
bool ShaderHandler::SetUniform(GLuint Type,GLuint Count,volatile void* Pointer,GLuint StageAdress,GLuint Location){
    DWORD Id=GetCurrentThreadId();
    if(Id==ThreadId){
        return InternSetUniform(Type,Count,Pointer,StageAdress,Location);
    }else{
        return ExternSetUniform(Type,Count,Pointer,StageAdress,Location);
    }
}

bool ShaderHandler::InternSetUniform(GLuint Type,GLuint Count,volatile void* Pointer,GLuint StageAdress,string UniformName){
    glBindProgramPipeline(ProgramPipeline);
    memory_fence();
    GLuint Location;
    //float tes[1]={1.0f};
    //std::cout<<Adress<<std::endl;
    //std::cout<<Adress<<std::endl;
    //glLinkProgram(ProgramList[StageAdress]);
    //glUseProgram(ProgramList[StageAdress]);
    ConsoleEcho(UniformName.c_str());
    ConsoleEcho("Stage Adress %i",StageAdress);
    Location=glGetUniformLocation(GLPrograms[StageAdress].Prg,UniformName.data());
    ConsoleEcho("uniform cation %i",Location);
    switch(Type){
        case(GL_FLOAT):
            std::cout<<*(float*)Pointer<<std::endl;
            //std::cout<<0.1<<std::endl;
            //glUniform1f(glGetUniformLocation(ProgramList[0],"ColorTest"),1.0f);
            glUniform1fv(Location,Count,(const float*)Pointer);
            break;
        case(GL_INT):
            glUniform1iv(Location,Count,(const int*)Pointer);
            break;
        case(GL_UNSIGNED_INT):
            glUniform1uiv(Location,Count,(GLuint*)Pointer);
            break;
        case(GL_DOUBLE):
            glUniform1dv(Location,Count,(GLdouble*)Pointer);

        }
    RestorePipelineState();
  //  glUseProgram(NULL);

}

bool ShaderHandler::InternSetUniform(GLuint Type,GLuint Count,volatile void* Pointer,GLuint StageAdress,GLuint Location){
    glBindProgramPipeline(ProgramPipeline);
    memory_fence();
    switch(Type){
        case(GL_FLOAT):
            std::cout<<*(float*)Pointer<<std::endl;
            //std::cout<<0.1<<std::endl;
            //glUniform1f(glGetUniformLocation(ProgramList[0],"ColorTest"),1.0f);
            glUniform1fv(Location,Count,(const float*)Pointer);
            break;
        case(GL_INT):
            glUniform1iv(Location,Count,(const int*)Pointer);
            break;
        case(GL_UNSIGNED_INT):
            glUniform1uiv(Location,Count,(GLuint*)Pointer);
            break;
        case(GL_DOUBLE):
            glUniform1dv(Location,Count,(GLdouble*)Pointer);

        }

  //  glUseProgram(NULL);
    RestorePipelineState();
}

void ShaderHandler::InternSetActiveInternProgram(GLuint * PrgAdress,GLuint* PrgType,int n){
        ConsoleEcho("Mise en place du prg");
        ShaderProgramSet=true;
        ConsoleEcho("Activation du Pipeline");
        ActivatePipeline();
        for(int i=0;i<n;i++){
                if(ValidateProgram(PrgAdress[i])){
                    ConsoleEcho("mise en places des Stage de rendu");
                    glUseProgramStages(ProgramPipeline,PrgType[i],GLPrograms[PrgAdress[i]].Prg);
                    //NoticeGlError(true);
                    glActiveShaderProgram(ProgramPipeline,GLPrograms[PrgAdress[i]].Prg);
                    //NoticeGlError(true);
                    //glUseProgram(ProgramList[PrgAdress[i]]);
                }else{
                    ShaderProgramSet=false;
                }
        }

         if(!ShaderProgramSet || !ValidatePipeline()){
                ConsoleEcho("Erreur d'intialisation du Program");
                ShaderProgramSet=false;
                for(int i=0;i<n;i++){
                    ConsoleEcho("Program: %i",i);
                    ConsoleEcho("AdressSoft:%i",PrgAdress[i]);
                    ConsoleEcho("AdressHard:%i",GLPrograms[PrgAdress[i]].Prg);
                    ConsoleEcho("Type:%i",PrgType[i]);

                }
         }else{
            ConsoleEcho("Succes de l'activation du Program Intern");
         }
}
GLuint ShaderHandler::GetLocation(string Name,GLuint PrgAdress){
    DWORD Id=GetCurrentThreadId();
    if(Id==ThreadId){
        return InternGetLocation(Name,PrgAdress);
    }else{
        return ExternGetLocation(Name,PrgAdress);
    }
}

GLuint ShaderHandler::ExternGetLocation(string Name,GLuint PrgAdress){
    if(ShaderReady && PrgAdress<GLPrograms.size()){
       if(GLPrograms[PrgAdress].ValidationState){
            //code de demande
            ShaderDemand=8;
            StringArgument=Name;
            GLuintArgument=PrgAdress;
            //on envoit la demande
            SetEvent(ShaderDemandSent);
            WaitForSingleObject(ShaderDemandAnswered,INFINITE);
            return GLuintArgument;
       }else{
        return 0;
       }
    }else{
        return 0;
    }

}

GLuint ShaderHandler::InternGetLocation(string Name,GLuint PrgAdress){
    if(ShaderReady && PrgAdress<GLPrograms.size()){

        return GLPrograms[PrgAdress].ValidationState?glGetUniformLocation(GLPrograms[PrgAdress].Prg,Name.c_str()):0;
    }else return 0;
}

AmbivalentUniform ShaderHandler::GetAmbivalentLocation(string Name,GLuint PrgAdress,GLuint Type1,GLuint Type2){
    DWORD Id=GetCurrentThreadId();
    if(Id==ThreadId){
        return InternGetAmbivalentLocation(Name,PrgAdress,Type1,Type2);
    }else{
        return ExternGetAmbivalentLocation(Name,PrgAdress,Type1,Type2);
    }
}

AmbivalentUniform ShaderHandler::ExternGetAmbivalentLocation(string Name,GLuint PrgAdress,GLuint Type1,GLuint Type2){
    if(ShaderReady && PrgAdress<GLPrograms.size()){
       if(GLPrograms[PrgAdress].ValidationState){
        //code de demande
        ShaderDemand=9;
        StringArgument=Name;
        PointerArgument=(void*)(new AmbivalentUniform);
        GLuintArgument=PrgAdress;
        GLuintArrayArgument=new GLuint[2];
        GLuintArrayArgument[0]=Type1;
        GLuintArrayArgument[1]=Type2;
        //on envoit la demande
        SetEvent(ShaderDemandSent);
        WaitForSingleObject(ShaderDemandAnswered,INFINITE);
        delete GLuintArrayArgument;

        return *((AmbivalentUniform*)PointerArgument);
        delete PointerArgument;
       }else{
            return {0,0,0,0};
       }
    }else{
        return {0,0,0,0};
    }
}
AmbivalentUniform ShaderHandler::InternGetAmbivalentLocation(string Name,GLuint PrgAdress,GLuint Type1,GLuint Type2){
    if(ShaderReady && PrgAdress<GLPrograms.size()){
        if(GLPrograms[PrgAdress].ValidationState){
            AmbivalentUniform AmbUni;
            AmbUni.NormalType=Type1;
            AmbUni.MSType=Type2;
            AmbUni.Normal=glGetUniformLocation(GLPrograms[PrgAdress].Prg,Name.c_str());
            AmbUni.MultiSampled=glGetUniformLocation(GLPrograms[PrgAdress].Prg,(Name+MultiSampTag).c_str());
            if(AmbUni.Normal==-1){
                ConsoleEcho(("Erreur dans la Recherche d'un AmbUni => Normal Non trouvé :"+Name).c_str());
            }
            if(AmbUni.MultiSampled==-1){
                ConsoleEcho(("Erreur dans la Recherche d'un AmbUni => MultiS Non trouvé :"+Name+MultiSampTag).c_str());
            }
            return AmbUni;
        }else{
            return {0,0,0,0};
        }
    }else return {0,0,0,0};
}

GLuint ShaderHandler::CreateNewPipeline(){
    DWORD Id=GetCurrentThreadId();
    if(Id==ThreadId){
        return InternCreateNewPipeline();
    }else{
        return ExternCreateNewPipeline();
    }
}



GLuint ShaderHandler::InternCreateNewPipeline(){
    if(ShaderReady){
        GLuint newpipe;
        glGenProgramPipelines(1,&newpipe);
        std::vector<GLPipeline>::iterator Itt;
        GLuint i=0;
        for(Itt=GLPipelines.begin();Itt!=GLPipelines.end();Itt++){
            if(!Itt->Allocated)break;
            i++;
        }
        if(Itt!=GLPipelines.end()){
            *Itt=GLPipeline(newpipe,false);
            Itt->Allocated=true;
            return i;
        }else{
            GLPipelines.push_back(GLPipeline(newpipe,false,true));
            return GLPipelines.size()-1;
        }

    }else{
        return -1;
    }
}

GLuint ShaderHandler::ExternCreateNewPipeline(){
    if(ShaderReady){
        GLuintArgument=0;
        ShaderDemand=10;
        SetEvent(ShaderDemandSent);
        WaitForSingleObject(ShaderDemandAnswered,INFINITE);
        return GLuintArgument;
    }else{
        return -1;
    }
}


GLuint ShaderHandler::SwapPipeline(GLuint n){
    DWORD Id=GetCurrentThreadId();
    if(Id==ThreadId){
        return InternSwapPipeline(n);
    }else{
        return ExternSwapPipeline(n);
    }
}
GLuint ShaderHandler::ExternSwapPipeline(GLuint n){
    GLuintArgument=n;
    ShaderDemand=11;
    SetEvent(ShaderDemandSent);
    WaitForSingleObject(ShaderDemandAnswered,INFINITE);
    return GLuintArgument;
}

GLuint ShaderHandler::InternSwapPipeline(GLuint n){
    if(ShaderReady && 0<=n<GLPipelines.size() &&n!=CurrentPipelineIndex){
        DesactivatePipeline();
        GLPipelines[CurrentPipelineIndex].ProgramSet=ShaderProgramSet;
        ProgramPipeline=GLPipelines[n].Pipeline;
        ShaderProgramSet=GLPipelines[n].ProgramSet;
        CurrentPipelineIndex=n;
        return n;
    }else{
        return CurrentPipelineIndex;
    }
}
void ShaderHandler::DeletePipeline(GLuint n){
    DWORD Id=GetCurrentThreadId();
    if(Id==ThreadId){
        return InternDeletePipeline(n);
    }else{
        return ExternDeletePipeline(n);
    }
}
void ShaderHandler::ExternDeletePipeline(GLuint n){
    ShaderDemand=12;
    GLuintArgument=n;
    SetEvent(ShaderDemandSent);
    WaitForSingleObject(ShaderDemandAnswered,INFINITE);
}


void ShaderHandler::InternDeletePipeline(GLuint n){
    if(ShaderReady && GLPipelines.size()>1 && GLPipelines.size()>n){
        if(CurrentPipelineIndex==n){//si le pipeline est déja l'actuelle on le change
            SwapPipeline(0);
        }
        glDeleteProgramPipelines(1,&(GLPipelines[n].Pipeline));
        GLPipelines[n].Allocated=false;
        if(n==GLPipelines.size()-1){
            while((GLPipelines.end()-1)->Allocated==false){
                GLPipelines.erase(GLPipelines.end()-1);
            }
        }
    }

}


int ShaderHandler::ExternAddShader(string FileName,GLuint Type,int ProgramIndex){
ConsoleEcho("un message envoyé");
if(ShaderReady){
    //Code pour la Demande
    ShaderDemand=3;
    StringArgument=FileName;
    GLuintArgument=Type;
    IntIndex=ProgramIndex;
    SetEvent(ShaderDemandSent);
    WaitForSingleObject(ShaderDemandAnswered,INFINITE);
    //ConsoleEcho("On me répond");
    return IntIndex;
    }
}

int ShaderHandler::AddShader(string FileName,GLuint Type,int ProgramIndex){
    DWORD Id=GetCurrentThreadId();
    if(Id==ThreadId){
        return InternAddShader(FileName,Type,ProgramIndex);
    }else{
        return ExternAddShader(FileName,Type,ProgramIndex);
    }
}


int ShaderHandler::InternAddShader(string FileName,GLuint Type,int ProgramIndex){
if(ShaderReady){

    try{
        int Size;
        FILE* ShadderFile;
        char * SourceCode;

        ShadderFile = fopen(FileName.c_str(), "r");
        if(ShadderFile == NULL){
            char buff[255];
            strcpy(buff,"AddShader::error file = ");
            strcat(buff,FileName.c_str());
            throw std::runtime_error(buff);

        }
        fseek(ShadderFile, 0, SEEK_END);
        Size = ftell(ShadderFile);
        rewind(ShadderFile);
        SourceCode = new char[Size+1];
        memset(SourceCode,0,Size+1);
        if(SourceCode == NULL){
            throw std::runtime_error("Allocation");
            fclose(ShadderFile);
        }
        fread(SourceCode,Size,1,ShadderFile);
        SourceCode[Size]= '\0';
        const char ** OpenedSourceCode=(const GLchar**)"";
        OpenedSourceCode=(const GLchar**)&SourceCode;
        fclose(ShadderFile);
        GLuint Shader = glCreateShader(Type);
        glShaderSource(Shader,1,OpenedSourceCode,NULL);
        glCompileShader(Shader);
        GLint Status;
        glGetShaderiv(Shader,GL_COMPILE_STATUS,&Status);
        if(Status==GL_FALSE){
            string FullError;
            string NameBuff;
            string ErrorBuff;
            int ErrorLength;
            glGetShaderiv(Shader,GL_INFO_LOG_LENGTH,&ErrorLength);
            char * ErrorA= new char[ErrorLength];
            glGetShaderInfoLog(Shader,ErrorLength,&ErrorLength,ErrorA);
            FullError="error compilation du fichier :";
            NameBuff=FileName;
            ErrorBuff=ErrorA;
            FullError+=NameBuff;
            FullError+=ErrorBuff;
            delete ErrorA;
            std::runtime_error ERR =std::runtime_error(FullError);
            throw ERR;
        }
        string Test="Reussite de la compilation:";
        Test+=(string)FileName;
        ConsoleEcho(Test.c_str());
        ConsoleEcho("Shader de type:%x",Type);
        //si il met 0 ???? on fait un nouveau programme
        if(ProgramIndex<0){
            GLuint Index=CreateNewProgram();
            glAttachShader(GLPrograms[Index].Prg,Shader);
            GLPrograms[Index].ShaderAttachedList.push_back(Shader);
            GLPrograms[Index].ShaderAttachedType.push_back(Type);
            ConsoleEcho("Envoyé au num:%i",Index);
            return Index;
        }else{
            ConsoleEcho("Envoyé au num:%i",ProgramIndex);
            glAttachShader(GLPrograms[ProgramIndex].Prg,Shader);
            GLPrograms[ProgramIndex].ShaderAttachedList.push_back(Shader);
            GLPrograms[ProgramIndex].ShaderAttachedType.push_back(Type);
            return ProgramIndex;
        }
        delete SourceCode;
        }catch(std::runtime_error A){
            ConsoleEcho((char*)A.what());
            return -1;
        }
    }
}
ShaderHandler::~ShaderHandler(){
    DWORD Id=GetCurrentThreadId();
    if(ThreadId==Id){
        InternShaderHandlerDestructor();
    }else{
        ExternShaderHandlerDestructor();
    }
}

void ShaderHandler::ExternShaderHandlerDestructor(){
    if(ShaderReady){
        ShaderDemand=2;
        SetEvent(ShaderDemandSent);
        ConsoleEcho("J'essaye de te detruire");
        WaitForSingleObject(ShaderDemandAnswered,INFINITE);
        ConsoleEcho("J'y suis arrivé");
    }
}

void ShaderHandler::InternShaderHandlerDestructor(){
    if(ShaderReady){
    std::cout<<ProgramPipeline<<std::endl;
    glBindProgramPipeline(0);
    glDeleteProgramPipelines(1,&ProgramPipeline);
    ConsoleEcho("je détuit tout");
    for(GLuint i=0;i<GLPrograms.size();i++){
        std::cout<<i<<std::endl;
        glDeleteProgram(GLPrograms[i].Prg);
        GLPrograms[i].DeleteAllShader();
    }
    for(std::vector<GLPipeline>::iterator Itt=GLPipelines.begin();Itt!=GLPipelines.end();Itt++){
        glDeleteProgramPipelines(1,&(Itt->Pipeline));
    }
    ShaderReady=false;
    //RestorePipelineState();
    }
//delete ProgramList;
//delete ShaderAttachedList;
//delete ShaderAttachedType;
}

GLuint LoadShader(unsigned int Type,char* FileName, GLuint Program){
	try{
	int Size;
	FILE* ShadderFile;
	char * SourceCode;

	ShadderFile = fopen(FileName, "r");
	if(ShadderFile == NULL) throw "error file";
	fseek(ShadderFile, 0, SEEK_END);
    Size = ftell(ShadderFile);
	rewind(ShadderFile);
	SourceCode = new char[Size+1];
	memset(SourceCode,0,Size+1);
	if(SourceCode == NULL){
		throw"Allocation";
		fclose(ShadderFile);
	}
	fread(SourceCode,Size,1,ShadderFile);
	SourceCode[Size]= '/0';
	const char ** OpenedSourceCode=(const GLchar**)"";
	OpenedSourceCode=(const GLchar**)&SourceCode;
	fclose(ShadderFile);
	GLuint Shader = glCreateShader(Type);
	glShaderSource(Shader,1,OpenedSourceCode,NULL);
	glCompileShader(Shader);
	GLint Status;
	glGetShaderiv(Shader,GL_COMPILE_STATUS,&Status);
	if(Status==GL_FALSE){
		string FullError;
		string NameBuff;
		string ErrorBuff;
		int ErrorLength;
		glGetShaderiv(Shader,GL_INFO_LOG_LENGTH,&ErrorLength);
		char * ErrorA= new char[ErrorLength];
		glGetShaderInfoLog(Shader,ErrorLength,&ErrorLength,ErrorA);
		FullError="error compilation du fichier :";
		NameBuff=FileName;
		ErrorBuff=ErrorA;
		FullError+=NameBuff;
		FullError+=ErrorBuff;
		delete ErrorA;
		std::runtime_error ERR =std::runtime_error(FullError);
		delete SourceCode;
		throw ERR;
	}
	string Test="Reussite de la compilation:";
	Test+=(string)FileName;
	ConsoleEcho(Test.c_str());
	glAttachShader(Program,Shader);
	delete SourceCode;
	return Shader;
	}catch(std::runtime_error A){
		ConsoleEcho((char*)A.what());
		return 0;
	}

}
