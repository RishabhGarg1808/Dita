#ifndef NET_MON___IP_UTILS_H
#define NET_MON___IP_UTILS_H
#include <string>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/IpAddress.h>
#include <pcapplusplus/IPv4Layer.h>

using namespace std;

string sourceIPExtractor(pcpp::Packet*);
string destIPExtractor(pcpp::Packet* );

#endif //NET_MON___IP_UTILS_H
