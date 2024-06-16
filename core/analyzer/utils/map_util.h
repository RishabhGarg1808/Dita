#ifndef NET_MON___MAP_UTIL_H
#define NET_MON___MAP_UTIL_H
#include <map>
#include <string>

void SSH_update_map(std::map<std::string, bool>&,const std::string&,bool);
void IP_update_map(std::map<std::string, int>&,const std::string&);
void TCP_update_map(std::map<std::pair<std::string,bool> ,std::pair<int,int>>&,const std::pair<std::string,bool>&,const std::pair<int,int>&);
bool is_existing(std::map<std::string, bool>&,const std::string&);
#endif //NET_MON___MAP_UTIL_H
