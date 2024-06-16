#include "map_util.h"

std::map<std::string,bool>::iterator SSH_it;
std::map<std::pair<std::string,bool> ,std::pair<int,int>>::iterator TCP_it;

void IP_update_map(std::map<std::string, int> &MAP,const std::string& ip){
    auto it = MAP.find(ip);
    if(it != MAP.end()){
        MAP[ip]++;
    }else{
        std::pair<std::string,int> IP;
        IP.first = ip;
        IP.second = 1;
        MAP.insert(IP);
    }
}

void SSH_update_map(std::map<std::string, bool> &MAP,const std::string& ip,bool val){
    SSH_it = MAP.find(ip);
    if(SSH_it != MAP.end()){
        MAP[ip] = val;
    }else{
        std::pair<std::string,bool> IP;
        IP.first = ip;
        IP.second = val;
        MAP.insert(IP);
    }
}
void TCP_update_map(std::map<std::pair<std::string,bool> ,std::pair<int,int>>&MAP,const std::pair<std::string,bool>& ip
                    ,const std::pair<int ,int>& port){

    TCP_it = MAP.find(ip);
    //CHeck if the value exists in the map if it does then update it else insert the values with key
    if(TCP_it != MAP.end()){
        MAP[ip] = port;
    }else{
        std::pair<std::pair<std::string,bool>,std::pair<int,int>> IP_PORT;
        IP_PORT.first = ip;
        IP_PORT.second = port;
        MAP.insert(IP_PORT);
    }
}
bool is_existing(std::map<std::pair<std::string,bool>, std::pair<int,int>>&MAP,const std::pair<std::string,bool>& ip){

    TCP_it = MAP.find(ip);
    if(TCP_it != MAP.end()){
        return true;
    }else{
        return false;
    }
}