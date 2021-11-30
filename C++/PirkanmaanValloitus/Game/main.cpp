#include "mainwindow.h"
#include "startdialog.h"

#include <QApplication>


int main(int argc, char* argv[])
{

    QApplication app(argc, argv);

    StartDialog startDialog;
    MainWindow mainWindow;

    QObject::connect(&startDialog, &StartDialog::seedChanged, &mainWindow,&MainWindow::setSeed);
    QObject::connect(&startDialog, &StartDialog::playersChanged, &mainWindow,&MainWindow::setPlayers);
    QObject::connect(&startDialog, &StartDialog::playerNamesGiven, &mainWindow,\
                     &MainWindow::addPlayers);

    if (startDialog.exec() == QDialog::Accepted){
        mainWindow.init();
        mainWindow.show();
    }
    return app.exec();
}
