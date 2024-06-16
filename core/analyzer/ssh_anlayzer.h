#ifndef NET_MON___SSH_ANLAYZER_H
#define NET_MON___SSH_ANLAYZER_H
#include <pcapplusplus/PcapLiveDeviceList.h>
#include <pcapplusplus/RawPacket.h>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/IpAddress.h>
#include "pcapplusplus/IPv4Layer.h"
#include "pcapplusplus/IPv6Layer.h"

void sshAnalyze(pcpp::Packet*);
std::string sourceIPExtractor(pcpp::Packet*);
#endif //NET_MON___SSH_ANLAYZER_H
