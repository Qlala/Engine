#include "Header.h"
thread_local bool AssetManager::IsManagerBound=false;
thread_local AssetManager* AssetManager::ManagerBound=NULL;

DWORD WINAPI AssetManagerWarper(void* AH){
static_cast<AssetManager*>(AH)->ThreadRoutine();
}

AssetManager::AssetManager(){
    Initied=true;
    DedicatedThread=CreateThread(NULL,0,AssetManagerWarper,(void*)this,NULL,&ThreadId);
    CheckingAsset=CreateEvent(NULL,true,false,GenStrWithUniqueCode("AssetManager_"));
}
void AssetManager::ThreadRoutine(){
    while(true){
        /*ConsoleEcho("AssetHandler=>Appelle");
        for(std::list<ParentAsset_UNTYPED*>::iterator Itt=Asset_Library.begin();Itt!=Asset_Library.end();Itt++){
            ConsoleEcho(("AssetHandle:"+(*Itt)->AskName()).c_str());
        }
        ConsoleEcho("Appelle Fini");*/
        CheckAssets();
        SleepEx(check_delay,true);
    }
};
bool LexOrder(string str1,string str2){//equivalent à >
    GLuint i=0;
    while(i<str1.size() && i<str2.size()){
            if(str1[i]!=str2[i]){
                break;
            }
            i++;
    }
    if(i==str1.size()){
        return false;
    }else if(i==str2.size()){
        return true;
    }else{
        return str1[i]>str2[i];
    }
}


std::vector<ParentAsset_UNTYPED*>::iterator AssetManager::SearchRank(string Name){
        ConsoleEcho("Search_Rank");
        ConsoleEcho(("Asset_Library:"+Name).c_str());
        std::vector<ParentAsset_UNTYPED*>::iterator Itt=Asset_Library.begin();
        while(Itt!=Asset_Library.end()){
            if(LexOrder((*Itt)->AskName(),Name))break;
            Itt++;
        }
        return Itt;
};

std::vector<ParentAsset_UNTYPED*>::iterator AssetManager::SearchRankDicho(string Name){
    //std::vector<ParentAsset_UNTYPED*>::iterator Itt=Asset_Library.begin();
    if(Asset_Library.size()==0){
        return Asset_Library.end();
    }else{
        if((Asset_Library.front())->AskName()==Name){
            return Asset_Library.begin();
        }else if((Asset_Library.back())->AskName()==Name){
            return Asset_Library.end();
        }else{
            size_t H=0;size_t B=Asset_Library.size();
            size_t m=(size_t)((H+B)/2);
            size_t lastm=-1;
            while(lastm!=m){
                if((*(Asset_Library.begin()+m))->AskName()==Name){
                    break;
                }else if(LexOrder((*(Asset_Library.begin()+m))->AskName(),Name)){
                    lastm=m;
                    B=m;
                    m=((H+B)/2);
                }else{
                    lastm=m;
                    H=m;
                    m=((H+B)/2);
                }
            }
            return Asset_Library.begin()+m;
        }
    }
}
void AssetManager::CheckAssets(){
    for(size_t i=0;i<Asset_Library.size();i++){
          std::vector<ParentAsset_UNTYPED*>::iterator Itt=Asset_Library.begin()+i;
            ConsoleEcho(("Asset:"+(*Itt)->AskName()).c_str());
            if(!(*Itt)->IsCreated()){
                DeleteAsset(Itt);
                i--;//on décrémente de un car on à éffacé l'asset qui opcupait cette place donc le tableau se décale don si on incrémentait on sauterait un asset
            }
    }
}

void AssetManager::DeleteAsset(std::vector<ParentAsset_UNTYPED*>::iterator Itt){
    ConsoleEcho("AssetManager=>Delete un Asset");
    delete (*Itt);
    Asset_Library.erase(Itt);
}
void AssetManager::DeleteAllAsset(){
    //Prendre le controle total de l'ASSET_Libray => plus d'asset éfacé
   while(Asset_Library.size()!=0){
        AssetManager::DeleteAsset(Asset_Library.begin());
    }
}

void AssetManager::UnboundManager(){
    IsManagerBound=false;
}
void AssetManager::BindManager(AssetManager* AM){
    ManagerBound=AM;
    IsManagerBound=AM->IsInitied();
}

AssetManager::~AssetManager(){
    Initied=false;
    TerminateThread(DedicatedThread,1000);
    CloseHandle(DedicatedThread);
    CloseHandle(CheckingAsset);
    //Effacement de tous le tableau => effacement de tous les pointer à la main
    DeleteAllAsset();
}


