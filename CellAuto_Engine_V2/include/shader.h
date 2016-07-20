#define MultiSampTag "_MS"
#pragma once
#include "Header.h"
extern GLuint LoadShader(unsigned int Type,char* FileName, GLuint Program);
union GL_Uniform{
int I;
unsigned int Ui;
unsigned char B;
float F;
double D;
};
struct GLProgram{
    GLuint Prg;
    bool ValidationState=false;
    bool Allocated=false;
    std::vector<GLuint> ShaderAttachedList;
    std::vector<int> ShaderAttachedType;
    GLProgram(GLuint prg,GLuint shader,GLuint type){
        Prg=prg;
        ShaderAttachedList.push_back(shader);
        ShaderAttachedType.push_back(type);
        ValidationState=false;
    }
    GLProgram(GLuint prg){
    Prg=prg;
    }
    void DeleteAllShader(){
        for(std::vector<GLuint>::iterator A=ShaderAttachedList.begin();A!=ShaderAttachedList.end();A++){
            glDeleteShader((*A));
        }
    }

};
struct GLPipeline{
    GLuint Pipeline;
    bool ProgramSet;
    bool Allocated=false;
    GLPipeline(GLuint pipe,bool set):Pipeline(pipe),ProgramSet(set){};
    GLPipeline(GLuint pipe,bool set,bool alloc):Pipeline(pipe),ProgramSet(set),Allocated(alloc){};
};



struct AmbivalentUniform{
    GLuint NormalType;
    GLuint MSType;
    GLuint Normal;
    GLuint MultiSampled;
    GLuint ChooseType(GLuint Type){
        if(MSType==Type){
            return MultiSampled;
        }else if(NormalType==Type){
            return Normal;
        }else{
            return 0;
        }
    }
    operator GLuint(){
        return NormalType;
    }
    operator GLuint*(){
        return &NormalType;
    }
    AmbivalentUniform operator =(GLuint A){
        NormalType=A;
        return *this;
    }
};

class ShaderHandler{
    //optimisation
    int ShaderIdleTime=0;
    int ShaderCheckTime=0;
    bool ShaderEnableCheck;
    //fonctionnement
    GLuint ProgramPipeline;
    GLuint CurrentPipelineIndex;
    std::vector<GLProgram> GLPrograms;
    std::vector<GLPipeline> GLPipelines;
    //ThreadMessage
    DWORD ThreadId;
    HANDLE ShaderDemandSent;
    HANDLE ShaderDemandAnswered;
    int ShaderDemand;
    string StringArgument;
    GLuint GLuintArgument;
    int IntIndex;
    int* IntArrayArgument;
    GLuint* GLuintArrayArgument;
    volatile void * PointerArgument;


    //bool d'etat
    bool ShaderReady=false;
    bool Activated=true;
    bool ShaderProgramSet=false;
    bool PipelineBound=false;
    //static thread_local bool AsyncMode;
        public:
        //verifie que le program peut tourner
        inline void ShaderChangeCheckMode(bool Enable){
            if(WaitForSingleObject(ShaderDemandSent,ShaderCheckTime)!= WAIT_OBJECT_0) ShaderEnableCheck=false;};

        ShaderHandler();
        void CreatePipeline();
        int ShaderCheckDemand();
        inline bool IsShaderReady(){return ShaderReady;}
        inline bool IsShaderProgramSet(){return ShaderProgramSet;}
        void ActivatePipeline();
        void DesactivatePipeline();
        void RestorePipelineState();
        GLuint CreateNewProgram();
        bool ValidateProgram(GLuint programadress);
        bool ValidatePipeline();


        //Code = 2
        ~ShaderHandler();
        void ExternShaderHandlerDestructor();
        void InternShaderHandlerDestructor();
        //Code = 4
        bool DeleteProgram(GLuint n);
        bool ExternDeleteProgram(GLuint n);
        bool InternDeleteProgram(GLuint n);
        //Code = 3
        int AddShader(string filename,GLuint ShaderType,int ProgramIndex=-1);
        int InternAddShader(string filename,GLuint ShaderType,int ProgramIndex=-1);
        int ExternAddShader(string filename,GLuint ShaderType,int ProgramIndex=-1);
        //Code 4
        void BindMonoProgram(GLuint PrgAdress);
        void ExternBindMonoProgram(GLuint PrgAdress);
        void InternBindMonoProgram(GLuint PrgAdress);
        //N adrress dans le tableau ,PrgAdress list de numero d'adress,PrgType : GL_***_BIT assemblage d'enum en tableau pour chaque programe
        //Code = 5
        void SetActiveInternProgram(GLuint* PrgAdress,GLuint* PrgType,int n);
        void ExternSetActiveInternProgram(GLuint* PrgAdress,GLuint* PrgType,int n);
        void InternSetActiveInternProgram(GLuint* PrgAdress,GLuint* PrgType,int n);
        //Code = 6
        bool SetUniform(GLuint Type, GLuint Count,volatile void *Pointer,GLuint StageAdress,string UniformName="");
        bool InternSetUniform(GLuint Type, GLuint Count,volatile void *Pointer,GLuint StageAdress,string UniformName="");
        bool ExternSetUniform(GLuint Type, GLuint Count,volatile void* Pointer,GLuint StageAdress,string UniformName="");
        //Code = 7
        bool SetUniform(GLuint Type, GLuint Count,volatile void* Pointer,GLuint StageAdress,GLuint Adress=-1);
        bool ExternSetUniform(GLuint Type, GLuint Count,volatile void* Pointer,GLuint StageAdress,GLuint Adress=-1);
        bool InternSetUniform(GLuint Type, GLuint Count,volatile void *Pointer,GLuint StageAdress,GLuint Adress=-1);
        //Code =8
        GLuint GetLocation(string Name,GLuint PrgAdress);
        GLuint ExternGetLocation(string Name,GLuint PrgAdress);
        GLuint InternGetLocation(string Name,GLuint PrgAdress);
        //Code = 9
        AmbivalentUniform GetAmbivalentLocation(string Name,GLuint PrgAdress,GLuint NType,GLuint MsType);
        AmbivalentUniform ExternGetAmbivalentLocation(string Name,GLuint PrgAdress,GLuint Type1,GLuint Type2);
        AmbivalentUniform InternGetAmbivalentLocation(string Name,GLuint PrgAdress,GLuint Type1,GLuint Type2);
        //Code =10
        GLuint CreateNewPipeline();
        GLuint ExternCreateNewPipeline();
        GLuint InternCreateNewPipeline();
        //Code = 11
        GLuint SwapPipeline(GLuint n);
        GLuint ExternSwapPipeline(GLuint n);
        GLuint InternSwapPipeline(GLuint n);
        //Code = 12
        void DeletePipeline(GLuint n);//ne peut pas detuire le dernier pipeline
        void ExternDeletePipeline(GLuint n);
        void InternDeletePipeline(GLuint n);
    protected:
    private:

};
