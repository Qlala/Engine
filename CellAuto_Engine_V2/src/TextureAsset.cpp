#include "Header.h"

void TextureAsset::ComputeDataSize(){
    GLuint paddingcorrection=((Width*BytePerPixel)%(BytePadding<1?1:BytePadding));
    DataSize=(Width+(paddingcorrection==0?0:(BytePadding-paddingcorrection)))*Depth*Height*BytePerPixel;
}
bool TextureAsset::IsMutable(){
    return RAMLOADED;
}

bool TextureAsset::IsBindable(){
    return VRAMLOADED;
}
bool TextureAsset::MakeMutable(){
    return LoadRam();
}

bool TextureAsset::UpdateDataMutation(){
    if(RAMLOADED){
        if(VRAMLOADED){
            UnloadVram();
            return LoadVRAM_FromRAM();
        }else if(FILELOADED){
            //=>transvasement vers le fichier ????
            return false;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

void TextureAsset::GetTextureInVRAMInfo(){
    if(VRAMLOADED){
        ConsoleEcho("Texture: obtention des VRAM Info");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(TextureType,TextureId);
        ConsoleEcho("Texture Bound");
        //LOD
        int level;
        //glGetTextureParameteriv(TextureType,GL_TEXTURE_MAX_LEVEL,&level);
        MipmapCount=level;
        ConsoleEcho("LOD:%i",level);
        //Info sur le format de la texture
        int format;
        glGetTexLevelParameteriv(TextureType,0,GL_TEXTURE_INTERNAL_FORMAT,&format);
        ConsoleEcho("Texture Format:%i",format);
        InternalFormat=format;
        Color_ByteFormat Temp=GLtool::GetColor_ByteFormat(InternalFormat);
        ColorFormat=Temp.ColorFormat;
        GlDataType=Temp.GLDataType;
        BytePerPixel=Temp.BitPerPixel/8;
        ComponentNumber=Temp.ComponentNumber;
        //BytePadding=0;
        //glPixelStore()



        //sample
        if(TextureType==GL_TEXTURE_2D_MULTISAMPLE || TextureType==GL_TEXTURE_2D_MULTISAMPLE_ARRAY){
            int sample;
            glGetTexLevelParameteriv(TextureType,0,GL_TEXTURE_SAMPLES,&sample);
            Sample=sample;
        }else{
            Sample=0;
        }
        //Taille
        int w,h,d;
        glGetTexLevelParameteriv(TextureType,0,GL_TEXTURE_WIDTH,&w);
        V_Width=w;
        if(TextureType!=GL_TEXTURE_1D){
            glGetTexLevelParameteriv(TextureType,0,GL_TEXTURE_HEIGHT,&h);
            V_Height=h;
            if(TextureType==GL_TEXTURE_3D){
                glGetTexLevelParameteriv(TextureType,0,GL_TEXTURE_DEPTH,&d);
                V_Depth=d;
            }else{
                V_Depth=1;
            }
        }else{
            V_Height=1;
        }
    ConsoleEcho("VRAM Info obtenu");
    glBindTexture(TextureType,0);//On retire bien la texture du binding
    }

}
void TextureAsset::CreateFromTextureId(GLuint type,GLuint id,bool become_owner,string name){
    if(!CREATED){
        TextureType=type;
        TextureId=id;
        GetTextureInVRAMInfo();
        CREATED=true;
        VRAMLOADED=true;
    }
}



void TextureAsset::Create(void* data,GLuint w,GLuint h,GLuint d,GLuint byteperpixel,GLuint colorformat,GLuint gldatatype,GLuint byteformat,GLuint padding,bool dont_copy_data){
    if(!CREATED){
        Width=w<1?1:w;
        Height=h<1?1:h;
        Depth=d<1?1:d;
        if(TextureType==0){//On cherche le type de texture
            if(Depth==1){
                if(Height==1){
                    TextureType=GL_TEXTURE_1D;
                }else{
                    TextureType=GL_TEXTURE_2D;
                }
            }else{
                TextureType=GL_TEXTURE_3D;
            }
        }
        BytePadding=padding;
        BytePerPixel=byteperpixel;
        GlDataType=gldatatype;
        ColorFormat=colorformat;
        InternalFormat=byteformat;
        ComputeDataSize();
        if(dont_copy_data){
            Bitmap=data;
        }else{
            Bitmap=malloc(DataSize);
            memcpy(Bitmap,data,DataSize);
        }
        RAMLOADED=true;
    }
}




bool TextureAsset::UnloadRam(){
    if(RAMLOADED){
        delete Bitmap;
        DataSize=0;
        RAMLOADED=false;
        UpdateCreatedState();
        return true;
    }else{
        return false;
    }
}

bool TextureAsset::UnloadVram(){
    if(VRAMLOADED){
        if(SharedTextureId){
            TextureId=0;
            VRAMLOADED=false;
        }else{
            glDeleteTextures(1,&TextureId);
            VRAMLOADED=false;
        }
        UpdateCreatedState();
        return true;
    }else{
        return false;
    }
}

bool TextureAsset::Destroy(){
    UnloadFile();
    FromFile=false;
    UnloadRam();
    UnloadVram();
    UpdateCreatedState();
    return !CREATED;
}
bool TextureAsset::LoadRam(){
    if(!RAMLOADED){
        if(VRAMLOADED){
            return LoadRAM_FromVRAM();
        }else if(FromFile){
            return OpenFile();
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool TextureAsset::LoadVram(){
    if(!VRAMLOADED){
        if(RAMLOADED){
            return LoadVRAM_FromRAM();
        }else if(FromFile){
            return LoadVRAM_FromFile();
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool TextureAsset::LoadFile(){
    if(FromFile&& !FILELOADED){
        PicFile=fopen(FilePath.c_str(),"rb");
        FILELOADED=(PicFile!=NULL);
        return FILELOADED;
    }else{
        return false;
    }
}
bool TextureAsset::UnloadFile(){
    if(FILELOADED){
        fclose(PicFile);
        FILELOADED=(PicFile!=NULL);
        return !FILELOADED;
    }else{
        return false;
    }
}
bool TextureAsset::OpenFile(){
    if(!RAMLOADED){
        bool File_Not_Loaded=false;
        if(!FILELOADED){
            LoadFile();
            File_Not_Loaded=true;
        }
        //recherche de l'externsion
        size_t pointPosition=FilePath.find_last_of(".",FilePath.length());
        string EXT=FilePath.substr(pointPosition);
        //Switch de selection du Type de Fichier
        if(EXT==".bmp"){
            return OpenBMPFromFile();
        }else{
            return false;
        }
        if(File_Not_Loaded){
            UnloadFile();
        }
    }else{
        return false;
    }
}


bool TextureAsset::OpenBMPFromFile(){
    ConsoleEcho("OUvertur du bmp");
    if(FILELOADED){
        try{
            TextureType=GL_TEXTURE_2D;
            char CHAR1,CHAR2;
            int HeaderSize;
            int DataOffset;
            int BitmapLength;
            //FileName=File;
            fseek (PicFile,0,SEEK_SET);
            if(!PicFile)throw"Unable to Open";
            CHAR1=fgetc(PicFile);
            CHAR2=fgetc(PicFile);
            if ( !(CHAR1=='B' && CHAR2=='M') )throw "Not a BMP";
            fread(&DataSize,1,sizeof(int),PicFile);
            for(int i=0;i<4;i++)fgetc(PicFile);
            fread(&DataOffset,1,sizeof(int),PicFile);
            fread(&HeaderSize,1,sizeof(int),PicFile);
            fread(&Width,1,sizeof(int),PicFile);
            fread(&Height,1,sizeof(int),PicFile);
            Depth=1;
            fgetc(PicFile);fgetc(PicFile);
            short BitCount=0;
            fread(&BitCount,1,sizeof(short),PicFile);
            //BitPerPixel=(int)BitCount;
            BytePerPixel=BitCount/8;
            //fread(&Compression,1,sizeof(int),picfile);
            for(int i=0;i<4;i++)fgetc(PicFile);
            fread(&BitmapLength,1,sizeof(int),PicFile);
            if(BitmapLength==0)BitmapLength=DataSize;
            /*for(int i=0;i<8;i++)fgetc(picfile);
            fread(&ColorUsed,1,sizeof(int),picfile);
            fread(&ColorImportant,1,sizeof(int),picfile);
            NumColor=std::powf(2,BitCount);*/
            fseek (PicFile,14+HeaderSize,SEEK_SET);
            fseek (PicFile,DataOffset,SEEK_SET);
            Bitmap= new BYTE[DataSize];
            fread(Bitmap,1,DataSize,PicFile);
            ColorFormat=GL_BGR;
            GlDataType=GL_UNSIGNED_BYTE;
            InternalFormat=GL_RGB8;
            ComponentNumber=3;
            RAMLOADED=true;
            //varaible pour la lecture OGL;
            BytePadding=4;

            /*=GL_BGR;
            ColorInterneSize=GL_UNSIGNED_BYTE;
            BitmapReady=true;*/
            return  true;
        }catch(char* A){
            string Error;
            Error+="Erreur de Fichier : /n ";
            Error+=A;
            Error+=" : ";
            Error+=FilePath;
                ConsoleEcho((char*)Error.data());
                    return  false;

        }
    }else{
        return false;
    }
}



template<typename T>
void TextureAsset::ConvertBMP(const GLuint NewBytePerPixel,const GLuint NewBytePadding,const GLuint NewComponentNumber,const int* SwapPatern,const T* DefaulState,const int NewColorFormat,const int NewGLDataType){
            if(RAMLOADED){
                ConsoleEcho("Texture : Convertion BMP");
                GLuint paddingcorrection=(Width*NewBytePerPixel%(NewBytePadding==0?1:NewBytePadding));
				GLsizei NewSize= Depth*Height*((paddingcorrection==0?0:(NewBytePadding-paddingcorrection))+Width)*(NewBytePerPixel);
				ConsoleEcho("New Size:%i",NewSize);
				BYTE* NewTex=(BYTE*)malloc(NewSize);

				int InternalInd=0;
				int ExternalInd=0;
				int Pack=0;
				int PackOut=0;
				GLuint DeltaComponent=NewComponentNumber;
				GLuint DeltaByte=NewBytePerPixel;
				int ByteAdd=0;
				int InternByteCount=BytePerPixel;
				ConsoleEcho("InternByteCount:%i",BytePerPixel);
				for(int h=0;h<Depth;h++){
                    for(int i=0;i<Height;i++){
                        for(int j=0;j<Width;j++){
                            for(int k=0;k<DeltaComponent;k++){
                                    if(k<DeltaByte && SwapPatern[k]<ComponentNumber){
                                        *(reinterpret_cast<T*>(NewTex+(ExternalInd))+k)=*(reinterpret_cast<T*>(Bitmap+InternalInd)+SwapPatern[k]);
                                    }else{
                                        *(reinterpret_cast<T*>(NewTex+(ExternalInd))+k)=DefaulState[k];
                                    }
                                }
                                InternalInd+=BytePerPixel;
                                ExternalInd+=NewBytePerPixel;
                                Pack+=BytePerPixel;
                                PackOut+=DeltaByte;
                        }
                        if(BytePadding!=0)
                            if(Pack%(BytePadding)!=0){InternalInd+=(BytePadding-Pack%BytePadding);
                            Pack=0;
                            }
                        if(NewBytePadding!=0)
                            if(PackOut%NewBytePadding!=0){ExternalInd+=(NewBytePadding-PackOut%NewBytePadding);
                            PackOut=0;
                            }
                    }
				}
				ConsoleEcho("Convertion Fini");
				BytePadding=NewBytePadding==0?1:NewBytePadding;
				//if(1==NewUnpackAlignement)Unpacked=false;
				BytePerPixel=NewBytePerPixel;
				delete Bitmap;
				Bitmap=(void*)NewTex;
				DataSize=NewSize;
				ColorFormat=NewColorFormat;
				GlDataType=NewGLDataType;
				ComponentNumber=NewComponentNumber;
        }
}
void TextureAsset::EchoTextureInfo(bool TextureData){
    ConsoleEcho("Texture Info");
    ConsoleEcho("Texture Type : %i",TextureId);
    //State
    if(CREATED)
        ConsoleEcho("Texture CREATED = True");
    else
        ConsoleEcho("Texture CREATED = False");
    if(RAMLOADED)
        ConsoleEcho("Texture RAMLOADED = True");
    else
        ConsoleEcho("Texture RAMLOADED = False");
    if(VRAMLOADED)
        ConsoleEcho("Texture VRAMLOADED = True");
    else
        ConsoleEcho("Texture VRAMLOADED = False");

    //RAM State
        ConsoleEcho("Ram State");
    ConsoleEcho("RAM : Size = %i",DataSize);
    ConsoleEcho("RAM : Width = %i",Width);
    ConsoleEcho("RAM : Height = %i",Height);
    ConsoleEcho("RAM : Depth = %i",Depth);
    ConsoleEcho("RAM : BytePerPixel = %i",BytePerPixel);
    ConsoleEcho("RAM : BytePadding = %i",BytePadding);
    ConsoleEcho("RAM : ComponentNumber = %i",ComponentNumber);
    ConsoleEcho("RAM : GlDataType = %i",GlDataType);

    //VRAM State
    ConsoleEcho("VRAM : TextureId = %i",TextureId);
    ConsoleEcho("VRAM : V_Width = %i",V_Width);
    ConsoleEcho("VRAM : V_Height = %i",V_Height);
    ConsoleEcho("VRAM : V_Depth = %i",V_Depth);
    ConsoleEcho("VRAM : Sample = %i",Sample);
    ConsoleEcho("VRAM : ColorFormat = %i",ColorFormat);
    ConsoleEcho("VRAM : InternalFormat = %i",InternalFormat);
    ConsoleEcho("VRAM : MipmapCount = %i",MipmapCount);

    if(TextureData && RAMLOADED){
        ConsoleEcho("Texture Data");
        char* temp=new char[DataSize+2];

        memcpy((void*)temp,Bitmap,DataSize);
        temp[DataSize]='\0';
        WriteToLog(temp,"Texture");
        delete temp;
    }

}
void TextureAsset::DefineParameters(){
    ParametersDefined=true;
    glTexParameteri(TextureType,GL_TEXTURE_BASE_LEVEL,0);
    glTexParameterf(TextureType,GL_TEXTURE_LOD_BIAS,0.0);
    glTexParameterf(TextureType,GL_TEXTURE_MIN_LOD,0.0);
    glTexParameterf(TextureType,GL_TEXTURE_MAX_LOD,0.0);
    glTexParameteri(TextureType,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(TextureType,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(TextureType,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(TextureType,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(TextureType,GL_TEXTURE_WRAP_R,GL_REPEAT);
}

void TextureAsset::GenerateMipmap(){
    if(VRAMLOADED){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(TextureType,TextureId);
        glGenerateMipmap(TextureType);
        float MipmapCountf=log2(std::max(V_Width,std::max(V_Height,V_Depth)));
        MipmapCount=(GLuint)MipmapCount;
        glTexParameteri(TextureType,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(TextureType,GL_TEXTURE_MAX_LEVEL,MipmapCount);
        glTexParameterf(TextureType,GL_TEXTURE_MAX_LOD,MipmapCountf);
    }
}

bool TextureAsset::LoadVRAM_FromRAM(){
    if(!VRAMLOADED && RAMLOADED){
        glGenTextures(1,&TextureId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(TextureType,TextureId);
        V_Width=Width<1?1:Width;
        V_Height=Height<1?1:Height;
        V_Depth=Depth<1?1:Depth;
        if(TextureType==GL_TEXTURE_1D){
            VRAMLOADED=true;
            if(BytePadding==0)glPixelStorei(GL_PACK_ALIGNMENT,BytePadding);
            glTexImage1D(TextureType,0,InternalFormat,V_Width,0,ColorFormat,GlDataType,Bitmap);
            DefineParameters();
            GenerateMipmap();
        }else if(TextureType==GL_TEXTURE_1D_ARRAY ||TextureType==GL_TEXTURE_2D  || TextureType==GL_TEXTURE_RECTANGLE){
            VRAMLOADED=true;
            if(BytePadding==0)glPixelStorei(GL_PACK_ALIGNMENT,BytePadding);
            glTexImage2D(TextureType,0,InternalFormat,V_Width,V_Height,0,ColorFormat,GlDataType,Bitmap);
            DefineParameters();
            GenerateMipmap();
        }else if(TextureType==GL_TEXTURE_CUBE_MAP){
            VRAMLOADED=true;
            BYTE* BitmapPtr=(BYTE*)Bitmap;
            if(BytePadding==0)glPixelStorei(GL_PACK_ALIGNMENT,BytePadding);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,InternalFormat,V_Width,V_Height,0,ColorFormat,GlDataType,Bitmap);
            BitmapPtr+=(DataSize/6);//il y 6 face
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,InternalFormat,V_Width,V_Height,0,ColorFormat,GlDataType,(void*)BitmapPtr);
            BitmapPtr+=(DataSize/6);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,InternalFormat,V_Width,V_Height,0,ColorFormat,GlDataType,(void*)BitmapPtr);
            BitmapPtr+=(DataSize/6);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,InternalFormat,V_Width,V_Height,0,ColorFormat,GlDataType,(void*)BitmapPtr);
            BitmapPtr+=(DataSize/6);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,InternalFormat,V_Width,V_Height,0,ColorFormat,GlDataType,(void*)BitmapPtr);
            BitmapPtr+=(DataSize/6);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,InternalFormat,V_Width,V_Height,0,ColorFormat,GlDataType,(void*)BitmapPtr);
            DefineParameters();
            GenerateMipmap();
        }else if(TextureType==GL_TEXTURE_2D_MULTISAMPLE){//Ne Charge Rien car on ne peut pas Uploadé de Donné(pour l'instant)
            VRAMLOADED=true;
            glTexImage2DMultisample(TextureType,Sample,InternalFormat,V_Width,V_Height,true);
            DefineParameters();
            //GenerateMipmap();
        }else if(TextureType==GL_TEXTURE_3D || TextureType==GL_TEXTURE_2D_ARRAY){
            VRAMLOADED=true;
            if(BytePadding==0)glPixelStorei(GL_PACK_ALIGNMENT,BytePadding);
            glTexImage3D(TextureType,0,InternalFormat,V_Width,V_Height,V_Depth,0,ColorFormat,GlDataType,Bitmap);
            DefineParameters();
            GenerateMipmap();

        }else if(TextureType==GL_TEXTURE_2D_MULTISAMPLE_ARRAY){//ne charge rien
            VRAMLOADED=true;
            glTexImage3DMultisample(TextureType,Sample,InternalFormat,V_Width,V_Height,V_Depth,true);
        }else{
            glDeleteTextures(1,&TextureId);
            VRAMLOADED=false;
        }
        glBindTexture(TextureType,0);
        return VRAMLOADED;
    }else{
        return false;
    }
}


bool TextureAsset::LoadRAM_FromVRAM(){
    if(VRAMLOADED && !RAMLOADED){
        ConsoleEcho("Texture:Load RAM from VRAM");
        GetTextureInVRAMInfo();
        ConsoleEcho("Vram Info Aquired");
        Width=V_Width<1?1:V_Width;
        Height=V_Height<1?1:V_Height;
        Depth=V_Depth<1?1:V_Depth;
        GLuint paddingcorrection=((Width*BytePerPixel)%(BytePadding<1?1:BytePadding));
        DataSize=(Width+(paddingcorrection==0?0:(BytePadding-paddingcorrection)))*Depth*Height*BytePerPixel;
        ConsoleEcho("Texture:Allocation de mémoire");
        Bitmap=malloc(DataSize);
        ConsoleEcho("Texture:Récupération de la Texture");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(TextureType,TextureId);
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        BytePadding=1;
        glGetTexImage(TextureType,0,ColorFormat,GlDataType,Bitmap);
        glBindTexture(TextureType,0);
        ConsoleEcho("Texture: terminé");
        RAMLOADED=true;
    }else{
        return false;
    }
};
bool TextureAsset::LoadVRAM_FromFile(){
    OpenFile();
    LoadVRAM_FromRAM();
    UnloadRam();
    return VRAMLOADED;
}

void TextureAsset::SetColorFormat(GLuint format){
    if(!RAMLOADED){
        ColorFormat=format;
    }
}


void TextureAsset::SaveBMP(string File){
    bool not_in_ram=false;
    if(VRAMLOADED && !RAMLOADED){
        LoadRAM_FromVRAM();
        not_in_ram=true;
    }
    if(RAMLOADED){
        //convertion vers bmp
        if(ColorFormat!=GL_BGR){
            int swappattern[3]={2,1,0};
            BYTE defaultval[3]={255,255,255};
            ConvertBMP(3,4,3,swappattern,defaultval,GL_BGR,GL_UNSIGNED_BYTE);
        }
        // ouverture du fichier
        ConsoleEcho("Ecriture du Fichier");
        FILE* newfile=fopen(File.c_str(),"wb+");
        //Header BMP
        fputc('B',newfile);
        fputc('M',newfile);
        fwrite((int*)&DataSize,sizeof(int),1,newfile);//Taile des Donnés
        fputc('\0',newfile);
        fputc('\0',newfile);
        fputc('\0',newfile);
        fputc('\0',newfile);
        int dataoffset=14+40;
        fwrite(&dataoffset,sizeof(int),1,newfile);
        ConsoleEcho("Header Windows");
        //Windows BITMAPINFOHEADER
        int zero=0;
        int one=1;
        int headersize=40;
        fwrite(&headersize,sizeof(int),1,newfile);//taile du header
        fwrite((int*)(&Width),sizeof(int),1,newfile);
        fwrite((int*)(&Height),sizeof(int),1,newfile);
        fwrite(&one,sizeof(short),1,newfile);//number of color plane =1
        short bitperpixel=BytePerPixel*8;
        fwrite(&bitperpixel,sizeof(short),1,newfile);//nombre de bit par pixel
        fwrite(&zero,sizeof(int),1,newfile);//Compression utilisé =>0 pas de compression
        fwrite(&zero,sizeof(int),1,newfile);//taille de l'image => mettre 0 si non compressé
        fwrite(&one,sizeof(int),1,newfile);//pixel par metre => Width
        fwrite(&one,sizeof(int),1,newfile);//Pixel par metre => height
        fwrite(&zero,sizeof(int),1,newfile);//nombre de couleur utilisé => 0 =2^n
        fwrite(&zero,sizeof(int),1,newfile);//nombre de couleur importante => 0 = ignoré
        fwrite(Bitmap,1,DataSize,newfile);
        fclose(newfile);
    }
    if(not_in_ram){
        UnloadRam();
    }
}


