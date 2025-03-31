#include "mainwindow.h"

#include <QApplication>

#include <windows.h>
#include <winuser.h>

#include "StickyNote/StickyNote.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qDebug()<<"init";

    // MainWindow w;
    // w.show();
    StickyNote::StickyNote s;
    s.show();

    return a.exec();
}
