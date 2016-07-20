#pragma once
extern void ConsoleEcho(const char* Message);
extern void ConsoleEcho(string Message);
extern void ConsoleEcho(const char* Message,int Integer);
extern void ConsoleEchof(const char* Message,float FloatP);
extern void NoticeError();
extern void NoticeFBError(GLenum Type);
extern void NoticeGlError(bool Confirm_no_error=false);
extern void WriteToLog(const char* Line,const char* Source="");
extern bool LogFileOpened;
extern bool ConsoleInit;
extern const char* GenStrWithUniqueCode(const char* Format,bool increment=true);
extern void DefineGLDebugCallback();
