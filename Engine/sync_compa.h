#include <windows.h>
#include <winspool.h>


#ifdef WIN32
    #define WIN32_SYNC
    //event
    #define Event DWORD
    #define NewEvent(default_state,auto_reset,name) CreateEvent(NULL,!auto_reset,default_state,name)
    #define CallEvent(event) SetEvent(event)
    #define DeleteEvent(event) CloseHandle(event)
    #define ResetEvent(event) ResetEvent(event)



    #define WAIT_SUCCEEDED WAIT_OBJECT_0
    #define WAIT_TIMEOUT WAIT_TIMEOUT
    #define WAIT_FAILED WAIT_FAILED
    #define WAIT_ABANDONED WAIT_ABANDONED
    #define WaitForOneObject(object,time) WaitForSingleObject(object,time)

#else
    #define COMPA_SYNC
    #include <condition_varaible>
    struct COMPLEX_EVENT{
        std::condition_variable event;
        bool auto_reset;
        inline void notify(){
            if(auto_reset)
                event.notify_one();
            else
                event.notify_all();
        }
    };
    inline COMPLEX_EVENT* CREATE_COMPLEX_EVENT(bool default_state,bool auto_reset){
        COMPLEX_EVENT* Nevt=new COMPLEX_EVENT;
        Nevt->event=std::condition_variable();
        Nevt->auto_reset = auto_reset;
        if(default_state){
           Nevt->notify();
        }
        return Nevt
    }

    #define Event COMPLEX_EVENT*
    #define NewEvent(default_state,auto_reset,name) CREATE_COMPLEX_EVENT(default_state,auto_reset)
    #define CallEvent(event) event->notify()
    #define DeleteEvent(event) delete event
    #define Reset


#endif // WIN32
