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

namespace StickyNote {
QT_BEGIN_NAMESPACE
namespace Ui { class StickyNote; }
QT_END_NAMESPACE

class StickyNote : public QWidget {
Q_OBJECT

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
};
} // StickyNote

#endif //StickyNote_H
