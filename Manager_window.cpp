#include "Manager_window.h"

Manager_window::Manager_window()
{

}

void Manager_window::add_window(QWidget* new_window,std::string name)
{
    QPair<std::string, QWidget*> new_pair;
    new_pair.first = name;
    new_pair.second = new_window;
    windows.push_back(new_pair);
}

void Manager_window::open_window(std::string name)
{
    for(QPair<std::string, QWidget*> &element : windows)
    {
        if(element.first==name)
        {
            window->centralWidget()->setParent(0);
            window->setCentralWidget(element.second);
            if(update_text_edit)
            {

            }
        }
    }
}

QWidget* Manager_window::get_window(std::string name)
{
    for(QPair<std::string, QWidget*> &element : windows)
    {
        if(element.first==name)
        {
            return element.second;
        }
    }
}
