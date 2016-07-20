#include "Header.h"
bool MixedScene::RenderInit(RenderObject* ptr){
    if(ShadersLoaded){
        ptr->Define3dRender(true);
        ptr->DefineDepthedPP(true);
        InitDeclaredShader(ptr);
        InitDeclaredTarget(ptr);
        AddShaderAsMainShader(RenderShaderList,RenderShaderN,ptr);
        AddShaderAsPostProcessShader(PostProcessShaderList,PostProcessShaderN,ptr);
        FrameColorLoc=ptr->GetLocation(FrameColorUniName,RealProgram(ProgramForSampling));
        FrameSizeUniLoc=ptr->GetLocation(FrameSizeUniName,RealProgram(ProgramForSampling));
        FrameColorAmbiLoc=ptr->GetAmbivalentLocation(FrameColorUniName,RealProgram(ProgramForSampling),GL_TEXTURE_2D,GL_TEXTURE_2D_MULTISAMPLE);
        MSAA_SampleLoc=ptr->GetLocation(MSAA_SampleUniName,RealProgram(ProgramForSampling));
        FrameColorAmbiLoc.MSType=GL_TEXTURE_2D_MULTISAMPLE;
        FrameColorAmbiLoc.NormalType=GL_TEXTURE_2D;
        ptr->PreparePostProcessBuffer();
        ptr->SetPostProcessState(true);
        ptr->RenderInFrameBuffer();
        Init_DeclaredMatrix(ptr);
        return true;
    }else{
    return false;
    }
}
bool MixedScene::RenderPostProc(RenderObject* ptr){
    ConsoleEcho("MixedScene PP");
    NoticeGlError(true);
    //SwapFrameToSource(ptr,FrameColorLoc,1);
    ptr->BindAmbivalentFrameToSlot(FrameColorAmbiLoc,1);
    ptr->ComToShaderFrameResolution(FrameSizeUniLoc);
    ptr->ComToShaderMSAAsample(MSAA_SampleLoc);
    //SwapZBufferToSource(ptr,FrameDepthLoc,2);
    SwapAllTargetToSource(ptr,3);
    NoticeGlError(true);
    glBegin(GL_POINTS);
        glVertex2f(0,0);
    glEnd();

   /* if(theta==100.0f)
        ptr->SaveFrameBufferTextureAsset("Test3.bmp");*/
    NoticeGlError(true);
}

MixedScene::MixedScene(){
    Ready=true;
}
inline void CutLastChar(char* str){
    int i=0;
    while(str[i]!='\0')i++;
    str[i-1]='\0';
}

GLuint MixedScene::GetPropertyVBOAdress(string Prop_Name,GLuint Prop_Type){
    for(std::vector<Scene_VertexProperty>::iterator Itt=PropertiesArray.begin();Itt!=PropertiesArray.end();Itt++){
        if(Itt->Name==Prop_Name and Itt->Type==Prop_Type){
            return Itt->Adress;
            break;
        }
    }
    return 0;
}

void LoadIniWarper(Scene* SC,void* P){
    char* F=static_cast<char*>(P);
    (static_cast<MixedScene*>(SC))->Intern_LoadFromIni2(F);
}
void MixedScene::LoadFromIni(const char*Filename){
    CallSceneAPC(LoadIniWarper,(void*)Filename);
}

