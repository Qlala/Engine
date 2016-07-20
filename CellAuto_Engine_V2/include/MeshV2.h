#pragma once
#include "Header.h"
struct Mesh_VertexProperty : Scene_VertexProperty{
    bool Defined=false;
    Asset<Buffer>* VBO_Asset;
};
struct Indices_Group{
    string Name;
    Asset<Buffer>* EBO_buffer;
    size_t Count;
    GLuint DataType;//type de d'indices GL_UNSIGNED_INT GL_UNSIGNED_SHORT
};


template<typename T>
class Mesh_DataType{
    T X;
    T Y;
    T Z;
    T TH_X;
    T TH_Y;
    T TH_Z;
    bool Coord_Updated=false;
    Mat<4,T> ModelMat;
    GLuint ModelMatrixLoc=-1;
    public:
    bool IsMatrixSet(){return ModelMatrixLoc!=-1;};
    void Translate(T x,T y,T z){X+=x;Y+=y;Z+=z;Coord_Updated=true;};
    void Rotate(T thx,T thy,T thz){TH_X+=thx;TH_Y+=thy;TH_Z+=thz;Coord_Updated=true;};
    void MoveTo(T x,T y,T z){X=x;Y=y;Z=z;Coord_Updated=true;};
    void RotateTo(T thx,T thy,T thz){TH_X=thx;TH_Y=thy;TH_Z=thz;Coord_Updated=true;};
    void PresetModelMatrix(GLuint loc){ModelMatrixLoc=loc;};
    void ProcessMatrix(){
        ModelMat=WorldMatrix<T>::ModelMatrix(X,Y,Z,TH_X,TH_Y,TH_Z);
    }
    void SetMatrix_4D(Mat<4,T> ModelMat,GLuint loc){
        glUniformMatrix4fv(loc,1,true,(T*)(ModelMat));
    }
    void UpdateMatrix(){
        if(Coord_Updated){
            ProcessMatrix();
            SetMatrix_4D(ModelMat,ModelMatrixLoc);
        }
    }
};


class Mesh: public Mesh_DataType<float>{

    std::vector<Mesh_VertexProperty> Properties;//ne sont juste les donné du mesh
    std::vector<Indices_Group> MeshIndicesList;//j"hésite à prévoir plusieur tableau d'indices; et bah je vais le faire lol
    size_t IndicesSize=0;

    size_t Vertex_Number=0;//Vertex de plus haut rang
    bool Ready=false;
    bool RAM_Loaded=false;
    bool VRAM_Loaded=false;
    string Name="Mesh_Default_Name";
    public:
        //Constructeur
        Mesh(string Filename);
        Mesh();
        void CreateFromFile(string file);
        void CreateFromIndicesTable(std::vector<GLuint> indices);


        //
        string AskName(){return Name;};
        void SetName(string name);


        void LoadRAM();
        void LoadVRAM();
        void UnloadVRAM();
        void UnloadRAM();
        //inddices function
        void AddIndices(string name,void* ptr,size_t SizeOfE,size_t Count,GLuint type);//sizeOfE taille d'un élément ;; Count nombre d'indices (pas de triangles especes de mongole)

        //Property_Function
        Scene_VertexProperty* PropertyItterator(GLuint I);
        void PropertyDefintion(GLuint i,bool state);//valid la property du mesh
        size_t PropertyNumber();
        bool LoadRAM_Property(GLuint n);
        bool LoadVRAM_Property(GLuint n);
        bool UnloadRAM_Property(GLuint n);
        bool UnloadVRAM_Property(GLuint n);//retourne vrais si est réussite
        template<typename T>
        void AddProperty(string Name,GLuint Type,GLuint ParamCount,std::vector<T> data);//on va verfié à chaque fois qu'il y a assez de donné => nombre de vertex correspond aux indices
        //Dispay
        void DrawProperty(GLuint i);
        void DrawProperty(std::vector<Mesh_VertexProperty>::iterator Itt);
        void DrawProperties(GLuint* tab,GLuint n);
        void DrawIndices(std::vector<Indices_Group>::iterator Itt);
        void DrawIndices(GLuint i);
        //Destructeurmun
        ~Mesh();

};

#include "../src/MeshV2-template.cpp"
