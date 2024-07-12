#include <iostream>
#include "Analyzer.h"

void Analyzer::icmp_analyze(pcpp::Packet *Packet) {
    if(Packet->isPacketOfType(pcpp::ICMP )
       || Packet->isPacketOfType(pcpp::ICMPv6) ){
        string srcIP = utils->sourceIPExtractor(Packet);
        //Make sure that only Incoming packets are analyzed
        if(srcIP != interface_ipv4 && srcIP != interface_ipv6){
            if(checkInMap(utils->sourceIPExtractor(Packet))){
                ping_map[srcIP] = ++ping_map[srcIP];
            }else{
                ping_map[srcIP] = 1;
            }
        }
    }
}

bool Analyzer::checkInMap(const string& val) {
    string pingSrc;
    auto it = ping_map.find(val);
    if(it != ping_map.end()){
        if(ping_map[val] >= THRESHOLD){
            pingSrc = val;
            emit emit_icmp();
        }
        return true;
    }
    return false;
}


