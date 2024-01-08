#ifndef VIDEOINPUTWINDOW_H
#define VIDEOINPUTWINDOW_H

#include "InputWindow.h"

class VideoInputWindow: public InputWindow
{
public:
    VideoInputWindow();
    VideoInputWindow(Manager_window* m_manager);
    QString m_texte;
private:
    QLineEdit* figcaption=new QLineEdit();
    QLineEdit* lien=new QLineEdit();
    void create_video(QString figcaption,QString lien);
public slots:
    void accept() override;
    void reject() override;
};

#endif // VIDEOINPUTWINDOW_H
