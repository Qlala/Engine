#pragma once
#include "Header.h"
char buff[255];
char buff2[255];
char buff4[255];
IniVar::operator bool(){
    if(Value.compare("True")==0 || Value.compare("true")){
        return true;
    }else{
        return false;
    }
}

IniVar::operator char(){
    char val;
    sscanf(Value.c_str(),"%x",&val);//en héxa
    return val;
}
IniVar::operator unsigned char(){
    unsigned char val;
    sscanf(Value.c_str(),"%x",&val);
    return val;
}

IniVar::operator short(){
    short val;
    sscanf(Value.c_str(),"%i",&val);
    return val;
}
IniVar::operator unsigned short(){
    unsigned short val;
    sscanf(Value.c_str(),"%u",&val);
    return val;
}

IniVar::operator int(){
    int val;
    sscanf(Value.c_str(),"%i",&val);
    return val;
}

IniVar::operator unsigned int(){
    unsigned int val;
    sscanf(Value.c_str(),"%u",&val);
    return val;
}

IniVar::operator float(){
    float val;
    sscanf(Value.c_str(),"%f",&val);
    return val;
}
IniVar::operator double(){
    double val;
    sscanf(Value.c_str(),"%f",&val);
    return val;
}
IniVar::operator long(){
    long val;
    sscanf(Value.c_str(),"%i",&val);
    return val;
}
IniVar::operator unsigned long(){
    long val;
    sscanf(Value.c_str(),"%u",&val);
    return val;
}
IniVar::operator long long(){
    long val;
    sscanf(Value.c_str(),"%i",&val);
    return val;
}
IniVar::operator unsigned long long(){
    long val;
    sscanf(Value.c_str(),"%u",&val);
    return val;
}
IniVar::operator string(){
    return Value;
}
GLuint IniVar::ConvertGL_Const(GLuint (func)(string)){
    return func(Value);
};


template<typename T>
T ConvertOnce(IniVar var){
    return(T)var;
}
/*template<GLuint N,GLuint I,typename ArgI,typename ...Args>
class ArgsType : ArgsType<N,I-1,Args...>{
ArgI argi;
public:
    void AffectArgi(std::vector<IniVar>::iterator Itt){
        argi=(ArgI)*Itt;
        Itt++;
        AffectArgi<I-1>(Itt);
    }
};

template<GLuint N,typename ArgI,typename ...Args>
class ArgsType<N,0,Args...>{
ArgI argi;
public:
    void AffectArgi(std::vector<IniVar>::iterator Itt){
        argi=(ArgI)*Itt;
    }
};
void testConvert(){
    IniProp inip={"test",{{"a","1.05"},{"b","4"}}};
    inip.Convert<2,A,float,GLuint>();
}*/



IniVar IniProp::Search(string format){
    std::vector<IniVar>::iterator Itt;
    for(Itt=Vars.begin();Itt!=Vars.end();Itt++){
        if(format.compare(Itt->Name)==0){
            break;
        }
    }
    if(Itt!=Vars.end()){
        return (*Itt);
    }else{
        ConsoleEcho(("Echec de la recherche dans"+Name+" de: "+format).c_str());
        return {"",""};
    }
}

void IniProp::Filter(std::vector<string> order){
    std::vector<IniVar> NewVars;
    std::vector<IniVar>::iterator VarItt;
    if(order.size()!=0 && Vars.size()!=0){
        for(std::vector<string>::iterator strItt=order.begin();strItt!=order.end();strItt++){
            for(VarItt=Vars.begin();VarItt!=Vars.end();VarItt++){
                if(strItt->compare(VarItt->Name)==0){//si les 2 string sont pareille
                    NewVars.push_back(*VarItt);
                    break;
                }
            }
            if(VarItt==Vars.end()){//on n'as rien trouvé
                NewVars.push_back({"",""});
            }
        }
        //on fait la copie
        Vars=NewVars;
    }
}

