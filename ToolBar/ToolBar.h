//
// Created by Jiang_Boyuan on 25-4-1.
//

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>

namespace ToolBar {
QT_BEGIN_NAMESPACE
namespace Ui { class ToolBar; }
QT_END_NAMESPACE

class ToolBar : public QWidget {
Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);
    ~ToolBar() override;
    bool saved;
    bool locked;
private:
    Ui::ToolBar *ui;
signals:
    void force_save();
public slots:
    void has_changed();
    void has_saved();
    void on_save_clicked();
    void on_lock_clicked();
    void on_exit_clicked();
};
} // ToolBar

#endif //TOOLBAR_H
