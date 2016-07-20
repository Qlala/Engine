#pragma once
#include "Header.h"
template<typename S>
std::vector<S> ConvertAll(std::vector<IniProp> props,std::vector<string> order,std::vector<IniType> structformat){
    std::vector<S> out;
    for(std::vector<IniProp>::iterator Itt=props.begin();Itt!=props.end();Itt++){
        S temp;
        Itt->Filter(order);
        temp=Itt->Convert<S>(structformat);
        out.push_back(temp);
    }
    return out;
}

extern std::vector<string> split(const string &text, const char sep);

template<typename T>
IniVar::operator std::vector<T>(){
    std::vector<string> preout;
    preout=split(Value,',');
    std::vector<T> out;
    for(std::vector<string>::iterator Itt=preout.begin();Itt!=preout.end();Itt++){
        out.push_back((T)(*Itt));
    }
    return out;
};
template<typename T>
std::vector<T> IniVar::ConvertVector(){
    std::vector<string> preout;
    preout=split(Value,',');
    std::vector<T> out;
    for(std::vector<string>::iterator Itt=preout.begin();Itt!=preout.end();Itt++){
        out.push_back((T)(IniVar){"",*Itt});//j'ai putain d'eu la flemmme lol
    }
    return out;
};
