#ifndef NET_MON___PROTOCOL_ANALYZER_H
#define NET_MON___PROTOCOL_ANALYZER_H
#include <pcapplusplus/PcapLiveDeviceList.h>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/SystemUtils.h>
#include "ip_analyzer.h"
#include "ssh_anlayzer.h"
#include "tcp_analyzer.h"
#include "udp_analyzer.h"

std::string getProtocolAsString(pcpp::ProtocolType);
void packetSwitcher(pcpp::Packet*);
#endif //NET_MON___PROTOCOL_ANALYZER_H
