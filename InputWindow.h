#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H
#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QTextEdit>
#include "Manager_window.h"

class InputWindow : public QObject
{
    Q_OBJECT
public:
    InputWindow();
    QWidget* m_window=new QWidget();
    Manager_window *m_manager_window;
    bool is_ready=false;
    QFormLayout* formLayout;
    QTextEdit* text_edit;
public slots:
    virtual void accept()=0;
    virtual void reject()=0;
signals:
    void update();
};

#endif // INPUTWINDOW_H
