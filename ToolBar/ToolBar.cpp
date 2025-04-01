//
// Created by Jiang_Boyuan on 25-4-1.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ToolBar.h" resolved

#include "ToolBar.h"
#include "ui_ToolBar.h"

#include <QStyle>

namespace ToolBar {
ToolBar::ToolBar(QWidget *parent) :
    QWidget(parent), ui(new Ui::ToolBar) {
    qDebug() << "ToolBar init";
    ui->setupUi(this);
    saved = true;
    locked = false;
    connect(ui->lock_btn,SIGNAL(clicked()),this,SLOT(on_lock_clicked()));
    connect(ui->exit_btn,SIGNAL(clicked()),this,SLOT(on_exit_clicked()));
}

ToolBar::~ToolBar() {
    delete ui;
}

void ToolBar::has_saved() {
    saved = true;
    ui->save_btn->setText("Saved");
    ui->save_btn->setIcon(QIcon::fromTheme("document-save"));
}

void ToolBar::has_changed() {
    saved = false;
    ui->save_btn->setText("Edited");
    ui->save_btn->setIcon(QIcon::fromTheme("mail-message-new"));
}

void ToolBar::on_save_clicked() {
    if (!saved) {
        // 按到这个按钮本身就会导致失去焦点发生保存, 所以什么都不用做
        // force_save();
    }
}

void ToolBar::on_lock_clicked() {
    locked = !locked;
    if (locked) {
        ui->lock_btn->setText("Lock");
        ui->lock_btn->setIcon(QIcon::fromTheme("media-record"));
    }else {
        ui->lock_btn->setText("No Lock");
        ui->lock_btn->setIcon(QIcon::fromTheme("media-optical"));
    }
}

void ToolBar::on_exit_clicked() {
    qApp->quit();
}
} // ToolBar
