#include "Header.h"
Scene_VertexProperty* Mesh::PropertyItterator(GLuint i){
    if(i<Properties.size()){
        return static_cast<Scene_VertexProperty*>(Properties.data()+i);
    }
}
size_t Mesh::PropertyNumber(){return Properties.size();}

void Mesh::PropertyDefintion(GLuint i,bool state){
    if(i<Properties.size())Properties[i].Defined=state;
}

void Mesh::CreateFromIndicesTable(std::vector<GLuint> indices){
    AddIndices("Indices_Default",indices.data(),sizeof(GLuint),indices.size(),GL_UNSIGNED_INT);
    Ready=true;
}
void Mesh::AddIndices(string name,void* ptr,size_t SizeOfE,size_t Count,GLuint type){
    Indices_Group IG;
    IG.Name=name;
    IG.Count=Count;
    IG.EBO_buffer=CreateAsset<Buffer>(AskName()+"_"+name,GL_ELEMENT_ARRAY_BUFFER,ptr,Count*SizeOfE);
    MeshIndicesList.push_back(IG);
}

void Mesh::DrawIndices(std::vector<Indices_Group>::iterator Itt){
    if(Itt->EBO_buffer->IsBindable()){
        Itt->EBO_buffer->Bind(0);
        glDrawElements(GL_TRIANGLES,Itt->Count,Itt->DataType,NULL);
    }
}
void Mesh::DrawIndices(GLuint i){
    DrawIndices(MeshIndicesList.begin()+i);
}

bool Mesh::LoadRAM_Property(GLuint n){
    if(n<Properties.size()){
        std::vector<Mesh_VertexProperty>::iterator Itt=Properties.begin()+n;
        if(Itt->Defined){
            Itt->VBO_Asset->LoadRAM();
            return Itt->VBO_Asset->IsRAMLoaded();
        }else return false;
    }else return false;
};

bool Mesh::LoadVRAM_Property(GLuint n){
    if(n<Properties.size()){
        std::vector<Mesh_VertexProperty>::iterator Itt=Properties.begin()+n;
        if(Itt->Defined){
            Itt->VBO_Asset->LoadVRAM();
            return Itt->VBO_Asset->IsVRAMLoaded();
        }else return false;
    }else return false;
};
bool Mesh::UnloadRAM_Property(GLuint n){
    if(n<Properties.size()){
            std::vector<Mesh_VertexProperty>::iterator Itt=Properties.begin()+n;
            if(Itt->Defined){
                Itt->VBO_Asset->UnloadRAM();
                return !Itt->VBO_Asset->IsRAMLoaded();
            }else return false;
        }else return false;
}
bool Mesh::UnloadVRAM_Property(GLuint n){
    if(n<Properties.size()){
            std::vector<Mesh_VertexProperty>::iterator Itt=Properties.begin()+n;
            if(Itt->Defined){
                Itt->VBO_Asset->UnloadVRAM();
                return !Itt->VBO_Asset->IsVRAMLoaded();
            }else return false;
        }else return false;
}

void Mesh::DrawProperty(GLuint i){
    DrawProperty(Properties.begin()+i);
}

void Mesh::DrawProperty(std::vector<Mesh_VertexProperty>::iterator Itt){
    if(Itt->VBO_Asset->IsBindable()){
        Itt->VBO_Asset->Bind(0);
        glVertexAttribPointer(Itt->Adress,Itt->ComponentSize*Itt->ParamCount,Itt->Type,false,0,NULL);
        if(Itt->Triggered)
            glUniform1i(Itt->Trigger,Itt->TriggerValue);
    }
}

void Mesh::DrawProperties(GLuint* tab,GLuint n){
    for(GLuint i=0;i<n;i++){
        if(i<Properties.size())DrawProperty(i);
    }
}
