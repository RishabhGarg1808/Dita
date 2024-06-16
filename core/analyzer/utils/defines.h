#ifndef NET_MON___DEFINES_H
#define NET_MON___DEFINES_H
#include <map>
#include <string>
#include <list>

extern std::map<std::string ,bool> SSH_ConnectionMap;
extern int DROPPED_SSH;
extern std::map<std::pair<std::string,bool> ,std::pair<int,int>> TCP_ConnectionMap_incoming;
extern std::map<std::pair<std::string,bool> ,std::pair<int,int>> TCP_ConnectionMap_outgoing;
extern std::map<std::string ,std::pair<int,int>> Untracked_TCP;
extern std::string interface_ipv4;
extern std::string interface_ipv6;
extern std::list<std::string> TCP_PendingConnectionWatchList;
extern std::multimap<int , std::string> SortedIPs;
#endif //NET_MON___DEFINES_H
