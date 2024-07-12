#ifndef DITA_UTILS_H
#define DITA_UTILS_H
#include "map"
#include "string"
#include "pcapplusplus/Packet.h"

using namespace std;

class Utils {

    map<string,string>::iterator SSH_it;
    map<pair<string,string> ,pair<int,int>>::iterator TCP_it;
    map<string ,pair<int,int>>::iterator UDP_it;

public:
//    template<typename type,typename key,typename value>
//    void update_map (const &type maptype,const &key mapkey,const &value mapvalue);
    void SSH_update_map(map<string,string>&,const string&,const string&);
    void IP_update_map(map<string, int>&,const string&);
    void TCP_update_map(map<pair<string,string> ,pair<int,int>>&,const pair<string,string>&,const pair<int,int>&);
    void UDP_update_map(map<string ,pair<int,int>>&,const string& ,const pair<int ,int>& );

    string sourceIPExtractor(pcpp::Packet*);
    string destIPExtractor(pcpp::Packet* );
};


#endif //DITA_UTILS_H
