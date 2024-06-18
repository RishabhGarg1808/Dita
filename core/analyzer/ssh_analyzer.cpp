#include <iostream>
#include "ssh_anlayzer.h"
#include <pcapplusplus/SSHLayer.h>
#include <pcapplusplus/TcpLayer.h>
#include "utils/map_util.h"
#include "utils/ip_utils.h"
#include "utils/defines.h"

int DROPPED_SSH=0;

/*  false : indicates the connection is dead(or previously requested)
    true :  indicates the connection is alive
    Both are used in the currSSH map to determine whether the connection is alive or not
*/
std::map<std::string ,std::string> SSH_ConnectionMap;

void sshAnalyze(pcpp::Packet* Packet){

    auto* SSHLayer = Packet->getLayerOfType<pcpp::SSHLayer>();
    auto ip = sourceIPExtractor(Packet);
    if(SSHLayer == NULL){
        DROPPED_SSH++;
    }else{
        if(Packet->isPacketOfType(pcpp::TCP)){
            auto *tcpLayer = Packet->getLayerOfType<pcpp::TcpLayer>();
            if(tcpLayer->getTcpHeader()->rstFlag == 1 || tcpLayer->getTcpHeader()->finFlag == 1){
                //Update the IP as no longer maintaining an ssh connection
                SSH_update_map(SSH_ConnectionMap, ip,"Dead");
            }else{
                //Update the IP is still maintaining a connection
                if(ip != interface_ipv4 && ip != interface_ipv6)
                SSH_update_map(SSH_ConnectionMap, ip, "Alive");
            }
        }else{
            DROPPED_SSH++;
        }
    }
}

