#include "Header.h"
void glBindTextureToSlot(GLuint Tex,GLuint Type,GLuint SLot){
    glActiveTexture(GL_TEXTURE0+SLot);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(Type,Tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(Type,0);
}
GLuint GLtool::DecodeGL_Type(string strA){
    ConsoleEcho("Décode:");
    ConsoleEcho(strA.c_str());
    if(strA.compare("GL_BYTE")==0){
        return GL_BYTE;
    }else if(strA.compare("GL_UNISGNED_BYTE")==0){
        return GL_UNSIGNED_BYTE;
    }else if(strA.compare("GL_INT")==0){
        return GL_INT;
    }else if(strA.compare("GL_UNISGNED_INT")==0){
        return GL_UNSIGNED_INT;
    }else if(strA.compare("GL_FLOAT")==0){
        return GL_FLOAT;
    }else if(strA.compare("GL_DOUBLE")==0){
        return GL_DOUBLE;
    }else if(strA.compare("GL_HALF_FLOAT")==0){
        return GL_HALF_FLOAT;
    }else if(strA.compare("GL_UNSIGNED_SHORT")==0){
        return GL_UNSIGNED_SHORT;
    }else if(strA.compare("GL_SHORT")==0){
        return GL_SHORT;
    }else{
        return 0;
    }
}
GLuint GLtool::DecodeGL_TexFormat(string strA){
    ConsoleEcho("Décode:");
    ConsoleEcho(strA.c_str());
    if(strA.compare("GL_RGBA")==0){
        return GL_RGBA;
    }else if(strA.compare("GL_RED")==0){
        return GL_RED;
    }else if(strA.compare("GL_RGB")==0){
        return GL_RGB;
    }else if(strA.compare("GL_RG")==0){
        return GL_RG;
    }else if(strA.compare("GL_DEPTH_COMPONENT")==0){
        return GL_DEPTH_COMPONENT;
    }else if(strA.compare("GL_STENCTIL_INDEX")==0){
        return GL_STENCIL_INDEX;
    }else if(strA.compare("GL_DEPTH_STENCIL")==0){
        return GL_DEPTH_STENCIL;
    }else if(strA.compare("GL_BGRA")==0){
        return GL_BGRA;
    }else if(strA.compare("GL_BGR")==0){
        return GL_BGR;
    }else if(strA.compare("GL_RED_INTEGER")==0){
        return GL_RED_INTEGER;
    }else if(strA.compare("GL_RG_INTEGER")==0){
        return GL_RG_INTEGER;
    }else if(strA.compare("GL_RGB_INTEGER")==0){
        return GL_RGB_INTEGER;
    }else if(strA.compare("GL_RGBA_INTEGER")==0){
        return GL_RGBA_INTEGER;
    }else if(strA.compare("GL_BGR_INTEGER")==0){
        return GL_BGR_INTEGER;
    }else if(strA.compare("GL_BGRA_INTEGER")==0){
        return GL_BGRA_INTEGER;
    }else{
        return 0;
    }
}
GLuint GLtool::DecodeGL_Shader(string strA){
    if(strA.compare("GL_VERTEX_SHADER")==0){
        return GL_VERTEX_SHADER;
    }else if(strA.compare("GL_GEOMETRY_SHADER")==0){
        return GL_GEOMETRY_SHADER;
    }else if(strA.compare("GL_FRAGMENT_SHADER")==0){
        return GL_FRAGMENT_SHADER;
    }else if(strA.compare("GL_TESS_CONTROL_SHADER")==0){
        return GL_TESS_CONTROL_SHADER;
    }else if(strA.compare("GL_TESS_EVALUATION_SHADER")==0){
        return GL_TESS_EVALUATION_SHADER;
    }else if(strA.compare("GL_COMPUTE_SHADER")==0){
        return GL_COMPUTE_SHADER;
    }else{
        return 0;
    }
}

GLuint GLtool::GetDedicated_ShaderBit(GLuint shader){
    switch(shader){
        case GL_VERTEX_SHADER:
            return GL_VERTEX_SHADER_BIT;
        break;
        case GL_GEOMETRY_SHADER:
            return GL_GEOMETRY_SHADER_BIT;
        break;
        case GL_FRAGMENT_SHADER:
            return GL_FRAGMENT_SHADER_BIT;
        break;
        case GL_TESS_CONTROL_SHADER:
            return GL_TESS_CONTROL_SHADER_BIT;
        break;
        case GL_TESS_EVALUATION_SHADER:
            return GL_TESS_EVALUATION_SHADER_BIT;
        break;
        case GL_COMPUTE_SHADER:
            return GL_COMPUTE_SHADER_BIT;
        break;
        default:
            return 0;

    }
}

GLuint GLtool::DecodeGL_Texture(string strA){
    if(strA.compare("GL_R8")==0){
        return GL_R8;
    }else if(strA.compare("GL_R8_SNORM")==0){
        return GL_R8_SNORM;
    }else if(strA.compare("GL_R16")==0){
        return GL_R16;
    }else if(strA.compare("GL_R16_SNORM")==0){
        return GL_R16_SNORM;
    }else if(strA.compare("GL_RG8")==0){
        return GL_RG8;
    }else if(strA.compare("GL_RG8_SNORM")==0){
        return GL_RG8_SNORM;
    }else if(strA.compare("GL_RG16")==0){
        return GL_RG16;
    }else if(strA.compare("GL_RG16_SNORM")==0){
        return GL_RG16_SNORM;
    }else if(strA.compare("GL_RGB4")==0){
        return GL_RGB4;
    }else if(strA.compare("GL_RGB5")==0){
        return GL_RGB5;
    }else if(strA.compare("GL_RGB565")==0){
        return GL_RGB565;
    }else if(strA.compare("GL_RGB8")==0){
        return GL_RGB8;
    }else if(strA.compare("GL_RGB8_SNORM")==0){
        return GL_RGB8_SNORM;
    }else if(strA.compare("GL_RGB10")==0){
        return GL_RGB10;
    }else if(strA.compare("GL_RGB12")==0){
        return GL_RGB12;
    }else if(strA.compare("GL_RGB16")==0){
        return GL_RGB16;
    }else if(strA.compare("GL_RGB16_SNORM")==0){
        return GL_RGB16_SNORM;
    }else if(strA.compare("GL_RGBA2")==0){
        return GL_RGBA2;
    }else if(strA.compare("GL_RGBA4")==0){
        return GL_RGBA4;
    }else if(strA.compare("GL_RGB5_A1")==0){
        return GL_RGB5_A1;
    }else if(strA.compare("GL_RGBA8")==0){
        return GL_RGBA8;
    }else if(strA.compare("GL_RGBA8_SNORM")==0){
        return GL_RGBA8_SNORM;
    }else if(strA.compare("GL_RGB10_A2")==0){
        return GL_RGB10_A2;
    }else if(strA.compare("GL_RGB10_A2UI")==0){
        return GL_RGB10_A2UI;
    }else if(strA.compare("GL_RGBA12")==0){
        return GL_RGBA12;
    }else if(strA.compare("GL_RGBA16")==0){
        return GL_RGBA16;
    }else if(strA.compare("GL_RGBA16_SNORM")==0){
        return GL_RGBA16_SNORM;
    }else if(strA.compare("GL_SRGB8")==0){
        return GL_SRGB8;
    }else if(strA.compare("GL_SRGB8_ALPHA8")==0){
        return GL_SRGB8_ALPHA8;
    }else if(strA.compare("GL_R16F")==0){
        return GL_R16F;
    }else if(strA.compare("GL_RG16F")==0){
        return GL_RG16F;
    }else if(strA.compare("GL_RGB16F")==0){
        return GL_RGB16F;
    }else if(strA.compare("GL_RGBA16F")==0){
        return GL_RGBA16F;
    }else if(strA.compare("GL_R32F")==0){
        return GL_R32F;
    }else if(strA.compare("GL_RG32F")==0){
        return GL_RG32F;
    }else if(strA.compare("GL_RGB32F")==0){
        return GL_RG32F;
    }else if(strA.compare("GL_RGBA32F")==0){
        return GL_RGBA32F;
    }else if(strA.compare("GL_R11F_G11F_B10F")==0){
        return GL_R11F_G11F_B10F;
    }else if(strA.compare("GL_RGB9_E5")==0){
        return GL_RGB9_E5;
    }else if(strA.compare("GL_R8I")==0){
        return GL_R8I;
    }else if(strA.compare("GL_R8UI")==0){
        return GL_R8UI;
    }else if(strA.compare("GL_R16I")==0){
        return GL_R16I;
    }else if(strA.compare("GL_R16UI")==0){
        return GL_R16UI;
    }else if(strA.compare("GL_R32I")==0){
        return GL_R32I;
    }else if(strA.compare("GL_R32UI")==0){
        return GL_R32UI;
    }else if(strA.compare("GL_RG8I")==0){
        return GL_RG8I;
    }else if(strA.compare("GL_RG8UI")==0){
        return GL_RG8UI;
    }else if(strA.compare("GL_RG16I")==0){
        return GL_RG16I;
    }else if(strA.compare("GL_RG16UI")==0){
        return GL_RG16UI;
    }else if(strA.compare("GL_RG32I")==0){
        return GL_RG32I;
    }else if(strA.compare("GL_RG32UI")==0){
        return GL_RG32UI;
    }else if(strA.compare("GL_RGB8I")==0){
        return GL_RGB8I;
    }else if(strA.compare("GL_RGB8UI")==0){
        return GL_RGB8UI;
    }else if(strA.compare("GL_RGB16I")==0){
        return GL_RGB16I;
    }else if(strA.compare("GL_RGB16UI")==0){
        return GL_RGB16UI;
    }else if(strA.compare("GL_RGB32I")==0){
        return GL_RGB32I;
    }else if(strA.compare("GL_RGB32UI")==0){
        return GL_RGB32UI;
    }else if(strA.compare("GL_RGBA8I")==0){
        return GL_RGBA8I;
    }else if(strA.compare("GL_RGBA8UI")==0){
        return GL_RGBA8UI;
    }else if(strA.compare("GL_RGBA16I")==0){
        return GL_RGBA16I;
    }else if(strA.compare("GL_RGBA16UI")==0){
        return GL_RGBA16UI;
    }else if(strA.compare("GL_RGBA32I")==0){
        return GL_RGBA32I;
    }else if(strA.compare("GL_RGBA32UI")==0){
        return GL_RGBA32UI;
    }else{
        return 0;
    }
}
GLuint GLtool::GetGL_DataTypeSize(GLuint type){
    switch(type){
        case GL_BYTE:
            return 1;
        break;
        case GL_UNSIGNED_BYTE:
            return 1;
        break;
        case GL_SHORT:
            return 2;
        break;
        case GL_UNSIGNED_SHORT:
            return 2;
        break;
        case GL_INT:
            return 4;
        break;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_FLOAT:
            return 4;
        break;
        case GL_HALF_FLOAT:
            return 2;
        break;
        case GL_DOUBLE:
            return 8;
        break;
        default:
            return 0;
    }
}


std::vector<Color_ByteFormat> GLtool::FormatList={
            {GL_R8,GL_RED,1,8,GL_UNSIGNED_BYTE,false,false,0,{8}},
            {GL_R8_SNORM,GL_RED,1,8,GL_BYTE,true,false,0,{8}},
            {GL_R16,GL_RED,1,16,GL_UNSIGNED_SHORT,false,false,0,{16}},
            {GL_R16_SNORM,GL_RED,1,16,GL_SHORT,true,false,0,{16}},
            {GL_RG8,GL_RG,2,16,GL_UNSIGNED_BYTE,false,false,0,{8,8}},
            {GL_RG8_SNORM,GL_RG,2,16,GL_BYTE,true,false,0,{8,8}},
            {GL_RG16,GL_RG,2,32,GL_UNSIGNED_SHORT,false,false,0,{16,16}},
            {GL_RG16_SNORM,GL_RG,2,32,GL_SHORT,true,false,0,{16,16}},
            {GL_R3_G3_B2,GL_RGB,3,8,GL_UNSIGNED_BYTE_3_3_2,false,false,0,{3,3,2}},
            {GL_RGB4,GL_RGB,3,16,GL_UNSIGNED_SHORT_4_4_4_4,false,false,0,{4,4,4}},
            {GL_RGB5,GL_RGB,3,16,GL_UNSIGNED_SHORT_4_4_4_4,false,false,0,{5,5,5}},
            {GL_RGB565,GL_RGB,3,16,GL_UNSIGNED_SHORT_5_6_5,false,false,0,{5,6,5}},
            {GL_RGB8,GL_RGB,3,24,GL_UNSIGNED_BYTE,false,false,0,{8,8,8}},
            {GL_RGB8_SNORM,GL_RGB,3,24,GL_BYTE,true,false,0,{8,8,8}},
            {GL_RGB10,GL_RGB,3,32,GL_UNSIGNED_INT_10_10_10_2,false,false,0,{10,10,10}},
            {GL_RGB12,GL_RGB,3,48,GL_UNSIGNED_INT,false,false,0,{12,12,12}},
            {GL_RGB16,GL_RGB,3,48,GL_UNSIGNED_INT,false,false,0,{16,16,16}},
            {GL_RGB16_SNORM,GL_RGB,3,48,GL_INT,false,false,0,{16,16,16}},
            {GL_RGBA2,GL_RGBA,4,8,GL_UNSIGNED_BYTE,false,false,0,{2,2,2,2}},
            {GL_RGBA4,GL_RGBA,4,16,GL_UNSIGNED_SHORT_4_4_4_4,false,false,0,{4,4,4,4}},
            {GL_RGB5_A1,GL_RGBA,4,16,GL_UNSIGNED_SHORT_5_5_5_1,false,false,0,{5,5,5,1}},
            {GL_RGBA8,GL_RGBA,4,32,GL_UNSIGNED_BYTE,false,false,0,{8,8,8,8}},
            {GL_RGBA8_SNORM,GL_RGBA,4,32,GL_BYTE,true,false,0,{8,8,8,8}},
            {GL_RGB10_A2,GL_RGBA,4,32,GL_UNSIGNED_INT_10_10_10_2,false,false,0,{10,10,10,2}},
            {GL_RGB10_A2UI,GL_RGBA_INTEGER,4,32,GL_UNSIGNED_INT_10_10_10_2,false,false,0,{10,10,10,2}},
            {GL_RGBA12,GL_RGBA,4,48,GL_UNSIGNED_INT,false,false,0,{12,12,12,12}},
            {GL_RGBA16,GL_RGBA,4,64,GL_UNSIGNED_INT,false,false,0,{16,16,16,16}},
            {GL_RGBA16_SNORM,GL_RGBA,4,64,GL_INT,true,false,0,{16,16,16,16}},
            {GL_SRGB8,GL_RGB,3,24,GL_UNSIGNED_INT_8_8_8_8,false,false,0,{8,8,8}},
            {GL_SRGB8_ALPHA8,GL_RGBA,4,32,GL_UNSIGNED_INT_8_8_8_8,false,false,0,{8,8,8,8}},
            {GL_R16F,GL_RED,1,16,GL_HALF_FLOAT,false,true,0,{16}},
            {GL_RG16F,GL_RG,2,32,GL_HALF_FLOAT,false,true,0,{16,16}},
            {GL_RGB16F,GL_RGB,3,48,GL_HALF_FLOAT,false,true,0,{16,16,16}},
            {GL_RGBA16F,GL_RGBA,4,64,GL_HALF_FLOAT,false,true,0,{16,16,16,16}},
            {GL_R32F,GL_RED,1,32,GL_FLOAT,false,true,0,{32}},
            {GL_RG32F,GL_RG,2,64,GL_FLOAT,false,true,0,{32,32}},
            {GL_RGB32F,GL_RGB,3,96,GL_FLOAT,false,true,0,{32,32,32}},
            {GL_RGBA32F,GL_RGBA,4,128,GL_FLOAT,false,true,0,{32,32,32,32}},
            {GL_R11F_G11F_B10F,GL_RGB,3,32,GL_UNSIGNED_INT_10F_11F_11F_REV,false,true,0,{11,11,10}},
            {GL_RGB9_E5,GL_RGB,3,32,GL_UNSIGNED_INT_5_9_9_9_REV,false,false,5,{9,9,9}},
            {GL_R8I,GL_RED_INTEGER,1,8,GL_BYTE,false,false,0,{8}},
            {GL_R8UI,GL_RED_INTEGER,1,8,GL_UNSIGNED_BYTE,false,false,0,{8}},
            {GL_R16I,GL_RED_INTEGER,1,16,GL_SHORT,false,false,0,{16}},
            {GL_R16UI,GL_RED_INTEGER,1,16,GL_UNSIGNED_SHORT,false,false,0,{16}},
            {GL_R32I,GL_RED_INTEGER,1,32,GL_INT,false,false,0,{32}},
            {GL_R32UI,GL_RED_INTEGER,1,32,GL_UNSIGNED_INT,false,false,0,{32}},
            {GL_RG8I,GL_RG_INTEGER,2,16,GL_BYTE,false,false,0,{8,8}},
            {GL_RG8UI,GL_RG_INTEGER,2,16,GL_UNSIGNED_BYTE,false,false,0,{8,8}},
            {GL_RG16I,GL_RG_INTEGER,2,32,GL_SHORT,false,false,0,{16,16}},
            {GL_RG16UI,GL_RG_INTEGER,2,32,GL_UNSIGNED_SHORT,false,false,0,{16,16}},
            {GL_RG32I,GL_RG_INTEGER,2,64,GL_INT,false,false,0,{32,32}},
            {GL_RG32UI,GL_RG_INTEGER,2,64,GL_UNSIGNED_INT,false,false,0,{32,32}},
            {GL_RGB8I,GL_RGB_INTEGER,3,24,GL_BYTE,false,false,0,{8,8,8}},
            {GL_RGB8UI,GL_RGB_INTEGER,3,24,GL_UNSIGNED_BYTE,false,false,0,{8,8,8}},
            {GL_RGB16I,GL_RGB_INTEGER,3,48,GL_SHORT,false,false,0,{16,16,16}},
            {GL_RGB16UI,GL_RGB_INTEGER,3,48,GL_UNSIGNED_SHORT,false,false,0,{16,16,16}},
            {GL_RGB32I,GL_RGB_INTEGER,3,96,GL_INT,false,false,0,{32,32,32}},
            {GL_RGB32UI,GL_RGB_INTEGER,3,96,GL_UNSIGNED_INT,false,false,0,{32,32,32}},
            {GL_RGB8I,GL_RGBA_INTEGER,4,32,GL_BYTE,false,false,0,{8,8,8,8}},
            {GL_RGB8UI,GL_RGBA_INTEGER,4,32,GL_UNSIGNED_BYTE,false,false,0,{8,8,8,8}},
            {GL_RGB16I,GL_RGBA_INTEGER,4,48,GL_SHORT,false,false,0,{16,16,16,16}},
            {GL_RGB16UI,GL_RGBA_INTEGER,4,48,GL_UNSIGNED_SHORT,false,false,0,{16,16,16,16}},
            {GL_RGB32I,GL_RGBA_INTEGER,4,128,GL_INT,false,false,0,{32,32,32,32}},
            {GL_RGB32UI,GL_RGBA_INTEGER,4,128,GL_UNSIGNED_INT,false,false,0,{32,32,32,32}},
            {0,0,0,0,0,false,false,0,{0}}
            };

