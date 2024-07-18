#include "Analyzer.h"
#include <string>
#include <list>
#include <algorithm>
#include <TcpLayer.h>

bool Analyzer::check_list(const std::string& ip_packet){
    if(TCP_PendingConnectionWatchList.empty()){
        return false;
    }else{
        auto it = find(TCP_PendingConnectionWatchList.begin(),
                            TCP_PendingConnectionWatchList.end(),
                            ip_packet);
        if(it != TCP_PendingConnectionWatchList.end()) {
            return true;
        }else{
            return false;

        }
    }
}
bool Analyzer::checkifOutgoing(const std::string& IP){

    auto it = TCP_ConnectionMap_outgoing.find(std::make_pair(IP, "Alive"));
    auto it_another = TCP_ConnectionMap_outgoing.find(std::make_pair(IP,"Dead"));

    if(it != TCP_ConnectionMap_outgoing.end() || it_another != TCP_ConnectionMap_outgoing.end()){
        return true;
    }else{
        return false;
    }

}

void Analyzer::ssh_close(const std::string& src){
    auto it = SSH_ConnectionMap.find(src);
    if(it != SSH_ConnectionMap.end()) {
        utils->SSH_update_map(SSH_ConnectionMap, src, "Dead");
        emit_ssh(src,"Disconnected");
    }
}

/* If a packet has SYN flag is set then we observe the packet for the connection
 * if the no ACK flag is set then the connection was dropped/terminated
 * if the RST/FIN flag is set then the connection was dropped/terminated
 * if the ACK flag is set then the connection was established
 * */

void Analyzer::tcp_analyze(pcpp::Packet *Packet) {
    auto *TcpLayer = Packet->getLayerOfType<pcpp::TcpLayer>();
    auto Packet_src = utils->sourceIPExtractor(Packet);
    auto Packet_dest = utils->destIPExtractor(Packet);
    auto port_pair = std::make_pair(TcpLayer->getSrcPort(),TcpLayer->getDstPort());

    if (TcpLayer->getTcpHeader()->synFlag == 1 && TcpLayer->getTcpHeader()->ackFlag == 0) {
        TCP_PendingConnectionWatchList.push_back(Packet_dest);
    }
    /* check if the packet originated for the Local_machine
    * if the packet originated from the local_machine classify the connection as outgoing
    * else classify the connection as incoming
    */
    if (TcpLayer->getTcpHeader()->synFlag == 1 || TcpLayer->getTcpHeader()->ackFlag == 1) {
        if (check_list(Packet_dest)) {
            TCP_PendingConnectionWatchList.remove(Packet_dest);
        }
        if (Packet_src == interface_ipv4 || Packet_src == interface_ipv6) {
            utils->TCP_update_map(TCP_ConnectionMap_outgoing, std::make_pair(Packet_dest, "Alive") ,port_pair);
        } else {
            if(checkifOutgoing(Packet_dest)){
                utils->TCP_update_map(TCP_ConnectionMap_incoming,std::make_pair(Packet_src,"Alive"), port_pair);
            }
        }
    }
    /* Performing the same for the TCP_CONNECTION_CLOSED */
    if (TcpLayer->getTcpHeader()->rstFlag == 1 || TcpLayer->getTcpHeader()->finFlag == 1) {
        ssh_close(Packet_src);
        ssh_close(Packet_dest);
            if (check_list(Packet_src)) {
                TCP_PendingConnectionWatchList.remove(Packet_src);
            }
            if (Packet_src == interface_ipv4 || Packet_src == interface_ipv6) {
                utils->TCP_update_map(TCP_ConnectionMap_outgoing, std::make_pair(Packet_dest, "Dead") , port_pair);
            } else {
                if(checkifOutgoing(Packet_src))
                    utils->TCP_update_map(TCP_ConnectionMap_incoming, std::make_pair(Packet_src, "Dead"), port_pair);
            }
    }

}