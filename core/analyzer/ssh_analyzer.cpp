#include <iostream>
#include "Analyzer.h"
#include <pcapplusplus/SSHLayer.h>
#include <pcapplusplus/TcpLayer.h>


/*  false : indicates the connection is dead(or previously requested)
    true :  indicates the connection is alive
    Both are used in the currSSH map to determine whether the connection is alive or not
*/
void Analyzer::sshAnalyze(pcpp::Packet* Packet){

    auto* SSHLayer = Packet->getLayerOfType<pcpp::SSHLayer>();
    auto ip = utils->sourceIPExtractor(Packet);
    if(SSHLayer == NULL){
        DROPPED_SSH++;
    }else{
        if(Packet->isPacketOfType(pcpp::TCP)){
            auto *tcpLayer = Packet->getLayerOfType<pcpp::TcpLayer>();
            if(tcpLayer->getTcpHeader()->rstFlag == 1 || tcpLayer->getTcpHeader()->finFlag == 1){
                //Update the IP as no longer maintaining an ssh connection
               utils->SSH_update_map(SSH_ConnectionMap, ip,"Dead");
               emit_ssh(ip,"Disconnected");
            }else{
                //Update the IP is still maintaining a connection
                if(ip != interface_ipv4 && ip != interface_ipv6) {
                    utils->SSH_update_map(SSH_ConnectionMap, ip, "Alive");
                    emit_ssh(ip,"Connected");
                }
            }
        }else{
            DROPPED_SSH++;
        }
    }
}



