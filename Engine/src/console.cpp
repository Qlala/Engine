
#include "LibHeader.h"
#define CONSOLE_ECHO
//zero therminating char*
HANDLE consoleInit;
HANDLE logFileOpened;
FILE* LogFile;
//bool LogFileOpened=false;
//bool ConsoleInit=false;
time_t rawtime;
struct tm * timeinfo;
clock_t tick;
int UniqueCode=0;
char buff3[255];
HANDLE consoleThread;
DWORD consoleThreadId;
string GLDebugSourceDecode(GLenum source){
    switch(source){
        case(GL_DEBUG_SOURCE_API):
            return "DBG_Source:API";
        break;
        case(GL_DEBUG_SOURCE_SHADER_COMPILER):
            return "DBG_Source:Shader_Compiler";
        break;
        case(GL_DEBUG_SOURCE_WINDOW_SYSTEM):
            return "DBG_Source:Window_System(WGL/GLX)";
        break;
        case(GL_DEBUG_SOURCE_THIRD_PARTY):
            return "DBG_Source:Third_Party";
        break;
        case(GL_DEBUG_SOURCE_APPLICATION):
            return "DBG_Source:This_Application";
        break;
        case(GL_DEBUG_SOURCE_OTHER):
            return "DBG_Source:Other";
        break;
        default:
            return "DBG_Source:Unknown";

    }

}

string GLDebugTypeDecode(GLenum type){
    switch(type){
        case(GL_DEBUG_TYPE_ERROR):
           return "DBG_Type:Error";
        break;
        case(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):
            return "DBG_Type:Deprecated_Behavior";
        break;
        case(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):
            return "DBG_Type:Undefined_Behavior";
        break;
        case(GL_DEBUG_TYPE_PERFORMANCE):
            return "DBG_Type:Performance";
        break;
        case(GL_DEBUG_TYPE_PORTABILITY):
            return "DBG_Type:Highly_Vendor_Specific";
        break;
        case(GL_DEBUG_TYPE_MARKER):
            return "DBG_Type:Marker";
        break;
        case(GL_DEBUG_TYPE_PUSH_GROUP):
            return "DBG_Type:Entering a debug group";
        break;
        case(GL_DEBUG_TYPE_POP_GROUP):
            return "DBG_Type:Leaving a debug group";
        break;
        case(GL_DEBUG_TYPE_OTHER):
            return "DBG_Type:Other";
        break;
        default:
            return "DBG_Type:Unknown";
    }


}
string GLDebugSeverityDecode(GLenum severity){
    switch(severity){
        case(GL_DEBUG_SEVERITY_HIGH):
            return "Severity:High";
        break;
        case(GL_DEBUG_SEVERITY_MEDIUM):
            return "Severity:Medium";
        break;
        case(GL_DEBUG_SEVERITY_LOW):
            return "Severity:Low";
        break;
        case(GL_DEBUG_SEVERITY_NOTIFICATION):
            return "Severity:Notification";
        break;
        default:
            return "Severity:Unknown";
    }
}








const char* GenStrWithUniqueCode(const char* Format,bool increment){
    strcpy(buff3,Format);
    strcat(buff3,"%i");
    sprintf(buff3,buff3,UniqueCode);
    if(increment)UniqueCode++;
    return buff3;
}

DWORD WINAPI logthreadfunc(LPVOID nil){
    SetEvent(consoleInit);
    std::cout<<"FileCreation"<<std::endl;
    LogFile=fopen("Log.txt","w");
    if (LogFile!=NULL){
            std::cout<<"FileCreated"<<std::endl;
            //memory_fence();
            SetEvent(logFileOpened);
    }

    SleepEx(INFINITE,true);
    return 0;
}
void initConsole(){
    std::cout<<"INIT CONSOLE EN COURS"<<std::endl;
    logFileOpened=CreateEvent(NULL,true,false,GenStrWithUniqueCode("logFileOpened",true));
    consoleInit=CreateEvent(NULL,true,false,GenStrWithUniqueCode("consoleInit",true));
    consoleThread=CreateThread(NULL,0,logthreadfunc,NULL,0,&consoleThreadId);
}
struct log_data{
 char* Line;
 char* Source;
};
char buff[255];
void writeToLog(const char* Line,const char* source){
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime(buff,255,"%x-%X:@",timeinfo);
    fputs(buff,LogFile);
    sprintf(buff,"%i::",(int)clock());
    fputs(buff,LogFile);
    fputs(source,LogFile);
    fputs(">>",LogFile);
    fputs(Line,LogFile);
    fputc('\n',LogFile);
}

