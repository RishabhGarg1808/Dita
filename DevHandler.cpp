#include <iostream>
#include <QMessageBox>
#include "DevHandler.h"

DevHandler::DevHandler(Graph *graph) {
    this->graph = graph;
    this->dev = nullptr;
    int ret = pcap_findalldevs(&alldevsp,errbuf);
    if(ret>0){
        QMessageBox messagebox(nullptr);
        messagebox.setWindowTitle("Error!");
        messagebox.setInformativeText("Error Finding Device");
        messagebox.exec();
        exit(1);
    }else{
        for(temp = alldevsp;temp;temp=temp->next){
            dev_list.push_back(temp->name);
            cntr++;
        }
        // M1: Free the device list allocated by pcap_findalldevs
        pcap_freealldevs(alldevsp);
        alldevsp = nullptr;
    }
}

DevHandler::~DevHandler() {
    // M1: Free device list if not already freed
    if (alldevsp != nullptr) {
        pcap_freealldevs(alldevsp);
        alldevsp = nullptr;
    }
}

void DevHandler::stop_capture() {
    if (dev && dev->isOpened()) {
        dev->stopCapture();
        std::cout << "Capture stopped" <<std::endl;
    }
}

void DevHandler::start_capture() {
    if (dev == NULL) {
        dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(dev_name);
    }
    if (dev == NULL)
    {
        QMessageBox messagebox(nullptr);
        messagebox.setWindowTitle("Error!");
        messagebox.setInformativeText("Can not find interface");
        messagebox.exec();
        return;

    }
    if (!dev->open()) {
        // Promiscuous mode (the default) needs CAP_NET_ADMIN/root. If the
        // process only has CAP_NET_RAW (e.g. 'pcap' group membership), retry
        // in non-promiscuous mode.
        pcpp::PcapLiveDevice::DeviceConfiguration cfg(pcpp::PcapLiveDevice::Normal);
        if (!dev->open(cfg)) {
            QMessageBox messagebox(nullptr);
            messagebox.setWindowTitle("Error!");
            messagebox.setInformativeText("Can not open device "
                "(insufficient privileges for packet capture?)");
            messagebox.exec();
        }
    }

    //bind the onPacketArrives function to a static block and pass it to the startCapture
    function<void(pcpp::RawPacket*, pcpp::PcapLiveDevice*, void*)> binded = bind(&Graph::onPacketArrives,graph,placeholders::_1,placeholders::_2,placeholders::_3);
    dev->startCapture(binded,&graph);
}

void DevHandler::select_dev(int dev_sel){
    if(dev_list.empty()) {
        std::cerr << "Device list is empty." << std::endl;
        exit(1);
    }
    if(dev_sel >= 1 && dev_sel<= cntr){
        dev_name = dev_list[dev_sel - 1];
        // Resolve the PcapPlusPlus device now so the caller can query it
        // (e.g. interface addresses) before the capture is started.
        dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(dev_name);
        std::cout << dev_name << " Selected" <<std::endl;
    }else{
        std::cerr << "Value null or range greater :  " << dev_sel << std::endl;
    }
}

std::vector<std::string> DevHandler::get_dev_list() {
    return dev_list;
}

pcpp::PcapLiveDevice *DevHandler::getDev() const {
    return dev;
}

DevHandler::DevHandler() {
    //Do nothing
    dev = nullptr;
}

