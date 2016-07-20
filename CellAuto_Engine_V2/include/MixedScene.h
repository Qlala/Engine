#pragma once
#include "Header.h"




class MixedScene : public Scene_Properties<Scene3d<float>>{
    float theta=0;

    //donnée de VBO pour la Scene
    GLuint VertexAdress;
    std::vector<Scene_VertexProperty> PropertiesArray;
    /*std::vector<string>PropertiesName;
    std::vector<GLuint>PropertiesAdress;
    std::vector<GLuint>PropertiesType;//GL_FLOAT or GL_UNSIGNED_INT;*/
    bool VBO_Data_Found=false;
    bool ShadersLoaded=false;

    bool UniformInitied=false;
    //Data pour les shader (trouvé dans l'ini)
    GLuint ProgramForMatrix;
    GLuint * RenderShaderList;
    GLuint RenderShaderN;

    GLuint ProgramForSampling;
    GLuint * PostProcessShaderList;
    GLuint PostProcessShaderN;

    //uniforms
    string FrameColorUniName;
    GLuint FrameColorLoc;
    AmbivalentUniform FrameColorAmbiLoc;

    string FrameDepthUniName;
    GLuint FrameDepthLoc;

    string FrameSizeUniName;
    GLuint FrameSizeUniLoc;
    string MSAA_SampleUniName;
    GLuint MSAA_SampleLoc;

    public:
        //Fonction d'init de la Scene
        MixedScene();
        void LoadFromIni(const char* Filename);
        void Intern_LoadFromIni(char* Filename);
        void Intern_LoadFromIni2(char* Filename);
        void InitUniform();
        //fonction du thread APC
        void CallSceneRoutine(){/*ConsoleEcho("SceneMixed");*/};
        //fonction de Comunication de scene avec les object
        GLuint GetPropertyVBOAdress(string Prop_Name,GLuint Prop_Type);
        GLuint GetVertexAdress(){return VertexAdress;};
        //Fonction de Rendu
        bool RenderInit(RenderObject* ptr);
        bool RenderUpdate(RenderObject* ptr);
        bool RenderPostProc(RenderObject* ptr);


};
