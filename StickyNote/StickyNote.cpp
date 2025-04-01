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
#include <QMouseEvent>
#include <windows.h>
#include <windowsx.h>
#include <QSettings>
#include <QFont>

namespace StickyNote {
StickyNote::StickyNote(QWidget *parent) : QWidget(parent), ui(new Ui::StickyNote), hWnd((HWND) winId()),
                                          elevator(hWnd) {
    qDebug() << "StickyNote init";
    ui->setupUi(this);
    qDebug() << "StickyNote: hWnd: " << hWnd;

    monitor.installHook();
    qRegisterMetaType<Monitor::Type>("Type"); //为了信号中能传递自定义枚举类型，如果传递常规参数，可省略该行
    connect(&monitor,SIGNAL(sendKeyType(Type)), this,SLOT(checkType(Type)));

    connect(ui->memo,SIGNAL(saved()), ui->tool_bar,SLOT(has_saved()));
    connect(ui->memo,SIGNAL(textChanged()), ui->tool_bar,SLOT(has_changed()));
    // connect(ui->tool_bar,SIGNAL(force_save()), ui->memo,SLOT(save_todo()));

    as_toolwindow();
    move_to_top();

    config = new QSettings("./Data/cfg.ini", QSettings::IniFormat);
    // config = new QSettings("D:/Codes/QT/DesktopMemo/Data/cfg.ini", QSettings::IniFormat);

    int font_size = config->value("/SN/FontSize", 14).toUInt();
    bool locked = config->value("/SN/Locked", false).toBool();
    ui->tool_bar->locked = !locked;
    ui->tool_bar->on_lock_btn_clicked();
    QFont ft;
    ft.setPointSize(font_size);
    ui->memo->setFont(ft);

    restoreGeometry(config->value("/SN/Geometry","300,300,300,300").toByteArray());

    inited = true;
}

StickyNote::~StickyNote() {
    config->setValue("/SN/Locked", ui->tool_bar->locked);
    config->setValue("/SN/Geometry", saveGeometry());
    delete config;
    delete ui;
    monitor.unInstallHook();
    qDebug() << "StickyNote exit";
}

void StickyNote::checkType(Monitor::Type type) {
    switch (type) {
        case Monitor::TEST1:
            qDebug() << "TEST1";
            break;
        case Monitor::TEST2:
            qDebug() << "TEST2";
            move_to_top();
            break;
        case Monitor::JUMP_TO_DESKTOP:
            qDebug() << "JUMP_TO_DESKTOP";
            move_to_top();
            break;
        default:
            break;
    }
}

void StickyNote::move_to_top() {
    HWND handle = (HWND) winId();
    qDebug() << "move self: " << handle << " to top";
    elevator.work();
}

void StickyNote::as_toolwindow() {
    HWND handle = (HWND) winId();
    SetWindowLong(handle,GWL_STYLE,GetWindowLong(handle, GWL_STYLE) & ~WS_CAPTION);
    SetWindowLong(handle,GWL_EXSTYLE,WS_EX_TOOLWINDOW);
}

void StickyNote::mousePressEvent(QMouseEvent *event) {
    if (!inited) return;
    if (ui->tool_bar->locked) return;
    // 当鼠标左键按下时记录鼠标的全局坐标与窗口左上角的坐标差
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->pos();
        event->accept();
    }
}

void StickyNote::mouseMoveEvent(QMouseEvent *event) {
    if (!inited) return;
    if (ui->tool_bar->locked) return;
    // 当鼠标左键被按下时移动窗口
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}
} // StickyNote
