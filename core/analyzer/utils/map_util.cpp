#include "Utils.h"


void Utils::IP_update_map(map<string, int> &MAP, const string& ip){
    auto result = MAP.try_emplace(ip, 1);
    if (result.second) {
        // Inserted, now increment
        result.first->second = 1;
    } else {
        // Exists, increment
        result.first->second++;
    }
}

void Utils::SSH_update_map(map<string,string> &MAP, const string& ip, const string& val){
    MAP.try_emplace(ip, val);
}

void Utils::TCP_update_map(map<pair<string,string> ,pair<int,int>>&MAP, const pair<string,string>& ip
                    , const pair<int ,int>& port){
    // S6: Use try_emplace for efficient single-pass insertion
    MAP.try_emplace(ip, port);
}

void Utils::UDP_update_map(map<string ,pair<int,int>>&MAP,
                           const string& ip, const pair<int ,int>& port){
    // S6: Use try_emplace for efficient single-pass insertion
    MAP.try_emplace(ip, port);
}