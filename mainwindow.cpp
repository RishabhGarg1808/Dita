// You may need to build the project (run Qt uic code generator) to get "ui_MainWIndow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    //set up nethogs for use
    initNetUtil();
    auto nethogs = new Nethogs();
    nethogs->start();
    //Set up the device top use
    initDevList();
    getselectedDev();
    DevHandle.start_capture();

    // Create and initialize the chart
    initStats();
    ui->Spline->setChart(initChart());
    ui->Spline->setRenderHint(QPainter::Antialiasing);
    ui->Spline->setRenderHint(QPainter::SmoothPixmapTransform);

    //Set upa timer for 1000ms and update the chart
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSeries);
    connect(ui->comboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(reinit()));
    connect(nethogs,&Nethogs::updateNetUtilSignal,this,&MainWindow::updateNetUtil);
}

MainWindow::~MainWindow() {
    DevHandle.stop_capture();
    delete ui;
}

void MainWindow::updateNetUtil(QVector<Line *> data, int n) {
    std::cout << "Lines Received \n" << std::flush;
//    for(int i = 0;i<=n;i++){
//        Line *line = data[i];
//        for(int row =i+1;row<statsModel->rowCount();++row){
//                statsModel->setItem(row,0,new QStandardItem(
//                        QString::fromStdString(std::to_string(line->m_pid))));
//                statsModel->setItem(row,1,new QStandardItem(
//                        QString::fromStdString(std::to_string(line->m_uid))));
//                statsModel->setItem(row,2,new QStandardItem(
//                        QString::fromStdString(line->m_name)));
//                statsModel->setItem(row,3,new QStandardItem(
//                        QString::fromStdString(line->devicename)));
//                statsModel->setItem(row,4,new QStandardItem(
//                        QString::fromStdString(std::to_string(line->sent_value))));
//                statsModel->setItem(row,5,new QStandardItem(
//                        QString::fromStdString(std::to_string(line->recv_value))));
//        }
//    }
}





