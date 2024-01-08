#ifndef Manager_window_H
#define Manager_window_H

#include <QMainWindow>

class Manager_window
{
public:
    Manager_window();
    QMainWindow *window;
    std::vector<QPair<std::string, QWidget*>> windows;
    void add_window(QWidget* new_window,std::string name);
    void open_window(std::string name);
    QWidget* get_window(std::string name);
    bool update_text_edit=false;
};

#endif // Manager_window_H
