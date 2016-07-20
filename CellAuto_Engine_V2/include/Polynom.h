#pragma once
#include "Header.h"

template<typename T>
class Polynom{
    std::vector<T> Coefs;
    T Zero=static_cast<T>(0);

    public:
        Polynom(){
            Coefs.push_back(Zero);
        }
        Polynom(std::initializer_list<T> coef){
            for(typename std::initializer_list<T>::const_iterator Itt=coef.begin();Itt!=coef.end();Itt++){
                Coefs.push_back((*Itt));
            }
        }
        Polynom(T x){
            Coefs.push_back(x);
        }
        Polynom(T x,size_t i){//Polynom avec un seul ocef au degrée i
            DefineCoef(i,x);
        }
        Polynom(const Polynom<T> &B){
            for(size_t i=0;i<B.CoefCount();i++){
                Coefs.empty();
                Coefs.push_back(B[i]);
            }
        }

        inline size_t deg()const{
            return Coefs.size()-1;
        }
        inline size_t CoefCount()const{
            return Coefs.size();
        }
        T Function(T x)const{
            T y=Zero;
            for(size_t i=0;i<CoefCount();i++){
                T Xpow=x;
                for(size_t k=1;k<i;k++){
                    Xpow=Xpow*x;
                }
                if(i==0){
                    y+=Coef(i);
                }else{
                    y+=Coef(i)*Xpow;
                }
            }
            return y;
        }
        T operator()(T x){
            return Function(x);
        }
        bool Routh_Sign(){
            bool sign=true;
            for(size_t i=0;i<CoefCount();i++){
                sign|=Coefs[i]>=Zero;
            }
            return sign;
        }

        void EchoPolynom(){
            string A="P(X)=";
            char buffp[100];
            for(size_t i=0;i<CoefCount();i++){
                sprintf(buffp,"%f",Coef(i));
                string B=(string)(buffp);
                A+=B;
                if(i>0){
                    A+="*X";
                    if(i>1){
                        A+="^";
                        sprintf(buffp,"%zu",i);
                        B=(string)(buffp);
                        A+=B;
                    }
                }
                if(i!=CoefCount()-1){
                    A+="+";
                }
            }
            ConsoleEcho(A.c_str());
        }

        void DefineCoef(size_t n,T x){
            for(size_t i=CoefCount();i<n+1;i++){
                Coefs.push_back(Zero);
            }
            Coefs[n]=x;
        }
        void AddCoef(size_t n,T x){
            if(n<CoefCount()){
                Coefs[n]+=x;
            }else{
                DefineCoef(n,x);
            }
        }

        void SubstractCoef(size_t n,T x){
            if(n<CoefCount()){
                Coefs[n]-=x;
            }else{
                DefineCoef(n,x);
            }
        }

        T Coef(size_t i)const{
            if(i<CoefCount()){
                return Coefs[i];
            }else{
                return Zero;
            }
        }
        T operator[](size_t i)const{
            return Coef(i);
        }

        T operator()(T x)const{
            return Function(x);
        }

        Polynom<T> operator +(T x)const{
            Polynom C=(*this)+Polynom(x);
            return C;
        }
        Polynom<T> operator *(T x)const{
            Polynom C;
            for(size_t i=0;i<CoefCount();i++){
                C.DefineCoef(i,Coef(i)*x);
            }
            return C;
        }
        Polynom<T> operator /(T x)const{
            Polynom C;
            for(size_t i=0;i<CoefCount();i++){
                C.DefineCoef(Coef(i)/x);
            }
            return C;
        }

        Polynom<T> operator -(T x)const{
            Polynom C=(*this)-Polynom(x);
            return C;
        }

        Polynom<T> operator +(Polynom<T> B)const {
            Polynom C;
            for(size_t i=0;i<std::max(CoefCount(),B.CoefCount());i++){
                T co=Zero;
                if(i<CoefCount())co+=Coef(i);
                if(i<B.CoefCount())co+=B[i];
                C.DefineCoef(i,co);
            }
            return C;
        }
        Polynom<T> operator +=(Polynom<T> B){
            for(size_t i=0;i<B.CoefCount();i++){
                AddCoef(i,B[i]);
            }
            return (*this);
        }
        Polynom<T> operator -=(Polynom<T> B){
            for(size_t i=0;i<B.CoefCount();i++){
                SubstractCoef(i,B[i]);
            }
            return (*this);
        }


        Polynom<T> operator -(Polynom<T> B)const {
            Polynom C;
            for(size_t i=0;i<std::max(CoefCount(),B.CoefCount());i++){
                T co=Zero;
                if(i<CoefCount())co-=Coef(i);
                if(i<B.CoefCount())co-=B[i];
                C.DefineCoef(i,co);
            }
            return C;
        }
        Polynom<T> operator *(Polynom<T> B)const {
            Polynom C;
            for(size_t i=0;i<CoefCount();i++){
                for(size_t j=0;j<B.CoefCount();j++){
                    C.AddCoef(i+j,Coef(i)*B.Coef(j));
                }
            }
            return C;
        }
        Polynom<T> operator /(Polynom<T> B)const {//Division euclidienne
            if(deg()>B.deg()){
                Polynom C;
                Polynom A=Polynom(*this);
                T MSC=B[B.deg()];//Coef le plus élevé de B
                for(size_t i=deg();i!=std::numeric_limits<size_t>::max();i--){//on parcours à l'envers
                    if(B.deg()>i){
                        break;
                    }else{
                        T val=A[i]/MSC;
                        size_t degree=i-B.deg(); //degrée du nombre que l'on trouve
                        A-=B*Polynom(val,degree);
                        A.DefineCoef(i,Zero);
                        if(A.Routh_Sign()){
                            C.AddCoef(degree,val);
                        }else{
                            break;
                        }
                    }
                }
                return C;
            }else{
                return Polynom(Zero);
            }
        }



};

class PolynomTool{
    public:
    static void TestPolynom(){
        ConsoleEcho("Test des Polynomes");
        Polynom<double> C={5,5,6,7};
        C.EchoPolynom();
        Polynom<double> A={5,2};
        A.EchoPolynom();
        ConsoleEcho("Test de la mulitplication");
        (A*C).EchoPolynom();
        (A*8).EchoPolynom();
        ConsoleEcho("Test de la Division");
        ((A*C)/C).EchoPolynom();
    }


};



