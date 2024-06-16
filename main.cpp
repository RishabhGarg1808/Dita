#include <QApplication>
#include <QMessageBox>
#include <QLabel>
#include <QWidget>
#include "unistd.h"
#include "mainwindow.h"

bool is_root() {
    return (geteuid() == 0);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow window;
    //check to ensure application has enough privileges
    if(!is_root()){
            auto *dialog = new QMessageBox();
            dialog->setWindowTitle("Error!");
            dialog->setInformativeText("Not Enough Permissions."
                                       "Please run the program as root!");
            dialog->exec();
            return 1;
    }
    window.show();
    return QApplication::exec();
}
