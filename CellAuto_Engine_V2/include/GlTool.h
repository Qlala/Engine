#pragma once
#include "Header.h"
extern void glBindTextureToSlot(GLuint Tex,GLuint Type,GLuint Slot);
struct Color_ByteFormat{
    GLuint ByteFormat;//Gl_RGBA8 ...
    GLuint ColorFormat;//GL_RED .. GL_RG
    GLuint ComponentNumber;//1 ,2 ,3 ,4
    GLuint BitPerPixel;//
    GLuint GLDataType;//GL_INT .. GL_FLOAT
    bool Normalize;// format de type Snorm
    bool FloatingPoint;//Format avec un F à la fin
    GLuint SharedByte;//bite partagé
    std::vector<GLuint> ColorActualBitFormat;//décomposition des Bytes selon les différent couleur

};
class GLtool{
    static std::vector<Color_ByteFormat> FormatList;
    public:

        static Color_ByteFormat GetColor_ByteFormat(GLuint internalformat){
            std::vector<Color_ByteFormat>::iterator Itt=FormatList.begin();
            while(Itt->ByteFormat!=internalformat && Itt!=FormatList.end()-1)Itt++;
            return (*Itt);
        };
        static GLuint DecodeGL_Type(string strA);

        static GLuint DecodeGL_Texture(string strA);
        static GLuint DecodeGL_Shader(string strA);
        static GLuint GetDedicated_ShaderBit(GLuint shader);
        static GLuint DecodeGL_TexFormat(string strA);
        static GLuint GetGL_DataTypeSize(GLuint type);
};



