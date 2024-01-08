#include "Writer.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

Writer::Writer()
{

}

void Writer::write_content(QString content_to_write)
{
    QFile file(file_path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Création d'un objet QTextStream associé au fichier
        QTextStream stream(&file);

        // Écriture dans le fichier
        stream << content_to_write.toUtf8();

                  // Fermeture du fichier
                  file.close();
    } else {
        // Gestion des erreurs si le fichier ne peut pas être ouvert en écriture
        qDebug() << "Erreur: Impossible d'ouvrir le fichier en écriture.";
    }
}

void Writer::create_file()
{
    if(file_path.isNull())
    {
        return;
    }
    QDir dossier(file_path);
    if (!dossier.exists()) {
        dossier.mkpath(".");
    }

    QFile file(file_path);
    if (file.open(QIODevice::WriteOnly)) {
        file.close();
    } else {
    }
}
