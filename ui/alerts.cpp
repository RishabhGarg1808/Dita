#include "mainwindow.h"
#include "../ui_mainwindow.h"
#include <QListWidgetItem>
#include "chrono"
#include "sstream"
#include "iomanip"

using namespace std;

void MainWindow::updateAlertsTabPing(const string& pingSrc){
    static QMap<string,QListWidgetItem*> pingMap;

    auto chronTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    const tm localTM = *localtime(&chronTime);
    stringstream timestamp ;
    timestamp << put_time(&localTM," %H:%M:%S on %b %d ");

    if(pingMap.contains(pingSrc)){
        pingMap[pingSrc]->setToolTip(QString::fromStdString(timestamp.str()));
    }else{
        auto *item = new QListWidgetItem;
        pingMap[pingSrc] = item;
        item->setText( "ICMP "+ QString::fromStdString(pingSrc));//+ " :: Pinged Host ");
        item->setToolTip(QString::fromStdString(timestamp.str()));
        ui->Alerts_list->addItem(item);
    }

}

void MainWindow::updateAlertsTabSSH(const string& ip,const string &status) {
    static QMap<string,QListWidgetItem*> sshMap;
    QString sshStat = QString::fromStdString("SSH " + ip + " :: " + status);

    auto chronTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    const tm localTM = *localtime(&chronTime);
    stringstream timestamp ;
    timestamp << put_time(&localTM," %H:%M:%S on %b %d ");

    if(sshMap.contains(ip)){
        sshMap[ip]->setText(sshStat);
        sshMap[ip]->setToolTip(QString::fromStdString(timestamp.str()));
    }else{
        auto *item = new QListWidgetItem;
        item->setText(sshStat);
        item->setToolTip(QString::fromStdString(timestamp.str()));
        sshMap[ip] = item;
        ui->Alerts_list->addItem(item);
    }
}