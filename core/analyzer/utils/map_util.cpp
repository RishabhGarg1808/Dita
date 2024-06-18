#include "map_util.h"

map<string,string>::iterator SSH_it;
map<pair<string,string> ,pair<int,int>>::iterator TCP_it;
map<string ,pair<int,int>>::iterator UDP_it;

void IP_update_map(map<string, int> &MAP,const string& ip){
    auto it = MAP.find(ip);
    if(it != MAP.end()){
        MAP[ip]++;
    }else{
        pair<string,int> IP;
        IP.first = ip;
        IP.second = 1;
        MAP.insert(IP);
    }
}

void SSH_update_map(map<string,string> &MAP,const string& ip,const string& val){
    SSH_it = MAP.find(ip);
    if(SSH_it != MAP.end()){
        MAP[ip] = val;
    }else{
        pair<string,string> IP;
        IP.first = ip;
        IP.second = val;
        MAP.insert(IP);
    }
}

void TCP_update_map(map<pair<string,string> ,pair<int,int>>&MAP,const pair<string,string>& ip
                    ,const pair<int ,int>& port){

    TCP_it = MAP.find(ip);
    //CHeck if the value exists in the map if it does then update it else insert the values with key
    if(TCP_it != MAP.end()){
        MAP[ip] = port;
    }else{
        pair<pair<string,string>,pair<int,int>> IP_PORT;
        IP_PORT.first = ip;
        IP_PORT.second = port;
        MAP.insert(IP_PORT);
    }
}

void UDP_update_map(map<string ,pair<int,int>>&MAP,
                    const string& ip,const pair<int ,int>& port){

    UDP_it = MAP.find(ip);
    //CHeck if the value exists in the map if it does then update it else insert the values with key
    if(UDP_it != MAP.end()){
        MAP[ip] = port;
    }else{
        pair<string,pair<int,int>> IP_PORT;
        IP_PORT.first = ip;
        IP_PORT.second = port;
        MAP.insert(IP_PORT);
    }
}

bool is_existing(map<pair<string,string>, pair<int,int>>&MAP,const pair<string,string>& ip){

    TCP_it = MAP.find(ip);
    if(TCP_it != MAP.end()){
        return true;
    }else{
        return false;
    }
}