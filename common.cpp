#include "common.h"

QString setDBPath(QString& nameFile)
{
    QString pathToExeFile;
    pathToExeFile = qApp->applicationDirPath();;

    pathToExeFile = pathToExeFile + "/" + nameFile;

    QFile file(pathToExeFile);
    if(file.exists())
    {
        qDebug() << "File is already create";
        return pathToExeFile;
    }
    else
    {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            file.close();
            qDebug() << "Creating file OK";
            return pathToExeFile;
        }
        else
        {
            qDebug() << "Error creating file!";
            exit (1);
        }
    }
}
