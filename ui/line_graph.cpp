#include <QtCharts/QSplineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QDateTime>

#include "mainwindow.h"
#include "../ui_mainwindow.h"

QChart *MainWindow::initChart() {

    auto *chart = new QChart();
    chart->setTitle("Traffic per protocol (packets/s)");
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

    auto QDateTimeAxis_X  = new QDateTimeAxis();
    QDateTimeAxis_X->setFormat("hh:mm");
    QDateTimeAxis_X->setTitleText("Time");

    chart->setAxisX(QDateTimeAxis_X, TCP);
    chart->setAxisX(QDateTimeAxis_X, UDP);
    chart->setAxisX(QDateTimeAxis_X, HTTP);
    chart->setAxisX(QDateTimeAxis_X, ICMP);
    chart->setAxisX(QDateTimeAxis_X, SSL);
    chart->setAxisX(QDateTimeAxis_X, SSH);

    auto *yAxis = new QValueAxis();
    yAxis->setTitleText("Packets / s");
    yAxis->setRange(0, 10);
    chart->addAxis(yAxis, Qt::AlignBottom);
    chart->setAxisY(yAxis, TCP);
    chart->setAxisY(yAxis, UDP);
    chart->setAxisY(yAxis, HTTP);
    chart->setAxisY(yAxis, ICMP);
    chart->setAxisY(yAxis, SSL);
    chart->setAxisY(yAxis, SSH);

    return chart;
}

void MainWindow::updateSeries() {
    // Packets per second since the previous tick. The counters are
    // atomics written on the capture thread; sampleRates() turns the
    // cumulative counts into per-second rates on this (GUI) thread.
    auto rates = graph->sampleRates();

    const double values[6] = {rates.tcp, rates.udp, rates.http,
                              rates.icmp, rates.ssl, rates.ssh};

    auto x = QDateTime::currentDateTime().toMSecsSinceEpoch();

    auto seriesList = ui->Spline->chart()->series();
    QXYSeries *series[] = {
        qobject_cast<QXYSeries*>(seriesList[0]),
        qobject_cast<QXYSeries*>(seriesList[1]),
        qobject_cast<QXYSeries*>(seriesList[2]),
        qobject_cast<QXYSeries*>(seriesList[3]),
        qobject_cast<QXYSeries*>(seriesList[4]),
        qobject_cast<QXYSeries*>(seriesList[5]),
    };

    double peak = 0;
    for (int i = 0; i < 6; ++i) {
        if (values[i] > peak) peak = values[i];
        if (!series[i]) continue;
        // Always append, even when idle (zero), so the time axis stays
        // continuous. Keep a small headroom over the 600 s X window so the
        // series (and render cost) never grow without bound.
        series[i]->append(x, values[i]);
        constexpr int kMaxPoints = 650;
        while (series[i]->count() > kMaxPoints) {
            series[i]->remove(0);
        }
    }

    // Rolling 10-minute X window; Y auto-scales to the recent peak.
    auto *chart = ui->Spline->chart();
    chart->axisX()->setRange(QDateTime::currentDateTime().addSecs(-600),
                             QDateTime::currentDateTime());
    if (auto *yAxis = qobject_cast<QValueAxis*>(chart->axisY())) {
        yAxis->setRange(0, qMax(10.0, peak * 1.25));
    }
}
