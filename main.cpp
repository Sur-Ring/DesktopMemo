#include <QApplication>

#include <windows.h>
#include <winuser.h>

#include "StickyNote/StickyNote.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qDebug()<<"init";

    StickyNote::StickyNote s;
    s.show();

    return a.exec();
}
