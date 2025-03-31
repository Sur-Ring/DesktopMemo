//
// Created by Jiang_Boyuan on 25-3-31.
//

#ifndef StickyNote_H
#define StickyNote_H

#include <QWidget>

#include <windows.h>
#include <winuser.h>
#include "Monitor.h"
#include "Elevator.h"

#include <windows.h>        //注意头文件
#include <windowsx.h>
#include <QMouseEvent>

namespace StickyNote {
QT_BEGIN_NAMESPACE
namespace Ui { class StickyNote; }
QT_END_NAMESPACE

class StickyNote : public QWidget {
Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
private:
    bool locked = false;
    int boundaryWidth = 4; // 可拖动距离
    QPoint m_dragPosition;  // 用于窗口移动的临时变量
public:
    explicit StickyNote(QWidget *parent = nullptr);
    ~StickyNote() override;
    void as_toolwindow();
private:
    Ui::StickyNote *ui;
    HWND hWnd;
    Monitor monitor;
    Elevator elevator;
private slots:
    void move_to_top();
    void checkType(Monitor::Type);
    void lock();
    void unlock();
};
} // StickyNote

#endif //StickyNote_H
