#ifndef IMAGEINPUTWINDOW_H
#define IMAGEINPUTWINDOW_H

#include "InputWindow.h"

class ImageInputWindow : public InputWindow
{
public:
    ImageInputWindow();
    ImageInputWindow(Manager_window* m_manager);
    QString m_texte;
private:
    QLineEdit* figcaption=new QLineEdit();
    QLineEdit* lien=new QLineEdit();
    void create_image(QString figcaption,QString lien);
public slots:
    void accept() override;
    void reject() override;
};

#endif // IMAGEINPUTWINDOW_H
