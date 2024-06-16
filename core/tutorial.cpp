#include <iostream>
#include <cstdlib>
#include "pcap.h"
#include "pcapplusplus/PcapLiveDeviceList.h"
#include "pcapplusplus/SystemUtils.h"
#include "analyzer/protocol_analyzer.h"
#include "analyzer/utils/defines.h"

std::string interface_ipv4;
std::string interface_ipv6;

void exit(){
    std::cout <<  " Exiting ";
    for(int i=0;i<=50;i++){
        std::cout<< ".";
    }
    std::cout << std::endl;
    exit(1);
}

struct PacketStats
{
    int ethPacketCount = 0;
    int ipv4PacketCount = 0;
    int ipv6PacketCount = 0;
    int tcpPacketCount = 0;
    int udpPacketCount = 0;
    int dnsPacketCount = 0;
    int httpPacketCount = 0;
    int sslPacketCount = 0;
    int sshPacketCount = 0;

    /**
    * Clear all stats
    */
    void clear() { ethPacketCount = 0; ipv4PacketCount = 0; ipv6PacketCount = 0; tcpPacketCount = 0; udpPacketCount = 0; tcpPacketCount = 0; dnsPacketCount = 0; httpPacketCount = 0; sslPacketCount = 0; }

    /**
    * C'tor
    */
    PacketStats() { clear(); }

    /**
    * Collect stats from a packet
    */
    void consumePacket(pcpp::Packet& packet)
    {
        if (packet.isPacketOfType(pcpp::Ethernet))
            ethPacketCount++;
        if (packet.isPacketOfType(pcpp::IPv4))
            ipv4PacketCount++;
        if (packet.isPacketOfType(pcpp::IPv6))
            ipv6PacketCount++;
        if (packet.isPacketOfType(pcpp::TCP))
            tcpPacketCount++;
        if (packet.isPacketOfType(pcpp::UDP))
            udpPacketCount++;
        if (packet.isPacketOfType(pcpp::DNS))
            dnsPacketCount++;
        if (packet.isPacketOfType(pcpp::HTTP))
            httpPacketCount++;
        if (packet.isPacketOfType(pcpp::SSL))
            sslPacketCount++;
        if(packet.isPacketOfType(pcpp::SSH))
            sshPacketCount++;
    }

    /**
    * Print stats to console
    */
    void printToConsole()
    {
        std::cout
                << "Ethernet packet count: " << ethPacketCount << std::endl
                << "IPv4 packet count:     " << ipv4PacketCount << std::endl
                << "IPv6 packet count:     " << ipv6PacketCount << std::endl
                << "TCP packet count:      " << tcpPacketCount << std::endl
                << "UDP packet count:      " << udpPacketCount << std::endl
                << "DNS packet count:      " << dnsPacketCount << std::endl
                << "HTTP packet count:     " << httpPacketCount << std::endl
                << "SSL packet count:      " << sslPacketCount << std::endl
                << "SSH packet count:      " << sshPacketCount << std::endl;
    }
};

static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
    // extract the stats object form the cookie
    PacketStats* stats = (PacketStats*)cookie;

    // parsed the raw packet
    pcpp::Packet parsedPacket(packet);
    packetSwitcher(&parsedPacket);
    // collect stats from packet
    stats->consumePacket(parsedPacket);
}

