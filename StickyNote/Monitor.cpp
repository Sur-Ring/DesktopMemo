//
// Created by Jiang_Boyuan on 25-3-31.
//

#include "Monitor.h"
#include <QDebug>
#include <windows.h>
#include <winsock2.h>

namespace StickyNote {
static HHOOK keyHook = nullptr; //钩子对象
static HWINEVENTHOOK FG_hook = nullptr; //钩子对象
static Monitor *monitor; //Qt界面中调用Hook类的对象

LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam) {
    //钩子消息函数，系统消息队列信息会返回到该函数中
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam; //lParam用于判断按键类型
    if (wParam == WM_KEYDOWN) {
        //wParam用于判断事件类型，当前为按键按下事件
        if (pkbhs->vkCode == 0x31 && GetAsyncKeyState(VK_CONTROL)) {
            qDebug() << "send TEST1";
            monitor->sendSignal(Monitor::TEST1);
        }
        if (pkbhs->vkCode == 0x32 && GetAsyncKeyState(VK_CONTROL)) {
            qDebug() << "send TEST2";
            monitor->sendSignal(Monitor::TEST2);
        }
        if (pkbhs->vkCode == 0x44 && GetAsyncKeyState(VK_LWIN)) {
            //按下Win+D
            // HWND hWnd = GetForegroundWindow();
            // if (hWnd != monitor->desktop) {
            //     qDebug() << "send signal";
            //     monitor->sendSignal(Monitor::JUMP_TO_DESKTOP);
            // }
        }
    }
    return CallNextHookEx(keyHook, nCode, wParam, lParam); //继续原有的事件队列
}

void CALLBACK WinEventProc(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD dwEventThread,
    DWORD dwmsEventTime
) {
    if (hwnd &&
        idObject == OBJID_WINDOW &&
        idChild == CHILDID_SELF &&
        event == EVENT_SYSTEM_FOREGROUND) {
        HWND hWnd = GetForegroundWindow();
        qDebug() << "front:" << hWnd << " desktop:" << monitor->desktop << "equal:" << (hWnd == monitor->desktop);
        if (hWnd == monitor->desktop) {
            qDebug() << "send signal";
            monitor->sendSignal(Monitor::JUMP_TO_DESKTOP);
            qDebug() << "wait return?";
        }
    }
}

Monitor::Monitor() {
    desktop = FindWindow(LPCWSTR(QString("Progman").utf16()), LPCWSTR(QString("Program Manager").utf16()));
    qDebug() << "desktop is: " << desktop;
}

void Monitor::installHook() {
    //安装钩子函数
    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, nullptr, 0);
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
    UnhookWindowsHookEx(keyHook);
    UnhookWinEvent(FG_hook);
    keyHook = nullptr;
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
