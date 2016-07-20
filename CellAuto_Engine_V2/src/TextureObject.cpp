#include "Header.h"

bool TextureObject::OpenBMP(const char* FileName){
    ConsoleEcho("OUvertur du bmp");
	try{
		char CHAR1,CHAR2;
		int HeaderSize;
		int DataOffset;
		//FileName=File;
		FILE * picfile=fopen(FileName,"rb" );
		if(!picfile)throw"Unable to Open";
		CHAR1=fgetc(picfile);
		CHAR2=fgetc(picfile);
		if ( !(CHAR1=='B' && CHAR2=='M') )throw "Not a BMP";
		fread(&BitmapSize,1,sizeof(int),picfile);
		for(int i=0;i<4;i++)fgetc(picfile);
		fread(&DataOffset,1,sizeof(int),picfile);
		fread(&HeaderSize,1,sizeof(int),picfile);
		fread(&width,1,sizeof(int),picfile);
		fread(&height,1,sizeof(int),picfile);
		fgetc(picfile);fgetc(picfile);
		short BitCount=0;
		fread(&BitCount,1,sizeof(short),picfile);
		BitPerPixel=(int)BitCount;
		BytePerPixel=BitCount/8;
		//fread(&Compression,1,sizeof(int),picfile);
		for(int i=0;i<4;i++)fgetc(picfile);
		fread(&BitmapLength,1,sizeof(int),picfile);
		if(BitmapLength==0)BitmapLength=BitmapSize;
		/*for(int i=0;i<8;i++)fgetc(picfile);
		fread(&ColorUsed,1,sizeof(int),picfile);
		fread(&ColorImportant,1,sizeof(int),picfile);
		NumColor=std::powf(2,BitCount);*/
		fseek (picfile,14+HeaderSize,SEEK_SET);
		fseek (picfile,DataOffset,SEEK_SET);
		Bitmap= new BYTE[BitmapSize];
		fread(Bitmap,1,BitmapSize,picfile);
		fclose(picfile);
		GLColorFormat=GL_BGR;
		GLDataFormat=GL_UNSIGNED_BYTE;
		ComponentNumber=3;
		Filled=true;
		//varaible pour la lecture OGL;
        UnpackAlignement=4;
        Unpacked=true;

		/*=GL_BGR;
		ColorInterneSize=GL_UNSIGNED_BYTE;
		 BitmapReady=true;*/
		 return  true;
	}catch(char* A){
		string Error;
		Error+="Erreur de Fichier : /n ";
		Error+=A;
		Error+=" : ";
		Error+=FileName;
			ConsoleEcho((char*)Error.data());
				return  false;

	}

}
bool TextureObject::Bind(GLuint Slot){
    if(Loaded){
        GLuint RealSlot=GL_TEXTURE0+Slot;
        ConsoleEcho("Text act");
        glActiveTexture(RealSlot);
        ConsoleEcho("Binding");
        glBindTexture(GL_TEXTURE_2D,TextureIndex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        return true;
    }else{
        return false;
    }
}
bool TextureObject::ConvertBMP(const GLuint NewByteCount,const GLuint NewUnpackAlignement,const GLuint NewComponentNumber,const int* SwapPatern,const BYTE* DefaulState,const int NewGLFormat,const int NewGLDataType){
            if(Filled){
				int NewSize= height*width*(NewByteCount)+(NewUnpackAlignement-(width*NewByteCount)%NewUnpackAlignement)*height;
				BYTE* NewTex=new BYTE[NewSize];
				int InternalInd=0;
				int ExternalInd=0;
				int Pack=0;
				int PackOut=0;
				int DeltaByte=NewByteCount;
				int ByteAdd=0;
				int InternByteCount=BytePerPixel;
				ConsoleEcho("InternByteCount");
				std::cout<<InternByteCount<<std::endl;
				for(int i=0;i<height;i++){
					for(int j=0;j<width;j++){
                        for(int k=0;k<DeltaByte;k++){
                                if(k<DeltaByte && SwapPatern[k]<InternByteCount){
                                    NewTex[ExternalInd+k]=Bitmap[InternalInd+SwapPatern[k]];
                                }else{
                                    NewTex[ExternalInd+k]=DefaulState[k];
                                }
                            }
                            InternalInd+=InternByteCount;
							ExternalInd+=DeltaByte;
							Pack+=InternByteCount;
							PackOut+=DeltaByte;
					}
					if(Pack%UnpackAlignement!=0){InternalInd+=(UnpackAlignement-Pack%UnpackAlignement);
					Pack=0;
					}
					if(PackOut%NewUnpackAlignement!=0){ExternalInd+=(NewUnpackAlignement-PackOut%NewUnpackAlignement);
					PackOut=0;
					}
				}
				UnpackAlignement=NewUnpackAlignement;
				if(1==NewUnpackAlignement)Unpacked=false;
				BitPerPixel=NewByteCount*8;
				BytePerPixel=NewByteCount;
				delete Bitmap;
				Bitmap=NewTex;
				GLColorFormat=NewGLFormat;
				GLDataFormat=NewGLDataType;
				ComponentNumber=NewComponentNumber;
        }
}

bool TextureObject::Load(int Mipmap,GLenum SpecialFormat){
    if(Filled && !Loaded){
        ConsoleEcho("Upload");
        glGenTextures(1,&TextureIndex);
        glBindTexture(GL_TEXTURE_2D,TextureIndex);
        ConsoleEcho("MomentCritique");
        if(Unpacked){glPixelStorei(GL_UNPACK_ALIGNMENT, UnpackAlignement);
        gluBuild2DMipmaps(GL_TEXTURE_2D,ComponentNumber,width,height,GLColorFormat,GLDataFormat,Bitmap);
        }else{
        ConsoleEcho("Methode pas casse geule");
        glTexImage2D(GL_TEXTURE_2D,Mipmap,GLColorFormat,width,height,0,SpecialFormat,GLDataFormat,Bitmap);
        //gluBuild2DMipmaps(GL_TEXTURE_2D,ComponentNumber,width,height,GLColorFormat,GLDataFormat,Bitmap);
        }
        ConsoleEcho("C'est passé");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //glGenerateMipmap(GL_TEXTURE_2D);
        Loaded=true;
    }
}

TextureObject::TextureObject(const char* FileName,int ForceMode){
int i=0;
bool AfterPoint=false;
string EXT="";
while(FileName[i]!='\0'){
    i++;
    if(FileName[i]=='.'){
        AfterPoint=true;
    }else if(AfterPoint){
        EXT+=toupper(FileName[i]);
    }
}
if(EXT.data()[0]=='B' && EXT.data()[1]=='M' && EXT.data()[2]=='P')OpenBMP(FileName);

}
