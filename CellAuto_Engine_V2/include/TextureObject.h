#pragma once
#include"Header.h"
enum{
TEX_DEFAULT=0,
TEX_BMP=1
};


class TextureObject{
private:
    //Texture
    BYTE * Bitmap;
    //proprité basique de la texture
    unsigned int width;
    unsigned int height;
    int BitPerPixel;
    int BytePerPixel;
    int ComponentNumber;
    int Format;
    GLenum GLColorFormat;//GL_RGB BGR ect
    GLenum GLDataFormat;//GL_UNSIGNED_BYTE et autre
    int BitmapSize=0;//Taille en BYTE
    int BitmapLength=0;//nombre de pixel
    //bool d'état
    bool Ready;
    bool Filled;
    //Info OGL
    bool Loaded=false;
    GLuint TextureIndex;
    int MimapNumber=0;

    //Info Image déclaré par les Formats
    bool Unpacked=false;//declaré par bmp
    short UnpackAlignement=1;


public:
    //get function
    GLuint GetTextureIndex(){return TextureIndex;};

    TextureObject(unsigned int widthA, unsigned int heightA,int BipPerPixel,int Format,int GLFormat);
    bool OpenBMP(const char* FileName);
    TextureObject(const char* FileName,int ForceMode=TEX_DEFAULT);
    bool Load(int Mipmap=-1,GLenum SpecialFormat=GL_RGBA);
    bool Bind(GLuint Slot);
    bool ConvertBMP(const GLuint ByteCount,const GLuint NewUnpackAlignement,const GLuint ComponentNumber,const int* SwapPattern,const BYTE* DefaultState,const int NewGLFormat,const int NewGLDataType);



protected:











};
