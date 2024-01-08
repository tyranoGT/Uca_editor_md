#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QTextEdit>
#include <QFont>
#include <iostream>
#include <QPair>
#include <QComboBox>
#include <QCheckBox>
#include <QTextBrowser>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include "InputWindow.h"
#include "TitreInputWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_manager_window->window=this;

    //home_window&&
    QWidget* home_window=new QWidget();
    QHBoxLayout* layout_home_window = new QHBoxLayout();
    home_window->setLayout(layout_home_window);

    QPushButton* ouvrir_fichier=new QPushButton("ouvrir un fichier");
    QPushButton* nouveau_fichier=new QPushButton("créer fichier");
    ouvrir_fichier->connect(ouvrir_fichier, SIGNAL(clicked()),this, SLOT(ouvrir_explorateur()));
    nouveau_fichier->connect(nouveau_fichier, SIGNAL(clicked()),this, SLOT(generate_file()));

    QMenuBar *menuBar = this->menuBar();
    QMenu *menu = menuBar->addMenu("Menu");

    QAction *actionItem1 = new QAction("Ouvrir un fichier", this);
    QAction *actionItem2 = new QAction("Créer un fichier", this);
    QAction *actionItem3 = new QAction("Sauvegarder", this);
    QAction *actionItem4 = new QAction("Sauvegarder sous", this);

    connect(actionItem1, &QAction::triggered, [=]() {
        ouvrir_explorateur();
    });

    connect(actionItem2, &QAction::triggered, [=]() {
        generate_file();
    });

    connect(actionItem3, &QAction::triggered, [=]() {
        save();
    });

    connect(actionItem4, &QAction::triggered, [=]() {
        get_file_path(false);
    });

    menu->addAction(actionItem1);
    menu->addAction(actionItem2);
    menu->addAction(actionItem3);
    menu->addAction(actionItem4);

    m_manager_window->add_window(home_window,"home_window");

    //window_edit
    QWidget* edit_window=new QWidget();
    QHBoxLayout* layout_edit_window = new QHBoxLayout();
    edit_window->setLayout(layout_edit_window);

    QVBoxLayout* layout_buttons = new QVBoxLayout();
    QWidget* buttons=new QWidget();
    buttons->setLayout(layout_buttons);
    layout_edit_window->addWidget(buttons);

    QHBoxLayout* layout_text_edit = new QHBoxLayout();
    QWidget* text_edit=new QWidget();
    text_edit->setLayout(layout_text_edit);
    layout_edit_window->addWidget(text_edit);

    TextEdit=new QTextEdit();
    layout_text_edit->addWidget(TextEdit);
    connect(TextEdit,SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(TextEdit,SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
    connect(TextEdit, SIGNAL(selectionChanged()), this, SLOT(clic_text_edit()));

    QPushButton* switch_preview_button=new QPushButton("Changer format");
    layout_buttons->addWidget(switch_preview_button);
    switch_preview_button->connect(switch_preview_button, SIGNAL(clicked()),this, SLOT(switch_preview()));

    QPushButton* add_puce_button=new QPushButton("puce");
    layout_buttons->addWidget(add_puce_button);
    add_puce_button->connect(add_puce_button, SIGNAL(clicked()),this, SLOT(add_puce()));

    QPushButton* add_image_button=new QPushButton("add_image");
    layout_buttons->addWidget(add_image_button);
    add_image_button->connect(add_image_button, SIGNAL(clicked()),this, SLOT(add_image()));

    QPushButton* add_video_button=new QPushButton("add_video");
    layout_buttons->addWidget(add_video_button);
    add_video_button->connect(add_video_button, SIGNAL(clicked()),this, SLOT(add_video()));

    QComboBox* comboBox = new QComboBox();
    comboBox->addItem("Normal");
    comboBox->addItem("Bold");
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeTextFormat(int)));
    layout_buttons->addWidget(comboBox);

    QComboBox* comboBox2 = new QComboBox();
    comboBox2->addItem("p");
    comboBox2->addItem("H1");
    comboBox2->addItem("H2");
    connect(comboBox2, SIGNAL(currentIndexChanged(int)), this, SLOT(changeTextWidth(int)));
    layout_buttons->addWidget(comboBox2);

    QCheckBox *toggleButton = new QCheckBox("Italic");
    QObject::connect(toggleButton, SIGNAL(toggled(bool)), this, SLOT(italic_switch(bool)));
    layout_buttons->addWidget(toggleButton);

    m_manager_window->add_window(edit_window,"edit_window");

    image_input=new ImageInputWindow(m_manager_window);
    m_manager_window->add_window(image_input->m_window,"image_input");
    QObject::connect(image_input, &ImageInputWindow::update,this,&MainWindow::end_dialogue);

    title_input=new TitreInputWindow(m_manager_window);
    m_manager_window->add_window(title_input->m_window,"title_input");
    QObject::connect(title_input, &TitreInputWindow::update,this,&MainWindow::end_dialogue);

    video_input=new VideoInputWindow(m_manager_window);
    m_manager_window->add_window(video_input->m_window,"video_input");
    QObject::connect(video_input, &VideoInputWindow::update,this,&MainWindow::end_dialogue);

    m_manager_window->open_window("home_window");
}