void IniProp::EchoData(){
    ConsoleEcho(("IniProp:"+Name).c_str());
    for(std::vector<IniVar>::iterator Itt2=Vars.begin();Itt2!=Vars.end();Itt2++){
                ConsoleEcho(("Name:"+Itt2->Name).c_str());
                ConsoleEcho(("Value="+Itt2->Value).c_str());
        }

}

inline GLuint CutAndGetLastChar(char* str){
    int i=0;
    while(str[i]!='\0')i++;
    str[i-1]='\0';
    return i-2;
}
DWORD WINAPI DecodeWarper(void* param){
    static_cast<IniDecoder*>(param)->DirectDecode();
    ExitThread(000);
}
void IniDecoder::Decode(string Filename){
    Filepath=Filename;
    Deccoding=true;
    ResetEvent(DecodeEnded);
    DecodeThread=CreateThread(NULL,0,DecodeWarper,static_cast<void*>(this),0,&DecodeThread_Id);
   // CloseHandle(DecodeThread);
}

IniDecoder::IniDecoder(){
    DecodeEnded=CreateEvent(NULL,true,true,(Filepath+(string)GenStrWithUniqueCode("",false)).c_str());
}
IniDecoder::~IniDecoder(){
    CloseHandle(DecodeEnded);
}
void IniDecoder::Decode(const char* filename){
    Deccoding=true;
    ResetEvent(DecodeEnded);
    Filepath=(string)filename;
    DecodeThread=CreateThread(NULL,0,DecodeWarper,static_cast<void*>(this),0,&DecodeThread_Id);
    //DecodeInProcess=CreateEvent(NULL,true,false,(Filepath+(string)GenStrWithUniqueCode("%i",false)).c_str());
}
void IniDecoder::DirectDecode(){
    DirectDecode(Filepath);
}



void IniDecoder::DirectDecode(string Filename){
    Deccoding=true;
    GLuint B=0;
    FILE * IniFile;
    ConsoleEcho("Ini:Début du deccodage askip");
    IniFile=fopen(Filename.c_str(),"r");
    if(IniFile!=NULL){
        ResetEvent(DecodeEnded);
        fgets(buff,255,IniFile);
        B=CutAndGetLastChar(buff);
        while(feof(IniFile)==0){//tant qu'on est pas au bout du fichier

            if(buff[0]=='['&&buff[B]==']'){//on rentre dans un group de propriété
                IniProp NewProp;
                memcpy(buff4,(buff+1),B-1);
                buff4[B-1]='\0';
                NewProp.Name=(string)buff4;
                fgets(buff,255,IniFile);
                B=CutAndGetLastChar(buff);
                while(buff[0]!='[' && feof(IniFile)==0){//On chercher des Propriété;
                    char* bufftemp=strtok(buff,"=");
                    if(bufftemp!=NULL){//Si on est bien devant une Var
                        IniVar newvar;
                        newvar.Name=(string)bufftemp;
                        bufftemp=strtok(NULL,"");
                        if(bufftemp!=NULL){
                            newvar.Value=(string)bufftemp;
                            NewProp.Vars.push_back(newvar);
                        }

                    }//que l'on est bien trouvé une variable ou non continue
                    fgets(buff,255,IniFile);
                    B=CutAndGetLastChar(buff);
                }
                IniProps.push_back(NewProp);
            }else{
                fgets(buff,255,IniFile);
                B=CutAndGetLastChar(buff);
            }
        }
        Filled=true;
    }else{
        ConsoleEcho(("Impossible d'ouvir le fichier mdr"+Filename).c_str());
    }
    Deccoding=false;
    fclose(IniFile);
    ConsoleEcho("Ini:Fin du deccodage");
    SetEvent(DecodeEnded);
}

