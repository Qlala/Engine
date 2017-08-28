#pragma once
#include "LibHeader.h"
#include "GlobalHeader.h"

class Scene;
class Engine{
    //gestion des etapes de rendu
    HANDLE globalStartEvent;//démarage de la première étape de rendu
    HANDLE globalFinishEvent;//fin de la dernier étape de rendu
    //gestion du processeur
    HANDLE ProcessHandle;
    HANDLE* processorOcuppationState;//rend compte de l'utilisation des processeur
    HANDLE processorDisponibilitySemaphore;//sémpahore libéré lorsqu'un programme
    DWORD processorCount;//il est relatif => un engine peut avoir acces qu'une partie des processeur d'une machine;
    bool processorInited;

    bool validated;
    bool locked;//on ne peut plus accedé au phase (empèche d'avoir des problème sur le moteur)

    //contexte de rendu
    HDC deviceContext;
    HGLRC glContext;

    public:
    //getter/setter
    inline HANDLE getGlobalStartEvent(){return globalStartEvent;};
    inline HANDLE getGlobalFinishEvent(){return globalFinishEvent;};
    inline HDC getHDC()(return deviceContext);
    inline bool isLocked(){return locked};
    //méthode
    void initProcessorHandling(int maxcore_allowed=0);
    bool askNewProcessor(DWORD waitTime,HANDLE thread);

    HGLRC getNewGlContext();


    void releaseProcessor(HANDLE thread);
    void validate();//-> valide le moteur (vérifie que toute les phase on bien une dépendance
    void lock();
    void unlock();
    void start();//lance l'init des phase
    void stop();//arrête toute les phase pour pouvoir changer le moteur;
    void pause();//mais en pause le rendu => les thread sont actif mais sont bloqué
    void resume();
};
enum phaseType{RENDER,COMPUTE,OTHER};//Les render communique avec l'engine en se passant

class Phase{
    //ces bolle ne doivent pas changer et son défini par le type de phase
    bool preparationNeeded;
    bool PhaseDependant;//dépend d'une autre phase
    //bool d'état
    bool inited;//vrais quand la phase a été initialisé;
    bool validated;


    std::vector<HANDLE> startEvents;//events qui lance la phase => est défini comme le début du rendu si non dépendant de la dernier phase
    HANDLE jobFinishEvent;
    HANDLE dedicatedThread;
    DWORD threadid;
    HANDLE workingEvent;//signalé tant que le thread doit tourné

    std::vector<Phase*> explicitPhaseDependancy;//phases dont dépent le rendu => l'orde est à la dépendance de l'utilisateur qui a concu le moteur;
    Engine* linkedEngine;//moteur auquel on a lié la phase
    HGLRC RenderContext;

    //fonction privé/protected
    protected:
    void waitStartCondition();


     //fonction public
    public:
    bool validate();//validation de la phase => appler lors de la validation du moteur
    void threadFun();//fonction appeler par le thread;
    void basicInit();

    inline HANDLE getWorkingEvent(){return workingEvent;};
    inline HANDLE getFinishEvent(){return jobFinishEvent;};

    void addExplicitPhaseDependancy(Phase* p);
    void resetExplicitDependacy();

    //fonction qui sont a redefinir
    virtual bool init();//fonction qui est appeler de toute facon a la validation de l'engine
    virtual bool prepareMainJob();//fonction est appeler une fois par rendu au début=> n'as pas lieu si le pas de needPreparation
    virtual bool mainJob();//appeler des que l'étape précendant est fini
    virtual bool finishJob();//appeler en fin de rendu
    virtual bool quit();//foncton_appeler lorsque l'on détruit la phase



};