void MainWindow::ouvrir_explorateur()
{
    QFileDialog* file_dialog=new QFileDialog();
    file_dialog->setNameFilter("Markdown Files (*.md)");
    QString file_path;
    if (file_dialog->exec() == QDialog::Accepted) {
        file_path = file_dialog->selectedFiles().first();
        qDebug() << "Selected Markdown file:" << file_path;
        open_file(file_path,false);
    }
}

void MainWindow::open_file(QString file_path,bool first_time)
{
    m_reader->file_path=file_path;
    m_writer->file_path=file_path;
    m_reader->read_all();
    changeTextWidth(0);
    str_content=m_reader->content;
    format_md=false;
    this->update_text_edit();
    if(first_time)
    {
        m_manager_window->open_window("title_input");
    }
    else
    {
        m_manager_window->open_window("edit_window");
    }
    is_save=true;
}

void MainWindow::get_file_path(bool to_create)
{
    QString file_path = QFileDialog::getSaveFileName(this, tr("Creer le fichier Markdown"), "", tr("Fichiers Markdown (*.md);;Tous les fichiers (*)"));
    if (file_path.isEmpty()) {
        return;
    }
    if(to_create)
    {
        m_writer->file_path=file_path;
        m_writer->write_content("");
        open_file(file_path,true);
        current_input_window=title_input;
        m_manager_window->open_window("title_input");
    }
    else
    {
        m_writer->write_content(str_content);
        open_file(file_path,false);
    }
}

void MainWindow::cursorPositionChanged()
{
    //qDebug()<<"cursor pos change";
    if(user_input==false)
    {
        return;
    }
    return;//enelver
    QTextCursor cursor = TextEdit->textCursor();
    int decal=search_caracteres_speciaux(-1,cursor.position(),false);
    special_to_add=new QPair<int,specials>();
    special_to_add->first=cursor.position()+decal;
    if(current_special!=NULL)
    {
        special_to_add->second=current_special;
    }
    else
    {
        special_to_add=NULL;
    }
    //qDebug()<<decal;
}

void MainWindow::clic_text_edit()
{
    if(format_md)
    {
        return;
    }
    QTextCursor cursor = TextEdit->textCursor();
    QString selected_text=cursor.selectedText();
    if(selected_text=="image")
    {
        //current_input_window=title_input;
        m_manager_window->open_window("image_input");
    }
    if(selected_text=="titre")
    {
        current_input_window=title_input;
        m_manager_window->open_window("title_input");
    }
    if(selected_text=="video")
    {
        current_input_window=video_input;
        m_manager_window->open_window("video_input");
    }
}

void MainWindow::end_dialogue()
{
    if(current_input_window!=NULL)
    {
        qDebug()<<"input window not null";
        if(current_input_window->is_ready)
        {
            if (typeid(*current_input_window) == typeid(TitreInputWindow)) {
                TitreInputWindow* titre_input_window = dynamic_cast<TitreInputWindow*>(current_input_window);
                user_input=false;
                QTextCursor cursor = TextEdit->textCursor();
                int decal=search_caracteres_speciaux(-1,cursor.position(),true);
                int pos=cursor.position()+decal;
                //str_content.insert(pos,titre_input_window->m_texte);
                str_content.replace(0,end_balise(3,"+++")+3,titre_input_window->m_texte);
                //cursor.insertText(titre_input_window->m_texte);
                //nb_caracteres_speciaux+=current_input_window->m_texte.size();
                qDebug()<<"input window"<<titre_input_window->m_texte;
                //qDebug()<<"fin titre :"<<end_balise(0,"+++");
                current_input_window=NULL;
            }
            else if(typeid(*current_input_window) == typeid(ImageInputWindow)) {
                ImageInputWindow* image_input_window = dynamic_cast<ImageInputWindow*>(current_input_window);
                user_input=false;
                QTextCursor cursor = TextEdit->textCursor();
                int decal=search_caracteres_speciaux(-1,cursor.position(),true);
                int pos=cursor.position()+decal;
                str_content.insert(pos,image_input_window->m_texte);
                //str_content.replace(0,end_balise(3,"+++")+3,titre_input_window->m_texte);
                //cursor.insertText(titre_input_window->m_texte);
                //nb_caracteres_speciaux+=current_input_window->m_texte.size();
                qDebug()<<"input window"<<image_input_window->m_texte;
                //qDebug()<<"fin titre :"<<end_balise(0,"+++");
                current_input_window=NULL;
            }
            qDebug()<<"input window ready";
            update_text_edit();
        }
        else
        {
            qDebug()<<"input window not ready";
        }
    }
    else
    {
        qDebug()<<"input window null";
    }
}

