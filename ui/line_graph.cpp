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

vector<long> MainWindow::calculateSMA(const vector<long> &graphList, int windowSize) {
    vector<long> smaValues(graphList.size(), 0.0);
    for (size_t i = 0; i < graphList.size(); ++i) {
        int start = std::max(0, static_cast<int>(i) - windowSize + 1);
        int end = i + 1;
        long sum = 0.0;
        for (int j = start; j < end; ++j) {
            sum += graphList[j];
        }
        smaValues[i] = sum / (end - start);
    }
    return smaValues;
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
    graphList = calculateSMA(graphList, 5);

    if (total > 0) {
        auto seriesList = ui->Spline->chart()->series();
        QXYSeries *series[] = {
            qobject_cast<QXYSeries*>(seriesList[0]),
            qobject_cast<QXYSeries*>(seriesList[1]),
            qobject_cast<QXYSeries*>(seriesList[2]),
            qobject_cast<QXYSeries*>(seriesList[3]),
            qobject_cast<QXYSeries*>(seriesList[4]),
            qobject_cast<QXYSeries*>(seriesList[5]),
        };
        // Points are added once per second and the X-axis shows the last
        // 600 seconds, so keep a small headroom and drop older points to
        // stop the series (and render cost) growing without bound.
        constexpr int kMaxPoints = 650;
        for (int s = 0; s < 6; ++s) {
            if (!series[s]) continue;
            series[s]->append(x, (graphList.at(s) * 100 / total));
            while (series[s]->count() > kMaxPoints) {
                series[s]->remove(0);
            }
        }
    }

    // adjusting the axis range to keep the latest data visible
    ui->Spline->chart()->axisX()->setRange(QDateTime::currentDateTime().addSecs(-600),
                                           QDateTime::currentDateTime()); // Show last 10 units on X-axis
    ui->Spline->chart()->axisY()->setRange(0,100);// Adjust Y-axis as needed

}