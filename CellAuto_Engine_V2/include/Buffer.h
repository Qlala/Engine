#pragma once
#include "Header.h"

class Buffer : public AssetType {
    /*
    bool CREATED;
    bool RAMLOADED; //Exist donc Attention
    bool VRAMLOADED;*/

    //GLuint DataType;//utilise les tag GL_INT...
    //GLsizei Size;//Size en nombre d'�l�ment

    //RAM State
    void* Data;
    GLsizei DataSize;//taille en byte des donn�s

    //VRAM state gestion
    GLenum BufferType=GL_SHADER_STORAGE_BUFFER;//Target dans openGl
    GLuint BufferId;
    GLuint BufferUsage=GL_DYNAMIC_DRAW;//GL_DYNAMIC GL_STREAM
    GLuint BufferStorageFlags=GL_MAP_READ_BIT|GL_MAP_WRITE_BIT|GL_DYNAMIC_STORAGE_BIT;//GL_MAP_**
    bool BufferMaped=false;//on considert que si le buffer est map� il est map� dans la Ram de Data

    //File gestion
    bool FromFile=false;
    FILE* File;
    bool FileOpened=false;
    string FileName;
    //Nom
    string Name="Nom de Buffer par default";
    public:
    Buffer(GLenum type,GLuint usage=GL_DYNAMIC_DRAW,GLuint storageflags=GL_MAP_READ_BIT|GL_MAP_WRITE_BIT|GL_DYNAMIC_STORAGE_BIT): BufferType(type),BufferStorageFlags(storageflags), BufferUsage(usage){}
    Buffer(GLenum type,void*data,GLsizei dsize,string name="Nom de Buffer par default");
    Buffer(){};
    bool Create(GLenum type,void*data,GLsizei dsize,string name="Nom de Buffer par default");

    //void set Functiond
    void SetName(string part1,string part2="");
    void SetBufferUsage(GLuint usage);
    void SetBufferStorageFlags(GLuint flags);
    void AddBufferStorageFlags(GLuint flags);

    void SetDataToRam(void* DataPointer,GLsizei datasize);//le pointeur contenant les donn� est copi�
    void SetDataToVRam(GLuint bufferid);
    void SetDataToVRam(void* DataPointer,GLsizei datasize);
    void SetDataInfo(GLuint Pack_Count,size_t ElementSize);//
    void SetFileName(string FileName);

    //Fonction obligatoire ( h�ritage)
    bool IsMutable();//renvoie true si les alt�ration sur les donn� on un sens
    bool IsBindable();//ezncvoiz true si les donn� sont Bindable
    bool MakeBindable();
    bool MakeMutable();//Rend Les Donn� alt�rable
    bool UpdateBufferModification();
    bool UpdateDataMutation();//met � jour les donn� depuis l'endroit ou elle sont alt�rable
    bool LoadRam();
    bool LoadVram();
    bool UnloadRam();
    bool UnloadVram();
    bool Bind(GLuint slot=0);
    bool Destroy();//D�truit Completement l'Objet :: renvoie true si l'op�ration r�ussi
    Buffer* Copy();//Copie les donn�  et l'�tat
    ~Buffer(){Destroy();};
    string AskName(){return Name;};
    //GetFunction
    size_t GetDataSize(){return DataSize;};

    //Fonction de rafraichissemnt
    void CheckCreated();

    //Fonction des Load
    bool LoadVRAM_FromRAM();
    bool LoadRAM_FromVRAM();
    bool MapBuffer();

    //Gestion des Fichier
    void LoadFile();
    void CloseFile();
    bool LoadRAM_FromFile();
    bool LoadVRAM_FromFile();

};
