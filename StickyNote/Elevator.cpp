//
// Created by Jiang_Boyuan on 25-3-31.
//

#include "Elevator.h"
#include <QDebug>

namespace StickyNote {
Elevator::Elevator(HWND hWnd) {
    this->hWnd = hWnd;
    qDebug() << "Elevator: hWnd: " << hWnd;
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
}

void Elevator::work() {
    qDebug() << "Elevator: set topmost";
    SetWindowPos(
        hWnd, // 目标窗口句柄
        HWND_TOPMOST, // 置于所有窗口之上
        0, 0, 0, 0, // 忽略位置和大小参数
        SWP_NOMOVE | SWP_NOSIZE // 保持当前位置和大小
    );
    m_pTimer->start(50);
}

void Elevator::handleTimeout() {
    qDebug() << "Elevator: unset topmost";
    SetWindowPos(
        hWnd,
        HWND_NOTOPMOST, // 恢复为普通窗口
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
    );
    m_pTimer->stop();
}
} // StickyNote
