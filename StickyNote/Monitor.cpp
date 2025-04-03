//
// Created by Jiang_Boyuan on 25-3-31.
//

#include "Monitor.h"
#include <QDebug>
#include <winsock2.h>
#include <windows.h>

namespace StickyNote {
static HWINEVENTHOOK FG_hook = nullptr; //钩子对象
static Monitor *monitor; //Qt界面中调用Hook类的对象

void CALLBACK WinEventProc(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD dwEventThread,
    DWORD dwmsEventTime
) {
    if (hwnd && idObject == OBJID_WINDOW && idChild == CHILDID_SELF && event == EVENT_SYSTEM_FOREGROUND) {
        HWND hWnd = GetForegroundWindow();
        qDebug() << "hwnd"<< hwnd <<" front:" << hWnd << " desktop:" << monitor->desktop << "WorkerW:" << monitor->WorkerW;

        if (hWnd == monitor->desktop || hWnd == monitor->WorkerW) {
            monitor->sendSignal(Monitor::JUMP_TO_DESKTOP);
        }

        TCHAR className[256];
        int length = GetClassName(hwnd, className, 256);
        QString aa = QString(className);
        qDebug() << "className:" << className<<" aa:" << aa<<" equal:"<<(aa == "WorkerW");
        if (aa == "WorkerW") {
            monitor->sendSignal(Monitor::JUMP_TO_DESKTOP);
        }
    }
}

Monitor::Monitor() {
    qDebug() << "Monitor init";
    desktop = FindWindow(LPCWSTR(QString("Progman").utf16()), LPCWSTR(QString("Program Manager").utf16()));
    WorkerW = FindWindow(LPCWSTR(QString("WorkerW").utf16()), LPCWSTR(QString("").utf16()));
    qDebug() << "desktop is: " << desktop;
}

void Monitor::installHook() {
    //安装钩子函数
    FG_hook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
        NULL, WinEventProc,
        0, 0,
        WINEVENT_OUTOFCONTEXT); // WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS

    monitor = this;
    qDebug() << "install hook";
}

void Monitor::unInstallHook() {
    //删除钩子函数
    UnhookWinEvent(FG_hook);
    FG_hook = nullptr;
    monitor = nullptr;
    qDebug() << "uninstall hook";
}

void Monitor::sendSignal(Type type) {
    //发送信号函数
    emit sendKeyType(type);
}
}


// https://blog.csdn.net/zjgo007/article/details/108628842
