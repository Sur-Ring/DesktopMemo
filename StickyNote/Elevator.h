//
// Created by Jiang_Boyuan on 25-3-31.
//

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>
#include <QTimer>
#include <windows.h>

namespace StickyNote {
class Elevator : public QObject {
    Q_OBJECT

public:
    Elevator(HWND hWnd);

    void work();

    int topmost_time = 50;

private:
    HWND hWnd;
    QTimer *m_pTimer;

public slots:
    void handleTimeout(); //超时处理函数
};
} // StickyNote

#endif //ELEVATOR_H
