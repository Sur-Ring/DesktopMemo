#include <QApplication>

#include <windows.h>
#include <winuser.h>

#include "StickyNote/StickyNote.h"

#include <QMessageLogContext>
#include <QDateTime>
#include <QFile>

void writeToLog(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QString prefix;
    switch (type) {
        case QtDebugMsg:
            prefix = "Debug";
        break;
        case QtInfoMsg:
            prefix = "Info";
        break;
        case QtWarningMsg:
            prefix = "Warning";
        break;
        case QtCriticalMsg:
            prefix = "Critical";
        break;
        case QtFatalMsg:
            prefix = "Fatal";
        break;
    }
    QString fileName = "run_" + QDateTime::currentDateTime().toString("yyyyMMdd") + ".log";
    // QFile logFile(QApplication::applicationDirPath() + "log/" + fileName);
    QFile logFile("./my_log.txt");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&logFile);
        out << msg <<"\n";
        logFile.close();
    }

    // 对于QtFatalMsg，确保标准处理被执行（默认是终止程序）
    if (type == QtFatalMsg)
        abort();
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // qInstallMessageHandler(writeToLog);
    qDebug()<<"init";

    StickyNote::StickyNote s;
    s.show();

    return a.exec();
}
