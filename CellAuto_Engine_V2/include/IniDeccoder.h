#pragma once
#include "Header.h"

struct IniVar{
    string Name;
    string Value;
    operator bool();
    operator char();
    operator unsigned char();
    operator short();
    operator unsigned short();
    operator int();
    operator unsigned int();
    operator float();
    operator double();
    operator string();
    operator long();
    operator unsigned long();
    operator long long();
    operator unsigned long long();
    template<typename T>
    operator std::vector<T>();
    template<typename T>
    std::vector<T> ConvertVector();
    GLuint ConvertGL_Const(GLuint (func)(string));
};
template<typename T>
extern T ConvertOnce(IniVar val);
struct A{
    float a;
    GLuint b;
};


enum IniType: unsigned char{
    INI_EMPTY_BOOL=0x10,
    INI_EMPTY_BYTE=0x20,
    INI_EMPTY_SHORT=0x30,
    INI_EMPTY_INT=0x40,
    INI_EMPTY_LONG=0x50,
    INI_EMPTY_FLOAT=0x70,
    INI_EMPTY_DOUBLE=0x80,
    INI_EMPTY_STRING=0x90,
    INI_BOOL=0x01,
    INI_CHAR=0x02,
    INI_UNSIGNED_CHAR=0x82,
    INI_SHORT=0x03,
    INI_UNSIGNED_SHORT=0x83,
    INI_INT=0x04,
    INI_UNSIGNED_INT=0x84,
    INI_LONG=0x05,
    INI_UNSIGNED_LONG=0x85,
    INI_LONG_LONG=0x06,
    INI_UNSIGNED_LONG_LONG=0x86,
    INI_FLOAT=0x07,
    INI_DOUBLE=0x08,
    INI_STRING=0x09,
    INI_GL_TYPE=0x0A,
    INI_GL_TEX=0x0B,
    INI_GL_TEX_FORMAT=0x0C,
    INI_GL_SHADER=0x0D//les normaux et pas les bits
};

struct IniProp{
    string Name;
    std::vector<IniVar> Vars;
    IniVar Search(string format);
    void Filter(std::vector<string> order);
    void EchoData();

