#include "dialog.h"

#include <QApplication>
//#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
