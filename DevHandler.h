#ifndef DITA_DEVHANDLER_H
#define DITA_DEVHANDLER_H

#include <vector>
#include "Graph/Graph.h"
#include "pcap.h"
#include "pcapplusplus/PcapLiveDeviceList.h"

class DevHandler {
    const char* dev_name;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevsp,*temp;
    std::vector<std::string> dev_list;
    pcpp::PcapLiveDevice* dev;
    Graph *graph;
    int cntr=1;
public:
    DevHandler();
    DevHandler(Graph *);
    std::vector<std::string> get_dev_list();
    void select_dev(int);
    void start_capture();
    void stop_capture();
    pcpp::PcapLiveDevice *getDev() const;
};


#endif //DITA_DEVHANDLER_H