int main() {
    int EXIT_SIGNAL=-1;
    const char *dev_name;
    char error[PCAP_BUF_SIZE];
    pcap_if_t *alldevsp,*temp;
    int ret = pcap_findalldevs(&alldevsp,error);
    int cntr=1;
    if(ret>0){
        std::cout << "Error Finding Device : " << ret << std::endl;
        exit(1);
    }else{
        std::cout << " DEVICE LIST: \n";
        for(temp = alldevsp;temp;temp=temp->next){
            std::cout << "\t"<< cntr << ". " << temp->name << std::endl;
            cntr++;
        }
    }

    int dev_sel = -1;
    std::cout << "Enter the device to select: ";
    std::cin >> dev_sel;

    temp = alldevsp;
    if(dev_sel >= 1 && dev_sel<= cntr){
        for(int i=1;i<=dev_sel;i++){
            if(dev_sel == i){
                dev_name = temp->name;
                break;
            }
            temp = temp->next;
        }
        std::cout << dev_name << " Selected" <<std::endl;
    }else{
        std::cout << "Invalid Selection : " << dev_sel ;
        exit();
    }
    pcpp::PcapLiveDevice *dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(dev_name);
    if (dev == NULL)
    {
        std::cerr << "Cannot find interface "<< dev_name << std::endl;
        return 1;
    }
    // get the interface IP address
    interface_ipv4 = dev->getIPv4Address().toString();
    interface_ipv6 = dev->getIPv6Address().toString();

    // before capturing packets let's print some info about this interface
    std::cout
            << "Interface info:" << std::endl
            << "   Interface Address v4:        " << dev->getIPv4Address() << std::endl
            << "   Interface Address v6:        " << dev->getIPv6Address()<< std::endl
            << "   Interface name:        " << dev->getName() << std::endl // get interface name
            << "   Interface description: " << dev->getDesc() << std::endl // get interface description
            << "   MAC address:           " << dev->getMacAddress() << std::endl // get interface MAC address
            << "   Default gateway:       " << dev->getDefaultGateway() << std::endl // get default gateway
            << "   Interface MTU:         " << dev->getMtu() << std::endl; // get interface MTU

    if (dev->getDnsServers().size() > 0)
        std::cout << "   DNS server:            " << dev->getDnsServers().at(0) << std::endl;

    // open the device before start capturing/sending packets
    if (!dev->open())
    {
        std::cerr << "Cannot open device" << std::endl;
        return 1;
    }

    PacketStats stats;


    std::cout << std::endl << "Starting async capture..." << std::endl;
    dev->startCapture(onPacketArrives, &stats);
    //pcpp::multiPlatformSleep(10);

    std::cout << "Capture is running; Press 0 to exit! :: ";
    std::cin >> EXIT_SIGNAL;
    EXIT:
    if(EXIT_SIGNAL >=1){
        std::cout << "Invalid Option;Retry!";
        std::cin >> EXIT_SIGNAL;
        goto EXIT;
    }

    dev->stopCapture();
    std::cout << "Results:" << std::endl;
    stats.printToConsole();

    std::cout << "===================SSH Connection Map ================" << std::endl;
    std::map<std::string,bool>::iterator it;
    for(it = SSH_ConnectionMap.begin();it != SSH_ConnectionMap.end();++it){
        std::cout << "IP : " << it->first << " " << "STATUS :" << it->second << std::endl;
    }

    std::cout<< "===================TCP Connection Map ================" << std::endl;
    std::map<std::pair<std::string,bool>,std::pair<int,int>>::iterator it_tcp;
    std::cout << "Incoming From: " << std::endl;
    for(it_tcp = TCP_ConnectionMap_incoming.begin();it_tcp != TCP_ConnectionMap_incoming.end();++it_tcp){
        std::cout << "IP : " << it_tcp->first.first << " -> " << it_tcp->first.second
        << " Port :" << it_tcp->second.first << " : " << it_tcp->second.second<< std::endl;

    }
    std::cout << "Outgoing To: " << std::endl;
    for(it_tcp = TCP_ConnectionMap_outgoing.begin();it_tcp != TCP_ConnectionMap_outgoing.end();++it_tcp){
        std::cout << "IP : " << it_tcp->first.first << " -> " << it_tcp->first.second
                << " Port :" << it_tcp->second.first << " : " << it_tcp->second.second<< std::endl;

    }
    sortIPs();
    std::cout<< "===================IP  Map ================" << std::endl;
    for(auto& pair : SortedIPs){
        std::cout << " IP : " << pair.second << " Count : " << pair.first << std::endl;
    }

    return 0;

}
