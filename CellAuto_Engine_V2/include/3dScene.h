#pragma once
#include "Header.h"
//template du format des opérateur 3d
template<typename T>
class Scene3d : public Scene{
    Mat<4,T,4> CameraMatrix;
    Mat<4,T,4> NotInvertedCamMatrix;
    bool CamMatrixUpdated=true;

    Mat<4,T,4> ProjectionMatrix;
    Mat<4,T,4> ModelMatrix;
    //zone des donné d'adress de Matrice
    bool ProjLocSet=false;
    bool CamLocSet=false;
    bool ModelLocSet=false;
    GLuint ProgramForCamMatrix;
    GLuint ProgramForProjMatrix;
    GLuint ProgramForModelMatrix;
    GLuint CamMatrixLocation;
    GLuint ProjMatrixLocation;
    GLuint ModelMatrixLocation;
    //Predeclared;
    bool Matrix_Declared=false;
    string ProjectionUniName;
    string CamMatrixUniName;
    string ModelMatrixUniName;
    public:
        void InitScene3d_FromIni(IniDecoder* ini){
            IniProp S3d=ini->SearchProp("Scene.Scene3D");
            ProgramForProjMatrix=(GLuint)S3d.Search("ShaderForProjMatrix");
            ProgramForCamMatrix=(GLuint)S3d.Search("ShaderForCamMatrix");
            ProgramForModelMatrix=(GLuint)S3d.Search("ShaderForModelMatrix");
            ProjectionUniName=(string)S3d.Search("ProjectionMatrix");
            CamMatrixUniName=(string)S3d.Search("CameraMatrix");
            ModelMatrixUniName=(string)S3d.Search("ModelMatrix");
            Matrix_Declared=true;
        }
        void Init_DeclaredMatrix(RenderObject* ptr){
            if(Matrix_Declared){
                SetCamMatrixInfo(CamMatrixUniName,ProgramForCamMatrix,ptr);
                SetProjMatrixInfo(ProjectionUniName,ProgramForProjMatrix,ptr);
                SetModelMatrixInfo(ModelMatrixUniName,ProgramForModelMatrix,ptr);
            }
        }

        //fonction identité (mise ici pour des problem de template)
        void CamIdentity(){
                CameraMatrix=Mat<4,T>(1);
                NotInvertedCamMatrix=Mat<4,T>(1);
                CamMatrixUpdated=false;
        };
        void ProjIdentity(){
            ProjectionMatrix=Mat<4,T>(1);
        };
        void ModelIdentity(){
            ModelMatrix=Mat<4,T>(1);
        }

        //Constructeur Scene 3d
        Scene3d(){CamIdentity();ProjIdentity(); ModelIdentity();};
        //fonction de déplacement de caméra
        void MoveCamera(T x,T y,T z){
            NotInvertedCamMatrix*=WorldMatrix<T>::ModelMatrix(x,y,z,0,0,0);
            CamMatrixUpdated=false;
        }
        void RotateCamera(T thx,T thy ,T thz){
            NotInvertedCamMatrix*=WorldMatrix<T>::ModelMatrix(0,0,0,thx,thy,thz);
            CamMatrixUpdated=false;
        }
        void MoveCameraTo(T x,T y,T z);
        void MoveAndCompute(T x,T y,T z);
        void RotateAndCompute(T x,T y,T z);

        void DefineProjection(T WHratio,T fov,T nearplane,T farplane){
            ProjectionMatrix=WorldMatrix<T>::Perspective(fov,WHratio,nearplane,farplane);
        }

        //calcul et transfer
        void ComputeCamMatrix(){
            if(!CamMatrixUpdated){
                CameraMatrix=NotInvertedCamMatrix.inverse();
                CamMatrixUpdated=true;
            }
        }
        void TransferMatrices();
        void SetModelMatrix(Mat<4,T> Model){
            ModelMatrix=Model;
        }
        void MulModelMatrix(Mat<4,T> Model){
            ModelMatrix*=Model;
        }
        void SetCamMatrixInfo(GLuint Location,GLuint Prg){
                ProgramForCamMatrix=Prg;
                CamMatrixLocation=Location;
                CamLocSet=true;
        }
        void SetProjMatrixInfo(GLuint Location, GLuint Prg){
            ProgramForProjMatrix=Prg;
            ProjMatrixLocation=Location;
            ProjLocSet=true;
        }

        void SetModelMatrixInfo(GLuint Location, GLuint Prg);
        void SetCamMatrixInfo(string Name,GLuint ScenePrg,RenderObject* RO){
            ProgramForCamMatrix=RealProgram(ScenePrg);
            CamMatrixLocation=RO->GetLocation(Name,ProgramForCamMatrix);
            ConsoleEcho("CamMat Location found:%x",CamMatrixLocation);
            CamLocSet=true;
        }

        void SetProjMatrixInfo(string Name,GLuint ScenePrg,RenderObject* RO){
            ProgramForProjMatrix=RealProgram(ScenePrg);
            ProjMatrixLocation=RO->GetLocation(Name,ProgramForProjMatrix);
            ConsoleEcho("ProjMat Location found:%x",ProjMatrixLocation);
            ProjLocSet=true;
        }

        void SetModelMatrixInfo(string Name,GLuint ScenePrg,RenderObject* RO){
            ProgramForModelMatrix=RealProgram(ScenePrg);
            ModelMatrixLocation=RO->GetLocation(Name,ProgramForProjMatrix);
           ConsoleEcho("ModelMat Location found:%x",ModelMatrixLocation);
            ModelLocSet=true;
        }
};
