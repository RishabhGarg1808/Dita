// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/analyzer/Analyzer.h"

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);


    analyzer = new Analyzer;
    graph = new Graph(analyzer);
    DevHandle = new DevHandler(graph);

    //Set up the device top use
    initDevList();
    getselectedDev();

    DevHandle->start_capture();
    //inti Alerts
    initService();

    //set up nethogs for use
    initNetUtil();
    auto nethogs = new Nethogs();
    nethogs->start();

    analyzer->setInterfaceIpv4(DevHandle->getDev()->getIPv4Address().toString());
    analyzer->setInterfaceIpv6(DevHandle->getDev()->getIPv6Address().toString());
    //set and initialize the status tab
    initStats();
    // Create and initialize the chart
    ui->Spline->setChart(initChart());
    ui->Spline->setRenderHint(QPainter::Antialiasing);
    ui->Spline->setRenderHint(QPainter::SmoothPixmapTransform);

    //Set up a timer for 1000ms and update the chart
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSeries);
    connect(ui->comboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(reinit()));
    connect(nethogs,&Nethogs::updateNetUtilSignal,this,&MainWindow::updateNetUtil);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateServiceStats);
    //connect(ICMP,&ICMP_analyze::emit_icmp,this,&MainWindow::updateAlertsTab);
}

MainWindow::~MainWindow() {
    DevHandle->stop_capture();
    quit_cb(0);
    delete ui;
}







