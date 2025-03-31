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

namespace StickyNote {
StickyNote::StickyNote(QWidget *parent) : QWidget(parent), ui(new Ui::StickyNote), hWnd((HWND) winId()),
                                          elevator(hWnd) {
    ui->setupUi(this);
    qDebug() << "StickyNote: hWnd: " << hWnd;

    monitor.installHook();
    qRegisterMetaType<Monitor::Type>("Type"); //为了信号中能传递自定义枚举类型，如果传递常规参数，可省略该行
    connect(&monitor,SIGNAL(sendKeyType(Type)), this,SLOT(checkType(Type)));

    connect(ui->memo,SIGNAL(saved()), ui->tool_bar,SLOT(has_saved()));
    connect(ui->memo,SIGNAL(textChanged()), ui->tool_bar,SLOT(has_changed()));
    connect(ui->tool_bar,SIGNAL(force_save()), ui->memo,SLOT(save_todo()));

    connect(ui->tool_bar,SIGNAL(lock()), this,SLOT(lock()));
    connect(ui->tool_bar,SIGNAL(unlock()), this,SLOT(unlock()));

    as_toolwindow();
    move_to_top();
}

StickyNote::~StickyNote() {
    delete ui;
    monitor.unInstallHook();
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

void StickyNote::lock() {
    locked = true;
}


void StickyNote::unlock() {
    locked = false;
}


void StickyNote::move_to_top() {
    HWND handle = (HWND) winId();
    qDebug() << "move self: " << handle << " to top";
    elevator.work();
}

void StickyNote::as_toolwindow() {
    HWND handle = (HWND) winId();
    SetWindowLong(handle,GWL_EXSTYLE,WS_EX_TOOLWINDOW);
    setWindowFlags(Qt::FramelessWindowHint);
}

void StickyNote::mousePressEvent(QMouseEvent *event) {
    if (locked) return;
    // 当鼠标左键按下时记录鼠标的全局坐标与窗口左上角的坐标差
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->pos();
        event->accept();
    }
}

void StickyNote::mouseMoveEvent(QMouseEvent *event) {
    if (locked) return;
    // 当鼠标左键被按下时移动窗口
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

bool StickyNote::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
    if (locked) return false;
    MSG *msg = (MSG *) message;
    switch (msg->message) {
        case WM_NCHITTEST:
            const auto ratio = devicePixelRatioF();
            int xPos = GET_X_LPARAM(msg->lParam) / ratio - this->frameGeometry().x();
            int yPos = GET_Y_LPARAM(msg->lParam) / ratio - this->frameGeometry().y();
            if (xPos < boundaryWidth && yPos < boundaryWidth) //左上角
                *result = HTTOPLEFT;
            else if (xPos >= width() - boundaryWidth && yPos < boundaryWidth) //右上角
                *result = HTTOPRIGHT;
            else if (xPos < boundaryWidth && yPos >= height() - boundaryWidth) //左下角
                *result = HTBOTTOMLEFT;
            else if (xPos >= width() - boundaryWidth && yPos >= height() - boundaryWidth) //右下角
                *result = HTBOTTOMRIGHT;
            else if (xPos < boundaryWidth) //左边
                *result = HTLEFT;
            else if (xPos >= width() - boundaryWidth) //右边
                *result = HTRIGHT;
            else if (yPos < boundaryWidth) //上边
                *result = HTTOP;
            else if (yPos >= height() - boundaryWidth) //下边
                *result = HTBOTTOM;
            else //其他部分不做处理，返回false，留给其他事件处理器处理
                return false;
            return true;
    }
    return false; //此处返回false，留给其他事件处理器处理
}
} // StickyNote
