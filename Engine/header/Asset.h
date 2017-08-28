#pragma once
#include "Header.h"
//on dif�rencie les asset (objet manag� pour la gestion de la m�moire) des assetType qui seront les objet eux m�me buffer/texture object
//lo'bjectif �tant que les scene /mesh / objet2d n'accede que a des asset qui pourront etre clon� pendant que les objet r�el ne sont qu'as un seul endroit
class AssetType{
    protected:
        bool CREATED=false;
        bool RAMLOADED=false;
        bool BUFFERLOADED=false;

    public:
        //Fonction d'analyse des bool�en
        inline bool IS_CREATED(){return CREATED;};
        inline bool IS_RAMLOADED(){return RAMLOADED;};// ram loaded et vram loaded sont ind�pendant
        inline bool IS_BUFFERLOADED(){return BUFFERLOADED;};
        //virtual AssetType(DataType Data);
        virtual bool IsMutable(){return false;};//renvoie true si les alt�ration sur les donn�s(en ram) on un sens
        virtual bool IsBindable(){return false;};
        virtual bool MakeMutable(){return false;};//Rend Les Donn� alt�rable
        virtual bool UpdateDataMutation(){return false;};//met � jour les donn� depuis l'endroit ou elle sont alt�rable
        //virtual bool Create(...);//fonction qui cr�e l'objet (avant le bool created est � false
        //la fonction create n'extiste pas vraiment mais elle est fortement sug�r�
        virtual bool LoadRam(){return false;};
        virtual bool LoadBuffer(){return false;};
        virtual bool UnloadRam(){return false;};
        virtual bool UnloadBuffer(){return false;};
        virtual bool Bind(GLuint slot=0){return false;};
        virtual bool Destroy(){return false;};//D�truit Completement l'Objet :: renvoie true si l'op�ration r�ussi
        virtual string AskName(){return "ASSET_VIRTUELL";};//permet de demand� un nom � l'objet pour nomm� l'asset
        //virtual AssetType//d�pend de la spec du asset type
        //static virtual GetFutureName(){};//pas vraiment d�fini on red�finira pour chaque type d'asset et pour chaque constructeur
        AssetType(){};
        ~AssetType(){};

};

template<typename REAL_ASSET_TYPE>//classe temporaire pour forc� l'heritage de la classe AssetType pour avoir acces au fonction d�clar�e
class DUMMY_ASSET_TYPE : REAL_ASSET_TYPE,AssetType{
};

template<typename ASSET_TYPE>//vrais ASSET_TYPE != DUMMY
class Asset{
    public:
        virtual bool IsCreated(){return false;};
        virtual bool IsBUFFERLoaded(){return false;};
        virtual bool IsRAMLoaded(){return false;};
        virtual bool IsDataMutable(){return false;};//Donn�e sont alt�rable
        virtual bool IsBindable(){return false;};
        virtual bool Bind(GLuint slot=0){return false;};
        virtual ASSET_TYPE* AccesData(){return NULL;};
        virtual bool UpdateData(){return false;};
        //autre fonction en d�bat
        virtual void LoadRAM(){};
        virtual void UnloadRAM(){};
        virtual void LoadBUFFER(){};
        virtual void UnloadBUFFER(){};
        virtual ~Asset(){};
        virtual Asset<ASSET_TYPE>* Clone(){return NULL;}//Clone l'Asset => le r�sultat d�pend tu vrais type d'asset;
        virtual Asset<ASSET_TYPE>* Copy(){return NULL;}//Copie l'asset => le r�sultat est n�cassairment un asset distinct du premier
        ASSET_TYPE* operator->() {
            return AccesData();
        };

};
class ParentAsset_UNTYPED;
template<typename REAL_ASSET_TYPE>
class ChildAsset;

template<typename ASSET_TYPE>
class ParentAsset_BREEDER{
    public:
    virtual ChildAsset<ASSET_TYPE>* CreateChildAsset(){};
};
template<typename REAL_ASSET_TYPE>
class ParentAsset_LEGACY :  public Asset<REAL_ASSET_TYPE> , public ParentAsset_BREEDER<REAL_ASSET_TYPE>{};

template<typename REAL_ASSET_TYPE>
class RealChildAsset;





