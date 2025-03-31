//
// Created by Jiang_Boyuan on 25-3-31.
//

// You may need to build the project (run Qt uic code generator) to get "ui_StickyNote.h" resolved

#include "StickyNote.h"
#include "ui_StickyNote.h"

#include "Monitor.h"
#include <QThread>
#include <Windows.h>
#include <Psapi.h>
#include <QEvent>
#include <QWindowStateChangeEvent>

namespace StickyNote {
StickyNote::StickyNote(QWidget *parent) : QWidget(parent), ui(new Ui::StickyNote), hWnd((HWND)winId()), elevator(hWnd) {
    ui->setupUi(this);

    qDebug()<<"StickyNote: hWnd: "<<hWnd;

    monitor.installHook();
    qRegisterMetaType<Monitor::Type>("Type");//为了信号中能传递自定义枚举类型，如果传递常规参数，可省略该行
    connect(&monitor,SIGNAL(sendKeyType(Type)),this,SLOT(checkType(Type)));

    as_toolwindow();
    move_to_top();
}

StickyNote::~StickyNote() {
    delete ui;
    monitor.unInstallHook();
}

void StickyNote::checkType(Monitor::Type type){
    switch (type) {
        case Monitor::TEST1:
            qDebug()<<"TEST1";
        break;
        case Monitor::TEST2:
            qDebug()<<"TEST2";
        move_to_top();
        break;
        case Monitor::JUMP_TO_DESKTOP:
            qDebug()<<"JUMP_TO_DESKTOP";
        move_to_top();
        break;
        default:
            break;
    }
}

void StickyNote::move_to_top() {
    HWND handle = (HWND) winId();
    qDebug()<<"move self: "<<handle<<" to top";
    elevator.work();
}

void StickyNote::as_toolwindow() {
    HWND handle = (HWND) winId();
    SetWindowLong(handle,GWL_EXSTYLE,WS_EX_TOOLWINDOW);
}

} // StickyNote
