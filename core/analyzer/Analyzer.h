#ifndef DITA_ANALYZER_H
#define DITA_ANALYZER_H

#include "pcapplusplus/PcapLiveDeviceList.h"
#include "pcapplusplus/Packet.h"
#include "pcapplusplus/SystemUtils.h"
#include "QObject"
#include <QReadWriteLock>

#include "utils/Utils.h"

class Analyzer : public QObject {
    Q_OBJECT
public:
    signals:
        void emit_icmp(const string&);
        void emit_ssh(const string&,const string&);
private:
    string interface_ipv4;
    string interface_ipv6;
    std::list<std::string> TCP_PendingConnectionWatchList;
    const int THRESHOLD=2;
    bool checkInMap(const string &val);
    map<string,int> ping_map;
    Utils *utils = new Utils;
public:
    map<pair<string,string> ,pair<int,int>> TCP_ConnectionMap_incoming;
    map<pair<string,string> ,pair<int,int>> TCP_ConnectionMap_outgoing;
    map<string ,pair<int,int>> UDP_ConnectionMap_incoming;
    map<string ,pair<int,int>> UDP_ConnectionMap_outgoing;
    map<string ,string> SSH_ConnectionMap;
    mutable QReadWriteLock mapLock;
public:
    string pingSrc;
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
    void cleanupDeadConnections();
    static string getProtocolAsString(pcpp::ProtocolType);

    const map<pair<string, string>, pair<int, int>> &getTcpConnectionMapIncoming() const;

    const map<pair<string, string>, pair<int, int>> &getTcpConnectionMapOutgoing() const;

    const map<string, pair<int, int>> &getUdpConnectionMapIncoming() const;

    const map<string, pair<int, int>> &getUdpConnectionMapOutgoing() const;

    // Methods that return copies of maps (thread-safe)
    map<pair<string,string>, pair<int,int>> getTcpIncomingCopy() const;
    map<pair<string,string>, pair<int,int>> getTcpOutgoingCopy() const;
    map<string, pair<int,int>> getUdpIncomingCopy() const;
    map<string, pair<int,int>> getUdpOutgoingCopy() const;
    map<string, string> getSshCopy() const;
};

#endif //DITA_ANALYZER_H