void MainWindow::onTextChanged()
{
    //qDebug()<<"Text edit change";
    //()<<"________________";
    //()<<"changement : "<<user_input;
    //()<<"avant : "<<str_content;
    //()<<"________________";
    if(user_input==false)
    {
        user_input=true;
        return;
    }
    if(format_md)
    {
        qDebug()<<"input in md";
        format_md=false;
        update_text_edit();
        return;
    }
    qDebug()<<"format : "<<format_md;
    /*
    if(special_to_add!=NULL)
    {
        qDebug()<<"add special position : "<<special_to_add->first;
        switch(special_to_add->second)
        {
        case H1:
            str_content.insert(special_to_add->first,"#");
            break;
        case H2:
            str_content.insert(special_to_add->first,"##");
            break;
        }
    }
    */
    int nb_caracteres_speciaux=search_caracteres_speciaux(-1,TextEdit->textCursor().position(),true);
    int balises=0;
    if(current_special!=NO)
    {
        //qDebug()<<"add special position : "<<special_to_add->first;
        int position=TextEdit->textCursor().position()-1;
        if(position<0)
        {
            position=0;
        }
        position=position+search_caracteres_speciaux(-1,position,true);
        switch(current_special)
        {
        case H1:
            str_content.insert(position,"#");
            balises+=1;
            break;
        case H2:
            str_content.insert(position,"##");
            balises+=2;
            break;
        case BOLD:
            str_content.insert(position,"**");
            balises+=2;
            break;
        case END_BOLD:
            str_content.insert(position,"**");
            balises+=2;
            break;
        case ITALIC:
            str_content.insert(position,"_");
            balises+=1;
            break;
        case END_ITALIC:
            str_content.insert(position,"_");
            balises+=1;
            break;
        }
        current_special=NO;
        //qDebug()<<"md : "<<str_content;
    }
    qDebug()<<"nb caracteres speciaux : "<<nb_caracteres_speciaux;
    int all_caracteres_speciaux=search_caracteres_speciaux(-1,str_content.size(),true);
    is_save=false;
    int previous_taille=str_content.size();
    str_content_preview=TextEdit->toPlainText();
    //qDebug()<<"normal: "<<str_content_preview;
    //qDebug()<<"md : "<<str_content;
    int difference_taille=str_content_preview.length()-previous_taille;
    //difference_taille+=all_caracteres_speciaux;//utile?
    difference_taille+=balises+1;
    if(!format_md)
    {
     difference_taille+=all_moins;
    }
    qDebug()<<"difference_taille : "<<difference_taille;
    if (difference_taille<0 && user_input==true) {
        QTextCursor cursor = TextEdit->textCursor();
         int deb_s=TextEdit->textCursor().position();
         bool on=true;
         int cpt=0;
         int previous_nb=0;
         int nb_caracteres_speciaux_t=0;
         while(on)
         {
                nb_caracteres_speciaux_t=search_caracteres_speciaux(-1,deb_s,true);//enlever
                if(nb_caracteres_speciaux_t==previous_nb)
                {
                    on=false;
                }
                else
                {
                    deb_s+=nb_caracteres_speciaux_t;
                    qDebug()<<"cpt : "<<cpt<<" nb: "<<nb_caracteres_speciaux_t;
                    previous_nb=nb_caracteres_speciaux_t;
                }
         }
            if(difference_taille==-1)
        {
            int nb_caracteres_speciaux=search_caracteres_speciaux(-1,TextEdit->textCursor().position(),false);
            qDebug()<<"sup : "<<str_content[cursor.position()+nb_caracteres_speciaux-2];
            str_content.removeAt(cursor.position()+nb_caracteres_speciaux-2);
        }
        else
        {
            for(int i=0;i<-difference_taille;i++)
            {
                int nb_caracteres_speciaux=search_caracteres_speciaux(-1,TextEdit->textCursor().position(),false);
                str_content.removeAt(cursor.position()+nb_caracteres_speciaux-2);
            }
        }
        //qDebug()<<"apres supression: "<<str_content;
        qDebug()<<"suppresion";
    }
    else
    {
        QTextCursor cursor = TextEdit->textCursor();
        //cursor.movePosition(QTextCursor::End);
        int pos=cursor.position()-1;//
        if(pos<0)
        {
            pos=0;
        }
        qDebug()<<"position "<<pos;
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        QString lastChar = cursor.selectedText();
        //qDebug()<<"user input : "<<lastChar;
        lastChar.replace(QChar(0x2029), '\n');
        //qDebug()<<"user input : "<<lastChar;
        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor,1);
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        QString previous_char = cursor.selectedText();
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor,1);
        previous_char.replace(QChar(0x2029), '\n');
        if(user_input==true)
        {
            qDebug()<<"user input : "<<lastChar<<" previous : "<<previous_char;
            qDebug()<<"nb caracteres speciaux : "<<nb_caracteres_speciaux;
            //str_content.append(lastChar);
            if(lastChar==QChar('\x0A'))
            {
                qDebug()<<"un retour ligne";
                //pos+=1;
                if(previous_char==QChar('\x0A'))
                {
                    qDebug()<<"double retour ligne";
                    return;
                }
                //str_content.insert(pos+nb_caracteres_speciaux,QChar('\x0A'));
                //qDebug()<<"apres user input("<<lastChar<<"): "<<str_content;
                //return;
            }
            int deb=TextEdit->textCursor().position();
            bool on=true;
            int cpt=0;
            int previous_nb=0;
            int nb_caracteres_speciaux_t=0;
            while(on)
            {
                nb_caracteres_speciaux_t=search_caracteres_speciaux(-1,deb,true);//enlever
                if(nb_caracteres_speciaux_t==previous_nb)
                {
                    on=false;
                }
                else
                {
                    deb+=nb_caracteres_speciaux_t;
                    qDebug()<<"cpt : "<<cpt<<" nb: "<<nb_caracteres_speciaux_t;
                    previous_nb=nb_caracteres_speciaux_t;
                }
            }
            int nb_char_normal=nb_char_search(-1,TextEdit->textCursor().position()+nb_caracteres_speciaux_t);
            //str_content.insert(pos+nb_caracteres_speciaux,lastChar);
            str_content.insert(nb_char_normal+nb_caracteres_speciaux_t+balises*2,lastChar);
            qDebug()<<"apres user input("<<lastChar<<"): "<<str_content<<" nb_caracteres_speciaux : "<<nb_caracteres_speciaux_t<<" nb_char : "<<nb_char_normal;
            user_input=true;
        }
    }
    //str_content=TextEdit->toPlainText();
}

