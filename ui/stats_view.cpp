#include <QComboBox>
#include <QTextBrowser>

#include "mainwindow.h"
#include "../ui_mainwindow.h"

void *MainWindow::initStats(){
    //Tamper the below html builder at your own risk
    QString htmlTemplate = R"(
          <div style='text-align: center;'><h3>INTERFACE STATS</h3></div>
            <table style='text-align: center; margin-top: 10px;'>
                <tr>
                <td>Name</td>
                <td>%1</td>
                </tr>
                <tr>
                <td>IPv4</td>
                <td>%2</td>
                </tr>
                <tr>
                <td>IPv6</td>
                <td>%3</td>
                </tr>
                <tr>
                <td>MAC</td>
                <td>%4</td>
                </tr>
                <tr>
                <td><pre>Gateway </pre></td>
                <td>%5</td>
                </tr>
                <tr>
                <td>DNS</td>
                <td>%6</td>
                </tr>
                <tr>
                <td>MTU</td>
                <td>%7</td>
                </tr>
    )";

    QString htmlContent = htmlTemplate.arg(
            QString::fromStdString(DevHandle->getDev()->getName()),
            QString::fromStdString(DevHandle->getDev()->getIPv4Address().toString()),
            QString::fromStdString(DevHandle->getDev()->getIPv6Address().toString()),
            QString::fromStdString(DevHandle->getDev()->getMacAddress().toString()),
            QString::fromStdString(DevHandle->getDev()->getDefaultGateway().toString()),
            QString::fromStdString(DevHandle->getDev()->getDnsServers().at(0).toString()),
            QString::number(DevHandle->getDev()->getMtu())
            );
    ui->devStats->setHtml(htmlContent);
    return nullptr;
}

void MainWindow::initDevList(){
    std::vector<std::string> dev_list = DevHandle->get_dev_list();
    for(auto &dev :dev_list){
        ui->comboBox->addItem(QString::fromStdString(dev));
    }
}
void MainWindow::getselectedDev(){
    return DevHandle->select_dev(ui->comboBox->currentIndex()+1);
}

void MainWindow::reinit() {
    std::cout << "Re-initializing the device" << std::endl;
    DevHandle->stop_capture();
    DevHandle->select_dev(ui->comboBox->currentIndex()+1);
    DevHandle->start_capture();
    analyzer->setInterfaceIpv4(DevHandle->getDev()->getIPv4Address().toString());
    analyzer->setInterfaceIpv6(DevHandle->getDev()->getIPv6Address().toString());
    initStats();
}


