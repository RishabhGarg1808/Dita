#include "../mainwindow.h"
#include "../ui_mainwindow.h"
#include "../core/analyzer/utils/defines.h"

#include <QString>
#include <QTreeView>
#include <QStandardItemModel>

void *MainWindow::initService(){
        serviceModel = new QStandardItemModel();
        ssh = new QStandardItem("SSH");
        tcp = new QStandardItem("TCP");
        udp = new QStandardItem("UDP");
        serviceModel->setHorizontalHeaderLabels(QStringList() << "SERVICE | STATUS");
        ssh->setFlags(ssh->flags() & ~Qt::ItemIsEditable);
        tcp->setFlags(tcp->flags() & ~Qt::ItemIsEditable);
        udp->setFlags(udp->flags() & ~Qt::ItemIsEditable);

        serviceModel->appendRow(ssh);
        serviceModel->appendRow(udp);
        serviceModel->appendRow(tcp);

        // Set the model on the tree view
        ui->servTree->setModel(serviceModel);

    return nullptr;
}

void MainWindow::updateServiceStats(){
    // QMap to store a reference to each item in the model
    static QHash<QStandardItem*,QStandardItem*> childItems;
    static QMap<QString, QStandardItem*> parentItems;

    std::map<std::pair<std::string,bool>,std::pair<int,int>>::iterator it_tcp;
    for(it_tcp = TCP_ConnectionMap_incoming.begin();it_tcp != TCP_ConnectionMap_incoming.end();++it_tcp){
        std::string status  = it_tcp->first.first + " -> " + std::to_string(it_tcp->first.second);
        std::string port = " Port :" + std::to_string(it_tcp->second.first) + " -> " + std::to_string(it_tcp->second.second);

        QString parentKey = QString::fromStdString(status + port);
        auto *parent = new QStandardItem(QString::fromStdString(status));
        auto *child = new QStandardItem(QString::fromStdString(port));

        childItems[parent] = child;
        if(parentItems.contains(parentKey)){
            if(childItems.contains(parent)){
                childItems[parent]->setText(QString::fromStdString(port));
            }
        }else{
            parent->appendRow(childItems[parent]);
            tcp->appendRow(parent);
            parentItems[parentKey] = parent;
            childItems[parent] = child;
        }
    }
}

