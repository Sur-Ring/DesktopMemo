//
// Created by Jiang_Boyuan on 25-3-31.
//

#ifndef MONITOR_H
#define MONITOR_H

#include <QObject>

#include <windows.h>

namespace StickyNote{

class Monitor :public QObject{
    Q_OBJECT
public:
    Monitor();
    enum Type{JUMP_TO_DESKTOP,TEST1,TEST2};
    void installHook();//安装钩子函数
    void unInstallHook();//删除钩子
    void sendSignal(Type type);
    Q_ENUM(Type)//注册枚举类型进元对象系统，否则无法将枚举类型作为信号参数传递
    HWND desktop;
signals:
    void sendKeyType(Type);//发送按键监控信号
};

}

#endif //MONITOR_H
