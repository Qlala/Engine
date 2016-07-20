#pragma once
#include "Header.h"

template<typename T>
void Mesh::AddProperty(string name,GLuint type,GLuint ParamCount,std::vector<T> data){
    Mesh_VertexProperty Np;
    Np.Name=name;
    Np.Type=type;
    Np.ParamCount=ParamCount;
    Np.VBO_Asset=CreateAsset(AskName()+"_"+name,GL_ARRAY_BUFFER,data.data(),data.size()*sizeof(data));
};
