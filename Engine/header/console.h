#pragma once
extern void consoleEcho(const char* Message,const char* source);
extern void consoleEcho(string Message);
extern void consoleEcho(const char* Message,int Integer);
extern void consoleEchof(const char* Message,float FloatP);
extern void NoticeError();
extern void NoticeFBError(GLenum Type);
extern void NoticeGlError(bool Confirm_no_error=false);
extern const char* GenStrWithUniqueCode(const char* Format,bool increment=true);
extern void DefineGLDebugCallback();
extern void initConsole();
