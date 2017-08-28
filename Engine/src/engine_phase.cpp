#include "engine_phase.h"
DWORD WINAPI threadLaucher(LPVOID me_ptr){
    Phase* me=(Phase*)me_ptr;
    return 0;
}
void Engine::initProcessorHandling(int maxcore_allowed=0){//fonction qui initilialise la gestion des processerur
        ProcessHandle=GetCurrentProcess();
        if(maxcore_allowed==0){
            SYSTEM_INFO sysinfo;
            GetSystemInfo(&sysinfo);
            processorCount= sysinfo.dwNumberOfProcessors;
        }else{
            processorCount= maxcore_allowed;
        }
        processorOcuppationState=new HANDLE[processorCount];
        processorDisponibilitySemaphore=CreateSemaphore(NULL,8,8,GenStrWithUniqueCode("processorDisponibilitySemaphore",true));
        for(DWORD i=0;i<processorCount;i++){
            processorOcuppationState[i]=CreateEvent(NULL,true,false,GenStrWithUniqueCode("processorOcuppationState",true));
        }
        NoticeError();
        processorInited=true;
};


void Engine::askNewProcessor(DWORD waitTime, HANDLE thread){
        DWORD res=WaitForSingleObject(processorDisponibilitySemaphore,waitTime);
        if(res==WAIT_OBJECT_0){
            for(int i=0;i<processorCount;i++){
                if(WaitForSingleObject(processorOcuppationState[i],0)==WAIT_OBJECT_0){
                    SetEvent(processorOcuppationState[i]);
                    SetThreadIdealProcessor(thread,i);
                    break;
                }
            }
            return true;
        }else{
            //on a pas trouvé de processeur vide mais on veut que le programme continue donc tant pis (ordenanceur devra tafer ^^)
            return false;
        }
};
void Engine::releaseProcessor(HANDLE thread){
        int core=SetThreadIdealProcessor(thread,MAXIMUM_PROCESSORS);
        if(core<processorCount){
            ResetEvent(processorOcuppationState[core]);
            ReleaseSemaphore(processorDisponibilitySemaphore,1,NULL);
        }
}
HGLRC Engine::getNewGlContext(){
    HGLRC nc=wglCreateContext(deviceContext);
    wglShareLists(nc,glContext);
    return nc;
}

void Phase::basicInit(){
    jobFinishEvent=CreateEvent(NULL,TRUE,FALSE,GenStrWithUniqueCode("phaseFin",true));
    workingEvent=CreateEvent(NULL,TRUE,FALSE,GenStrWithUniqueCode("phaseWorking",true));
    dedicatedThread=CreateThread(NULL,0,threadLaucher,(void*)this,&threadid);
    RenderContext=LinkedEngine->getNewGlContext();

    inited=true;
}

void Phase::threadFun(){
   wglMakeCurrent(linkedEngine->getHDC(),RenderContext);
    while(1){
        WaitForSingleObject(workingEvent,INFINITE);
        WaitForSingleObject(linkedEngine->getGlobalFinishEvent())
        if(preparationNeeded)prepareMainJob();
        waitStartCondition();//on attend le top dépaer
        mainJob();
        SetEvent(jobFinishEvent);
        WaitForSingleObject(linkedEngine->getGlobalFinishEvent(),INFINTE);
        finishJob();
        ResetEvent(jobFinishEvent);
    };

}



void Phase::waitStartCondition(){
    WaitForMultipleObjects(startEvents.size(),startEvents.data(),true,INFINITE);
}

void Phase::addExplicitPhaseDependancy(Phase* p){//il faudrait controllé que la phase est bien pas vérouillé
    if(not linkedEngine->isLocked()){//ou juste le moteur en faite
        explicitPhaseDependancy.push_back(p);
        startEvents.push(p->getFinishEvent());
    }
}

void Phase::resetExplicitDependacy(){
    if(not linkedEngine->isLocked()){
    explicitPhaseDependancy.clear();
    startEvents.clear();
    startEvents.push(linkedEngine->getGlobalStartEvent());
    }
}
