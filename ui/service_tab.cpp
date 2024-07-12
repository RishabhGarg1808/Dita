#include "../mainwindow.h"
#include "../ui_mainwindow.h"

#include <QString>
#include <QTreeView>
#include <QStandardItemModel>

void *MainWindow::initService(){
        serviceModel = new QStandardItemModel();
        ssh = new QStandardItem("SSH");
        tcp = new QStandardItem("TCP");
        udp = new QStandardItem("UDP");

        UdpIncoming = new QStandardItem(QString::fromStdString("Incoming"));
        UdpOutgoing = new QStandardItem(QString::fromStdString("Outgoing"));
        TcpIncoming = new QStandardItem(QString::fromStdString("Incoming"));
        TcpOutgoing = new QStandardItem(QString::fromStdString("Outgoing"));

        serviceModel->setHorizontalHeaderLabels(QStringList() << "SERVICE | STATUS");
        ssh->setFlags(ssh->flags() & ~Qt::ItemIsEditable);
        tcp->setFlags(tcp->flags() & ~Qt::ItemIsEditable);
        udp->setFlags(udp->flags() & ~Qt::ItemIsEditable);

        UdpIncoming->setFlags(UdpIncoming->flags() & ~Qt::ItemIsEditable);
        UdpOutgoing->setFlags(UdpIncoming->flags() & ~Qt::ItemIsEditable);

        serviceModel->appendRow(ssh);
        serviceModel->appendRow(udp);
        serviceModel->appendRow(tcp);

        udp->appendRow(UdpIncoming);
        udp->appendRow(UdpOutgoing);
        tcp->appendRow(TcpIncoming);
        tcp->appendRow(TcpOutgoing);
        // Set the model on the tree view
        ui->servTree->setModel(serviceModel);

    return nullptr;
}

void MainWindow::update_tcp(){
    static QMap<QString,QStandardItem*> parentItemsIncoming;
    static QMap<QString,QStandardItem*> parentItemsOutgoing;

    //lambda expression to check whether the child is present in the parent or not
    auto checkPresent =
            [&] ( QStandardItem* item ,const QString& value){
                for(int i =0 ;i<item->rowCount();++i){
                    if(item->child(i) == nullptr){
                        break;
                    }
                    if(item->child(i)->text() == value){
                        return true;
                    }
                }
                return false;
            };

    //incoming
    for(auto& it: analyzer->TCP_ConnectionMap_incoming){
        QString parentStr = QString::fromStdString(it.first.first + " : " + it.first.second);
        QString childStr = QString::fromStdString("Port : " +
                to_string(it.second.first) + " -> " + to_string(it.second.second));

        auto *parent = new QStandardItem(parentStr);
        auto *child = new QStandardItem(childStr);

        auto parentKey = QString::fromStdString(it.first.first);
        if(parentItemsIncoming.contains(parentKey)){
            if(parentItemsIncoming[parentKey]->text() != parentStr){
                parentItemsIncoming[parentKey]->setText(parentStr);
            }
            if(!checkPresent(parentItemsIncoming[parentKey], childStr)){
                parentItemsIncoming[parentKey]->appendRow(child);
            }
        }else{
            parent->appendRow(child);
            TcpIncoming->appendRow(parent);
            parentItemsIncoming[parentKey] = parent;
        }

    }

    //outgoing
    for(auto& it: analyzer->TCP_ConnectionMap_outgoing){
        QString parentStr = QString::fromStdString(it.first.first + " : " + it.first.second);
        QString childStr = QString::fromStdString("Port : " +
                                                  to_string(it.second.first) + " -> " + to_string(it.second.second));

        auto *parent = new QStandardItem(parentStr);
        auto *child = new QStandardItem(childStr);
        auto parentKey = QString::fromStdString(it.first.first);
        if(parentItemsOutgoing.contains(parentKey)){
            if(parentItemsOutgoing[parentKey]->text() != parentStr){
                parentItemsOutgoing[parentKey]->setText(parentStr);
            }
            if(!checkPresent(parentItemsOutgoing[parentKey], childStr)){
                parentItemsOutgoing[parentKey]->appendRow(child);
            }
        }else{
            parent->appendRow(child);
            TcpOutgoing->appendRow(parent);
            parentItemsOutgoing[parentKey] = parent;
        }

    }
}

void MainWindow::update_udp() {
    static QMap<QString,QStandardItem*> parentItemsIncoming;
    static QMap<QString,QStandardItem*> parentItemsOutgoing;

    //lambda expression to check whether the child is present in the parent or not
    auto checkPresent =
            [&] ( QStandardItem* item ,const QString& value){
        for(int i =0 ;i<item->rowCount();++i){
            if(item->child(i) == NULL){
                break;
            }
            if(item->child(i)->text() == value){
                return true;
            }
        }
        return false;
    };

    //Incoming
    for(auto & it : analyzer->UDP_ConnectionMap_incoming){
        QString parentStr = QString::fromStdString(it.first);
        QString childStr =  QString::fromStdString( "Port : " + to_string(it.second.first) +
                " -> " + to_string(it.second.second));

        auto *parent = new QStandardItem(parentStr);
        auto *child = new QStandardItem(childStr);

        if(parentItemsIncoming.contains(parentStr)){
           if(!checkPresent(parentItemsIncoming[parentStr], childStr)){
            parentItemsIncoming[parentStr]->appendRow(child);
           }
        }else{
            parent->appendRow(child);
            UdpIncoming->appendRow(parent);
            parentItemsIncoming[parentStr] = parent;
        }
    }

    //Outgoing
    for(auto & it : analyzer->UDP_ConnectionMap_outgoing){
        QString parentStr = QString::fromStdString(it.first);
        QString childStr = QString::fromStdString( "Port : " + to_string(it.second.first) +
                          " -> " + to_string(it.second.second));

        auto *parent = new QStandardItem(parentStr);
        auto *child = new QStandardItem(childStr);

        if(parentItemsOutgoing.contains(parentStr)){
            if(!checkPresent(parentItemsOutgoing[parentStr], childStr)){
                parentItemsOutgoing[parentStr]->appendRow(child);
            }
        }else{
            parent->appendRow(child);
            UdpOutgoing->appendRow(parent);
            parentItemsOutgoing[parentStr] = parent;
        }
    }


}
void MainWindow::update_ssh() {
    static QMap<QString, QStandardItem*> parentItems;
    for(auto & it :  analyzer->SSH_ConnectionMap){
        string connection = it.first + " :: " + it.second;
        auto *child = new QStandardItem(QString::fromStdString(connection));
        auto parentKey = QString::fromStdString(it.first);
        if(parentItems.contains(parentKey)) {
            parentItems[parentKey]->setText(QString::fromStdString(connection));
        }else{
            parentItems[parentKey] = child;
            ssh->appendRow(child);
        }
    }
}
void MainWindow::updateServiceStats(){
    update_tcp();
    update_udp();
    update_ssh();
}

