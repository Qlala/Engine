#pragma once
#include "Header.h"

class Mesh_Scene : public Scene_Properties<Scene3d<float>>{
    //
    std::vector<Mesh> MeshTable;

    AddMesh(Mesh* M);
    RemoveMesh(GLuint index);
    Fill_MeshProperties(Mesh* M){
        size_t s=M->PropertyNumber();
        for(GLuint i=0;i<s;i++){
            M->PropertyDefintion(i,FillVBO_Property(M->PropertyItterator(i)));
        }
    }


};

