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
    quit_cb(0);
    delete ui;
}

void MainWindow::updateNetUtil(QVector<Line *> data, int n) {
    for(int i = 0; i < n; i++){
        Line *line = data[i];
        if (i < statsModel->rowCount()) {
            // Update existing and if pid == 0 skip
            if(line->m_pid == 0) continue;
            statsModel->item(i, 0)->setText(QString::fromStdString(std::to_string(line->m_pid)));
            statsModel->item(i, 1)->setText(QString::fromStdString(std::to_string(line->m_uid)));
            statsModel->item(i, 2)->setText(QString::fromStdString(line->m_name));
            statsModel->item(i, 3)->setText(QString::fromStdString(line->devicename));
            statsModel->item(i, 4)->setText(QString::fromStdString(std::to_string(line->sent_value)));
            statsModel->item(i, 5)->setText(QString::fromStdString(std::to_string(line->recv_value)));
        } else {
            // Add new row if it doesn't exist and check for pid != 0
            if(line->m_pid == 0) continue;
            QList<QStandardItem *> items;
            items.append(new QStandardItem(QString::fromStdString(std::to_string(line->m_pid))));
            items.append(new QStandardItem(QString::fromStdString(std::to_string(line->m_uid))));
            items.append(new QStandardItem(QString::fromStdString(line->m_name)));
            items.append(new QStandardItem(QString::fromStdString(line->devicename)));
            items.append(new QStandardItem(QString::fromStdString(std::to_string(line->sent_value))));
            items.append(new QStandardItem(QString::fromStdString(std::to_string(line->recv_value))));
            statsModel->appendRow(items);
        }
    }
    float total_sent =0;
    float total_recv =0;

    for(int i = 0; i < statsModel->rowCount(); i++){
        QStandardItem *sent = statsModel->item(i, 4);
        QStandardItem *recv = statsModel->item(i, 5);
        total_sent += sent->text().toFloat();
        total_recv += recv->text().toFloat();
    }
    QString proc= QString::fromStdString("\u2211 Processes : " + std::to_string(statsModel->rowCount()));
    QString sent= QString::fromStdString("\u2211 Sent : " + std::to_string(total_sent) +  + " KBps");
    QString recv= QString::fromStdString( "\u2211 Received : " + std::to_string(total_recv) +  + " KBps");
    totalModel->setHorizontalHeaderLabels(QStringList() << sent<< recv << proc);
}