void MainWindow::add_image()
{
    current_input_window=image_input;
    m_manager_window->open_window("image_input");
}

void MainWindow::add_video()
{
    current_input_window=video_input;
    m_manager_window->open_window("video_input");
}

void MainWindow::italic_switch(bool state)
{
    QTextCursor cursor = TextEdit->textCursor();
    QTextCharFormat charFormat = cursor.charFormat();
    if (state == true) {
        charFormat.setFontItalic(true);
        current_special=ITALIC;
    } else {
        charFormat.setFontItalic(false);
        current_special=END_ITALIC;
    }
    cursor.mergeCharFormat(charFormat);
    TextEdit->setTextCursor(cursor);
}

int MainWindow::nb_char_search(int direction,int position)
{
    int nb_carac=0;
    bool on_structure=false;
    if(position>str_content.size()-1)
    {
        position=str_content.size()-1;
    }
    qDebug()<<"search :"<<str_content.mid(0,position);
    for(int i=position;i>-1;i--)
    {
        QChar c=str_content[i];
        if(c==QChar('<'))
        {
            if(str_content.mid(i+1, 7)=="figure>")
            {
                on_structure=false;
            }
            else if(str_content.mid(i+1, 6)=="video>")
            {
                on_structure=false;
            }
            else if(str_content.mid(i+1, 8)=="/figure>")
            {
                on_structure=true;
                if(on_structure)
                {
                    nb_carac-=9;
                }
            }
            else if(str_content.mid(i+1, 7)=="/video>")
            {
                on_structure=true;
                if(on_structure)
                {
                    nb_carac-=8;
                }
            }
        }
        if(c==QChar('+'))
        {
            if(str_content.mid(i+1, 2)=="++")
            {
                on_structure=!on_structure;
                if(on_structure)
                {
                    nb_carac-=3;
                }
            }
        }
        if(c!=QChar('#') && c!=QChar('*') && c!=QChar('_') && c!=QChar('[') && c!=QChar(']') && on_structure==false)
        {
            nb_carac+=1;
        }
    }
    if(on_structure)
    {
       qDebug()<<"struc non finie";
       return 0;
    }
    return nb_carac;
}

