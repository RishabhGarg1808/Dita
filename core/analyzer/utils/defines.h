#ifndef NET_MON___DEFINES_H
#define NET_MON___DEFINES_H
#include <map>
#include <string>
#include <list>

using namespace std;

extern map<string ,string> SSH_ConnectionMap;
extern int DROPPED_SSH;
extern map<pair<string,string> ,pair<int,int>> TCP_ConnectionMap_incoming;
extern map<pair<string,string> ,pair<int,int>> TCP_ConnectionMap_outgoing;
extern map<string ,pair<int,int>> UDP_ConnectionMap_incoming;
extern map<string,pair<int,int>> UDP_ConnectionMap_outgoing;
extern map<string ,pair<int,int>> Untracked_TCP;
extern string interface_ipv4;
extern string interface_ipv6;
extern list<string> TCP_PendingConnectionWatchList;
extern multimap<int ,string> SortedIPs;
#endif //NET_MON___DEFINES_H
