#pragma once
#define _USE_MATH_DEFINES
//#include <math.h>
#include "Header.h"
class ShaderHandler;

template<GLuint N,typename T=float,GLuint P=N>
class Mat{
    private:
        T Matrice[N][P];
        const size_t Size=N*P;
        const T Zero=(T)0;//je connais meme pas la valeur de zéro putain
        const GLuint InternN=N;
        const GLuint InternP=P;
        //Line major de base ,(i=>ligne , J Colone)
        //partout i et j sont les rang informatique
    public:
        //definition comme une matrice de 0
        Mat(){
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                    Matrice[i][j]=Zero;
                }
            }
        }
        //vu que je ne connais pas le type de variable faire Mat(1) fait l'identité
        Mat(T a){
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                    if(i==j)
                        Matrice[i][j]=a;
                    else
                        Matrice[i][j]=Zero;
                }
            }
        }

        //matrices comme tableau
        Mat(T A[N][P]){
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                        Matrice[i][j]=A[i][j];
                }
            }
        }
        Mat(T A[N*P]){
            GLuint Itt=0;
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                        Matrice[i][j]=A[Itt];
                        Itt++;
                }
            }
        };
        //Mat(T A[P]);

        //pointeur sur l'un des coef
       T* operator()(GLuint i,GLuint j){
           if(i<N && j<P)
                return (&Matrice[i][j]);
            else
                return NULL;
       }
       //acces à la valeur
       T operator[](GLuint ij[2])const{
            if(ij[0]<N && ij[1]<P)
                return (Matrice[ij[0]][ij[1]]);
       }
        T operator[](std::initializer_list<GLuint> IJ)const{
            if( IJ.size()==2 && *(IJ.begin())<N && *(IJ.begin()+1)<P)
                return (Matrice[*(IJ.begin())][*(IJ.begin()+1)]);
       }
       //acces au coef(i,j) depuis la classe
        T coef(GLuint i,GLuint j)const{
            if(i<N && j<P)
                return Matrice[i][j];
        }
        //affectation du coef i,j
       void operator()(GLuint i,GLuint j ,T value){
            if(i<N && j<P)
                Matrice[i][j]=value;
       }
       //transposé
       Mat<P,T,N> transpose()const{
            Mat<P,T,N> MatOut=Mat<P,T,N>();
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                    MatOut(j,i,Matrice[i][j]);
                }
            }
            return MatOut;
       }
       /*T[N*P] vectorize(){
        return (T[N*P])Matrice;
       }*/

       Mat<N,T,P> inverse()const{//méthode des commatrice ou de Laplace
           if(N==P){
                T DetA=det();
                Mat<N,T,P> MatOut=Mat<N,T,P>();
                if(DetA!=0){
                    for(GLuint i=0;i<N;i++){
                        for(GLuint j=0;j<N;j++){
                            T Value=((((*this)%i)/j).det()/DetA)*(1-2*((int)(i+j)%2));
                            //transposition caché ici
                            MatOut(j,i,Value);
                        }
                    }
                }
                return MatOut;
           }else return Mat<N,T,P>();
       }


       Mat<1,T,P> row(GLuint i){
            Mat<1,T,P> MatOut=Mat<1,T,P>();
            for(GLuint j=0;j<P;j++){
                MatOut(0,j,Matrice[{i,j}]);
            }
            return MatOut;
       }

       Mat<N,T,1> col(GLuint j){
            Mat<N,T,1> MatOut=Mat<N,T,1>();
            for(GLuint i=0;i<N;i++){
                MatOut(i,0,Matrice[{i,j}]);
            }
            return MatOut;
       }
       T det()const{
            if(N!=P){
                return Zero;
            }else {
                T** Matr_T=new T*[N];
                for(GLuint i=0;i<N;i++){//on transform la matrice en pointeur
                    Matr_T[i]=new T[N];
                    for(GLuint j=0;j<N;j++){
                        Matr_T[i][j]=Matrice[i][j];
                        //ConsoleEchof("valeur:%f",Matr_T[i][j]);
                    }
                }
                T detA=detRecursive(N,Matr_T);
                for(GLuint i=0;i<N;i++){
                    delete Matr_T[i];
                }
                delete Matr_T;
                return detA;
            }
       }

       T detRecursive(GLuint n,T** Mtr_T)const{
            if(n<2){
                return Mtr_T[0][0];
            }else{
                T detA=Zero;
                for(GLuint i=0;i<n;i++){
                    if(Mtr_T[i][0]!=Zero){//si on a un zero on calcul pas
                        T** NewMtr_T=new T*[n-1];
                        int Itti=0;
                        for(GLuint k=0;k<n;k++){
                            if(k!=i){
                                NewMtr_T[Itti]=new T[n-1];
                                for(GLuint j=1;j<n;j++){// developement par rapport à la 1ere collone
                                        //ConsoleEcho("Erreur Avavnt");
                                        NewMtr_T[Itti][j-1]=Mtr_T[k][j];
                                        //ConsoleEcho("Erreur Apres");
                                    }
                            Itti++;
                            }
                        }
                        //conversion pour éviter les problem de FFFFFF
                        detA+=detRecursive(n-1,NewMtr_T)*(((int)i)%2*(-2)+1)*Mtr_T[i][0];
                        //on delete
                        for(GLuint k=0;k<n-1;k++){
                            delete NewMtr_T[k];
                        }
                        delete NewMtr_T;
                    }
                }
                return detA;
            }
        }




       Mat(std::initializer_list<T> MatriceList){
            if(MatriceList.size()==Size)
                LineMajorInit(MatriceList);
       }

       void ColumnMajorInit(std::initializer_list<T> L){
            typename std::initializer_list<T>::const_iterator Itt;
            Itt=L.begin();
            for(GLuint j=0;j<P;j++){
                for(GLuint i=0;i<N;i++){
                    Matrice[i][j]=(*Itt);
                    Itt++;
                }
            }
       }
        void LineMajorInit(std::initializer_list<T> L){
            typename std::initializer_list<T>::const_iterator Itt;
            Itt=L.begin();
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                    Matrice[i][j]=(*Itt);
                    Itt++;
                }
            }
       }
       void BindDataf(GLuint Location){
            glUniform1fv(Location,N*P,(T**)Matrice);
       }

       void BindDataf(GLenum MatrixMode,bool KeepMode=true){
            if(KeepMode){
                GLint ExMode;
                glGetIntegerv(GL_MATRIX_MODE,&ExMode);
                glMatrixMode(MatrixMode);
                glLoadMatrixf((float*)Matrice);
                glMatrixMode(ExMode);
            }else{
                glMatrixMode(MatrixMode);
                glLoadMatrixf((float*)Matrice);
            }
       }
       void UniformTransfer(ShaderHandler* Shad,GLuint Stage,GLuint Location){
            Shad->ExternSetUniform(GL_FLOAT,N*P,(void*)Matrice,Stage,Location);
       }
        void EchoData(){
            char buff[N*P*20+10];
            char buffm[20];
            strcpy(buff,"{");
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                    sprintf(buffm,"%f",Matrice[i][j]);
                    strcat(buff,buffm);
                    if(j<P-1)
                        strcat(buff,",");
                }
                if(i<N-1)
                    strcat(buff,",\n");
            }
            strcat(buff,"}");
            ConsoleEcho(buff);
        }
        Mat<N,T,P> operator=(Mat<N,T,P> Mat2){
           Mat<N,T,P> MatOut=Mat();
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                    *(*this)(i,j)=*Mat2(i,j);
                }
            }
        }

        Mat<N,T,P> operator+(Mat<N,T,P> Mat2)const{
           Mat<N,T,P> MatOut=Mat();
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                    (*MatOut(i,j))=(*this)[{i,j}]+Mat2[{i,j}];
                }
            }
            return MatOut;
        }
        Mat<N,T,P> operator-(Mat<N,T,P> Mat2)const{
           Mat<N,T,P> MatOut=Mat();
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                    (*MatOut(i,j))=(*this)[{i,j}]-Mat2[{i,j}];
                }
            }
            return MatOut;
        }
        template<GLuint K>
        Mat<N,T,P> operator*(Mat<P,T,K> Mat2)const{
           Mat<N,T,K> MatOut=Mat();
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<K;j++){
                    T sum=Zero;
                    for(GLuint k=0;k<P;k++){
                        sum+=coef(i,k)*Mat2[{k,j}];
                    }
                    MatOut(i,j,sum);
                }
            }
            return MatOut;
        }
        //défini que pour les matrices carré
        Mat<N,T,N> operator*=(Mat<N,T,N> Mat2){
           T MatOut[N][N] ;
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<N;j++){
                    T sum=Zero;
                    for(GLuint k=0;k<N;k++){
                        sum+=coef(i,k)*Mat2[{j,k}];
                    }
                    MatOut[i][j]=sum;
                }
            }
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<N;j++){
                    Matrice[i][j]=MatOut[i][j];
                }
            }
        }
        //Conversion vers une matrice d'un autre type
        template<typename T2>
        operator Mat<N,T2,P>(){
           Mat<N,T2,P> MatOut= Mat<N,T2,P>();
           for(GLuint i=0;i<N;i++){
            for(GLuint j=0;j<P;j++){
                MatOut(i,j,(T2)Matrice[i][j]);
            }
           }
        }

        Mat<N,T,P> operator-(){
           Mat<N,T,P> MatOut=Mat();
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                    (*MatOut(i,j))=-*(*this)(i,j);
                }
            }
            return MatOut;
        }
        //concatenez 2 matrices par les colones
        template<GLuint p>
        Mat<N,T,P+p> operator<<(Mat<N,T,p> Mat2)const{
           Mat<N,T,P+p> MatOut=Mat<N,T,P+p>();
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                    MatOut(i,j,coef(i,j));
                }
            }
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<p;j++){
                    MatOut(i,j+P,Mat2[{i,j}]);
                }
            }
            return MatOut;
        }
        //concatenez 2 matrices par les lignes
        template<GLuint n>
        Mat<N+n,T,P> operator^(Mat<n,T,P> Mat2)const{
           Mat<N+n,T,P> MatOut=Mat<N+n,T,P>();
            for(GLuint i=0;i<N;i++){
                for(GLuint j=0;j<P;j++){
                    MatOut(i,j,coef(i,j));
                }
            }
            for(GLuint i=0;i<n;i++){
                for(GLuint j=0;j<P;j++){
                    MatOut(i+n,j,Mat2[{i,j}]);
                }
            }
            return MatOut;
        }
        // retire la colonne j
        Mat<N,T,P-1> operator/(GLuint j)const{
            Mat<N,T,P-1> MatB =Mat<N,T,P-1>();
            if(j<P){
                for(GLuint i=0;i<N;i++){
                    GLuint Itt=0;//iterateur sur les colone de la matrice destination
                    for(GLuint k=0;k<P;k++){

                        if(k!=j){
                            MatB(i,Itt,Matrice[i][k]);
                            Itt++;
                        }
                    }
                }
                return MatB;
            }else{
                return MatB;
            }
        }
        // retire la ligne i
        Mat<N-1,T,P> operator%(GLuint i)const{
            Mat<N-1,T,P> MatB =Mat<N-1,T,P>();
            if(i<N){
                for(GLuint j=0;j<P;j++){
                    GLuint Itt=0;//iterateur sur les lignes de la matrice destination
                    for(GLuint k=0;k<N;k++){

                        if(k!=i){
                            MatB(Itt,j,Matrice[k][j]);
                            Itt++;
                        }
                    }
                }
                return MatB;
            }else{
           return MatB;
            }
        }
        //operateur de Linéarisation
        operator T*(){
            return (T*)Matrice;
        }


        //determinant par rapport à la colone 1
        T operator~(void)const{
            return this->det();
        }






};
template<typename T=float>
class WorldMatrix{
    public:
        constexpr static T Zero=(T)0;
        static Mat<4,T,4> ModelMatrixRad(T x,T y,T z,T Thx=(T)0,T Thy=(T)0,T Thz=(T)0){
            T Ctx=cos(Thx);
            T Cty=cos(Thy);
            T Ctz=cos(Thz);
            T Stx=sin(Thx);
            T Sty=sin(Thy);
            T Stz=sin(Thz);
            Mat<4,T,4> MatOut={Ctz*Cty,-Stz*Cty,Sty,x,
                            (Ctz*Stx*Sty+Stz*Ctx),(-Stz*Sty*Ctx+Ctz*Ctx),-Stx*Cty,y,
                            (-Ctz*Ctx*Sty+Stz*Stx),(Stz*Ctx*Sty+Ctz*Stx),Cty*Ctx,z,
                            0,0,0,1};
            return MatOut;
        }
        static Mat<4,T,4> ModelMatrix(T x,T y,T z,T Thx=(T)0,T Thy=(T)0,T Thz=(T)0){
            T convertRatio=(M_PI_4/45);
            T Ctx=cos(Thx*convertRatio);
            T Cty=cos(Thy*convertRatio);
            T Ctz=cos(Thz*convertRatio);
            T Stx=sin(Thx*convertRatio);
            T Sty=sin(Thy*convertRatio);
            T Stz=sin(Thz*convertRatio);
            Mat<4,T,4> MatOut={Ctz*Cty,-Stz*Cty,Sty,x,
                            (Ctz*Stx*Sty+Stz*Ctx),(-Stz*Sty*Ctx+Ctz*Ctx),-Stx*Cty,y,
                            (-Ctz*Ctx*Sty+Stz*Stx),(Stz*Ctx*Sty+Ctz*Stx),Cty*Ctx,z,
                            0,0,0,1};
            return MatOut;
        }
        static Mat<4,T,4> Perspective(T fovy,T aspect,T zNear,T zFar){
            if(aspect!=Zero){
                //T f=tan(M_PI_2-(fovy*M_PI_4/90));//cotangeante de fovy
                //T f=atan(fovy*M_PI_4/90);
                //T f=cos(fovy*M_PI_4/90)/sin(fovy*M_PI_4/90);
                T f=1/tan(fovy*M_PI/360);
                /*T ymax = zNear * tan(fovy * M_PI_4/90);
                T ymin = -ymax;
                T xmax = ymax * aspect;
                T xmin = ymin * aspect;

                T width = xmax - xmin;
                T height = ymax - ymin;

                T depth = zFar - zNear;
                T q = -(zFar + zNear) / depth;
                T qn = -2 * (zFar * zNear) / depth;

                T w = 2 * zNear / width;
                w = w / aspect;
                T h = 2 * zNear / height;*/
                /*Mat<4,T,4> MatOut={w,0,0,0,
                                    0,h,0,0,
                                    0,0,q,qn,
                                    0,0,-1,0};*/
                Mat<4,T,4> MatOut={f/aspect,0,0,0,
                                    0,f,0,0,
                                    0,0,(zFar + zNear)/(zNear-zFar),-1,
                                    0,0,2*(zFar*zNear)/(zNear-zFar),0};
                return MatOut;

            }else return Mat<4,T,4>();
        }
        static void TestMatrix(){
            Mat<4,double,4> MatA={1.2,2.1,5.2,3,
                                1.3,4.3,2,5.2,
                                1,4,6,2.3,
                                7,9,15,1};
            Mat<4,double,4> MatC={4,11,11,11,
                                11,4,11,11,
                                11,11,4,11,
                                11,11,11,4};
            Mat<4,double,4> MatD={1,2,0,0,
                                0,1,0,1,
                                6,0,1,5,
                                0,0,1,1};
                        *MatA(1,1)=1;
                        MatA.EchoData();
            Mat<4,double> MatB=Mat<4,double>(1);
            MatB.EchoData();
            (MatA+MatB).EchoData();
            ConsoleEcho("Test mulMatrix");
            (MatC*MatD).EchoData();
            (MatA<<MatB).EchoData();
            (MatA^MatB).EchoData();
            (MatA/2).EchoData();
            (MatA%2).EchoData();
            ConsoleEcho("test des inverse");
            (MatB.inverse()).EchoData();
            (MatB.inverse()*MatB).EchoData();
            (MatA.inverse()).EchoData();
            (MatA.inverse()*MatA).EchoData();
            ConsoleEchof("det B=%f",MatB.det());
            ConsoleEchof("det B=%f",Mat<3>(1).det());
            ConsoleEchof("det B=%f",MatD.det());
            WorldMatrix<float>::ModelMatrix(0,0,0,60,0,0).EchoData();
            (WorldMatrix<double>::ModelMatrix(0,0,0,0,0,60)*WorldMatrix<double>::ModelMatrix(0,0,0,0,60,0)).EchoData();
            WorldMatrix<double>::ModelMatrix(0,0,0,0,60,60).EchoData();
        }



};

