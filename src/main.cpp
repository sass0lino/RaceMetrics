#include <QApplication>
#include <QIcon>
#include "gui/MainWindow.h"


int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":assets/icon.svg"));

    MainWindow w;
    w.show();

    return app.exec();
}