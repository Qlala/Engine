#pragma once
#include"Header.h"

template<typename ASSET_TYPE,typename ...ARGS>
extern Asset<ASSET_TYPE>* CreateAsset(string Name,ARGS... args);
/*template<typename ASSET_TYPE,typename ...ARGS>
extern Asset<ASSET_TYPE>* CreateAsset(ARGS... args);*/

class AssetManager{
    public:
        static thread_local AssetManager* ManagerBound;
        static thread_local bool IsManagerBound;
    private:

    HANDLE DedicatedThread;
    HANDLE CheckingAsset;//mutex
    DWORD ThreadId;
    bool Initied=false;
    std::vector<ParentAsset_UNTYPED*> Asset_Library;
    DWORD check_delay=100;
    public:
        bool IsInitied(){return Initied;};
        AssetManager();
        ~AssetManager();
        template<typename PARENT_ASSET>//demande l'asset et pas l'asset type
        void AddParentAssetToLibrary(PARENT_ASSET* parentasset);


        template<typename ASSET_TYPE>
        ParentAsset<ASSET_TYPE>* CreateNewParentAsset(ASSET_TYPE * newassettype);
        template<typename ASSET_TYPE>
        Asset<ASSET_TYPE>* CreateNewAsset(ASSET_TYPE * newassettype);
        template<typename ASSET_TYPE>
        Asset<ASSET_TYPE>* CreateAsset(ASSET_TYPE * newassettype);//induit que l'asset n'est ni forcémùent nouveau ni forcément parent
        template<typename ASSET_TYPE,typename ...ARGS>
        Asset<ASSET_TYPE>* CreateAsset(string name,ARGS... args);

        std::vector<ParentAsset_UNTYPED*>::iterator SearchRank(string Name);
        std::vector<ParentAsset_UNTYPED*>::iterator SearchRankDicho(string Name);

        template<typename ASSET_TYPE>
        bool SearchParentAsset(string Name,ParentAsset<ASSET_TYPE>* &offpointer);
        void CheckAssets();//vérifie que les asset exist encore en leur demandant si il sont crée

        void ThreadRoutine();

        void DeleteAsset(std::vector<ParentAsset_UNTYPED*>::iterator Itt);
        void DeleteAsset(string name);
        void DeleteAllAsset();
    //fonction statique
        static void BindManager(AssetManager* AM);
        static void UnboundManager();
};


#include "../src/AssetHandler-template.cpp"