class ParentAsset_UNTYPED{//type pour polymorphisme d'Asset ind�pendant du type
    public:
    virtual ~ParentAsset_UNTYPED(){ConsoleEcho("Virtual Parent Asset Destructor");};
    virtual void SelfDestruct(){};
    virtual bool IsCreated(){return false;};
    virtual string AskName(){return "Parent_Asset:Virtuel Pure";};
    template<typename ASSET_TYPE>
    RealChildAsset<ASSET_TYPE>* CreateChildAsset(){
        return static_cast<ParentAsset_BREEDER<ASSET_TYPE>*>(this)->CreateChildAsset();
    };
};
//Les Class R

template<typename REAL_ASSET_TYPE>
class ChildAsset : public Asset<REAL_ASSET_TYPE>{
    public:
//    virtual GLuint ChildId;
    /*bool Voted_RAMLoad;//vote de l'enfant pour charg� en ram ou en Vram
    bool Voted_VRAMLoad;*/
    virtual GLuint GetChildId(){return 0;};
    virtual bool Had_VotedBUFFERLoad(){return false;};
    virtual bool Had_VotedRAMLoad(){return false;};
    virtual void DeclareParentDeath(){};//envoie � tous lse enfant que le parent est mort
    virtual void DeclareParent(const ParentAsset_LEGACY<REAL_ASSET_TYPE>* Parent,GLuint ID){};//donn� le parent et un num�ro unique qui correspond au nombre d'enfant eu par le parent
};

enum{DEFAULT_UNLOAD,DEFAULT_LOAD,DEMOCRACY};//mode de gestion des enfants
template<typename REAL_ASSET_TYPE>
class ParentAsset : public ParentAsset_LEGACY<REAL_ASSET_TYPE>,public ParentAsset_UNTYPED{
    private:
        //Type des donn� � fournir pour cr�er un un AssetObject de type REAL_ASSET_TYPE
        typedef DUMMY_ASSET_TYPE<REAL_ASSET_TYPE> ASSET_TYPE;
        //vrais donn�es
        REAL_ASSET_TYPE* DATA;
        string AssetName;
        bool Exist;//renvoie si l'Asset existe encore ou si il est d�truit
        GLuint ChildCount=0;
        GLuint ChildTotal=0;//total d'enfant eu :: ne diminue pas
        bool ChildDependant=true;
        std::vector<ChildAsset<REAL_ASSET_TYPE>*> Children;//liste de tous les enfant;
        //Child Gestion
        GLuint ChildGestion=DEFAULT_LOAD;//par default on maintient le syst�me charg� si un enfant le demand
        GLuint BUFFER_UnloadDemand;
        GLuint BUFFER_LoadDemand;
        GLuint RAM_UnloadDemand;
        GLuint RAM_LoadDemand;
    public:
        bool IsCreated(){return (Exist)?DATA->IS_CREATED():false;};
        bool IsBUFFERLoaded(){return (Exist)?DATA->IS_BUFFERLOADED():false;};
        bool IsRAMLoaded(){return  (Exist)?DATA->IS_RAMLOADED():false;};
        string AskName(){ConsoleEcho("Asset=>RealData");
            return(IsCreated())?DATA->AskName():"Parent Asset :No Data";
            }

        ParentAsset(REAL_ASSET_TYPE* data){//op�ration de copie de donn� attention;
            DATA=data;
            Exist=DATA->IS_CREATED();
            AssetName=DATA->AskName();
        }
        void SelfDestruct(){
            if(Exist){
                ConsoleEcho("SelfDestruct");
                Exist=!DATA->Destroy();//Destro renvoie true si il y a reussite
                ConsoleEcho("Reussite de la destruction de:"+AssetName);
                ConsoleEcho("%i",(int)(Exist));
                delete DATA;
                for(typename std::vector<ChildAsset<REAL_ASSET_TYPE>*>::iterator A=Children.begin();A!=Children.end();A++){
                    (*A)->DeclareParentDeath();
                }
            }
        }
        ~ParentAsset(){
            ConsoleEcho("Parent Asset Destructor");
            SelfDestruct();
        }


        bool IsDataMutable(){return (Exist)?DATA->MakeMutable():false;}
        bool IsBindable(){return (Exist)?DATA->IsBindable():false;}

        bool Bind(GLuint slot=0){
            if(IsBUFFERLoaded()){
                return DATA->Bind(slot);
            }else{
                ConsoleEcho(("Erreur dans l'affectation de L'asset :"+AssetName).c_str());
                return false;
            }
        };

        REAL_ASSET_TYPE* AccesData(){
            if(Exist){
                DATA->MakeMutable();
                return DATA;
            }
        }

        bool UpdateData(){
            return (Exist)?DATA->UpdateDataMutation():false;
        };