void MixedScene::Intern_LoadFromIni2(char* Filename){
    ConsoleEcho("Ouverture de L'ini:");
    IniDecoder dec;
    dec.Decode(Filename);
    dec.EchoData();
    /*VertexAdress=(GLuint)(dec.SearchProp("VBO_Definition").Search("VertexAdress"));
    PropertiesArray=ConvertAll<SceneProperty>(dec.SearchProps("VBO_Definition.Property"),{"PropertyName","PropertyType","PropertyAdress"},{INI_STRING,INI_GL_TYPE,INI_UNSIGNED_INT});
    */
    InitProperties_FromIni(&dec);
    InitShaders_FromIniDecoder(&dec);
    InitRenderTargets_FromIniDecoder(&dec);
    InitScene3d_FromIni(&dec);
    IniProp SR=dec.SearchProp("Shader.Render");
    //shader list
    std::vector<GLuint> RenderShadervec=SR.Search("ShaderList").ConvertVector<GLuint>();
    RenderShaderList=new GLuint[RenderShadervec.size()];
    RenderShaderN=RenderShadervec.size();
    memcpy(RenderShaderList,RenderShadervec.data(),RenderShadervec.size()*sizeof(GLuint));
    IniProp SP=dec.SearchProp("Shader.PostProcess");
    //PP
    ProgramForSampling=(GLuint)SP.Search("ShaderForSampling");
    std::vector<GLuint> PostShadervec=SP.Search("ShaderList").ConvertVector<GLuint>();
    PostProcessShaderList=new GLuint[PostShadervec.size()];
    PostProcessShaderN=PostShadervec.size();
    memcpy(PostProcessShaderList,PostShadervec.data(),PostShadervec.size()*sizeof(GLuint));
    FrameColorUniName=(string)SP.Search("FrameColorUniform");
    FrameDepthUniName=(string)SP.Search("FrameDepthUniform");
    FrameSizeUniName=(string)SP.Search("FrameSizeUniform");
    MSAA_SampleUniName=(string)SP.Search("FrameMSAA_Sample");
    ConsoleEcho("Fin de lecture de l'ini");
    ShadersLoaded=true;
};

