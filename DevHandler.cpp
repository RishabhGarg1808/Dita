#include <iostream>
#include <QMessageBox>
#include "DevHandler.h"

DevHandler::DevHandler() {
    int ret = pcap_findalldevs(&alldevsp,errbuf);

    if(ret>0){
        auto messagebox = new QMessageBox();
        messagebox->setWindowTitle("Error!");
        messagebox->setInformativeText("Error Finding Device");
        messagebox->exec();
        exit(1);
    }else{
        for(temp = alldevsp;temp;temp=temp->next){
            dev_list.push_back(temp->name);
            cntr++;
        }
    }
}

void DevHandler::stop_capture() {
    dev->stopCapture();
    std::cout << "Capture stopped" <<std::endl;
}

void DevHandler::start_capture() {
    dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(dev_name);
    if (dev == NULL)
    {
        auto messagebox = new QMessageBox();
        messagebox->setWindowTitle("Error!");
        messagebox->setInformativeText("Can not find interface");
        messagebox->exec();
        return;

    }
    if (!dev->open()) {
        auto messagebox = new QMessageBox();
        messagebox->setWindowTitle("Error!");
        messagebox->setInformativeText("Can not open device");
        messagebox->exec();
    }

    dev->startCapture(Graph::onPacketArrives, &graph);
}

void DevHandler::select_dev(int dev_sel){;
    temp = alldevsp;
    if(dev_sel >= 1 && dev_sel<= cntr){
        for(int i=1;i<=dev_sel;i++){
            if(dev_sel == i){
                dev_name = temp->name;
                break;
            }
            temp = temp->next;
        }
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