        //fonction propre au Real Asset
        void LoadRAM(){
            if(IsCreated())DATA->LoadRam();
        }
        void UnloadRAM(){
            if(IsCreated())DATA->UnloadRam();
        }
        void LoadBUFFER(){
            if(IsCreated())DATA->LoadBuffer();
        }
        void UnloadBUFFER(){
            if(IsCreated())DATA->UnloadBuffer();
        }


        //fonction pour les enfant
        typename std::vector<ChildAsset<REAL_ASSET_TYPE>*>::iterator SearchChildId(GLuint Id){//on pourrais cherch� par dichotomie
            typename std::vector<ChildAsset<REAL_ASSET_TYPE>*>::iterator Itt=Children.begin();
            while((*Itt)->GetChildId()!=Id && Itt!=Children.end()){Itt++;};
            return Itt;
        }

        void DeclareChildDeath(GLuint MyId){
            ConsoleEcho("Je perd un enfant :"+AskName());
            ChildCount--;
            typename std::vector<ChildAsset<REAL_ASSET_TYPE>*>::iterator ChildToDel=SearchChildId(MyId);
            TakeOutChildDemand((*ChildToDel)->Had_VotedRAMLoad(),(*ChildToDel)->Had_VotedBUFFERLoad());
            Children.erase(ChildToDel);
            if(ChildDependant && ChildCount==0){
                ConsoleEcho("Ah tous mes enfant sont mort");
                SelfDestruct();
            }
        }
        void AssessChildDemand(){//fonction qui ajoute l'enfant dans les demandes de gestion
            if(IsBUFFERLoaded()){
                BUFFER_LoadDemand++;
            }else{
                BUFFER_UnloadDemand++;
            }
            if(IsRAMLoaded()){
               RAM_LoadDemand++;
            }else{
                RAM_UnloadDemand++;
            }
        }
        void TakeOutChildDemand(bool VotedBuffer,bool VotedBuffer){
            if(VotedBuffer){
                BUFFER_LoadDemand--;
            }else{
                BUFFER_UnloadDemand--;
            }
            if(VotedRam){
               RAM_LoadDemand--;
            }else{
                RAM_UnloadDemand--;
            }

        }
        //Fonction qui d�partage en fonction du type de gestion
        bool AssessVote(GLuint ProLoad,GLuint ConLoad,bool CurrentState){
            switch(ChildGestion){
                case(DEFAULT_LOAD):
                    return(ProLoad!=0);
                break;
                case(DEFAULT_UNLOAD):
                    return ConLoad!=0;
                break;
                case(DEMOCRACY):
                    if(ProLoad=ConLoad){
                        return CurrentState;
                    }else return ProLoad>ConLoad;
                break;
                default:
                  return(ProLoad!=0);
            }
        }

        //fonction de vote pour les enfant(les enfant controle tous seul qu'il ne vote pas 2 fois)
        void AskLoadRAM(){
            RAM_LoadDemand++;
            RAM_UnloadDemand--;
            if(AssessVote(RAM_LoadDemand,RAM_UnloadDemand,IsRAMLoaded()) && !IsRAMLoaded()){//si le vote gagne et que ce n'est pas d�ja appliqu�
                LoadRAM();
            }
        }
        void AskUnloadRAM(){
            RAM_UnloadDemand++;
            RAM_LoadDemand--;
            if(!AssessVote(RAM_LoadDemand,RAM_UnloadDemand,IsRAMLoaded()) && IsRAMLoaded()){//si le vote gagne et que ce n'est pas d�ja appliqu�
                UnloadRAM();
            }
        }
        void AskLoadBUFFER(){
            BUFFER_LoadDemand++;
            BUFFER_UnloadDemand--;
            if(AssessVote(BUFFER_LoadDemand,BUFFER_UnloadDemand,IsBUFFERLoaded()) && !IsBUFFERLoaded()){//si le vote gagne et que ce n'est pas d�ja appliqu�
                LoadBUFFER();
            }
        }
        void AskUnloadBUFFER(){
            BUFFER_UnloadDemand++;
            BUFFER_LoadDemand--;
            if(!AssessVote(BUFFER_LoadDemand,BUFFER_UnloadDemand,IsBUFFERLoaded()) && IsBUFFERLoaded()){//si le vote gagne et que ce n'est pas d�ja appliqu�
                UnloadBUFFER();
            }
        }
        bool AskForDataAcess(bool RAM_Vote,bool BUFFER_Vote){
            return (BUFFER_LoadDemand-(GLuint)BUFFER_Vote)==0;
        }

