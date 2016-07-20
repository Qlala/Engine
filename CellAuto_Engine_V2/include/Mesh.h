#pragma once
#include "Header.h"
class MixedScene;
class MeshBeta{
    //position du mesh
    double x=0;
    double y=0;
    double z=0;
    double Thx=0;
    double Thy=0;
    double Thz=0;
    float ModelMatrix[16];
    //bool de verification
    bool BufferLoaded=false;
    bool VBOAdressSet=false;
    bool Ready=false;
    bool MeshMooved=false;
    bool Color=false;
    //Textured est prio sur couleur
    GLuint TextureOrColorProperty=0;
    //donné du mesh
    std::vector<float> VertexArray;
    std::vector<GLuint> IndexArray;
    std::vector<string> FloatPropertiesName;//il seront comparé avec ceux de la SCene pour pouvoir retrouvé eur adress
    std::vector<GLuint> FloatPropertiesGroupSize;
    std::vector<std::vector<float>> FloatPropertiesArray;
    std::vector<string> GLuintPropertiesName;
    std::vector<GLuint> GLuintPropertiesGroupSize;
    std::vector<std::vector<GLuint>> GLuintPropertiesArray;
    //buffer and callList
    GLuint CallList=0;
    GLuint VertexBuffer=0;
    GLuint IndexBuffer=0;
    //la Premier Propriété
    std::vector<GLuint> FloatPropertiesBuffer;
    std::vector<GLuint> GLuintPropertiesBuffer;
    //Texture
    //adressage Buffer
    GLuint VertexAdress;
    std::vector<GLuint> FloatPropertiesAdress;
    std::vector<GLuint> GLuintPropertiesAdress;
    public:
        //déplacement
        inline void Translate(double xa, double ya,double za){x+=xa;y+=ya;z+=za;MeshMooved=true;}
        inline void Rotate(double tx,double ty ,double tz){Thx+=tx;Thy+=ty;Thz+=z;MeshMooved=true;}
        //matrice
        void ProcessModelMatrix();
        //chargement mémoire
        void GlobalLoad(MixedScene* SC);
        void LoadBuffer();
        void LoadTexture();
        void GetVBOAdress(MixedScene* SC);
        void PrepareCallList();
        void CallDrawList();
        virtual void SetUniformForDrawing();
        virtual void DrawCalls();
        virtual void RawDrawCalls();


};
