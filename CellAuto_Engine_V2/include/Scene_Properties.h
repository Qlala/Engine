#pragma once
#include "Header.h"
struct Scene_VertexProperty{
    string Name;
    GLuint Type;//GL_FLOAT or GL_UNSIGNED_INT
    GLuint Adress;
    GLuint ParamCount;//nombre de composante d'un objet/vertex
    GLuint ProgramForTrigger;
    string TiggerName;
    GLuint Trigger;//les triger sont forcément des uniform int
    GLuint TriggerValue;
    GLuint ComponentSize;//Taile d'un Vertex
    bool Triggered;
    void CallTrigger(){
        if(Triggered)glUniform1i(Trigger,TriggerValue);
    }
};
struct Scene_MeshGlobalData{
    string Name;
    GLuint AdressOfUniform;
    GLuint SizeOfElem;//taille d'un élément du buffer
    GLuint Padding;
    GLuint ProgramForTrigger;
    GLuint Trigger;
    GLuint TriggerValue;
    bool Triggered;
        void CallTrigger(){
        if(Triggered)glUniform1i(Trigger,TriggerValue);
    }
};

template <class ParentSceneType>
class Scene_Properties : public ParentSceneType{
    GLuint BaseGeometryVertex_VBOAdress;//adress des vertex base de la géometry d'un objet
    string VertexDimUniform_Name;
    GLuint ProgramForVertexUniform;
    GLuint VertexDimUniform;//Donne la Dimension du Vertex
    //PostProcess
    GLuint PostProcess_VertexAdress;
    GLuint PostProcess_VertexDimUniform;
    string PostProcess_VertexDimUniform_Name;
    //bool
    bool PostProcess_Input=false;//signal que le post process peut prendre des imput
    bool Properties_Defined=false;
    bool Properties_Initied=false;
    std::vector<Scene_VertexProperty> Property_Library;
    public:
        void InitProperties_FromIni(IniDecoder* ini);
        bool FillVBO_Property(Scene_VertexProperty* outProp);//La Scene property est partielment remplis et est completé par la scene
        //Doivent etre remplis : Name/Type/ParamCount
        void DeleteProperty(string Name);
        void InitTriggerUniform(RenderObject* ptr);


};
#include "../src/Scene_Properties.cpp"
