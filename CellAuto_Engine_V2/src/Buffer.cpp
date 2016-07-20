#include "Header.h"
void Buffer::SetName(string part1,string part2){
    Name=part1+part2;
}
void Buffer::SetBufferUsage(GLuint usage){
    BufferUsage=usage;
}
void Buffer::SetBufferStorageFlags(GLuint flags){
    BufferStorageFlags=flags;
}
void Buffer::AddBufferStorageFlags(GLuint flags){
    BufferStorageFlags|=flags;
}
void Buffer::SetDataToRam(void* DataPointer,GLsizei datasize){
    DataSize=datasize;
    Data=malloc(DataSize);
    memcpy(Data,DataPointer,DataSize);
    RAMLOADED=true;
}



bool Buffer::MapBuffer(){
    if(!RAMLOADED && VRAMLOADED){
        glBindBuffer(BufferType,BufferId);
        glMapBuffer(BufferType,GL_READ_WRITE);
        BufferMaped=true;
        RAMLOADED=true;
        return true;
    }else{
        return false;
    }
}
Buffer::Buffer(GLenum type,void*data,GLsizei datasize,string name){
    Create(type,data,datasize,name);
}


bool Buffer::Create(GLenum type,void*data,GLsizei datasize,string name){
    BufferType=type;
    SetDataToRam(data,datasize);
    SetName(name);
    CheckCreated();
}
bool Buffer::IsMutable(){
    return RAMLOADED;
}

bool Buffer::MakeBindable(){
    return LoadVram();
}
bool Buffer::MakeMutable(){
    return LoadRam();
}
bool Buffer::IsBindable(){
    return VRAMLOADED;
}

bool Buffer::UpdateDataMutation(){
    UnloadVram();
    return LoadVram();
};

bool Buffer::UpdateBufferModification(){
    UnloadRam();
    LoadRam();
}
void Buffer::SetDataToVRam(GLuint id){
    glGenBuffers(1,&BufferId);
    glBindBuffer(GL_READ_BUFFER,id);
    int64_t size;
    glGetBufferParameteri64v(GL_READ_BUFFER,GL_BUFFER_SIZE,&size);
    glBindBuffer(GL_COPY_WRITE_BUFFER,BufferId);
    glCopyBufferSubData(GL_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,0,size);
    VRAMLOADED=true;
    CheckCreated();
}
void Buffer::SetDataToVRam(void* DataPointer,GLsizei datasize){
    glGenBuffers(1,&BufferId);
    glBindBuffer(GL_COPY_WRITE_BUFFER,BufferId);
    glBufferData(BufferType,datasize,DataPointer,BufferUsage);
    VRAMLOADED=true;
    CheckCreated();
}

Buffer* Buffer::Copy(){
    Buffer* NB=new Buffer(BufferType);
    NB->SetBufferUsage(BufferUsage);
    NB->SetBufferStorageFlags(BufferStorageFlags);
    if(RAMLOADED){
        NB->SetDataToRam(Data,DataSize);
    }
    if(VRAMLOADED and not BufferMaped){
        NB->SetDataToVRam(BufferId);
    }else if(VRAMLOADED and BufferMaped){
        NB->SetDataToVRam(Data,DataSize);
    }
    if(FromFile){//à compléter

    }
    return NB;
}

bool Buffer::Bind(GLuint slot){
    if(VRAMLOADED){
        glBindBuffer(BufferType,BufferId);
        return true;
    }else{
        return false;
    }
}
void Buffer::CheckCreated(){
    CREATED=RAMLOADED||VRAMLOADED||FromFile;
}

bool Buffer::LoadVRAM_FromRAM(){
    if(RAMLOADED ){
        if(!VRAMLOADED){
            glGenBuffers(1,&BufferId);
            glBindBuffer(BufferType,BufferId);
            glBufferData(BufferType,DataSize,Data,BufferUsage);
            VRAMLOADED=true;
            ConsoleEcho((Name+":Chargé depuis la Ram->Vram").c_str());
            return true;
        }else if(VRAMLOADED && BufferMaped){
            glBindBuffer(BufferType,BufferId);
            glUnmapBuffer(BufferType);
            BufferMaped=false;
            RAMLOADED=false;
            VRAMLOADED=true;
            ConsoleEcho((Name+":Chargé depuis la Ram->Vram").c_str());
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool Buffer::LoadRAM_FromVRAM(){
    if(VRAMLOADED ){
        if(BufferMaped){
            void* RealData=malloc(DataSize);
            glBindBuffer(BufferType,BufferId);
            memcpy(RealData,Data,DataSize);
            return true;
        }else{
            glBindBuffer(BufferType,BufferId);
            glGetBufferParameteriv(BufferType,GL_BUFFER_SIZE,&DataSize);
            Data=malloc(DataSize);
            memcpy(Data,glMapBuffer(BufferType,GL_READ_ONLY),DataSize);
            glUnmapBuffer(BufferType);
            RAMLOADED=true;
            return true;
        }
    }else{
        return false;
    }
}
bool Buffer::LoadRam(){
    if(CREATED){
        if(VRAMLOADED){
            return LoadRAM_FromVRAM();
        }else if(FromFile){
            return LoadRAM_FromFile();
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool Buffer::LoadVram(){
    if(CREATED){
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



bool Buffer::UnloadRam(){
    if(RAMLOADED){
        if(BufferMaped){//si le buffer est mapé il ne faut pas suprimé le pointer mais juste le démapé
            glBindBuffer(BufferType,BufferId);
            glUnmapBuffer(BufferType);
            RAMLOADED=false;
            CheckCreated();
        }else{
            delete Data;
            RAMLOADED=false;
            CheckCreated();
            return true;
        }
    }else{
        return false;
    }
}

bool Buffer::UnloadVram(){
    if(VRAMLOADED){
        if(BufferMaped){//si le buffer est mapé il faut d'abord mettre sont contenu en RAM
            LoadRAM_FromVRAM();//on met le contendu buffer dans la Ram
            glBindBuffer(BufferType,BufferId);
            glUnmapBuffer(BufferType);
            glDeleteBuffers(1,&BufferId);
        }else{
            glDeleteBuffers(1,&BufferId);
            VRAMLOADED=false;
            CheckCreated();
            return true;
        }
    }else{
        return false;
    }
}



void Buffer::LoadFile(){
    if(FromFile){
        File=fopen(FileName.c_str(),"rw");
        FileOpened=true;
        //complété avec des opération de lecture de donné
    }
}



bool Buffer::LoadRAM_FromFile(){//pas fini
    if(FromFile){
        if(!FileOpened)LoadFile();
        RAMLOADED=true;
    }
}
bool Buffer::LoadVRAM_FromFile(){//pas fini
    if(FromFile){
        if(!FileOpened)LoadFile();
        VRAMLOADED=true;
    }
}

void Buffer::CloseFile(){
    if(FromFile && FileOpened){
        fclose(File);
        FileOpened=false;
    }
}

bool Buffer::Destroy(){
    UnloadRam();
    UnloadVram();
    CloseFile();
    CREATED=false;
}
