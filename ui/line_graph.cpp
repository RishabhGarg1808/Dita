#include <QtCharts/QSplineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QDateTime>

#include "mainwindow.h"
#include "../ui_mainwindow.h"

QChart *MainWindow::initChart() {

    auto *chart = new QChart();
    chart->setTitle("Service Feed (%)");
    TCP = new QSplineSeries();
    UDP = new QSplineSeries();
    HTTP = new QSplineSeries();
    ICMP = new QSplineSeries();
    SSL = new QSplineSeries();
    SSH = new QSplineSeries();

    TCP->setName("TCP");
    UDP->setName("UDP");
    HTTP->setName("HTTP");
    ICMP->setName("ICMP");
    SSL->setName("SSL");
    SSH->setName("SSH");
    SSH->setColor(Qt::yellow   );

    chart->addSeries(TCP);
    chart->addSeries(UDP);
    chart->addSeries(HTTP);
    chart->addSeries(ICMP);
    chart->addSeries(SSL);
    chart->addSeries(SSH);

    chart->createDefaultAxes();

    auto QDateTimeAxis_X  = new QDateTimeAxis();
    QDateTimeAxis_X->setFormat("hh:mm");
    QDateTimeAxis_X->setTitleText("Time");

    chart->setAxisX(QDateTimeAxis_X, TCP);
    chart->setAxisX(QDateTimeAxis_X, UDP);
    chart->setAxisX(QDateTimeAxis_X, HTTP);
    chart->setAxisX(QDateTimeAxis_X, ICMP);
    chart->setAxisX(QDateTimeAxis_X, SSL);
    chart->setAxisX(QDateTimeAxis_X, SSH);

    return chart;
}

void MainWindow::updateSeries() {
    chrono::time_point<chrono::steady_clock> now = chrono::steady_clock::now();
    // Insert data to graphs

    auto tcp_y = graph->ServiceSt.tcpPacketCount;
    auto udp_y = graph->ServiceSt.udpPacketCount;
    auto http_y = graph->ServiceSt.httpPacketCount;
    auto icmp_y = graph->ServiceSt.icmpPacketCount;
    auto ssl_y = graph->ServiceSt.sslPacketCount;
    auto ssh_y = graph->ServiceSt.sshPacketCount;

    auto total = graph->ServiceSt.totalPacketCount;
    auto x = QDateTime::currentDateTime().toMSecsSinceEpoch();

    vector<long> graphList = {tcp_y,udp_y,http_y,icmp_y,ssl_y,ssh_y};
    //Graph::smoothData(graphList,3);
    if (total > 0) {
        TCP->append(x, (graphList.at(0) * 100/total ));
        UDP->append(x, (graphList.at(1) * 100/total ));
        HTTP->append(x, (graphList.at(2) * 100/total ));
        ICMP->append(x, (graphList.at(3) * 100/total ));
        SSL->append(x, (graphList.at(4) * 100/total ));
        SSH->append(x, (graphList.at(5) * 100/total ));
    }

    // adjusting the axis range to keep the latest data visible
    ui->Spline->chart()->axisX()->setRange(QDateTime::currentDateTime().addSecs(-600),
                                           QDateTime::currentDateTime()); // Show last 10 units on X-axis
    ui->Spline->chart()->axisY()->setRange(0,100);// Adjust Y-axis as needed

    //Reset the graph data every 3 seconds to aggregate the data evenly;
//    if(gphUtil%2==0){
//        graph->reset();
//    }
//    gphUtil++;
}