int MainWindow::search_caracteres_speciaux(int direction,int position,bool add_incomplete) //-1 gauche
{
    int nb_carac_s=0;
    bool title=false;
    bool image=false;
    bool video=false;
    if(position>str_content.size()-1)
    {
        position=str_content.size()-1;
    }
    for(int i=position;i>-1;i--)
    {
        QChar c=str_content[i];
        //qDebug()<<c;
        if(c==QChar('#') || c==QChar('*') || c==QChar('_') || c==QChar('[') || c==QChar(']'))
        {
            nb_carac_s+=1;
        }
        if(c==QChar('<'))
        {
            if(str_content.mid(i+1, 7)=="figure>")
            {
                int fin_image=end_balise(i+1,"/figure>");
                qDebug()<<"fin image :"<<fin_image;
                if(add_incomplete)
                {
                    nb_carac_s+=fin_image;
                }
                else
                {
                    nb_carac_s+=fin_image;//
                }
                image=!image;
            }
            else if(str_content.mid(i+1, 6)=="video>")
            {
                int fin_titre=end_balise(i+1,"/video>")+7;
                qDebug()<<"fin titre :"<<fin_titre;
                if(add_incomplete)
                {
                    nb_carac_s+=fin_titre-i;
                }
                else
                {
                    nb_carac_s+=fin_titre;//
                }
                video=!video;
            }
            else if(str_content.mid(i+1, 8)=="/figure>")
            {
                image=!image;
            }
            else if(str_content.mid(i+1, 7)=="/video>")
            {
                video=!video;
            }
        }
        if(c==QChar('+'))
        {
            if(str_content.mid(i+1, 2)=="++")
            {
                if(title)
                {
                    int fin_titre=end_balise(i+3,"+++")+3;
                    qDebug()<<"fin titre :"<<fin_titre;
                    if(add_incomplete)
                    {
                        nb_carac_s+=fin_titre-i;
                    }
                    else
                    {
                        nb_carac_s+=fin_titre;//
                    }
                }
                title=!title;
            }
        }
        if(c==QChar('\x0A'))
        {

        }
    }
    if(title)
    {
        int fin_titre=end_balise(position,"+++")+3;
        qDebug()<<"fin titre :"<<fin_titre;
        if(add_incomplete)
        {
            nb_carac_s+=fin_titre-nb_carac_s;
        }
        else
        {
            nb_carac_s+=fin_titre;//
        }
    }
    if(video)
    {
        int fin_video=end_balise(position,"</video>")+7;
        qDebug()<<"fin video :"<<fin_video;
        if(add_incomplete)
        {
            nb_carac_s+=fin_video-nb_carac_s;
        }
        else
        {
            nb_carac_s+=fin_video;//
        }
    }
    if(image)
    {
        qDebug("image non finie");
        int fin_image=end_balise(0,"</figure>");//position?
        qDebug()<<"fin image :"<<fin_image;
        if(add_incomplete)
        {
            nb_carac_s+=fin_image-nb_carac_s;
        }
        else
        {
            nb_carac_s+=fin_image;//
        }
    }
    return nb_carac_s;
}

void MainWindow::add_puce()
{
    QTextCursor cursor = TextEdit->textCursor();
    QTextCharFormat charFormat = cursor.charFormat();
    cursor.mergeCharFormat(charFormat);
    TextEdit->setTextCursor(cursor);
    str_content.insert(cursor.position(),"*");
}

void MainWindow::switch_preview()
{
    format_md=!format_md;
    update_text_edit();
}

void MainWindow::save()
{
    m_writer->write_content(str_content);
}

void MainWindow::changeTextFormat(int index) {
    QTextCursor cursor = TextEdit->textCursor();
    QTextCharFormat charFormat = cursor.charFormat();
    switch (index) {
    case 0:
        charFormat.setFontWeight(QFont::Normal);
        current_special=END_BOLD;
        break;
    case 1:
        charFormat.setFontWeight(QFont::Bold);
        current_special=BOLD;
        break;
    default:
        break;
    }
    cursor.mergeCharFormat(charFormat);
    TextEdit->setTextCursor(cursor);
}

