#include "Reader.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

Reader::Reader()
{

}

void Reader::read_all()
{
    QFile file(file_path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        // Read and print the content of the file
        content = in.readAll();
        qDebug() << "File content:" << content;
        file.close();
    } else {
        qDebug() << "Failed to open file for reading";
    }
}
