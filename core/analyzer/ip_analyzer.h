
#ifndef NET_MON___IP_ANALYZER_H
#define NET_MON___IP_ANALYZER_H

#include <pcapplusplus/PcapLiveDeviceList.h>
#include <pcapplusplus/RawPacket.h>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/SystemUtils.h>
#include <pcapplusplus/IpAddress.h>
#include <pcapplusplus/PcapLiveDeviceList.h>
#include <queue>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>

void ipv4_analyze(pcpp::Packet* );
void ipv6_analyze(pcpp::Packet* );
void sortIPs();
#endif //NET_MON___IP_ANALYZER_H