void MainWindow::changeTextWidth(int index) {
    QTextCursor cursor = TextEdit->textCursor();
    QTextCharFormat charFormat = cursor.charFormat();
    switch (index) {
    case 0:
        charFormat.setFontWeight(QFont::Normal);
        charFormat.setFontPointSize(9);
        break;
    case 1:
        charFormat.setFontWeight(QFont::Bold);
        charFormat.setFontPointSize(16);
        current_special=H1;
        break;
    case 2:
        charFormat.setFontWeight(QFont::Bold);
        charFormat.setFontPointSize(12);
        current_special=H2;
        break;
    default:
        break;
    }
    cursor.mergeCharFormat(charFormat);
    TextEdit->setTextCursor(cursor);
}

void MainWindow::generate_file()
{
    TextEdit->clear();
    str_content="";
    is_save=true;
    get_file_path(true);
}

void MainWindow::update_text_edit()
{
    user_input=false;
    m_reader->content=str_content;
    std::vector<QPair<specials,int>*>* caracteres_speciaux=new std::vector<QPair<specials,int>*>;
    TextEdit->clear();
    str_content_preview="";
    QTextCursor cursor = TextEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    QTextCharFormat defaultFormat;
    cursor.mergeCharFormat(defaultFormat);
    TextEdit->setCurrentCharFormat(defaultFormat);
    bool previous_diez=false;
    bool previous_italic=false;
    bool previous_bold=false;
    bool previous_lien=false;
    bool on_lien=false;
    bool on_image=false;
    bool on_title=false;
    for (int i = 0; i < m_reader->content.length(); ++i) {
        bool is_insert=true;
        QChar currentChar = m_reader->content.at(i);
        if (currentChar == QChar('#')) {
            caracteres_speciaux->push_back(new QPair<specials,int>);
            if(!format_md)
            {
                is_insert=false;
            }
            QChar nextChar=QChar(' ');
            if(i+1<m_reader->content.length())
            {
                nextChar = m_reader->content.at(i+1);
            }
            if(nextChar==QChar('#'))
            {
                caracteres_speciaux->back()->second=i;
                caracteres_speciaux->back()->first=H2;
            }
            else if(previous_diez==false)
            {
                caracteres_speciaux->back()->first=H1;
                caracteres_speciaux->back()->second=i;
            }
            previous_diez=true;
        }
        else
        {
            previous_diez=false;
        }
        if(currentChar==QChar('\xa'))
        {


        }
        if(currentChar==QChar('_'))
        {
            if(!format_md)
            {
                is_insert=false;
            }
            if(previous_italic==false)
            {
                caracteres_speciaux->push_back(new QPair<specials,int>);
                caracteres_speciaux->back()->second=i;
                caracteres_speciaux->back()->first=ITALIC;
            }
            else
            {
                caracteres_speciaux->push_back(new QPair<specials,int>);
                caracteres_speciaux->back()->second=i;
                caracteres_speciaux->back()->first=END_ITALIC;
            }
            previous_italic=!previous_italic;
        }
        if(currentChar==QChar('*'))
        {
            if(!format_md)
            {
                is_insert=false;
            }
            QChar nextChar=QChar(' ');
            if(i+1<m_reader->content.length())
            {
                nextChar = m_reader->content.at(i+1);
            }
            if(nextChar==QChar('*'))
            {
                if(previous_bold==false)
                {
                    caracteres_speciaux->push_back(new QPair<specials,int>);
                    caracteres_speciaux->back()->second=i;
                    caracteres_speciaux->back()->first=BOLD;
                }
                else
                {
                    caracteres_speciaux->push_back(new QPair<specials,int>);
                    caracteres_speciaux->back()->second=i+2;
                    caracteres_speciaux->back()->first=END_BOLD;
                }
                previous_bold=!previous_bold;
            }
        }
        if(currentChar==QChar('['))
        {
            if(!format_md)
            {
                is_insert=false;
            }
            caracteres_speciaux->push_back(new QPair<specials,int>);
            caracteres_speciaux->back()->second=i;
            caracteres_speciaux->back()->first=LIEN;
        }
        if(currentChar==QChar(']'))
        {
            if(!format_md)
            {
                is_insert=false;
            }
            on_lien=true;
            //caracteres_speciaux->push_back(new QPair<specials,int>);
            //caracteres_speciaux->back()->second=i+1;
            //caracteres_speciaux->back()->first=END_LIEN;
        }
        if(currentChar==QChar('('))
        {
            if(!format_md)
            {
                //currentChar=' ';
                //is_insert=false;
            }
            //caracteres_speciaux->push_back(new QPair<specials,int>);
            //caracteres_speciaux->back()->second=i;
            //caracteres_speciaux->back()->first=LIEN;
        }
        if(currentChar==QChar(')'))
        {
            if(on_lien)
            {
                if(!format_md)
                {
                    //is_insert=false;
                }
                caracteres_speciaux->push_back(new QPair<specials,int>);
                caracteres_speciaux->back()->second=i+1;
                caracteres_speciaux->back()->first=END_LIEN;
                on_lien=false;
            }
        }
        if(currentChar==QChar('<'))
        {
            if(m_reader->content.mid(i+1, 7)=="figure>")
            {
                caracteres_speciaux->push_back(new QPair<specials,int>);
                caracteres_speciaux->back()->second=i;
                caracteres_speciaux->back()->first=IMAGE;
                on_image=true;
            }
            else if(m_reader->content.mid(i+1, 8)=="/figure>")
            {
                caracteres_speciaux->push_back(new QPair<specials,int>);
                caracteres_speciaux->back()->second=i+9;
                caracteres_speciaux->back()->first=END_IMAGE;
                on_image=false;
            }
            else if(m_reader->content.mid(i+1, 5)=="video")
            {
                caracteres_speciaux->push_back(new QPair<specials,int>);
                caracteres_speciaux->back()->second=i;
                caracteres_speciaux->back()->first=VIDEO;
                on_image=true;
            }
            else if(m_reader->content.mid(i+1, 7)=="/video>")
            {
                caracteres_speciaux->push_back(new QPair<specials,int>);
                caracteres_speciaux->back()->second=i+8;
                caracteres_speciaux->back()->first=END_VIDEO;
                on_image=false;
            }
        }
        if(currentChar==QChar('+'))
        {
            if(m_reader->content.mid(i+1, 2)=="++")
            {
                on_title=!on_title;
                if(on_title)
                {
                    caracteres_speciaux->push_back(new QPair<specials,int>);
                    caracteres_speciaux->back()->second=i;
                    caracteres_speciaux->back()->first=TITLE;
                }
                else
                {
                    caracteres_speciaux->push_back(new QPair<specials,int>);
                    caracteres_speciaux->back()->second=i+3;
                    caracteres_speciaux->back()->first=END_TITLE;
                }
            }
        }
        if(is_insert)
        {
            str_content_preview.append(currentChar);
        }
    }
    user_input=false;
    TextEdit->setPlainText(str_content_preview);
    bool is_italic=false;
    bool is_bold=false;
    int start_italic=0;
    int start_bold=0;
    int start_lien=0;
    int start_image=0;
    int start_video=0;
    int start_title=0;
    all_moins=0;
    for(int i=0;i<caracteres_speciaux->size();i++)
    {
        user_input=false;
        QTextCharFormat police;
        int size=10;
        int position_start=caracteres_speciaux->at(i)->second;
        int nb_carac=0;
        switch (caracteres_speciaux->at(i)->first) {
        case H1:
            size=16;
            all_moins+=1;
            is_bold=true;
            police=create_police(size,is_bold,is_italic,Qt::black,false);
            change_text_format(position_start-all_moins,-1,police);
            is_bold=false;
            size=10;
            break;
        case H2:
            size=12;
            all_moins+=2;
            is_bold=true;
            police=create_police(size,is_bold,is_italic,Qt::black,false);
            change_text_format(position_start,-1,police);
            is_bold=false;
            size=10;
            break;
        case ITALIC:
            all_moins+=1;
            is_italic=true;
            start_italic=caracteres_speciaux->at(i)->second;
            break;
        case END_ITALIC:
            police=create_police(size,is_bold,is_italic,Qt::black,false);
            if(format_md)
            {
                change_text_format(start_italic,caracteres_speciaux->at(i)->second-start_italic,police);
            }
            else
            {
                position_start=start_italic-all_moins;
                if(position_start<0)
                {
                    position_start=0;
                }
                change_text_format(position_start,(caracteres_speciaux->at(i)->second-position_start)-all_moins,police);
            }
            all_moins+=1;
            is_italic=false;
            break;
        case BOLD:
            all_moins+=2;
            is_bold=true;
            start_bold=caracteres_speciaux->at(i)->second;
            break;
        case END_BOLD:
            police=create_police(size,is_bold,is_italic,Qt::black,false);
            if(format_md)
            {
                change_text_format(start_bold,caracteres_speciaux->at(i)->second-start_bold,police);
            }
            else
            {
                position_start=start_bold-all_moins;
                if(position_start<0)
                {
                    position_start=0;
                }
                change_text_format(position_start,(caracteres_speciaux->at(i)->second-position_start)-all_moins,police);
            }
            all_moins+=2;
            is_bold=false;
            break;
        case LIEN:
            all_moins+=1;
            start_lien=caracteres_speciaux->at(i)->second;
            break;
        case END_LIEN:
            police=create_police(size,is_bold,is_italic,Qt::blue,true);
            if(format_md)
            {
                change_text_format(start_lien,caracteres_speciaux->at(i)->second-start_lien,police);
            }
            else
            {
                position_start=start_lien-all_moins;
                if(position_start<0)
                {
                    position_start=0;
                }
                change_text_format(position_start,(caracteres_speciaux->at(i)->second-position_start)-all_moins,police);
            }
            all_moins+=1;
            break;
        case IMAGE:
            start_image=caracteres_speciaux->at(i)->second;
            break;
        case END_IMAGE:
            police=create_police(size,is_bold,is_italic,QColor(0,179,0),false);
            if(format_md)
            {
                change_text_format(start_image,caracteres_speciaux->at(i)->second-start_image,police);
            }
            else
            {
                position_start=start_image-all_moins;
                if(position_start<0)
                {
                    position_start=0;
                }
                cursor.setPosition(position_start);
                cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,(caracteres_speciaux->at(i)->second-position_start)-all_moins);
                QString selected_text=cursor.selectedText();
                all_moins+=selected_text.length()-5;
                user_input=false;
                cursor.insertText("image");
                user_input=false;
                change_text_format(position_start,6,police);
            }
            break;
        case VIDEO:
            start_video=caracteres_speciaux->at(i)->second;
            break;
        case END_VIDEO:
            police=create_police(size,is_bold,is_italic,QColor(128,0,128),false);
            if(format_md)
            {
                change_text_format(start_video,caracteres_speciaux->at(i)->second-start_video,police);
            }
            else
            {
                position_start=start_video-all_moins;
                if(position_start<0)
                {
                    position_start=0;
                }
                cursor.setPosition(position_start);
                cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,(caracteres_speciaux->at(i)->second-position_start)-all_moins);
                QString selected_text=cursor.selectedText();
                all_moins+=selected_text.length()-5;
                user_input=false;
                cursor.insertText("video");
                user_input=false;
                change_text_format(position_start,5,police);
            }
            break;
        case TITLE:
            start_title=caracteres_speciaux->at(i)->second;
            break;
        case END_TITLE:
            police=create_police(size,is_bold,is_italic,QColor(255,69,0),false);
            if(format_md)
            {
                change_text_format(start_title,caracteres_speciaux->at(i)->second-start_title,police);
            }
            else
            {
                position_start=start_title-all_moins;
                if(position_start<0)
                {
                    position_start=0;
                }
                cursor.setPosition(position_start);
                cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,(caracteres_speciaux->at(i)->second-position_start)-all_moins);
                QString selected_text=cursor.selectedText();
                all_moins+=selected_text.length()-5;
                user_input=false;
                cursor.insertText("titre");
                user_input=false;
                change_text_format(position_start,5,police);
            }
            break;
        default:
            break;
        }
    }
    QTextCharFormat police=create_police(10,false,false,Qt::blue,false);
    cursor.mergeCharFormat(police);
}

QTextCharFormat MainWindow::create_police(int size,bool is_bold,bool is_italic,QColor color,bool is_soulign)
{
    QTextCharFormat format;
    format.setFontPointSize(size);
    format.setFontItalic(is_italic);
    if(is_bold)
    {
        format.setFontWeight(QFont::Bold);
    }
    else
    {
        format.setFontWeight(QFont::Normal);
    }
    if(is_soulign)
    {
        format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        format.setUnderlineColor(color);
    }
    format.setForeground(color);
    return format;
}

int MainWindow::end_balise(int position_start,QString balise)
{
    QString next_text=str_content.mid(position_start);
    //next_text.replace("\n", " ");
    //qDebug()<<next_text;
    int position_end = next_text.indexOf(balise);
    if(position_end==-1)
    {
        return 0;
    }
    position_end+=balise.length();//?
    return position_end;
}

void MainWindow::change_text_format(int start,int decal,QTextCharFormat format)
{
    QTextCursor cursor = TextEdit->textCursor();
    cursor.setPosition(start);
    if(decal==-1)
    {
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    }
    else
    {
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, decal);
    }
    cursor.mergeCharFormat(format);
}

MainWindow::~MainWindow()
{
    delete ui;
}
