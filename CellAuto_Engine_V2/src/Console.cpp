
#include "Header.h"
//zero therminating char*
HANDLE InWrite;
HANDLE InWriteToLog;
FILE* LogFile;
bool LogFileOpened=false;
bool ConsoleInit=false;
time_t rawtime;
struct tm * timeinfo;
clock_t tick;
int UniqueCode=0;
char buff3[255];
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



void APIENTRY GLCallBack(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userparam){
    string ErrorBuff="";
    ErrorBuff+=GLDebugSourceDecode(source);
    ErrorBuff+="|";
    ErrorBuff+=GLDebugTypeDecode(type);
    ErrorBuff+="|";
    ErrorBuff+=GLDebugSeverityDecode(severity);
    ErrorBuff+="|";
    ErrorBuff+=(string)message;
    WriteToLog(ErrorBuff.c_str(),"GL_ErrorCB");
}
void DefineGLDebugCallback(){
    ConsoleEcho("Initialisation de la fonction de Debug");
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLCallBack,nullptr);
    NoticeGlError(true);
    ConsoleEcho("Fini");

}




const char* GenStrWithUniqueCode(const char* Format,bool increment){
    strcpy(buff3,Format);
    strcat(buff3,"%i");
    sprintf(buff3,buff3,UniqueCode);
    if(increment)UniqueCode++;
    return buff3;
}


void WriteToLog(const char* Line,const char* Source){
    #ifdef WRITE_LOG
    if(!LogFileOpened){
        InWriteToLog=CreateMutex(NULL,false,"Console.InWriteToLog");
        std::cout<<"FileCreation"<<std::endl;
        LogFile=fopen("Log.txt","w");
        if (LogFile!=NULL){
            LogFileOpened=true;
            //std::cout<<"FileCreated"<<std::endl;
            //memory_fence();
        }
    }
    WaitForSingleObject(InWriteToLog,INFINITE);
    char buff[255];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime(buff,255,"%x-%X:@",timeinfo);
    fputs(buff,LogFile);

    sprintf(buff,"%i::",(int)clock());
    fputs(buff,LogFile);
    fputs(Source,LogFile);
    fputs(">>",LogFile);
    fputs(Line,LogFile);
    fputc('\n',LogFile);
    ReleaseMutex(InWriteToLog);
    #endif

}
void ConsoleEcho(const char* Message,int Integer){
    char buff[255];
    sprintf(buff,Message,Integer);
    ConsoleEcho(buff);
}
void ConsoleEchof(const char* Message,float FloatP){
    char buff[255];
    sprintf(buff,Message,FloatP);
    ConsoleEcho(buff);
}
void ConsoleEcho(string Message){
    ConsoleEcho(Message.c_str());
}

void ConsoleEcho(const char* Message){
    #ifdef CONSOLE_ECHO || WRITE_LOG
    #ifdef CONSOLE_ECHO
        if(!ConsoleInit){
            InWrite=CreateMutex(NULL,false,"Console.InWrite");
            std::cout<<"Mutex Created"<<std::endl;
            ConsoleInit=true;
        }
    WaitForSingleObject(InWrite,INFINITE);
    int i =0;
    std::cout<<"Console:";
    while(Message[i]!='\0'){
            i++;
    }
    int charN=i+1;
    i=0;
    int j=0;
    #endif
    char* BUFF=new char[charN];
    while(Message[i]!='\0'){
    if(Message[i]=='\n'){
            BUFF[j]='\0';
            char* Buff2=new char[j+1];
            for(int k=0;k<j+1;k++)Buff2[k]=BUFF[k];
            #ifdef CONSOLE_ECHO
            std::cout<<Buff2<<std::endl;
            #endif // CONSOLE_ECHO
            WriteToLog(Buff2,"Console");
            j=0;
            delete Buff2;
    }
        else {
            BUFF[j]=Message[i];
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
        WriteToLog(Buff2,"Console");
        delete Buff2;
        delete BUFF;
    #ifdef CONSOLE_ECHO
    ReleaseMutex(InWrite);
    #endif // CONSOLE_ECHO
    #endif // CONSOLE_ECHO
}

void NoticeError(){
        DWORD errorCode=GetLastError();
        if(!ConsoleInit){
            InWrite=CreateMutex(NULL,true,"Console.InWrite");
        }
    WaitForSingleObject(InWrite,INFINITE);
    int i =0;



        LPVOID lpMsgBuf;
        DWORD OverError=FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),(LPSTR)&lpMsgBuf, 1, NULL);
    char buff[255];
    char buff2[255];
    strcpy(buff,"Error(");
    sprintf(buff2,"%i",errorCode);
    strcat(buff,buff2);
    strcat(buff,")");
    WriteToLog((LPSTR)lpMsgBuf,buff);
    std::cout<<buff<<":"<<(LPSTR)lpMsgBuf<<std::endl;


    if(OverError==0){
        errorCode=GetLastError();
        std::cout<<"OverError("<<errorCode<<"):"<<(LPSTR)lpMsgBuf<<std::endl;
    }
    LocalFree(lpMsgBuf);
    ReleaseMutex(InWrite);

}
void NoticeFBError(GLenum Type){
    ConsoleEcho("Search for FB error");
    GLenum error=glCheckFramebufferStatus(Type);
    char buff[255];
    char buff2[255];
    switch(error){
        case(GL_FRAMEBUFFER_COMPLETE):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Complete");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_FRAMEBUFFER_UNDEFINED):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Undefined");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_FRAMEBUFFER_UNSUPPORTED):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Unsupported");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer incomplete attachment");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Incomplete Missing Attachment");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Incomplete Draw Buffer");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Incomplete Read Buffer");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Incomplete MultiSample");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS):
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):FrameBuffer Incomplete Layer Target");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        default:
            strcpy(buff,"FBError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Uncknown");
            WriteToLog((LPSTR)buff,"GL_Error");
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
                    WriteToLog((LPSTR)buff,"GL_Error");
                    std::cout<<buff<<std::endl;
            }
            break;
        case(GL_INVALID_ENUM):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Invalid Enum");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_INVALID_VALUE):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Invalid Value");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_INVALID_OPERATION):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Invalid Operation");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_INVALID_FRAMEBUFFER_OPERATION):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Invalid Framebuffer Operation");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_OUT_OF_MEMORY):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Out of Memory");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_CONTEXT_LOST):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Context Lost");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_STACK_OVERFLOW):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Stack overflow");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_STACK_UNDERFLOW):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Stack underflow");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;
        case(GL_TABLE_TOO_LARGE):
            strcpy(buff,"GLError(");
            sprintf(buff2,"%i",error);
            strcat(buff,buff2);
            strcat(buff,"):Table too large");
            WriteToLog((LPSTR)buff,"GL_Error");
            std::cout<<buff<<std::endl;
            break;

    }

}