void MixedScene::Intern_LoadFromIni(char* Filename){//Atttention Quentin les saut de ligne dans les .ini le fait bugger (pense à fixer ca un jour stp connard)
    ConsoleEcho("Ouverture de L'ini:");
    ConsoleEcho(Filename);
    FILE* Ini=fopen(Filename,"r+");
    char buff[255];
    char* buffp;
    char* buffp2;
    //tableau pour les shader
    struct ShaderStor{
        string FP;GLuint Type;GLuint BitType;GLuint Nbr;bool operator<(const ShaderStor St){return Nbr<St.Nbr;}};
    struct RenderTar{
        string N;GLuint Res;GLuint Rank;GLuint Format;float Ratio;GLuint Program;bool operator<(const RenderTar tr){return Rank<tr.Rank;}
    };
    std::list<ShaderStor> ShadArray;
    std::list<RenderTar> TarList;
    if(Ini!=NULL){
        fgets(buff,255,Ini);
        while(feof (Ini)==0){
            CutLastChar(buff);
            // recherche de bloc
            if(strcmp(buff,"[VBO_Definition]")==0){
                fgets(buff,255,Ini);
                while(buff[0]!='['&& feof(Ini)==0){
                    CutLastChar(buff);
                    buffp=strtok(buff,"=");
                    if(strcmp(buffp,"VertexAdress")==0){
                        buffp=strtok(NULL,"");
                        sscanf(buffp,"%u",&VertexAdress);
                        VBO_Data_Found=true;//on a trouvé la donnée importante pour faire fonctionné les shader
                    }
                    fgets(buff,255,Ini);
                }
            //bloc de vbo def
            }else if(strcmp(buff,"[VBO_Definition.Property]")==0){
                fgets(buff,255,Ini);
                int Count=0;// on compte lse propriétésnque l'on trouve
                Scene_VertexProperty Prop;
                /*string NameA;
                GLuint Adress;
                GLuint Type;*/
                while(buff[0]!='['&& feof(Ini)==0){
                    CutLastChar(buff);
                    buffp=strtok(buff,"=");
                    //recherche de propriété du block
                    if(strcmp(buffp,"PropertyName")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)Prop.Name=(string)buffp;
                        Count++;
                    }else if(strcmp(buffp,"PropertyAdress")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)sscanf(buffp,"%u",&(Prop.Adress));
                        Count++;
                    }else if(strcmp(buffp,"PropertyType")==0){
                        buffp=strtok(NULL,"");
                        Prop.Type=GLtool::DecodeGL_Type((string)buffp);
                        Count++;
                    }
                    fgets(buff,255,Ini);
                }
                //si on a tous réunis
                if(Count==3){
                    PropertiesArray.push_back(Prop);
                    /*PropertiesName.push_back(NameA);
                    ConsoleEcho("on a trouvé ca");
                    ConsoleEcho(NameA.c_str());
                    PropertiesAdress.push_back(Adress);
                    PropertiesType.push_back(Type);*/
                }
            //nouveau bloc: Shader Declaration
            }else if(strcmp(buff,"[ShaderDeclaration]")==0){
                fgets(buff,255,Ini);
                short Count=0;// on compte lse propriétésnque l'on trouve
                ShaderStor Stor;
                while(buff[0]!='['&& feof(Ini)==0){
                    CutLastChar(buff);
                    buffp=strtok(buff,"=");
                    //recherche de propriété du block
                    if(strcmp(buffp,"Filepath")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)Stor.FP=(string)buffp;
                        Count++;
                    }else if(strcmp(buffp,"Type")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)
                            if(strcmp(buffp,"GL_GEOMETRY_SHADER")==0){
                                Stor.Type=GL_GEOMETRY_SHADER;
                                Stor.BitType=GL_GEOMETRY_SHADER_BIT;
                                Count++;
                            }else if(strcmp(buffp,"GL_VERTEX_SHADER")==0){
                                Stor.Type=GL_VERTEX_SHADER;
                                Stor.BitType=GL_VERTEX_SHADER_BIT;
                                Count++;
                            }else if(strcmp(buffp,"GL_FRAGMENT_SHADER")==0){
                                Stor.Type=GL_FRAGMENT_SHADER;
                                Stor.BitType=GL_FRAGMENT_SHADER_BIT;
                                Count++;
                            }else if(strcmp(buffp,"GL_TESS_CONTROL_SHADER")==0){
                                Stor.Type=GL_TESS_CONTROL_SHADER;
                                Stor.BitType=GL_TESS_CONTROL_SHADER_BIT;
                                Count++;
                            }else if(strcmp(buffp,"GL_TESS_EVALUATION_SHADER")==0){
                                Stor.Type=GL_TESS_EVALUATION_SHADER;
                                Stor.BitType=GL_TESS_EVALUATION_SHADER_BIT;
                                Count++;
                            }
                    }else if(strcmp(buffp,"ProgramNumber")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)sscanf(buffp,"%u",&(Stor.Nbr));
                        Count++;

                    }
                    fgets(buff,255,Ini);
                }
                //fin de bloc opération
                if(Count==3){
                        ShadArray.push_back(Stor);
                }


               }else if(strcmp(buff,"[RenderTarget]")==0){
                fgets(buff,255,Ini);
                short Count=0;// on compte lse propriétésnque l'on trouve
                RenderTar Tar;
                while(buff[0]!='['&& feof(Ini)==0){
                    CutLastChar(buff);
                    buffp=strtok(buff,"=");
                    //recherche de propriété du block
                    if(strcmp(buffp,"Name")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)Tar.N=(string)buffp;
                        ConsoleEcho(Tar.N.c_str());
                        Count++;
                    }else if(strcmp(buffp,"Resolution")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)
                            if(strcmp(buffp,"GL_RGB8")==0){
                                Tar.Res=GL_RGB8;
                                Count++;
                            }else if(strcmp(buffp,"GL_DEPTH_COMPONENT16")==0){
                                Tar.Res=GL_DEPTH_COMPONENT16;
                                Count++;
                            }else if(strcmp(buffp,"GL_R8")==0){
                                Tar.Res=GL_R8;
                                Count++;
                            }else if(strcmp(buffp,"GL_R16")==0){
                                Tar.Res=GL_R16;
                                Count++;
                            }else if(strcmp(buffp,"GL_R32F")==0){
                                Tar.Res=GL_R32F;
                                Count++;
                            }else if(strcmp(buffp,"GL_DEPTH_COMPONENT32F")==0){
                                Tar.Res=GL_DEPTH_COMPONENT32F;
                                Count++;
                            }else if(strcmp(buffp,"GL_RGB16F")==0){
                                Tar.Res=GL_RGB16F;
                                Count++;
                            }else if(strcmp(buffp,"GL_RGB32F")==0){
                                Tar.Res=GL_RGB32F;
                                Count++;
                            }else if(strcmp(buffp,"GL_RGBA8")==0){
                                Tar.Res=GL_RGBA8;
                                Count++;
                            }else if(strcmp(buffp,"GL_RGBA16")==0){
                                Tar.Res=GL_RGBA16;
                                Count++;
                            }
                    }else if(strcmp(buffp,"Rank")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)sscanf(buffp,"%u",&(Tar.Rank));
                        Count++;
                    }else if(strcmp(buffp,"Program")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)sscanf(buffp,"%u",&(Tar.Program));
                        Count++;
                    }else if(strcmp(buffp,"Format")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)
                            if(strcmp(buffp,"GL_RGB")==0){
                                Tar.Format=GL_RGB;
                                Count++;
                            }else if(strcmp(buffp,"GL_DEPTH_COMPONENT")==0){
                                Tar.Format=GL_DEPTH_COMPONENT;
                                Count++;
                             }else if(strcmp(buffp,"GL_RED")==0){
                                Tar.Format=GL_RED;
                                Count++;
                            }else if(strcmp(buffp,"GL_BGR")==0){
                                Tar.Format=GL_BGR;
                                Count++;
                            }else if(strcmp(buffp,"GL_RGBA")==0){
                                Tar.Format=GL_RGBA;
                                Count++;
                            }
                    }else if(strcmp(buffp,"Ratio")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)sscanf(buffp,"%f",&(Tar.Ratio));
                        Count++;
                    }
                    fgets(buff,255,Ini);
                }
                if(Count==6){
                    TarList.push_back(Tar);
                }
            }else if(strcmp(buff,"[Shader.Render]")==0){
                fgets(buff,255,Ini);
                while(buff[0]!='['&& feof(Ini)==0){
                    CutLastChar(buff);
                    buffp=strtok(buff,"=");
                    //recherche de propriété du block
                    if(strcmp(buffp,"ShaderForMatrix")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)sscanf(buffp,"%i",&ProgramForMatrix);
                    }else if(strcmp(buffp,"ShaderList")==0){
                        buffp=strtok(NULL,"");
                        int I=0;
                        RenderShaderN=1;
                        while(buffp[I]!='\0'){
                            I++;
                            if(buffp[I]==',')RenderShaderN++;
                        }
                        RenderShaderList=new GLuint[RenderShaderN];
                        buffp2=strtok(buffp,",");
                        GLuint A;
                        sscanf(buffp2,"%i",&A);
                        RenderShaderList[0]=A;
                        for(int i=1;i<RenderShaderN;i++){
                            buffp2=strtok(NULL,",");
                            sscanf(buffp2,"%i",&A);
                            RenderShaderList[i]=A;
                        }

                    /*}else if(strcmp(buffp,"ProjectionMatrix")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)ProjectionUniName=(string)buffp;
                    }else if(strcmp(buffp,"CameraMatrix")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)CamMatrixUniName=(string)buffp;
                    }else if(strcmp(buffp,"ModelMatrix")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)ModelMatrixUniName=(string)buffp;*/
                    }

                    fgets(buff,255,Ini);
                }
            }else if(strcmp(buff,"[Shader.PostProcess]")==0){
                fgets(buff,255,Ini);
                while(buff[0]!='['&& feof(Ini)==0){
                    CutLastChar(buff);
                    buffp=strtok(buff,"=");
                    //recherche de propriété du block
                    if(strcmp(buffp,"ShaderForSampling")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)sscanf(buffp,"%i",&ProgramForSampling);
                    }else if(strcmp(buffp,"ShaderList")==0){
                        buffp=strtok(NULL,"");
                        int I=0;
                        PostProcessShaderN=1;
                        while(buffp[I]!='\0'){
                            I++;
                            if(buffp[I]==',')PostProcessShaderN++;
                        }
                        PostProcessShaderList=new GLuint[PostProcessShaderN];
                        buffp2=strtok(buffp,",");
                        GLuint A;
                        sscanf(buffp2,"%i",&A);
                        PostProcessShaderList[0]=A;
                        for(int i=1;i<PostProcessShaderN;i++){
                            buffp2=strtok(NULL,",");
                            sscanf(buffp2,"%i",&A);
                            PostProcessShaderList[i]=A;
                        }

                    }else if(strcmp(buffp,"FrameColorUniform")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)FrameColorUniName=(string)buffp;
                    }else if(strcmp(buffp,"FrameDepthUniform")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)FrameDepthUniName=(string)buffp;
                    }else if(strcmp(buffp,"FrameSizeUniform")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)FrameSizeUniName=(string)buffp;
                    }else if(strcmp(buffp,"FrameMSAA_Sample")==0){
                        buffp=strtok(NULL,"");
                        if(buffp!=NULL)MSAA_SampleUniName=(string)buffp;
                    }
                    fgets(buff,255,Ini);
                }
            //dernier cas ou nouveau block
            }else{
                fgets(buff,255,Ini);
            }
        }//fin du bloc while
    }else{
        ConsoleEcho("Echec du chargement de L'Ini");
        GetCurrentDirectory(255,buff);
        ConsoleEcho(buff);
    }
    //operation de fin de chargement
    fclose(Ini);
  ConsoleEcho("Ini de Scene:Lecture Fini");

  ShadArray.sort();
  TarList.sort();
  GLuint LastNbr=0; //je me rapelle que les program user commence à 1
  for(std::list<ShaderStor>::iterator A =ShadArray.begin();A!=ShadArray.end();A++){
    if(LastNbr==A->Nbr){
        MergeShaderWithLast(A->FP,A->Type,A->BitType);
    }else{
        NewShader(A->FP,A->Type,A->BitType);
        LastNbr=A->Nbr;
    }
    ShadersLoaded=true;
  }
  for(std::list<RenderTar>::iterator A =TarList.begin();A!=TarList.end();A++){
        NewRenderTarget(A->N,A->Res,A->Format,A->Program);
        ConsoleEcho(A->N.c_str());
  }
}
void MixedScene::InitUniform(){
    WaitContext(INFINITE);
    if(Context_Received){
        // on s'ocupe d'abord des Uniform qui sont simple
        /*ProjectionLoc=glGetUniformLocation(ShaderArray[ProgramForMatrix].Program,ProjectionUniName.c_str());
        CamMatrixLoc=glGetUniformLocation(ShaderArray[ProgramForMatrix].Program,CamMatrixUniName.c_str());
        ModelMatrixLoc=glGetUniformLocation(ShaderArray[ProgramForMatrix].Program,ModelMatrixUniName.c_str());*/
        UniformInitied=true;
    }

};