        ChildAsset<REAL_ASSET_TYPE>* CreateChildAsset(){//cr�ation d'un enfant � partir d'un parent
            ChildCount++;
            RealChildAsset<REAL_ASSET_TYPE>* Child= new RealChildAsset<REAL_ASSET_TYPE>();
            Child->DeclareParent(this,ChildTotal);
            ChildTotal++;
            AssessChildDemand();
            Children.push_back(Child);
            return static_cast<ChildAsset<REAL_ASSET_TYPE>*>(Child);
        };
        Asset<REAL_ASSET_TYPE>* Clone(){
            if(Exist){
                Asset<REAL_ASSET_TYPE>* casted=static_cast<Asset<REAL_ASSET_TYPE>*>(CreateChildAsset());
                return casted;
            }
        }
        Asset<REAL_ASSET_TYPE>* Copy(){
            if(Exist){
                Asset<REAL_ASSET_TYPE>* casted=static_cast<Asset<REAL_ASSET_TYPE>*>(new ParentAsset<REAL_ASSET_TYPE>(DATA->Copy()));
            }
        }


};
template<typename REAL_ASSET_TYPE>
class RealChildAsset : public ChildAsset<REAL_ASSET_TYPE>{//tres compliqu� c'est pour pouvoir acc�d� � une fonction particuliere
    private:
        ParentAsset<REAL_ASSET_TYPE>* Parent;
        GLuint ChildId;
        bool Voted_RAMLoad;//vote de l'enfant pour charg� en ram ou en Vram
        bool Voted_BUFFERLoad;
        //Tres tres important
        bool Parent_Alive=false;//Deport� pour �vit� les violation de m�moire
    public:
        bool Had_VotedBUFFERLoad(){return Voted_BUFFERLoad;}
        bool Had_VotedRAMLoad(){return Voted_RAMLoad;}
        GLuint GetChildId(){return ChildId;};
        void SelfDestruct(){
            if(Parent_Alive)Parent->DeclareChildDeath(ChildId);
        }
        ~RealChildAsset(){
            SelfDestruct();
        }
        bool IsCreated(){return (Parent_Alive)?Parent->IsCreated():false;};
        bool IsBUFFERLoaded(){return (Parent_Alive)?Parent->IsBUFFERLoaded():false;};
        bool IsRAMLoaded(){return (Parent_Alive)?Parent->IsRAMLoaded():false;};
        bool Bind(GLuint slot=0){return (Parent_Alive)?Parent->Bind(slot):false;};

        void DeclareParent(ParentAsset_LEGACY<REAL_ASSET_TYPE>* Papa,GLuint ID){
            Parent=static_cast<ParentAsset<REAL_ASSET_TYPE>*>(Papa);
            ChildId=ID;
            Voted_RAMLoad=IsRAMLoaded();
            Voted_BUFFERLoad=IsBUFFERLoaded();
            Parent_Alive=Papa->IsCreated();
        };

        REAL_ASSET_TYPE* AccesData(){
            if(IsCreated()){
                return Parent->AccesData();
            }else{
                return nullptr;
            }
        }



        //Fonction de vote
        void Vote_RAMLoad(){
            if(!Voted_RAMLoad){
                Voted_RAMLoad=true;
                if(Parent_Alive)Parent->AskLoadRAM();
            }
        }
        void Vote_RAMUnload(){
            if(Voted_RAMLoad){
                Voted_RAMLoad=false;
                if(Parent_Alive)Parent->AskUnloadRAM();
            }
        }
        void Vote_BUFFERLoad(){
            if(!Voted_BUFFERLoad){
                Voted_BUFFERLoad=true;
                if(Parent_Alive)Parent->AskLoadBUFFER();
            }
        }
        void Vote_BUFFERUnload(){
            if(Voted_BUFFERLoad){
                Voted_BUFFERLoad=false;
                if(Parent_Alive)Parent->AskUnloadBUFFER();
            }
        }

        void LoadRAM(){if(Parent_Alive)Vote_RAMLoad();}
        void UnloadRAM(){if(Parent_Alive)Vote_RAMUnload();}
        void LoadBUFFER(){if(Parent_Alive)Vote_BUFFERLoad();}
        void UnloadBUFFER(){if(Parent_Alive)Vote_BUFFERUnload();};

        void DeclareParentDeath(){
            Parent_Alive=false;
        }

        Asset<REAL_ASSET_TYPE>* Clone(){
            return Parent->Clone();
        }
};


