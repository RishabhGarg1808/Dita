#include "tcp_analyzer.h"
#include "utils/defines.h"
#include "utils/map_util.h"
#include "utils/ip_utils.h"
#include <string>
#include <list>
#include <algorithm>

/* false : indicates the connection is dead(or previously requested)
 *  true :  indicates the connection is alive
 *  Both are used in the TCP map to determine whether the connection is alive or not
 */

std::map<std::pair<std::string,bool> ,std::pair<int,int>> TCP_ConnectionMap_incoming;
std::map<std::pair<std::string,bool> ,std::pair<int,int>> TCP_ConnectionMap_outgoing;

std::list<std::string> TCP_PendingConnectionWatchList;

/* If a packet has SYN flag is set then we observe the packet for the connection
 * if the no ACK flag is set then the connection was dropped/terminated
 * if the RST/FIN flag is set then the connection was dropped/terminated
 * if the ACK flag is set then the connection was established
 * */

bool check_list(const std::string& ip_packet){
    if(TCP_PendingConnectionWatchList.empty()){
        return false;
    }else{
        auto it = std::find(TCP_PendingConnectionWatchList.begin(),
                            TCP_PendingConnectionWatchList.end(),
                            ip_packet);
        if(it != TCP_PendingConnectionWatchList.end()) {
            return true;
        }else{
            return false;

        }
    }
}
bool checkifOutgoing(const std::string& IP){

    auto it = TCP_ConnectionMap_outgoing.find(std::make_pair(IP, true));
    auto it_another = TCP_ConnectionMap_outgoing.find(std::make_pair(IP,false));

    if(it != TCP_ConnectionMap_outgoing.end() || it_another != TCP_ConnectionMap_outgoing.end()){
        return true;
    }else{
        return false;
    }

}

void ssh_close(const std::string& src){
    auto it = SSH_ConnectionMap.find(src);
    if(it != SSH_ConnectionMap.end()) {
        SSH_update_map(SSH_ConnectionMap, src, false);
    }
}

void tcp_analyze(pcpp::Packet *Packet) {
    auto *TcpLayer = Packet->getLayerOfType<pcpp::TcpLayer>();
    auto Packet_src = sourceIPExtractor(Packet);
    auto Packet_dest = destIPExtractor(Packet);
    auto port_pair = std::make_pair(TcpLayer->getSrcPort(),TcpLayer->getDstPort());

    if (TcpLayer->getTcpHeader()->synFlag == 1 && TcpLayer->getTcpHeader()->ackFlag == 0) {
        TCP_PendingConnectionWatchList.push_back(Packet_dest);
    }
    /* check if the packet originated for the Local_machine
    * if the packet originated from the local_machine classify the connection as outgoing
    * else classify the connection as incoming
    */
    if (TcpLayer->getTcpHeader()->synFlag == 1 && TcpLayer->getTcpHeader()->ackFlag == 1) {
        if (check_list(Packet_dest)) {
            TCP_PendingConnectionWatchList.remove(Packet_dest);
        }
        if (Packet_src == interface_ipv4 || Packet_src == interface_ipv6) {
            TCP_update_map(TCP_ConnectionMap_outgoing, std::make_pair(Packet_dest, true) ,port_pair);
        } else {
            if(checkifOutgoing(Packet_dest)){
            TCP_update_map(TCP_ConnectionMap_incoming,std::make_pair(Packet_src,true), port_pair);
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
                TCP_update_map(TCP_ConnectionMap_outgoing, std::make_pair(Packet_dest, false) , port_pair);
            } else {
                if(checkifOutgoing(Packet_src))
                TCP_update_map(TCP_ConnectionMap_incoming, std::make_pair(Packet_dest, false), port_pair);
            }

    }

}