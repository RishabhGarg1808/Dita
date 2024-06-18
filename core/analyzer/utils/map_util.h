#ifndef NET_MON___MAP_UTIL_H
#define NET_MON___MAP_UTIL_H
#include <map>
#include <string>

using namespace std;

void SSH_update_map(map<string,string>&,const string&,const string&);
void IP_update_map(map<string, int>&,const string&);
void TCP_update_map(map<pair<string,string> ,pair<int,int>>&,const pair<string,string>&,const pair<int,int>&);
void UDP_update_map(map<string ,pair<int,int>>&,const string& ,const pair<int ,int>& );
bool is_existing(map<string, bool>&,const string&);
#endif //NET_MON___MAP_UTIL_H
