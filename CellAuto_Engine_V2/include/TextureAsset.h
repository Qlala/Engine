#pragma once
#include "Header.h"
struct GLSampler{
    GLuint Id;
};




class TextureAsset : public AssetType{
    private:
        /*bool CREATED;
        bool RAMLOADED;
        bool VRAMLOADED;*/
        string Name;

        //FileState
        bool FromFile=false;
        string FilePath;
        FILE * PicFile;
        bool FILELOADED=false;

        //Global State
        GLuint TextureType=0;//GL_Texture2D

        //Ram State
        void* Bitmap;
        GLsizei DataSize;
        GLuint Height=0;
        GLuint Width=0;
        GLuint Depth=0;//nombre de coupe pour les texture array ou profondeur
        GLuint BytePerPixel=0;
        GLuint BytePadding=0;//quand on change de ligne le nombre de byte est arondi � un multiple de cette valeur (un peu comme pour les bmp)
        GLuint ComponentNumber=0;//Nombre de composantes
        GLuint GlDataType=GL_UNSIGNED_BYTE;//GL_FLOAT... GL_UNSIGNED_BYTE
        GLuint ColorFormat;//GL_RGBA....
        void* Store_DataDetail;
        template<typename T>
        T DataDetail(){return (T*)Store_DataDetail;}

        //Vram State

        GLuint TextureId;
        GLuint V_Width=0;//Les v sont la meme chose que leur �quivalent en RAM mais peuvent etre dif�rent car ne d�crivant pas le m�me espace m�moire
        GLuint V_Height=0;
        GLuint V_Depth=0;
        GLuint Sample=0;//nombre de Smaple pour les texture multisample

        GLuint InternalFormat;//GL_RGBA8 GL_R16I...
        GLuint MipmapCount=1;//Nombre de mipmap � cr�er => 1 par default
        bool SharedTextureId=false;
        bool Imutable_Storage=false;
        bool ParametersDefined=false;//si on cr�e des sampler au lieu de cr�er des parametre il restera � faux

        //GLSampler
        bool CreateSampler;//indique que la texture va cr�er des sampler pour chaque commande d'�tat et le renvera pour une future R�activation
        std::vector<GLSampler> Samplers;

    public:
        BYTE* ByteMap(){return (BYTE*)Bitmap;};
        TextureAsset(GLuint type){TextureType=type;};
        TextureAsset(){};
        ~TextureAsset(){Destroy();};
        TextureAsset(string Filename){
            CREATED=true;
            FilePath=Filename;
            Name=Filename;
            FromFile=true;
        };
        //Fonction Create
            //1er �tat => Non Load� ni Vram ni RAM
                void CreateFromFile(string filename);
            //1er �tat dans Ram
                void CreateRAMEmptyTexture(GLuint byteperpixel,GLuint padding,GLuint colorformat,GLuint byteformat,GLuint gldatatype);
                void Create(void* Data,GLuint W,GLuint H,GLuint D,GLuint byteperpixel,GLuint colorformat,GLuint gldatatype,GLuint byteformat,GLuint padding=0,bool dont_copy_data=false);//Dims => pointeur toujours de taille 3
            //1er �tat => VRAM
                void CreateFromTextureId(GLuint TextureType,GLuint Id,bool Become_Owner,string name="");

                void CreateVRAMEmptyTexture(GLuint texturetype,GLuint* Dims,GLuint colorformat,GLuint internalformat,string name="");//Dims =[W,H,Depth]
        //Fonction h�rit�
        bool IsMutable();//renvoie true si les alt�ration sur les donn� on un sens
        bool IsBindable();
        bool MakeMutable();//Rend Les Donn� alt�rable
        bool UpdateDataMutation();//met � jour les donn� depuis l'endroit ou elle sont alt�rable
        bool UpdateBufferModification();//Met � jour depuis la m�moire d�di�
        //virtual bool Create(...);//fonction qui cr�e l'objet (avant le bool created est � false
        //la fonction create n'extiste pas vraiment mais elle est fortement sug�r�
        bool LoadRam();
        bool LoadVram();
        bool UnloadRam();
        bool UnloadVram();
        bool Bind(GLuint slot=0){return false;};
        bool Destroy();//D�truit Completement l'Objet :: renvoie true si l'op�ration r�ussi
        TextureAsset* Copy(){return NULL;};
        string AskName(){
            ConsoleEcho(("Texture Name:"+Name).c_str());
            return Name;};

        //Fonction de Load;
        bool LoadRAM_FromVRAM();
        bool LoadVRAM_FromRAM();
        bool LoadVRAM_FromFile();
        void GenerateMipmap();
        void DefineParameters();
        void ComputeDataSize();//Width,Depth ,Height,BytePerPixel et BytePadding doivent etre set

        void UpdateCreatedState(){CREATED=FromFile||VRAMLOADED||RAMLOADED;};
        //Fonction sp�cialis�
        void GetTextureInVRAMInfo();


        bool LoadFile();
        bool OpenFile();
        bool OpenBMPFromFile();
        bool UnloadFile();
        //fonction user
        void SetColorFormat(GLuint format);
        template<typename T>
        void ConvertBMP(const GLuint NewBytePerPixel,const GLuint NewBytePadding,const GLuint NewComponentNumber,const int* SwapPatern,const T* DefaulState,const int NewColorFormat,const int NewGLDataType);
        void SaveBMP(string Filename);
        void EchoTextureInfo(bool TextureData);//texture data rajoute les donn� de la texture dans ce qui est echo



};