std::vector<IniProp> IniDecoder::SearchProps(string format){
    if(Deccoding)WaitForSingleObject(DecodeEnded,INFINITE);
    if(Filled){
        std::vector<IniProp> Out;
        bool Not_Full_String;//signal que le string contient un symbol "*"
        size_t starPos=format.find("*");
        Not_Full_String=(format[starPos]=='*');
        for(std::vector<IniProp>::iterator Itt=IniProps.begin();Itt!=IniProps.end();Itt++){
            if(Not_Full_String){
                if(Itt->Name.compare(0,starPos,format,0,starPos)==0){
                    Out.push_back(*Itt);
                }
            }else{
                if(format.size()==Itt->Name.size()){
                    if(Itt->Name.compare(format)==0){//ils sont identique
                        Out.push_back(*Itt);
                    }
                }
            }
        }
        /*ConsoleEcho("ce que l'on à trouvé");
        for(std::vector<IniProp>::iterator Itt=Out.begin();Itt!=Out.end();Itt++){
            ConsoleEcho(Itt->Name.c_str());
            for(std::vector<IniVar>::iterator Itt2=Itt->Vars.begin();Itt2!=Itt->Vars.end();Itt2++){
                ConsoleEcho(("Name:"+Itt2->Name).c_str());
                ConsoleEcho(("Value="+Itt2->Value).c_str());
            }
        }*/
        ConsoleEcho("Fini");
        return Out;
    }
}
std::vector<string> split(const string &text, const char sep) {
  std::vector<string> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

IniProp IniDecoder::SearchProp(string format){
    if(Deccoding)WaitForSingleObject(DecodeEnded,INFINITE);
    if(Filled){
        bool Found=false;
        bool Not_Full_String;//signal que le string contient un symbol "*"
        size_t starPos=format.find("*");
        Not_Full_String=(format[starPos]=='*');
        for(std::vector<IniProp>::iterator Itt=IniProps.begin();Itt!=IniProps.end();Itt++){
            if(Not_Full_String){
                if(Itt->Name.compare(0,starPos,format,0,starPos)==0){
                    return *Itt;
                    Found=true;
                    break;
                }
            }else{
                if(format.size()==Itt->Name.size()){
                    if(Itt->Name.compare(format)==0){//ils sont identique
                        ConsoleEcho(("Trouvé"+Itt->Name).c_str());
                        return *Itt;
                        Found=true;
                        break;
                    }
                }
            }
        }
        if(!Found){
            return {"",{}};
        }
        /*ConsoleEcho("ce que l'on à trouvé");
        for(std::vector<IniProp>::iterator Itt=Out.begin();Itt!=Out.end();Itt++){
            ConsoleEcho(Itt->Name.c_str());
            for(std::vector<IniVar>::iterator Itt2=Itt->Vars.begin();Itt2!=Itt->Vars.end();Itt2++){
                ConsoleEcho(("Name:"+Itt2->Name).c_str());
                ConsoleEcho(("Value="+Itt2->Value).c_str());
            }
        }*/
        ConsoleEcho("Fini");
    }
}
/*IniProp IniDecoder::SearchProp_Deccoding(string format,bool sure_found){
    if(WaitForSingleObject(DecodeInProcess,0)==WAIT_OBJECT_0){
        if(sure_found){
            for(std::vector<IniProp>::iterator Itt=IniProps.begin();Itt!=IniProps.end();Itt++){
                    if(format.size()==Itt->Name.size()){
                        if(Itt->Name.compare(format)==0){//ils sont identique
                            Out.push_back(*Itt);
                        }
                }
            }
        }else{


    }
}*/


void IniDecoder::EchoData(){
    memory_fence();
    ConsoleEcho("IniDeccoder");
    if(Deccoding)WaitForSingleObject(DecodeEnded,INFINITE);
    if(Filled){
        for(std::vector<IniProp>::iterator Itt=IniProps.begin();Itt!=IniProps.end();Itt++){
            ConsoleEcho(Itt->Name.c_str());
            for(std::vector<IniVar>::iterator Itt2=Itt->Vars.begin();Itt2!=Itt->Vars.end();Itt2++){
                ConsoleEcho(("Name:"+Itt2->Name).c_str());
                ConsoleEcho(("Value="+Itt2->Value).c_str());
            }
        }
    }



}

