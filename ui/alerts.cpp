#include "mainwindow.h"
#include "../ui_mainwindow.h"
#include <QListWidgetItem>
#include "chrono"
#include "sstream"
#include "iomanip"

using namespace std;

void MainWindow::updateAlertsTabPing(const string& pingSrc){
    static QMap<string,QDateTime> pingMap;

    auto chronTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    const tm localTM = *localtime(&chronTime);
    stringstream timestamp ;
    timestamp << put_time(&localTM," %H:%M:%S on %b %d ");

    if(pingMap.contains(pingSrc)){
        // Update tooltip for existing entry
        int idx = 0;
        for(int i = 0; i < ui->Alerts_list->count(); i++){
            QListWidgetItem *item = ui->Alerts_list->item(i);
            if(item && item->text().contains(QString::fromStdString(pingSrc))){
                item->setToolTip(QString::fromStdString(timestamp.str()));
                break;
            }
        }
    }else{
        auto *item = new QListWidgetItem;
        pingMap[pingSrc] = QDateTime::currentDateTime();
        item->setText( "ICMP "+ QString::fromStdString(pingSrc));
        item->setToolTip(QString::fromStdString(timestamp.str()));
        ui->Alerts_list->addItem(item);
    }

}

void MainWindow::updateAlertsTabSSH(const string& ip,const string &status) {
    static QMap<string,QDateTime> sshMap;
    QString sshStat = QString::fromStdString("SSH " + ip + " :: " + status);

    auto chronTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    const tm localTM = *localtime(&chronTime);
    stringstream timestamp ;
    timestamp << put_time(&localTM," %H:%M:%S on %b %d ");

    if(sshMap.contains(ip)){
        // Update text for existing entry
        for(int i = 0; i < ui->Alerts_list->count(); i++){
            QListWidgetItem *item = ui->Alerts_list->item(i);
            if(item && item->text().startsWith("SSH " + QString::fromStdString(ip))){
                item->setText(sshStat);
                item->setToolTip(QString::fromStdString(timestamp.str()));
                break;
            }
        }
    }else{
        auto *item = new QListWidgetItem;
        item->setText(sshStat);
        item->setToolTip(QString::fromStdString(timestamp.str()));
        sshMap[ip] = QDateTime::currentDateTime();
        ui->Alerts_list->addItem(item);
    }
}