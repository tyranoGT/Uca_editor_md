#ifndef TITREINPUTWINDOW_H
#define TITREINPUTWINDOW_H

#include "InputWindow.h"
#include <QLineEdit>
#include <QCalendarWidget>

class TitreInputWindow : public InputWindow
{
public:
    TitreInputWindow();
    TitreInputWindow(Manager_window* m_manager);
    QString m_texte;
private:
    QLineEdit* titre_line_edit=new QLineEdit();
    QCalendarWidget* calendar=new QCalendarWidget();
    void create_title(QString titre,QString date);
public slots:
    void accept() override;
    void reject() override;
};

#endif // TITREINPUTWINDOW_H
