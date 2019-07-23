#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QApplication>
#include <QFile>
#include <QDebug>

/** @brief Функция создания файла текстовой базы данных и проверки его существования.
 * @param nameFile - имя файла.
 */
QString setDBPath(QString& nameFile);

#endif // COMMON_H
