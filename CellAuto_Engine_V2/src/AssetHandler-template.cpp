#pragma once
#include "AssetManager.h"

template<typename PARENT_ASSET>
void AssetManager::AddParentAssetToLibrary(PARENT_ASSET* parentasset){
    ParentAsset_UNTYPED* casted=static_cast<ParentAsset_UNTYPED*>(parentasset);
    Asset_Library.insert(SearchRankDicho(casted->AskName()),casted);

};
template<typename ASSET_TYPE>
 ParentAsset<ASSET_TYPE>* AssetManager::CreateNewParentAsset(ASSET_TYPE * newassettype){
    ParentAsset<ASSET_TYPE>* PA= new ParentAsset<ASSET_TYPE>(newassettype);
    AddParentAssetToLibrary(PA);
    return PA;
};
template<typename ASSET_TYPE>
Asset<ASSET_TYPE>* AssetManager::CreateNewAsset(ASSET_TYPE * newassettype){
    ParentAsset<ASSET_TYPE>* PA= new ParentAsset<ASSET_TYPE>(newassettype);
    AddParentAssetToLibrary(PA);
    return PA->Clone();
};
template<typename ASSET_TYPE>
Asset<ASSET_TYPE>* AssetManager::CreateAsset(ASSET_TYPE * newassettype){
    //on recharche d'abord si il y a déja le même asset
    string name=newassettype->AskName();
    std::vector<ParentAsset_UNTYPED*>::iterator Itt=SearchRankDicho(name);
    if((*Itt)->AskName()==name){
        return static_cast<ParentAsset<ASSET_TYPE>*>((*Itt))->Clone();
    }else{
        ParentAsset<ASSET_TYPE>* PA= new ParentAsset<ASSET_TYPE>(newassettype);
        AddParentAssetToLibrary(PA);
        return static_cast<ParentAsset<ASSET_TYPE>*>(PA)->Clone();
    }
};
template<typename ASSET_TYPE,typename ...ARGS>
Asset<ASSET_TYPE>* AssetManager::CreateAsset(string name,ARGS... args){
    //on recharche d'abord si il y a déja le même asset
    if(Asset_Library.size()>0){
        std::vector<ParentAsset_UNTYPED*>::iterator Itt=SearchRankDicho(name);
        if((*Itt)->AskName()==name){
            return static_cast<ParentAsset<ASSET_TYPE>*>((*Itt))->Clone();
        }else{
            ParentAsset<ASSET_TYPE>* PA= new ParentAsset<ASSET_TYPE>(new ASSET_TYPE(args...));
            AddParentAssetToLibrary(PA);
            return static_cast<ParentAsset<ASSET_TYPE>*>(PA)->Clone();
        }
    }else{//équivaent aux else plus haut
        ParentAsset<ASSET_TYPE>* PA= new ParentAsset<ASSET_TYPE>(new ASSET_TYPE(args...));
        AddParentAssetToLibrary(PA);
        return PA->Clone();
    }
};

template<typename ASSET_TYPE>
bool AssetManager::SearchParentAsset(string Name,ParentAsset<ASSET_TYPE>* &offpointer){
    std::vector<ParentAsset_UNTYPED*>::iterator Itt=SearchRankDicho(Name);
    if((*Itt)->AskName()==Name){
        offpointer=static_cast<ParentAsset<ASSET_TYPE>*>(*Itt);
        return true;
    }else{
        offpointer=nullptr;
        return false;
    }

};

template<typename ASSET_TYPE,typename ...ARGS>
Asset<ASSET_TYPE>* CreateAsset(string Name,ARGS... args){
    if(AssetManager::IsManagerBound){
            return AssetManager::ManagerBound->CreateAsset<ASSET_TYPE>(Name,args...);
    }else{
            ParentAsset<ASSET_TYPE>* PA= new ParentAsset<ASSET_TYPE>(new ASSET_TYPE(args...));
            return static_cast<Asset<ASSET_TYPE>*>(PA);
    }
}
