#include "Header.h"
/*
void MeshBeta::LoadBuffer(){
    if(Ready&&!BufferLoaded){
        int propnumber=FloatPropertiesArray.size()+GLuintPropertiesArray.size();
        GLuint* Buffers=new GLuint[2+propnumber];
        glGenBuffers(2+propnumber,Buffers);
        VertexBuffer=Buffers[0];
        IndexBuffer=Buffers[1];
        //chargement des donné du vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*3,VertexArray.data(),GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*3,IndexArray.data(),GL_STATIC_DRAW);
       int I=2;
        std::vector<GLuint>::iterator SizeItt=FloatPropertiesGroupSize.begin();
        for(std::vector<std::vector<float>>::iterator Itt=FloatPropertiesArray.begin();Itt!=FloatPropertiesArray.end();Itt++){
            glBindBuffer(GL_ARRAY_BUFFER,Buffers[I]);
            FloatPropertiesBuffer.push_back(Buffers[I]);
            glBufferData(GL_ARRAY_BUFFER,sizeof(float)*(*SizeItt),Itt->data(),GL_STATIC_DRAW);
            SizeItt++;
            I++;
        }
        std::vector<GLuint>::iterator SizeItt2=GLuintPropertiesGroupSize.begin();
        for(std::vector<std::vector<GLuint>>::iterator Itt=GLuintPropertiesArray.begin();Itt!=GLuintPropertiesArray.end();Itt++){
            glBindBuffer(GL_ARRAY_BUFFER,Buffers[I]);
            GLuintPropertiesBuffer.push_back(Buffers[I]);
            glBufferData(GL_ARRAY_BUFFER,sizeof(GLuint)*(*SizeItt2),Itt->data(),GL_STATIC_DRAW);
            I++;
            SizeItt2++;
        }
         glBindBuffer(GL_ARRAY_BUFFER,0);
         BufferLoaded=true;
    }
}
void MeshBeta::GlobalLoad(MixedScene* SC){
    GetVBOAdress(SC);
    LoadBuffer();
}
void MeshBeta::RawDrawCalls(){
            glEnableVertexAttribArray(VertexAdress);
            glVertexAttribPointer(VertexAdress,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,VertexArray.data());
            //on charge les propriété GLuint
            {
                std::vector<std::vector<GLuint>>::iterator ArrItt=GLuintPropertiesArray.begin();
                std::vector<GLuint>::iterator SizItt=GLuintPropertiesGroupSize.begin();
                for(std::vector<GLuint>::iterator AdrItt=GLuintPropertiesAdress.begin();AdrItt!=GLuintPropertiesAdress.end();AdrItt++){
                    glEnableVertexAttribArray(*AdrItt);
                    glVertexAttribPointer(*AdrItt,(*SizItt),GL_UNSIGNED_INT,GL_FALSE,sizeof(GLuint)*(*SizItt),ArrItt->data());
                    SizItt++;
                    ArrItt++;
                    glDisableVertexAttribArray(*AdrItt);

                }
            }
            {
                std::vector<std::vector<float>>::iterator ArrItt=FloatPropertiesArray.begin();
                std::vector<GLuint>::iterator SizItt=FloatPropertiesGroupSize.begin();
                for(std::vector<GLuint>::iterator AdrItt=FloatPropertiesAdress.begin();AdrItt!=FloatPropertiesAdress.end();AdrItt++){
                    glEnableVertexAttribArray(*AdrItt);
                    glVertexAttribPointer(*AdrItt,(*SizItt),GL_FLOAT,GL_FALSE,sizeof(float)*(*SizItt),ArrItt->data());
                    SizItt++;
                    ArrItt++;
                    glDisableVertexAttribArray(*AdrItt);
                }

                //LES INDICES
                glDrawElements(GL_TRIANGLES,IndexArray.size(),GL_UNSIGNED_INT,IndexArray.data());
                glDisableVertexAttribArray(VertexAdress);
            }

}

void MeshBeta::DrawCalls(){
    if(VBOAdressSet){
        if(BufferLoaded){
            glEnableVertexAttribArray(VertexAdress);
            glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
            glVertexAttribPointer(VertexAdress,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,NULL);
            //on charge les propriété GLuint
            {
                std::vector<GLuint>::iterator BuffItt=GLuintPropertiesBuffer.begin();
                std::vector<GLuint>::iterator SizItt=GLuintPropertiesGroupSize.begin();
                for(std::vector<GLuint>::iterator AdrItt=GLuintPropertiesAdress.begin();AdrItt!=GLuintPropertiesAdress.end();AdrItt++){
                    glEnableVertexAttribArray(*AdrItt);
                    glBindBuffer(GL_ARRAY_BUFFER,*BuffItt);
                    glVertexAttribPointer(*AdrItt,(*SizItt),GL_UNSIGNED_INT,GL_FALSE,sizeof(GLuint)*(*SizItt),NULL);
                    BuffItt++;
                    SizItt++;
                    glDisableVertexAttribArray(*AdrItt);

                }
            }
            {
                std::vector<GLuint>::iterator BuffItt=FloatPropertiesBuffer.begin();
                std::vector<GLuint>::iterator SizItt=FloatPropertiesGroupSize.begin();
                for(std::vector<GLuint>::iterator AdrItt=FloatPropertiesAdress.begin();AdrItt!=FloatPropertiesAdress.end();AdrItt++){
                    glEnableVertexAttribArray(*AdrItt);
                    glBindBuffer(GL_ARRAY_BUFFER,*BuffItt);
                    glVertexAttribPointer(*AdrItt,(*SizItt),GL_FLOAT,GL_FALSE,sizeof(float)*(*SizItt),NULL);
                    BuffItt++;
                    SizItt++;
                    glDisableVertexAttribArray(*AdrItt);
                }

                //LES INDICES
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);
                glDrawElements(GL_TRIANGLES,IndexArray.size(),GL_UNSIGNED_INT,NULL);

                glDisableVertexAttribArray(VertexAdress);
            }
        }else{
            RawDrawCalls();
        }

    }
}

void MeshBeta::GetVBOAdress(MixedScene* SC){
    VertexAdress=SC->GetVertexAdress();
    for(std::vector<string>::iterator Itt=FloatPropertiesName.begin();Itt!=FloatPropertiesName.end();Itt++){
        FloatPropertiesAdress.push_back(SC->GetPropertyVBOAdress(*Itt,GL_FLOAT));
    }
    for(std::vector<string>::iterator Itt=GLuintPropertiesName.begin();Itt!=GLuintPropertiesName.end();Itt++){
        GLuintPropertiesAdress.push_back(SC->GetPropertyVBOAdress(*Itt,GL_UNSIGNED_INT));
    }
    VBOAdressSet=true;
}
void MeshBeta::PrepareCallList(){
    if(CallList==0){
        CallList=glGenLists(1);
        glNewList(CallList,GL_COMPILE);
            DrawCalls();
        glEndList();
        NoticeError();
    }

}
*/
