#include "battleship.h"
#include <QApplication>
#include "guiserver.h"
#include "field.h"
#include "battlefield.h"
#include <QTextBrowser>
#include <QScopedPointer>
#include "master.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Master coMaster;

//    QTextBrowser *qtbHistory = new QTextBrowser;
//    qtbHistory->setObjectName("qtbConnectionHistory");
//    qtbHistory->show();




    return a.exec();
}