bool MixedScene::RenderUpdate(RenderObject* ptr){
    ConsoleEcho("Rendu Scene Mixed");

        //ptr->DesactivatePipeline();
        /*glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glRotatef(theta, 0.0f, 0.0f, 1.0f);*/
            //(WorldMatrix<float>::ModelMatrix(0,0,0,0,0,theta).inverse()).BindDataf(GL_MODELVIEW,false);
           // glMatrixMode(GL_PROJECTION);
            //WorldMatrix<float>::Perspective(70,16/9,0.1,100000).BindDataf(GL_PROJECTION,false);
           //glMatrixMode(GL_PROJECTION);
           // glLoadIdentity();
            //gluPerspective(70,16/9,0.1,1000);
            //float Proj[16];
            //glGetFloatv(GL_PROJECTION_MATRIX,Proj);
            /*Mat<4> gluProj=Mat<4>(Proj);
            gluProj.EchoData();*/
            //WorldMatrix<float>::Perspective(80,16/9,0.1,1000).EchoData();
            //glTranslatef(0,0,cos(theta));
            //CamIdentity();
            MoveCamera(0,0,0);
            RotateCamera(0,0,0.1);
            SetModelMatrix(WorldMatrix<float>::ModelMatrix(0,0,-5,0,0,0));
            DefineProjection(16/9,70,1,10000);
            TransferMatrices();
            glBegin(GL_TRIANGLES);
            //glBegin(GL_POINTS);
                glColor3f(1.0f, 0.0f, 0.0f);   glVertex3f(0.0f,   1.0f,-0.0f);
            //glEnd()

            //glBegin(GL_POINTS);
                glColor3f(0.0f, 1.0f, 0.0f);   glVertex3f(0.87f,  -0.5f,-0.0f);
            //glEnd();

            //glBegin(GL_POINTS);
                glColor3f(0.0f, 0.0f, 1.0f);   glVertex3f(-0.87f, -0.5f,-0.0f);
            glEnd();

            theta += 1.0f;

}