    template<typename S>
    S Convert(std::vector<IniType> structformat){
        S structmem;
        void* structout=(void*)&structmem;
        size_t sizeIncrement=0;
        std::vector<IniVar>::iterator VarItt=Vars.begin();
        for(std::vector<IniType>::iterator Itt=structformat.begin();Itt!=structformat.end();Itt++){
            if(VarItt!=Vars.end()){
                switch(*Itt){
                    case INI_EMPTY_BOOL:
                        sizeIncrement+=sizeof(bool);
                        structout=reinterpret_cast<void*>(reinterpret_cast<bool*>(structout)+1);
                    break;
                    case INI_EMPTY_BYTE:
                        sizeIncrement+=sizeof(char);
                        structout=reinterpret_cast<void*>(reinterpret_cast<char*>(structout)+1);
                    break;
                    case INI_EMPTY_SHORT:
                        sizeIncrement+=sizeof(short);
                        structout=reinterpret_cast<void*>(reinterpret_cast<short*>(structout)+1);
                    break;
                    case INI_EMPTY_INT:
                        sizeIncrement+=sizeof(int);
                        structout=reinterpret_cast<void*>(reinterpret_cast<int*>(structout)+1);
                    break;
                    case INI_EMPTY_LONG:
                        sizeIncrement+=sizeof(long);
                        structout=reinterpret_cast<void*>(reinterpret_cast<long*>(structout)+1);
                    break;
                    case INI_EMPTY_FLOAT:
                        sizeIncrement+=sizeof(float);
                        structout=reinterpret_cast<void*>(reinterpret_cast<float*>(structout)+1);
                    break;
                    case INI_EMPTY_DOUBLE:
                        sizeIncrement+=sizeof(double);
                        structout=reinterpret_cast<void*>(reinterpret_cast<double*>(structout)+1);
                    break;
                    case INI_EMPTY_STRING:
                        sizeIncrement+=sizeof(string);
                        structout=reinterpret_cast<void*>(reinterpret_cast<string*>(structout)+1);
                    break;
                    case INI_BOOL:
                        *(reinterpret_cast<bool*>(structout))=(bool)(*VarItt);
                        sizeIncrement+=sizeof(bool);
                        structout=reinterpret_cast<void*>(reinterpret_cast<bool*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_CHAR:
                        *(reinterpret_cast<char*>(structout))=(char)(*VarItt);
                        sizeIncrement+=sizeof(char);
                        structout=reinterpret_cast<void*>(static_cast<char*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_UNSIGNED_CHAR:
                        *(reinterpret_cast<unsigned char*>(structout))=(unsigned char)(*VarItt);
                        sizeIncrement+=sizeof(unsigned char);
                        structout=reinterpret_cast<void*>(static_cast<unsigned char*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_SHORT:
                        *(reinterpret_cast<short*>(structout))=(short)(*VarItt);
                        sizeIncrement+=sizeof(short);
                        structout=reinterpret_cast<void*>(static_cast<short*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_UNSIGNED_SHORT:
                        *(reinterpret_cast<unsigned short*>(structout))=(unsigned short)(*VarItt);
                        sizeIncrement+=sizeof(unsigned short);
                        structout=reinterpret_cast<void*>(static_cast<unsigned short*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_INT:
                        *(reinterpret_cast<int*>(structout))=(int)(*VarItt);
                        sizeIncrement+=sizeof(int);
                        structout=reinterpret_cast<void*>(static_cast<int*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_UNSIGNED_INT:
                        *(reinterpret_cast<unsigned int*>(structout))=(unsigned int)(*VarItt);
                        sizeIncrement+=sizeof(unsigned int);
                        structout=reinterpret_cast<void*>(static_cast<unsigned int*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_LONG:
                        *(reinterpret_cast<long*>(structout))=(long)(*VarItt);
                        sizeIncrement+=sizeof(long);
                        structout=reinterpret_cast<void*>(static_cast<long*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_UNSIGNED_LONG:
                        *(reinterpret_cast<unsigned long*>(structout))=(unsigned long int)(*VarItt);
                        sizeIncrement+=sizeof(unsigned long int);
                        structout=reinterpret_cast<void*>(static_cast<unsigned long*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_LONG_LONG:
                        *(reinterpret_cast<long long*>(structout))=(long long)(*VarItt);
                        sizeIncrement+=sizeof(long long int);
                        structout=reinterpret_cast<void*>(static_cast<long long*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_UNSIGNED_LONG_LONG:
                        *(reinterpret_cast<unsigned long long*>(structout))=(unsigned long long)(*VarItt);
                        sizeIncrement+=sizeof(unsigned long long);
                        structout=reinterpret_cast<void*>(static_cast<unsigned long long*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_FLOAT:
                        *(reinterpret_cast<float*>(structout))=(float)(*VarItt);
                        sizeIncrement+=sizeof(float);
                        structout=reinterpret_cast<void*>(static_cast<float*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_DOUBLE:
                        *(reinterpret_cast<double*>(structout))=(double)(*VarItt);
                        sizeIncrement+=sizeof(double);
                        structout=reinterpret_cast<void*>(static_cast<double*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_STRING:
                        *(reinterpret_cast<string*>(structout))=(string)(*VarItt);
                        sizeIncrement+=sizeof(string);
                        structout=reinterpret_cast<void*>(reinterpret_cast<string*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_GL_TYPE:
                        *(reinterpret_cast<GLuint*>(structout))=VarItt->ConvertGL_Const(GLtool::DecodeGL_Type);
                        sizeIncrement+=sizeof(GLuint);
                        structout=reinterpret_cast<void*>(reinterpret_cast<GLuint*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_GL_TEX:
                        *(reinterpret_cast<GLuint*>(structout))=VarItt->ConvertGL_Const(GLtool::DecodeGL_Texture);
                        sizeIncrement+=sizeof(GLuint);
                        structout=reinterpret_cast<void*>(reinterpret_cast<GLuint*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_GL_TEX_FORMAT:
                        *(reinterpret_cast<GLuint*>(structout))=VarItt->ConvertGL_Const(GLtool::DecodeGL_TexFormat);
                        sizeIncrement+=sizeof(GLuint);
                        structout=reinterpret_cast<void*>(reinterpret_cast<GLuint*>(structout)+1);
                        VarItt++;
                    break;
                    case INI_GL_SHADER:
                        *(reinterpret_cast<GLuint*>(structout))=VarItt->ConvertGL_Const(GLtool::DecodeGL_Shader);
                        sizeIncrement+=sizeof(GLuint);
                        structout=reinterpret_cast<void*>(reinterpret_cast<GLuint*>(structout)+1);
                        VarItt++;
                    break;
                }
            }
            if(sizeIncrement>sizeof(S)){
                ConsoleEcho("Convertion Overflow");
                break;
            }
        }
        return structmem;
    }
};


template<GLuint N,typename S,typename ...Args>
extern void testConvert();
template<typename S>
extern std::vector<S> ConvertAll(std::vector<IniProp> props,std::vector<string> order,std::vector<IniType> structformat);
//Iniprop est le tableau avec les prop Order=> est la suite des ne noms des vars qui décrit la maniere dont elle sont organisés dans la structure et structfroamt est l'equivalence en type de ces variable
class IniDecoder{
    //Thread Control
    HANDLE DecodeEnded;
    HANDLE DecodeThread;
    DWORD DecodeThread_Id;

    //Bool d'état
    bool Deccoding;
    bool Filled;
    //Data
    std::vector<IniProp> IniProps;
    string Filepath;
    public:
        IniDecoder();
        ~IniDecoder();
        void DirectDecode(string Filename);
        void EchoData();
        void Decode(string Filename);
        void Decode(const char* Filename);
        void DirectDecode();
        std::vector<IniProp> SearchProps(string format);
        IniProp SearchProp(string format);
        IniProp SearchProp_Deccoding(string format,bool sure_found);//cherche un propiété alors que le programe déccode encore => sure_found force à attendre la fin du décode si la propriété n'est pas trouvé



};
#include "../src/IniDeccoder-template.cpp"

