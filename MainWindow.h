#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Manager_window.h"
#include "Writer.h"
#include "Reader.h"
#include "InputWindow.h"
#include "TitreInputWindow.h"
#include "ImageInputWindow.h"
#include "VideoInputWindow.h"

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Manager_window *m_manager_window=new Manager_window();
    Reader* m_reader=new Reader();
    Writer* m_writer=new Writer();
    QTextEdit* TextEdit;
    bool format_md=false;
    QString str_content_preview="";
    QString str_content;
    bool is_save;
    bool user_input=false;
    enum specials { H1, H2, ITALIC,END_ITALIC,BOLD,END_BOLD,LIEN,END_LIEN,IMAGE,END_IMAGE,VIDEO,END_VIDEO,NO,TITLE,END_TITLE};
    QPair<int,specials>* special_to_add=NULL;
    specials current_special=NO;
    InputWindow* current_input_window;
    int all_moins;
    ImageInputWindow* image_input;
    TitreInputWindow* title_input;
    VideoInputWindow* video_input;
    QString new_file_path;
    void update_text_edit();
private:
    Ui::MainWindow *ui;
    int nb_char_search(int direction,int position);
    int search_caracteres_speciaux(int direction,int position,bool add_incomplete); //-1 gauche
    QTextCharFormat create_police(int size,bool is_bold,bool is_italic,QColor color,bool is_soulign);
    void change_text_format(int start,int decal,QTextCharFormat format);
    int end_balise(int position_start,QString balise);
    void open_file(QString file_path,bool first_time);
public slots:
    void ouvrir_explorateur();
    void get_file_path(bool to_create);
    void generate_file();
    void save();
    void switch_preview();
    void changeTextFormat(int index);
    void changeTextWidth(int index);
    void italic_switch(bool state);
    void onTextChanged();
    void cursorPositionChanged();
    void add_puce();
    void add_image();
    void add_video();
    void clic_text_edit();
    void end_dialogue();
};
#endif // MAINWINDOW_H
