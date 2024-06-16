#ifndef DITA_MAINWINDOW_H
#define DITA_MAINWINDOW_H

#include <QVector>
#include <QtCharts/QChart>
#include <QTimer>
#include <QStandardItemModel>
#include <QLineSeries>
#include <QDateTime>
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
private:
    Ui::MainWindow *ui;
    QLineSeries *TCP{}, *UDP{}, *HTTP{}, *ICMP{}, *SSL{}, *SSH{};
    QTimer *timer;
    DevHandler DevHandle;
    QDateTime time = QDateTime::currentDateTime();
    QStandardItemModel *totalModel;
    QStandardItemModel *statsModel;

private:
    QChart *initChart();
    void initDevList();
    void getselectedDev();
    void *initAlert();
    void initNetUtil();
    void *initStats();
};


#endif //DITA_MAINWINDOW_H
