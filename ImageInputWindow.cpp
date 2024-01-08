#include "ImageInputWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QObject>
#include "MainWindow.h"

ImageInputWindow::ImageInputWindow()
{

}

ImageInputWindow::ImageInputWindow(Manager_window* m_manager)
{
    m_manager_window=m_manager;
    m_window=new QWidget();
    QVBoxLayout* layout_window3 = new QVBoxLayout();
    //layout_window3->setAlignment(Qt::AlignHCenter);
    m_window->setLayout(layout_window3);
    //QInputDialog* input_dialogue=new QInputDialog();

    QWidget* form=new QWidget();
    formLayout = new QFormLayout();

    formLayout->addRow("Figcaption", figcaption);
    formLayout->addRow("lien :",lien);

    form->setLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &InputWindow::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &InputWindow::reject);
    layout_window3->addWidget(form);
    layout_window3->addWidget(buttonBox,0,Qt::AlignCenter);
}

void ImageInputWindow::accept()
{
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

    create_image(figcaption->text(),lien->text());
    is_ready=true;
    m_manager_window->open_window("edit_window");
    emit update();
}

void ImageInputWindow::reject()
{
    m_manager_window->open_window("edit_window");
}

void ImageInputWindow::create_image(QString figcaption,QString lien)
{
    m_texte=QString("\n<figure>\n"
                      "<picture>\n"
                      "<!-- AVIF -->\n"
                      "<source type=\"image/avif\" srcset=\"img/"+lien+"\">\n"
                               "<source type=\"image/webp\" srcset=\"img/"+lien+"\">\n"
                               "<!-- JPEG de repli pour les navigateurs qui ne supportent ni AVIF ni WebP -->"
                               "<img src=\"img/"+lien+"\" loading=\"lazy\">\n"
                               "</picture>\n"
                               "<figcaption><em>"+figcaption+"\n"
                                     "</em></figcaption>\n"
                                     "</figure>\n");
}

