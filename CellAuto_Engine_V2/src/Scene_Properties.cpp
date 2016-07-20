#pragma once
#include "Header.h"
template <class ParentSceneType>
void Scene_Properties<ParentSceneType>::InitProperties_FromIni(IniDecoder* ini){
    Property_Library=ConvertAll<Scene_VertexProperty>(ini->SearchProps("VBO_Definition.Property"),{"PropertyName","PropertyType","PropertyAdress","ParametersCount","ProgramForTrigger","Trigger","TriggerValue","Triggered"},{INI_STRING,INI_GL_TYPE,INI_UNSIGNED_INT,INI_UNSIGNED_INT,INI_UNSIGNED_INT,INI_STRING,INI_EMPTY_INT,INI_UNSIGNED_INT,INI_BOOL});
    for(std::vector<Scene_VertexProperty>::iterator Itt=Property_Library.begin();Itt!=Property_Library.end();Itt++){
        Itt->ComponentSize=GLtool::GetGL_DataTypeSize(Itt->Type);
    }
    Properties_Defined=true;
    BaseGeometryVertex_VBOAdress=(GLuint)(ini->SearchProp("VBO_Definition").Search("VertexAdress"));
    VertexDimUniform_Name=(string)(ini->SearchProp("VBO_Definition").Search("VertexNumberUniform"));
    ProgramForVertexUniform=(GLuint)(ini->SearchProp("VBO_Definition").Search("ProgramForUniform"));
}

template <typename ParentSceneType>
void Scene_Properties<ParentSceneType>::InitTriggerUniform(RenderObject* ptr){
    if(Properties_Defined){
        for(std::vector<Scene_VertexProperty>::iterator Itt=Property_Library.begin();Itt!=Property_Library.end();Itt++){
            if(Itt->Triggered){
                Itt->Trigger=ptr->GetLocation(Itt->TiggerName,ParentSceneType::RealProgram(Itt->ProgramForTrigger));
            }
        }

    }
    VertexDimUniform=ptr->GetLocation(VertexDimUniform_Name,ParentSceneType::RealProgram(ProgramForVertexUniform));
};
template <typename ParentSceneType>
bool Scene_Properties<ParentSceneType>::FillVBO_Property(Scene_VertexProperty* outProp){
    bool found=false;
    for(std::vector<Scene_VertexProperty>::iterator Itt=Property_Library.begin();Itt!=Property_Library.end();Itt++){
        if(outProp->Name==Itt->Name && outProp->Type==Itt->Type && outProp->ParamCount==Itt->ParamCount){
            *outProp=(*Itt);
            found=true;
            break;
        }
    }
    return found;
}
template <typename ParentSceneType>
void Scene_Properties<ParentSceneType>::DeleteProperty(string name){
    for(std::vector<Scene_VertexProperty>::iterator Itt=Property_Library.begin();Itt!=Property_Library.end();Itt++){
        if(Itt->Name==name){
            Property_Library.erase(Itt);
        }

    }
}
