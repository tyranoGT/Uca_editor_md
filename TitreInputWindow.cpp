#include "TitreInputWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QObject>

TitreInputWindow::TitreInputWindow()
{

}

TitreInputWindow::TitreInputWindow(Manager_window* m_manager)
{
    m_manager_window=m_manager;
    m_window=new QWidget();
    QVBoxLayout* layout_window3 = new QVBoxLayout();
    //layout_window3->setAlignment(Qt::AlignHCenter);
    m_window->setLayout(layout_window3);
    //QInputDialog* input_dialogue=new QInputDialog();

    QWidget* form=new QWidget();
    formLayout = new QFormLayout();

    formLayout->addRow("Titre", titre_line_edit);
    formLayout->addRow("Date :",calendar);

    form->setLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &InputWindow::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &InputWindow::reject);
    layout_window3->addWidget(form);
    layout_window3->addWidget(buttonBox,0,Qt::AlignCenter);
}

void TitreInputWindow::accept()
{
    QDate selectedDate = calendar->selectedDate();
    //qDebug()<<lineEdit1->text()<<" "<<lineEdit2->text();
    // Récupérez les QLineEdit du QFormLayout
    /*
    for (int i = 0; i < formLayout->rowCount(); ++i) {
        QLayoutItem* item = formLayout->itemAt(i, QFormLayout::FieldRole);

        if (item) {
            QWidget* widget = item->widget();

            if (widget && widget->isWidgetType() && qobject_cast<QLineEdit*>(widget)) {
                QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget);
                QString text = lineEdit->text();
                qDebug() << "Texte du QLineEdit à la ligne" << i << ":" << text;
            }
        }
    }
    */

    create_title(titre_line_edit->text(),selectedDate.toString("yyyy-MM-dd"));
    is_ready=true;
    m_manager_window->open_window("edit_window");
    emit update();
}

void TitreInputWindow::reject()
{
    m_manager_window->open_window("edit_window");
}

void TitreInputWindow::create_title(QString title,QString date)
{
    m_texte=QString("+++\n"
                      "title = "+title+"\n"
                      "date = "+date+"\n"
                      "draft = false\n"
                    "+++\n");
}

