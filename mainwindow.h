#ifndef DITA_MAINWINDOW_H
#define DITA_MAINWINDOW_H

#include <QVector>
#include <QtCharts/QChart>
#include <QTimer>
#include <QStandardItemModel>
#include <QLineSeries>
#include <QDateTime>
#include <QTreeWidget>
#include "DevHandler.h"
#include "ui/netutil_tab.h"
#include "ui/line.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QWidget {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void updateSeries();
    void reinit();
    void updateNetUtil(QVector<Line*>, int);
    void updateServiceStats();
    void updateAlertsTabPing(const string&);
    void updateAlertsTabSSH(const string&,const string&);

private:
    int gphUtil =0;
    Analyzer *analyzer;
    Graph *graph;
    QString unit = " KBps";
    Ui::MainWindow *ui;
    QLineSeries *TCP{}, *UDP{}, *HTTP{}, *ICMP{}, *SSL{}, *SSH{};
    QTimer *timer,*timer2;
    DevHandler *DevHandle;
    QDateTime time = QDateTime::currentDateTime();
    QStandardItemModel *totalModel,*statsModel,*serviceModel,*alertsModel;
    QStandardItem *ssh,*tcp,*udp;
    QStandardItem *UdpIncoming,*UdpOutgoing,*TcpIncoming,*TcpOutgoing;
private:
    QChart *initChart();
    void initDevList();
    void getselectedDev();
    void *initService();
    void initNetUtil();
    void *initStats();

//functions  for the service tab
    void update_tcp();
    void update_udp();
//    void update_http();
//    void update_icmp();
    void update_ssh();
};


#endif //DITA_MAINWINDOW_H
