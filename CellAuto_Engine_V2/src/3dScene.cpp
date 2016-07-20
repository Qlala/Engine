#include "Header.h"





template<>
void Scene3d<float>::TransferMatrices(){
    ComputeCamMatrix();
    if(CamLocSet)glUniformMatrix4fv(CamMatrixLocation,1,false,(float*)CameraMatrix);
    if(ProjLocSet)glUniformMatrix4fv(ProjMatrixLocation,1,false,(float*)ProjectionMatrix);
    if(ModelLocSet)glUniformMatrix4fv(ModelMatrixLocation,1,true,(float*)(ModelMatrix));
}
template<>
void Scene3d<double>::TransferMatrices(){
    ComputeCamMatrix();
    if(CamLocSet)glUniformMatrix4dv(CamMatrixLocation,1,false,(double*)CameraMatrix);
    if(ProjLocSet)glUniformMatrix4dv(ProjMatrixLocation,1,false,(double*)ProjectionMatrix);
    if(ModelLocSet)glUniformMatrix4dv(ModelMatrixLocation,1,true,(double*)ModelMatrix);
    }




