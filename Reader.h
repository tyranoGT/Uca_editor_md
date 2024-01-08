#ifndef READER_H
#define READER_H

#include <QString>

class Reader
{
public:
    Reader();
    QString file_path;
    void read_all();
    QString content;
};

#endif // READER_H