void Call_consoleEcho(const char* Line,const char* Source){

        int i =0;
        std::cout<<Source<<":";
        while(Line[i]!='\0'){
                i++;
        }
        int charN=i+1;
        i=0;
        int j=0;
        char* BUFF=new char[charN];
        while(Line[i]!='\0'){
        if(Line[i]=='\n'){
                BUFF[j]='\0';
                char* Buff2=new char[j+1];
                for(int k=0;k<j+1;k++)Buff2[k]=BUFF[k];
                #ifdef CONSOLE_ECHO
                std::cout<<Buff2<<std::endl;
                #endif // CONSOLE_ECHO
                writeToLog(Buff2,Source);
                j=0;
                delete Buff2;
        }
            else {
                BUFF[j]=Line[i];
                j++;
            }
            i++;
        };
            BUFF[j]='\0';
            char* Buff2=new char[j+1];
            for(int k=0;k<j+1;k++)Buff2[k]=BUFF[k];
            #ifdef CONSOLE_ECHO
            std::cout<<Buff2<<std::endl;
            #endif // CONSOLE_ECHO
            writeToLog(Buff2,Source);
    //ecriture dans le log
}
VOID CALLBACK APC_consoleEcho(ULONG_PTR dwParam){
log_data* d=(log_data*)dwParam;
Call_consoleEcho(d->Line,d->Source);
delete d->Line;
delete d->Source;
delete d;
}
void consoleEcho(const char* Line,const char* Source="Console"){
    if(WaitForSingleObject(logFileOpened,100)==WAIT_OBJECT_0){
        log_data* d=new log_data;
        d->Line=(char*)malloc(strlen(Line)+1);
        d->Source=(char*)malloc(strlen(Source)+1);
        strcpy(d->Line,Line);
        strcpy(d->Source,Source);
        QueueUserAPC(APC_consoleEcho,consoleThread,(ULONG_PTR)d);
    }else{
        std::cout<<"problem";
    }
}
void consoleEcho(const char* Message,int Integer){
    char buff[255];
    sprintf(buff,Message,Integer);
    consoleEcho(buff);
}
void consoleEchof(const char* Message,float FloatP){
    char buff[255];
    sprintf(buff,Message,FloatP);
    consoleEcho(buff);
}
void consoleEcho(string Message){
    consoleEcho(Message.c_str());
}


void NoticeError(){//releve les erreur de l'API microsoft
        DWORD errorCode=GetLastError();



        LPVOID lpMsgBuf;
        DWORD OverError=FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),(LPSTR)&lpMsgBuf, 1, NULL);
    char buff[255];
    char buff2[255];
    strcpy(buff,"Error(");
    sprintf(buff2,"%i",errorCode);
    strcat(buff,buff2);
    strcat(buff,")");
    consoleEcho((LPSTR)lpMsgBuf,buff);


    if(OverError==0){
        errorCode=GetLastError();
        strcpy(buff,"OverError(");
        sprintf(buff2,"%i",errorCode);
        strcat(buff,buff2);
        strcat(buff,")");
        consoleEcho((LPSTR)lpMsgBuf,buff);
    }
    LocalFree(lpMsgBuf);

}
void NoticeFBError(GLenum Type){
    consoleEcho("Search for FB error");
    GLenum error=glCheckFramebufferStatus(Type);
    char buff[255];
    char buff2[255];
    switch(error){
        case(GL_FRAMEBUFFER_COMPLETE):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Complete");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_FRAMEBUFFER_UNDEFINED):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Undefined");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_FRAMEBUFFER_UNSUPPORTED):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Unsupported");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer incomplete attachment");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Incomplete Missing Attachment");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Incomplete Draw Buffer");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Incomplete Read Buffer");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Incomplete MultiSample");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Incomplete Layer Target");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        default:
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Uncknown");
            consoleEcho((LPSTR)buff,"GL_Error");
    }
}

void NoticeGlError(bool Confirm_no_error){
    GLenum error=glGetError();
    char buff[255];
    char buff2[255];
    switch(error){
        case(GL_NO_ERROR):
            if(Confirm_no_error){
                    strcpy(buff,"GLError(");
                    sprintf(buff2,"%i",error);
                    strcat(buff,buff2);
                    strcat(buff,"):No Error");
                    consoleEcho((LPSTR)buff,"GL_Error");
            }
            break;
        case(GL_INVALID_ENUM):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Invalid Enum");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_INVALID_VALUE):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Invalid Value");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_INVALID_OPERATION):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Invalid Operation");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_INVALID_FRAMEBUFFER_OPERATION):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Invalid Framebuffer Operation");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_OUT_OF_MEMORY):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Out of Memory");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_CONTEXT_LOST):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Context Lost");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_STACK_OVERFLOW):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Stack overflow");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_STACK_UNDERFLOW):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Stack underflow");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;
        case(GL_TABLE_TOO_LARGE):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Table too large");
            consoleEcho((LPSTR)buff,"GL_Error");
            break;

    }

}
void APIENTRY GLCallBack(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userparam){
    string ErrorBuff="";
    ErrorBuff+=GLDebugSourceDecode(source);
    ErrorBuff+="|";
    ErrorBuff+=GLDebugTypeDecode(type);
    ErrorBuff+="|";
    ErrorBuff+=GLDebugSeverityDecode(severity);
    ErrorBuff+="|";
    ErrorBuff+=(string)message;
    consoleEcho(ErrorBuff.c_str(),"GL_ErrorCB");
}
void DefineGLDebugCallback(){
    consoleEcho("Initialisation de la fonction de Debug");
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLCallBack,nullptr);
    NoticeGlError(true);
    consoleEcho("Fini");

}
