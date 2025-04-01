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
#include <QPainter>

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

    set_window_style();
    move_to_top();

    config = new QSettings("./Data/cfg.ini", QSettings::IniFormat);
    // config = new QSettings("D:/Codes/QT/DesktopMemo/Data/cfg.ini", QSettings::IniFormat);

    int font_size = config->value("/SN/FontSize", 14).toUInt();
    config->setValue("/SN/FontSize", font_size);
    bool locked = config->value("/SN/Locked", false).toBool();
    ui->tool_bar->locked = !locked;
    ui->tool_bar->on_lock_btn_clicked();
    QFont ft;
    ft.setPointSize(font_size);
    ui->memo->setFont(ft);

    int topmost_time = config->value("/SN/TopMost", 50).toUInt();
    config->setValue("/SN/TopMost", topmost_time);
    elevator.topmost_time = topmost_time;

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

void StickyNote::set_window_style() {
    HWND handle = (HWND) winId();
    setWindowFlags(Qt::Tool|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);      //启用样式背景绘制
    setAttribute(Qt::WA_TranslucentBackground); //背景透明
}

void StickyNote::paintEvent(QPaintEvent*event){
    QPainter p(this);
    p.setBrush(QColor(0, 0, 0, 96));//填充黑色半透明
    p.drawRect(this->rect());//绘制半透明矩形，覆盖整个窗体
    QWidget::paintEvent(event);
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

bool StickyNote::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
    if (!inited) return false;
    if (ui->tool_bar->locked) return false;
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
