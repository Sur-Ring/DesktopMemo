//
// Created by Jiang_Boyuan on 25-4-1.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Memo.h" resolved

#include "Memo.h"
#include "ui_Memo.h"

#include <QFile>

// QString todo_file_path = "Data/todo.txt";
QString todo_file_path = "D:/Codes/QT/DesktopMemo/Data/todo.txt";

namespace StickyNote {
Memo::Memo(QWidget *parent) :
    QPlainTextEdit(parent), ui(new Ui::Memo) {
    ui->setupUi(this);
    load_todo();
}

Memo::~Memo() {
    save_todo();
    delete ui;
}

void Memo::focusOutEvent(QFocusEvent *e) {
    qDebug() << "Widget lost focus";
    save_todo();
}

void Memo::load_todo() {
    QFile file(todo_file_path);
    file.open(QIODevice::ReadOnly);
    if (file.isOpen()) {
        QTextStream in(&file);
        setPlainText(in.readAll());
        file.close();
    }
}

void Memo::save_todo() {
    QFile file(todo_file_path);
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    if (file.isOpen()) {
        QTextStream in(&file);
        in << toPlainText();
        file.close();
    }
}

} // StickyNote
