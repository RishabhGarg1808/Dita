// You may need to build the project (run Qt uic code generator) to get "ui_MainWIndow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/analyzer/utils/defines.h"

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);


    //Set up the device top use
    initDevList();
    getselectedDev();
    DevHandle.start_capture();
    //inti Alerts
    initService();

    //set up nethogs for use
    initNetUtil();
    auto nethogs = new Nethogs();
    nethogs->start();

    //set and initialize the status tab
    initStats();
    // Create and initialize the chart
    ui->Spline->setChart(initChart());
    ui->Spline->setRenderHint(QPainter::Antialiasing);
    ui->Spline->setRenderHint(QPainter::SmoothPixmapTransform);

    //Set upa timer for 1000ms and update the chart
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSeries);
    connect(ui->comboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(reinit()));
    connect(nethogs,&Nethogs::updateNetUtilSignal,this,&MainWindow::updateNetUtil);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateServiceStats);
}

MainWindow::~MainWindow() {
    DevHandle.stop_capture();
    quit_cb(0);
    delete ui;
}







