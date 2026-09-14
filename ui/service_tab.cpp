#include "mainwindow.h"
#include "../ui_mainwindow.h"

#include <QString>
#include <QTreeView>
#include <QStandardItemModel>

void MainWindow::initService(){
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

}


void MainWindow::update_tcp(){
    // NOTE: rows are rebuilt from scratch every tick (see updateServiceStats),
    // so items must never be cached across ticks - that would be a
    // use-after-free once the previous tick's rows were removed.

    //incoming - use thread-safe copy
    auto tcp_incoming = analyzer->getTcpIncomingCopy();
    for(auto& it: tcp_incoming){
        QString parentStr = QString::fromStdString(it.first.first + " : " + it.first.second);
        QString childStr = QString::fromStdString("Port : " +
                to_string(it.second.first) + " -> " + to_string(it.second.second));

        auto *parent = new QStandardItem(parentStr);
        TcpIncoming->appendRow(parent);
        parent->appendRow(new QStandardItem(childStr));
    }

    //outgoing - use thread-safe copy
    auto tcp_outgoing = analyzer->getTcpOutgoingCopy();
    for(auto& it: tcp_outgoing){
        QString parentStr = QString::fromStdString(it.first.first + " : " + it.first.second);
        QString childStr = QString::fromStdString("Port : " +
                                                  to_string(it.second.first) + " -> " + to_string(it.second.second));

        auto *parent = new QStandardItem(parentStr);
        TcpOutgoing->appendRow(parent);
        parent->appendRow(new QStandardItem(childStr));
    }
}

void MainWindow::update_udp() {
    // Rows are rebuilt from scratch every tick; never cache items across ticks.

    //Incoming - use thread-safe copy
    auto udp_incoming = analyzer->getUdpIncomingCopy();
    for(auto & it : udp_incoming){
        QString parentStr = QString::fromStdString(it.first);
        QString childStr =  QString::fromStdString( "Port : " + to_string(it.second.first) +
                " -> " + to_string(it.second.second));

        auto *parent = new QStandardItem(parentStr);
        UdpIncoming->appendRow(parent);
        parent->appendRow(new QStandardItem(childStr));
    }

    //Outgoing - use thread-safe copy
    auto udp_outgoing = analyzer->getUdpOutgoingCopy();
    for(auto & it : udp_outgoing){
        QString parentStr = QString::fromStdString(it.first);
        QString childStr = QString::fromStdString( "Port : " + to_string(it.second.first) +
                " -> " + to_string(it.second.second));

        auto *parent = new QStandardItem(parentStr);
        UdpOutgoing->appendRow(parent);
        parent->appendRow(new QStandardItem(childStr));
    }
}

void MainWindow::update_ssh() {
    // Rows are rebuilt from scratch every tick; never cache items across ticks.
    auto ssh_map = analyzer->getSshCopy();
    for(auto & it : ssh_map){
        string connection = it.first + " :: " + it.second;
        auto *child = new QStandardItem(QString::fromStdString(connection));
        ssh->appendRow(child);
    }
}
void MainWindow::updateServiceStats(){
    // Clear tree before updating to remove stale entries (B3)
    TcpIncoming->removeRows(0, TcpIncoming->rowCount());
    TcpOutgoing->removeRows(0, TcpOutgoing->rowCount());
    UdpIncoming->removeRows(0, UdpIncoming->rowCount());
    UdpOutgoing->removeRows(0, UdpOutgoing->rowCount());
    ssh->removeRows(0, ssh->rowCount());

    update_tcp();
    update_udp();
    update_ssh();
}



