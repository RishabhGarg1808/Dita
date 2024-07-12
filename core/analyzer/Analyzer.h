#ifndef DITA_ANALYZER_H
#define DITA_ANALYZER_H

#include "pcapplusplus/PcapLiveDeviceList.h"
#include "pcapplusplus/Packet.h"
#include "pcapplusplus/SystemUtils.h"
#include "QObject"

#include "utils/Utils.h"

class Analyzer : public QObject {
    Q_OBJECT
public:
    signals:
        void emit_icmp();
private:
    string interface_ipv4;
    string interface_ipv6;
    std::list<std::string> TCP_PendingConnectionWatchList;
    const int THRESHOLD=1;
    bool checkInMap(const string &val);
    map<string,int> ping_map;
    Utils *utils = new Utils;
    int DROPPED_SSH=0;
public:
    map<pair<string,string> ,pair<int,int>> TCP_ConnectionMap_incoming;
    map<pair<string,string> ,pair<int,int>> TCP_ConnectionMap_outgoing;
    map<string ,pair<int,int>> UDP_ConnectionMap_incoming;
    map<string ,pair<int,int>> UDP_ConnectionMap_outgoing;
    map<string ,string> SSH_ConnectionMap;
public:
    //main function which branches to other sub-functions
    void packetSwitcher(pcpp::Packet*);

    void setInterfaceIpv4(const string &interfaceIpv4);

    void setInterfaceIpv6(const string &interfaceIpv6);

    void icmp_analyze(pcpp::Packet*);
    void sshAnalyze(pcpp::Packet*);
    void tcp_analyze(pcpp::Packet*);
    void udp_analyze(pcpp::Packet*);

    //tcp fucntions
    void ssh_close(const string& );

    void setSshConnectionMap(const map<std::string, std::string> &sshConnectionMap);

    bool checkifOutgoing(const std::string&);
    bool check_list(const std::string& );

    //other util functions
    static string getProtocolAsString(pcpp::ProtocolType);

    const map<pair<string, string>, pair<int, int>> &getTcpConnectionMapIncoming() const;

    const map<pair<string, string>, pair<int, int>> &getTcpConnectionMapOutgoing() const;

    const map<string, pair<int, int>> &getUdpConnectionMapIncoming() const;

    const map<string, pair<int, int>> &getUdpConnectionMapOutgoing() const;
};

#endif //DITA_ANALYZER_H
