#include "Header.h"
bool RenderObject::CheckFBComplete(){
    ConsoleEcho("Check FB Complete");
    GLenum test=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(test!=GL_FRAMEBUFFER_COMPLETE){
        ConsoleEcho("FrameBuffer incomplet\0");
        return true;
    }else{
        ConsoleEcho("FrameBuffer complet\0");
        return false;
    }
}
void RenderObject::SetPostProcessState(bool state){
    if(PostBufferInited && this->RenderPPProgramInitied and state){
        RenderPostProcActivated=true;
        ConsoleEcho("RO : PostProcess Activé");
    }
}




void RenderObject::PrepareBuffer(){
    glGenFramebuffers(1,&FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);

    glGenTextures(1,&ColorTexture);
    glBindTexture(GL_TEXTURE_2D,ColorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_RGBA32F, RenderWidth, RenderHeight);
    glTexImage2D(GL_TEXTURE_2D,0,ColorBitDepth,RenderWidth,RenderHeight,0,GL_RGBA,GL_FLOAT,NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,ColorTexture,0);
    GLuint buff[1]={GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1,buff);
    if(B3D_Rendering){
         glGenTextures(1,&ZbufferTexture);
        glBindTexture(GL_TEXTURE_2D,ZbufferTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_DEPTH_COMPONENT32F, RenderWidth, RenderHeight);
        glTexImage2D(GL_TEXTURE_2D,0,ZBitDepth,RenderWidth,RenderHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
        glBindTexture(GL_TEXTURE_2D,0);

        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,ZbufferTexture,0);

    }
    GLenum test=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(test!=GL_FRAMEBUFFER_COMPLETE){
        ConsoleEcho("FrameBuffer incomplet\0");
        BufferInited=false;
    }else{
        ConsoleEcho("FrameBuffer complet\0");
        BufferInited=true;
    }
}
void RenderObject::PrepareSamplingBuffer(int Sample){
    MSAA_Sample=Sample;
    ConsoleEcho("Buffer avec un MSAA:%i",MSAA_Sample);
    glEnable( GL_MULTISAMPLE );
    glGenFramebuffers(1,&FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);

    glGenTextures(1,&ColorTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,ColorTexture);
    //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_RGBA32F, RenderWidth, RenderHeight);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,MSAA_Sample,ColorBitDepth,RenderWidth,RenderHeight,GL_FALSE);
    glBindTexture(GL_TEXTURE_2D,0);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,ColorTexture,0);
    GLuint buff[1]={GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1,buff);

    GLenum test=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(test!=GL_FRAMEBUFFER_COMPLETE){
        ConsoleEcho("FrameBuffer incomplet\0");
        BufferInited=false;
    }else{
        ConsoleEcho("FrameBuffer complet\0");
        BufferInited=true;
    }

}
 void RenderObject::InitZbuffer(){
    if(0<MSAA_Sample){
        glEnable( GL_MULTISAMPLE);
        glGenTextures(1,&ZbufferTexture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,ZbufferTexture);

        /*glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
        //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_DEPTH_COMPONENT32F, RenderWidth, RenderHeight);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,MSAA_Sample,ZBitDepth,RenderWidth,RenderHeight,GL_FALSE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
        glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D_MULTISAMPLE,ZbufferTexture,0);
        B3D_Rendering=true;
    }else{
        glGenTextures(1,&ZbufferTexture);
        glBindTexture(GL_TEXTURE_2D,ZbufferTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_DEPTH_COMPONENT32F, RenderWidth, RenderHeight);
        glTexImage2D(GL_TEXTURE_2D,0,ZBitDepth,RenderWidth,RenderHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
        glBindTexture(GL_TEXTURE_2D,0);
        glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,ZbufferTexture,0);
        B3D_Rendering=true;
    }

 };
 void RenderObject::InitStencilBuffer(){
    if(BufferInited){
        if(0<MSAA_Sample){
            glEnable( GL_MULTISAMPLE);
            glGenTextures(1,&StencilBuffer);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,StencilBuffer);

            /*glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
            //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_DEPTH_COMPONENT32F, RenderWidth, RenderHeight);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,MSAA_Sample,StencilDepth,RenderWidth,RenderHeight,GL_FALSE);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
            glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_TEXTURE_2D_MULTISAMPLE,StencilBuffer,0);
            Stenciled_Rendering=true;
        }else{
            glGenTextures(1,&StencilBuffer);
            glBindTexture(GL_TEXTURE_2D,StencilBuffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_DEPTH_COMPONENT32F, RenderWidth, RenderHeight);
            glTexImage2D(GL_TEXTURE_2D,0,StencilDepth,RenderWidth,RenderHeight,0,GL_STENCIL_INDEX,GL_FLOAT,NULL);
            glBindTexture(GL_TEXTURE_2D,0);
            glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_TEXTURE_2D,StencilBuffer,0);
            Stenciled_Rendering=true;
        }

    }


 }


void RenderObject::InitDepthPPBuffer(){
    glGenTextures(1,&DepthDisplayTexture);
    glBindTexture(GL_TEXTURE_2D,DepthDisplayTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_DEPTH_COMPONENT32F, RenderWidth, RenderHeight);
    glTexImage2D(GL_TEXTURE_2D,0,ZBitDepth,RenderWidth,RenderHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    glBindTexture(GL_TEXTURE_2D,0);
    glBindFramebuffer(GL_FRAMEBUFFER,PostProcessBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,DepthDisplayTexture,0);
    DepthedPostProcess=true;
};

void RenderObject::DeleteDepthPPBuffer(){
    glDeleteTextures(1,&DepthDisplayTexture);
    DepthedPostProcess=false;
}

void RenderObject::DeleteStencilBuffer(){
    glDeleteTextures(1,&StencilBuffer);
    Stenciled_Rendering=false;
}


void RenderObject::DefineMainShaderProgram(GLuint* PrgAdress,GLuint* PrgType,int n){
    if(IsShaderReady()){
        Render_Pipeline=CreateNewPipeline();
        SwapPipeline(Render_Pipeline);
        SetActiveInternProgram(PrgAdress,PrgType,n);
        if(IsShaderProgramSet()){
            RenderShaderAdress=PrgAdress;
            RenderShaderType=PrgType;
            RenderShaderN=n;
            RenderDefaultProgramInitied=true;
        }else{
            DeletePipeline(Render_Pipeline);
        }
    }
};

void RenderObject::RestoreMainShader(){
    if(RenderDefaultProgramInitied){
        SwapPipeline(Render_Pipeline);
        ActivatePipeline();
    }else{
        DesactivatePipeline();
    }
}
void RenderObject::ClearMainShader(bool del){

    RenderDefaultProgramInitied=false;
    if(del){
        for(int i=0;i<RenderShaderN;i++){
            DeleteProgram(RenderShaderAdress[i]);
        }
    }
    DeletePipeline(Render_Pipeline);
    delete RenderShaderAdress;
    delete RenderShaderType;
}
void RenderObject::ClearPPShader(bool del){

    RenderPPProgramInitied=false;
    if(del){
        for(int i=0;i<RenderPPShaderN;i++){
            DeleteProgram(RenderPPShaderAdress[i]);
        }
    }
    DeletePipeline(PP_Pipeline);
    delete RenderPPShaderAdress;
    delete RenderPPShaderType;
}


void RenderObject::DefinePPShaderProgram(GLuint* PrgAdress,GLuint* PrgType,int n){
    if(IsShaderReady()){
        PP_Pipeline=CreateNewPipeline();
        SwapPipeline(PP_Pipeline);
        SetActiveInternProgram(PrgAdress,PrgType,n);
        if(IsShaderProgramSet()){
            RenderPPShaderAdress=PrgAdress;
            RenderPPShaderType=PrgType;
            RenderPPShaderN=n;
            RenderPPProgramInitied=true;
            ConsoleEcho("Ro:PostProcessDefine");
        }else{
            DeletePipeline(PP_Pipeline);
        }
    }
};
void RenderObject::RestorePPShader(){
    if(RenderPPProgramInitied){
        SwapPipeline(PP_Pipeline);
        ActivatePipeline();
        //InternSetActiveInternProgram(RenderPPShaderAdress,RenderPPShaderType,RenderPPShaderN);
    }else{
        DesactivatePipeline();
    }
}
void RenderObject::PrepareRenderTarget(){
    if(!PostBufferInited)PreparePostProcessBuffer();
    glBindFramebuffer(GL_FRAMEBUFFER,PostProcessBuffer);

}
void RenderObject::PreparePostProcessBuffer(){
    if(!PostBufferInited){
        ConsoleEcho("Génération du PostProcessBuffer");
        //Génration des framebuffer
        glGenFramebuffers(1,&PostProcessBuffer);
        glGenTextures(1,&ColorDisplayTexture);
        glBindTexture(GL_TEXTURE_2D,ColorDisplayTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_RGBA32F, RenderWidth, RenderHeight);
        glTexImage2D(GL_TEXTURE_2D,0,ColorBitDepth,RenderWidth,RenderHeight,0,GL_RGBA,GL_FLOAT,NULL);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,0);
        glBindFramebuffer(GL_FRAMEBUFFER,PostProcessBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,ColorDisplayTexture,0);
        GLuint buff[1]={GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1,buff);
        if(DepthedPostProcess)glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,DepthDisplayTexture,0);
        PostBufferInited=true;
        NoticeFBError(GL_FRAMEBUFFER);
    }
}

void RenderObject::DeletePostBuffer(){
    if(PostBufferInited){
        glDeleteFramebuffers(1,&PostProcessBuffer);
        glDeleteTextures(1,&ColorDisplayTexture);
        DeleteDepthPPBuffer();
        PostBufferInited=false;
    }

}

bool RenderObject::CallRender(){
    WaitForSingleObject(RenderHung,INFINITE);
    if(SourceBound && BufferInited){
        RenderObject* me=static_cast<RenderObject*>(this);
        if(!SourceInitied){
            SourceInitied=Source->RenderInit(me);
        }
        if(SourceInitied){
            EnableRenderMethod();
            RestoreMainShader();

            if(RenderPostProcActivated){
                //Les fonction sont par défault dans le contexte habituelle
                RenderInFrameBuffer();
                if(AutoCleanBuffer)CleanBuffer();
                Source->RenderUpdate(me);
                //ConsoleEcho("Rendu avec le PP");
                RestorePPShader();
                EnablePostProcessMethod();
                PrepareRenderTarget();
                if(AutoCleanBuffer){
                    ClearColor();
                    ClearDepth();
                }
                Source->RenderPostProc(me);

                FinishRender(PostProcessBuffer);
            }else{
                //PrepareRenderTarget();
                RenderInFrameBuffer();
                if(AutoCleanBuffer)CleanBuffer();
                Source->RenderUpdate(me);
                FinishRender(FrameBuffer);
            }

        }
    }else{
        glBindProgramPipeline(0);
        //ConsoleEcho("et hop un Rendu");
        PrepareRenderTarget();
        if(AutoCleanBuffer)CleanBuffer();
        EnableRenderMethod();
        glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glRotatef(theta, 0.0f, 0.0f, 1.0f);

            glBegin(GL_TRIANGLES);
            //glBegin(GL_POINTS);
                glColor3f(1.0f, 0.0f, 0.0f);   glVertex3f(0.0f,   1.0f,1.0);
            //glEnd();

            //glBegin(GL_POINTS);
                glColor3f(0.0f, 1.0f, 0.0f);   glVertex3f(0.87f,  -0.5f,1.0f);
            //glEnd();

            //glBegin(GL_POINTS);
                glColor3f(0.0f, 0.0f, 1.0f);   glVertex3f(-0.87f, -0.5f,1.0f);
            glEnd();

            theta += 1.0f;
        FinishRender(FrameBuffer);
    }

    ReleaseMutex(RenderHung);
}
void RenderObject::RenderInFrameBuffer(){
    if(!BufferInited)PrepareSamplingBuffer(MSAA_Sample);
    glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);
    glViewport(0,0,RenderWidth,RenderHeight);
    //glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,ColorTexture,0);
}

bool RenderObject::BindRenderSource(RenderSource*Src){
    Source=Src;
    //RenderObject* me=static_cast<RenderObject*>(this);
    SourceInitied=false;
    SourceBound=true;
    ConsoleEcho("Binding compltement fini");
    RenderPostProcActivated=Src->IsPostProcess();
}

void RenderObject::UnbindRenderSource(){
    if(SourceInitied){
        RenderObject* me=static_cast<RenderObject*>(this);
        Source->RenderOnQuit(me);
        SourceInitied=false;
    }
    Source=nullptr;
    SourceBound=false;
}
bool RenderObject::AskGlContext(){
    ConsoleEcho("Tentative d'envoie de Context");
    if(Context_Initied){
        ConsoleEcho("Tentative d'envoie de Context : Initialisation Confirmé");
        bool A=wglMakeCurrent(DeviceContext,SharedRenderContext);
        NoticeError();
        return A;


    }else return false;

}


void RenderObject::CleanBuffer(){
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glClearDepthf(pow(10,16));
    if(B3D_Rendering)glClear(GL_DEPTH_BUFFER_BIT);
}
void RenderObject::DeleteBuffer(){
    if(BufferInited){
        glDeleteFramebuffers(1,&FrameBuffer);
        GLuint tex[2]={ColorTexture,ZbufferTexture};
        glDeleteTextures(2,tex);
        BufferInited=false;
    }
}

void RenderObject::DeleteZbuffer(){
    if(B3D_Rendering){
        glDeleteTextures(1,&ZbufferTexture);
        B3D_Rendering=false;
    }
}

void RenderObject::SwapBufferRule(int RW,int RH,int sample,GLuint* TextureType,GLuint* TextureIndex,int N){
    if(BufferInited){
        DeleteBuffer();
    }
    RenderWidth=RW;
    RenderHeight=RH;
    MSAA_Sample=sample;
    if(MSAA_Sample>0){
         glEnable( GL_MULTISAMPLE );
        glGenFramebuffers(1,&FrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);

        glGenTextures(1,&ColorTexture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,ColorTexture);
        //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_RGBA32F, RenderWidth, RenderHeight);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,MSAA_Sample,ColorBitDepth,RenderWidth,RenderHeight,GL_FALSE);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,ColorTexture,0);


        GLuint* buff=new GLuint[N+1];
        buff[0]=GL_COLOR_ATTACHMENT0;
        for(int i=0;i<N;i++){
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1+i,TextureType[i],TextureIndex[i],0);
            buff[1+i]=GL_COLOR_ATTACHMENT0+i;
        }
        glDrawBuffers(N+1,buff);
        delete buff;
        if(B3D_Rendering){
            glGenTextures(1,&ZbufferTexture);
            glBindTexture(GL_TEXTURE_2D,ZbufferTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_DEPTH_COMPONENT32F, RenderWidth, RenderHeight);
            glTexImage2D(GL_TEXTURE_2D,0,ZBitDepth,RenderWidth,RenderHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
            glBindTexture(GL_TEXTURE_2D,0);

            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,ZbufferTexture,0);
        }
        BufferInited=true;
    }else{
        glDisable(GL_MULTISAMPLE);
        glGenFramebuffers(1,&FrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);

        glGenTextures(1,&ColorTexture);
        glBindTexture(GL_TEXTURE_2D,ColorTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_RGBA32F, RenderWidth, RenderHeight);
        glTexImage2D(GL_TEXTURE_2D,0,ColorBitDepth,RenderWidth,RenderHeight,0,GL_RGBA,GL_FLOAT,NULL);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,0);

        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,ColorTexture,0);
        //on met les Donnné des buffer proposé en mémoire
        GLuint* buff=new GLuint[N+1];
        buff[0]=GL_COLOR_ATTACHMENT0;
        for(int i=0;i<N;i++){
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1+i,TextureType[i],TextureIndex[i],0);
            buff[1+i]=GL_COLOR_ATTACHMENT1+i;
        }
        glDrawBuffers(N+1,buff);
        delete buff;
        if(B3D_Rendering){
            glGenTextures(1,&ZbufferTexture);
            glBindTexture(GL_TEXTURE_2D,ZbufferTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //glTexStorage2D(GL_TEXTURE_2D, 1 ,GL_DEPTH_COMPONENT32F, RenderWidth, RenderHeight);
            glTexImage2D(GL_TEXTURE_2D,0,ZBitDepth,RenderWidth,RenderHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
            glBindTexture(GL_TEXTURE_2D,0);

            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,ZbufferTexture,0);
        }
        BufferInited=true;
    }


}
void RenderObject::ResizeRender(int H, int W){

}


void RenderObject::DefineDrawBuffer(GLuint* TextureType,GLuint* TextureIndex,int N){
    glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);
    GLuint* buff=new GLuint[N+1];
    buff[0]=GL_COLOR_ATTACHMENT0;
    for(int i=0;i<N;i++){
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1+i,TextureType[i],TextureIndex[i],0);
        buff[1+i]=GL_COLOR_ATTACHMENT0+i;
    }
    glDrawBuffers(N+1,buff);
    delete buff;

}

void RenderObject::EnableRenderMethod(){
    if(MSAA_Sample>0){
        glEnable(GL_MULTISAMPLE);
    }else{
        glDisable(GL_MULTISAMPLE);
    }

    if(B3D_Rendering){
        glEnable(GL_DEPTH_TEST);
    }else{
        glDisable(GL_DEPTH_TEST);
    }
}

void RenderObject::EnablePostProcessMethod(){
    if(DepthedPostProcess){
            glEnable(GL_DEPTH_TEST);
        }else{
            glDisable(GL_DEPTH_TEST);
        }
        glDisable(GL_MULTISAMPLE);
}

RenderObject::RenderObject(){
    RenderHung=CreateMutex(NULL,false,GenStrWithUniqueCode("RenderObject.RenderHung"));
}

RenderObject::~RenderObject(){
    CloseHandle(RenderHung);
    if(RenderDefaultProgramInitied){
        delete RenderShaderAdress;
        delete RenderShaderType;
    }
    if(RenderPPProgramInitied){
        delete RenderPPShaderAdress;
        delete RenderPPShaderType;
    }
}

void RenderObject::ClearColor(){
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
}
void RenderObject::ClearDepth(){
    glClear(GL_DEPTH_BUFFER_BIT);
}

void RenderObject::DefineAutoClean(bool state){
    WaitForSingleObject(RenderHung,INFINITE);
    AutoCleanBuffer=state;
    ReleaseMutex(RenderHung);
}

void RenderObject::Define3dRender(bool state){
    if(state && !B3D_Rendering){
        InitZbuffer();
    }else if(!state && B3D_Rendering){
        DeleteZbuffer();
    }
}

void RenderObject::DefineDepthedPP(bool state){
    if(state && !DepthedPostProcess){
        InitDepthPPBuffer();
    }else if(!state && DepthedPostProcess){
        DeleteDepthPPBuffer();
    }
};

void RenderObject::BindFrameToSlot(GLuint slot){
    ConsoleEcho("Bin fram Slot");
    if(BufferInited){
            glActiveTexture(GL_TEXTURE0+slot);

            glBindTexture((MSAA_Sample>0)?GL_TEXTURE_2D_MULTISAMPLE:GL_TEXTURE_2D,ColorTexture);
            NoticeGlError(true);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture((MSAA_Sample>0)?GL_TEXTURE_2D_MULTISAMPLE:GL_TEXTURE_2D,0);
            NoticeGlError(true);
    }
}

void RenderObject::BindAmbivalentTextureToSlot(AmbivalentUniform AmbUni,GLuint TexToBind,GLuint slot){
    ConsoleEcho("BindAmbivalentFrame to Slot");
    if(MSAA_Sample>0){
        ConsoleEcho("MS");
        glUniform1i(AmbUni.MultiSampled,slot);
        glActiveTexture(GL_TEXTURE0+slot);
        glBindTexture(AmbUni.MSType,TexToBind);
        NoticeGlError(true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(AmbUni.MSType,0);
    }else{
        ConsoleEcho("Normal");
        glUniform1i(AmbUni.Normal,slot);
        ConsoleEcho("BindTexture");
        glActiveTexture(GL_TEXTURE0+slot);
        glBindTexture(AmbUni.NormalType,TexToBind);
        NoticeGlError(true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(AmbUni.NormalType,0);
    }
}

void RenderObject::SetAmbivalentUniform(AmbivalentUniform AmbUni,GLuint slot){
if(MSAA_Sample>0){
        glUniform1i(AmbUni.MultiSampled,slot);
    }else{
        glUniform1i(AmbUni.Normal,slot);
    }
};




void RenderObject::BindAmbivalentFrameToSlot(AmbivalentUniform AmbUni,GLuint slot){
    BindAmbivalentTextureToSlot(AmbUni,ColorTexture,slot);
};
void RenderObject::BindAmbivalentStencilToSlot(AmbivalentUniform AmbUni,GLuint slot){
    BindAmbivalentTextureToSlot(AmbUni,StencilBuffer,slot);
};
void RenderObject::BindAmbivalentDepthToSlot(AmbivalentUniform AmbUni,GLuint slot){
    BindAmbivalentTextureToSlot(AmbUni,ZbufferTexture,slot);
};


void RenderObject::BindZBufferToSlot(GLuint slot){
    if(BufferInited){
            glActiveTexture(GL_TEXTURE0+slot);
            glBindTexture(GL_TEXTURE_2D,ZbufferTexture);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,0);
    }
}

GLuint RenderObject::AddBufferRule(GLuint Color,GLuint Type,GLuint BufferFormat,float Ratio){
    DrawBuffer rule={Color,Type,BufferFormat,Ratio,false,0};
    DrawBuffers.push_back(rule);
    return DrawBuffers.size()-1;
}
GLuint RenderObject::AddBufferRule(GLuint Color,GLuint BufferFormat){
    GLuint Type=MSAA_Sample>0?GL_TEXTURE_2D_MULTISAMPLE:GL_TEXTURE_2D;
    DrawBuffer rule={Color,Type,BufferFormat,1.0,false,0};
    DrawBuffers.push_back(rule);
    return DrawBuffers.size()-1;
}

void RenderObject::InitDrawBuffer(GLuint N,bool Force){
    std::vector<DrawBuffer>::iterator A=DrawBuffers.begin()+N;
    InitDrawBuffer(A,Force);
}

void RenderObject::InitDrawBuffer(std::vector<DrawBuffer>::iterator A,bool Force){
    if(Force==true){
        if(!A->Initied){
                int W,H;
                W=RenderWidth*A->Ratio;
                H=RenderHeight*A->Ratio;
                glGenTextures(1,&(A->Buffer));
                glBindTexture(A->TextureType,A->Buffer);
                glTexParameteri(A->TextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(A->TextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(A->TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(A->TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                if(A->TextureType==GL_TEXTURE_2D_MULTISAMPLE){
                    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,MSAA_Sample,A->ColorRes,W,H,GL_FALSE);
                }else{
                    glTexImage2D(A->TextureType,0,A->ColorRes,W,H,0,A->BufferFormat,GL_FLOAT,NULL);
                }
                glBindTexture(A->TextureType,0);
                A->Initied=true;
        }
    }else{
        if(A->Initied){
            glDeleteBuffers(1,&(A->Buffer));
            A->Initied=false;
        }
        int W,H;
        W=RenderWidth*A->Ratio;
        H=RenderHeight*A->Ratio;
        glGenTextures(1,&(A->Buffer));
        glBindTexture(A->TextureType,A->Buffer);
        glTexParameteri(A->TextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(A->TextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(A->TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(A->TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if(A->TextureType==GL_TEXTURE_2D_MULTISAMPLE){
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,MSAA_Sample,A->ColorRes,W,H,GL_FALSE);
        }else{
            glTexImage2D(A->TextureType,0,A->ColorRes,W,H,0,A->BufferFormat,GL_FLOAT,NULL);
            glGenerateMipmap(A->TextureType);
        }
        glBindTexture(A->TextureType,0);
        A->Initied=true;
    }
}

void RenderObject::ActivateBufferRule(bool Force){
    GLenum* DBufr=new GLenum[DrawBuffers.size()+1];
    DBufr[0]=GL_COLOR_ATTACHMENT0;
    int I=1;
    glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);
    for(std::vector<DrawBuffer>::iterator A=DrawBuffers.begin();A!=DrawBuffers.end();A++){
        if(Force && A->Initied){
            glDeleteBuffers(1,&(A->Buffer));
            A->Initied=false;
        }
        if(!A->Initied){
            int W,H;
            W=RenderWidth*A->Ratio;
            H=RenderHeight*A->Ratio;
            glGenTextures(1,&(A->Buffer));
            glBindTexture(A->TextureType,A->Buffer);
            glTexParameteri(A->TextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(A->TextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(A->TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(A->TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            if(A->TextureType==GL_TEXTURE_2D_MULTISAMPLE){
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,MSAA_Sample,A->ColorRes,W,H,GL_FALSE);
            }else{
                glTexImage2D(A->TextureType,0,A->ColorRes,W,H,0,A->BufferFormat,GL_FLOAT,NULL);
            }
            glBindTexture(A->TextureType,0);
            A->Initied=true;
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+I,A->TextureType,A->Buffer,0);
        DBufr[I]=GL_COLOR_ATTACHMENT0+I;
        I++;
        }
    }
    glDrawBuffers(I+1,DBufr);

}

GLuint RenderObject::AccessBuffer(GLuint N){
    std::vector<DrawBuffer>::iterator Itt=DrawBuffers.begin();
    return (Itt+(N))->Buffer;
}
void RenderObject::DeleteAllBufferRule(){
    for(std::vector<DrawBuffer>::iterator A=DrawBuffers.begin();A!=DrawBuffers.end();A++){
        if(A->Initied){
            glDeleteBuffers(1,&(A->Buffer));
        }
    }
    DrawBuffers.clear();
}
void RenderObject::BindBufferToSlot(GLuint buffer,GLuint slot){
    if(0<buffer<DrawBuffers.size()){
        std::vector<DrawBuffer>::iterator A=DrawBuffers.begin()+buffer;
        if(A->Initied){
            glActiveTexture(GL_TEXTURE0+slot);
            glBindTexture(A->TextureType,A->Buffer);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(A->TextureType,0);
        }
    }
}

void RenderObject::SetBufferRule(GLuint* Plan,GLuint N){
    NoticeGlError();
    ConsoleEcho("Validation du Plan");
    GLuint* DBufr=new GLuint[N+1];
    DBufr[0]=GL_COLOR_ATTACHMENT0;
    glBindFramebuffer(GL_FRAMEBUFFER,FrameBuffer);
    for(GLuint I=0;I<N;I++){
        if(Plan[I]<DrawBuffers.size()){
            std::vector<DrawBuffer>::iterator A=DrawBuffers.begin()+Plan[I];
            ConsoleEcho("Ajout du buffer:%i",I+1);
            if(!A->Initied){
                int W,H;
                NoticeGlError(true);
                ConsoleEcho("Création d'une Texture:%i",A->TextureType);
                W=RenderWidth*A->Ratio;
                H=RenderHeight*A->Ratio;
                ConsoleEcho("de Taille H:%i",H);
                ConsoleEcho("W:%i",W);
                ConsoleEcho("De Résolution:%i",A->ColorRes);
                ConsoleEcho("De format:%i",A->BufferFormat);
                glGenTextures(1,&(A->Buffer));
                ConsoleEcho("De Buffer:%i",A->Buffer);
                glBindTexture(A->TextureType,A->Buffer);
                /*glTexParameteri(A->TextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(A->TextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(A->TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(A->TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
                NoticeGlError(true);
                if(A->TextureType==GL_TEXTURE_2D_MULTISAMPLE){
                    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,MSAA_Sample,A->ColorRes,W,H,GL_FALSE);
                }else{
                    glTexImage2D(A->TextureType,0,A->ColorRes,W,H,0,A->BufferFormat,GL_FLOAT,NULL);
                    glGenerateMipmap(A->TextureType);
                }
                glBindTexture(A->TextureType,0);
                A->Initied=true;
            }
            NoticeGlError(true);
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+I+1,A->TextureType,A->Buffer,0);
            NoticeGlError(true);
            DBufr[I+1]=GL_COLOR_ATTACHMENT0+I+1;
            ConsoleEcho("Fin de l'ajout du buffer");

        }
    }
    /*glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,MSAA_Sample>0?GL_TEXTURE_2D_MULTISAMPLE:GL_TEXTURE_2D,ColorTexture,0);
    if(B3D_Rendering){
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,MSAA_Sample>0?GL_TEXTURE_2D_MULTISAMPLE:GL_TEXTURE_2D,ZbufferTexture,0);
    }*/
    glDrawBuffers(N+1,DBufr);
    NoticeFBError(GL_FRAMEBUFFER);
    //glBindFramebuffer(GL_FRAMEBUFFER,0);
    //delete DBufr;
}


void RenderObject::DeleteBufferRule(GLuint N){
    std::vector<DrawBuffer>::iterator Itt=DrawBuffers.begin();
    Itt+=(N);
    if(Itt->Initied)
        glDeleteBuffers(1,&(Itt->Buffer));
    DrawBuffers.erase(Itt);
}

//Communication to Shader

void RenderObject::ComToShaderFrameResolution(GLuint Location){
    glUniform2i(Location,RenderWidth,RenderHeight);
}
void RenderObject::ComToShaderMSAAsample(GLuint Location){
    glUniform1i(Location,MSAA_Sample);
}
void RenderObject::ComToShaderDepthActivated(GLuint Location){
    glUniform1i(Location,(int)B3D_Rendering);
}


//Fonction user
void RenderObject::SaveFrameBufferTextureAsset(string Filename){
    TextureAsset* tex =new TextureAsset();
    tex->CreateFromTextureId(GL_TEXTURE_2D,ColorDisplayTexture,false,"FrameBuffer");
    tex->LoadRAM_FromVRAM();
    tex->EchoTextureInfo(true);
    tex->SaveBMP(Filename);
    tex->EchoTextureInfo(true);
}

void RenderObject::ShareContext(){
    wglMakeCurrent(DeviceContext,UserSharedRenderContext);
}

void RenderObject::GiveBackContext(){
    wglMakeCurrent(DeviceContext,NULL);
}




