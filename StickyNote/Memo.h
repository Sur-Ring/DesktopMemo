//
// Created by Jiang_Boyuan on 25-4-1.
//

#ifndef MEMO_H
#define MEMO_H

#include <QPlainTextEdit>

namespace StickyNote {
QT_BEGIN_NAMESPACE
namespace Ui { class Memo; }
QT_END_NAMESPACE

class Memo : public QPlainTextEdit {
Q_OBJECT

public:
    explicit Memo(QWidget *parent = nullptr);
    ~Memo() override;
    void load_todo();
    void save_todo();
    void focusOutEvent(QFocusEvent *e);
private:
    Ui::Memo *ui;
private slots:
    void on_text_changed();
};
} // StickyNote

#endif //MEMO_H
