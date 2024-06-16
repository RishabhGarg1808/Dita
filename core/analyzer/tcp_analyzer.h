#ifndef NET_MON___TCP_ANALYZER_H
#define NET_MON___TCP_ANALYZER_H
#include "pcapplusplus/PcapLiveDeviceList.h"
#include <pcapplusplus/TcpLayer.h>
#include <string>

void tcp_analyze(pcpp::Packet*);
#endif //NET_MON___TCP_ANALYZER_H
