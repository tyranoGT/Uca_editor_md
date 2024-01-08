#ifndef WRITER_H
#define WRITER_H

#include <QString>

class Writer
{
public:
    Writer();
    QString file_path;
    void write_content(QString content_to_write);
    void create_file();
};

#endif // WRITER_H
