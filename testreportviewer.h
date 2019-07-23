#ifndef TESTREPORTVIEWER_H
#define TESTREPORTVIEWER_H

#include <QWidget>
#include <QAbstractItemView>
#include <QDebug>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtXml>
#include <QString>
#include <QFile>
#include <QFont>
#include <QColor>
#include <QStringList>
#include <QFileDialog>
#include <QAbstractItemModel>
#include "common.h"

namespace Ui {
class TestReportViewer;
}

class TestReportViewer : public QWidget
{
    Q_OBJECT

public:
    explicit TestReportViewer(QWidget* parent = 0);
    ~TestReportViewer();

private slots:
    void on_selectPushButton_clicked();

    void on_parsePushButton_clicked();

    void on_clearViewPushButton_clicked();

private:
    Ui::TestReportViewer* ui;
    QTreeWidgetItem* caseItem;
    QTreeWidgetItem* functionItem;
    QStringList list_files;

    void readReport(const QDomNode& node);
    void clearView();
};

#endif // TESTREPORTVIEWER_H
