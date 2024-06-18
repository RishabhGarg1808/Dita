#ifndef DITA_UDP_ANALYZER_H
#define DITA_UDP_ANALYZER_H
#include "pcapplusplus/PcapLiveDeviceList.h"
#include <pcapplusplus/UdpLayer.h>
#include <string>

void udp_analyze(pcpp::Packet*);

#endif //DITA_UDP_ANALYZER_H
