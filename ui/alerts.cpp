#include "../mainwindow.h"
#include <QListWidgetItem>


void MainWindow::updateAlertsTab(){
    string pingSrc;
    auto *item = new QListWidgetItem;
    item->setText(QString::fromStdString(pingSrc));
    ui->Alerts_list->addItem(item);
}