#include "testreportviewer.h"
#include "ui_testreportviewer.h"

TestReportViewer::TestReportViewer(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TestReportViewer)
{
    ui->setupUi(this);

    QStringList  lst;
    lst << "Name test" << "Result" << "Description";
    ui->treeWidget->setHeaderLabels(lst);

    clearView();
}

TestReportViewer::~TestReportViewer()
{
    delete ui;
}

void TestReportViewer::clearView()
{
    ui->treeWidget->setSortingEnabled(false);
    ui->treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->parsePushButton->setDisabled(true);
    ui->treeWidget->clear();
}

void TestReportViewer::readReport(const QDomNode& node)
{
    if(node.isElement())
    {
        QDomElement domElement = node.toElement();
        if(!domElement.isNull())
        {
            if(domElement.tagName() == "TestCase")
            {
                caseItem = new QTreeWidgetItem(ui->treeWidget);
                caseItem->setText(0, domElement.attribute("name", ""));
                QFont font;
                font.setBold(true);
                font.setUnderline(true);
                caseItem->setFont(0, font);
            }
            if(domElement.tagName() == "testcase")
            {
                functionItem = new QTreeWidgetItem(ui->treeWidget);
                QString className = domElement.attribute("classname", "");
                QString testName = domElement.attribute("name", "");
                QString numberLine = domElement.attribute("line", "");
                QString textResultTest = "Passed!";
                QString descriptionFail = "";
                QDomNode subDomNode = domElement.firstChild();
                QDomElement subDomElement = subDomNode.toElement();
                if(!subDomNode.isNull() && !subDomElement.isNull())
                {
                    if(subDomElement.tagName() == "failure")
                    {
                        numberLine = subDomElement.attribute("line", "");
                        descriptionFail = subDomElement.text();
                        textResultTest = "Failed!";
                    }
                }
                QString testPosition = className + "::" + testName + "()"
                        + ", line " + numberLine + ": ";
                QString message = testPosition + textResultTest + descriptionFail;
                functionItem->setText(0, testPosition);
                functionItem->setText(1, textResultTest);
                QFont font;
                font.setBold(true);
                functionItem->setFont(1, font);
                if(textResultTest == "Passed!")
                    functionItem->setTextColor(1, QColor(Qt::darkGreen));
                else
                    functionItem->setTextColor(1, QColor(Qt::red));
                functionItem->setText(2, descriptionFail);
            }
        }
    }

    QDomNode domNode = node.firstChild();
    while(!domNode.isNull())
    {
        if(domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull())
            {
                if(domElement.tagName() == "TestFunction")
                {
                    functionItem = new QTreeWidgetItem(caseItem);
                    functionItem->setText(0, domElement.attribute("name", "") + "()");
                    if(domElement.attribute("name", "") == "initTestCase"
                            || domElement.attribute("name", "") == "cleanupTestCase")
                    {
                        QPalette pal;
                        functionItem->setTextColor(0, QColor(Qt::darkGray));
                    }
                }
                else if(domElement.tagName() == "Incident")
                {
                    if(domElement.attribute("type", "") == "pass")
                    {
                        functionItem->setText(1, "Passed!");
                        QFont font;
                        font.setBold(true);
                        functionItem->setFont(1, font);
                        QPalette pal;
                        functionItem->setTextColor(1, QColor(Qt::darkGreen));
                    }
                    else
                    {
                        functionItem->setText(1, "Failed!");
                        QFont font;
                        font.setBold(true);
                        functionItem->setFont(1, font);
                        functionItem->setTextColor(1, QColor(Qt::red));
                    }
                }
                else if(domElement.tagName() == "Description")
                    functionItem->setText(2, domElement.text());
            }
        }
        readReport(domNode);
        domNode = domNode.nextSibling();
    }
}

void TestReportViewer::on_selectPushButton_clicked()
{
    list_files = QFileDialog::getOpenFileNames(0, "Select reports", "C:/", "*.xml");
    ui->treeWidget->clear();
    if(list_files.isEmpty())
    {
        caseItem = new QTreeWidgetItem(ui->treeWidget);
        caseItem->setText(0, "Nothing selected");
        ui->treeWidget->resizeColumnToContents(0);
    }
    else
    {
        QStringList list_names;
        foreach(QString path, list_files)
        {
            QString name = path.split("/").last();
            list_names << name;
        }
        caseItem = new QTreeWidgetItem(ui->treeWidget);
        caseItem->setText(0, "Selected files:");
        foreach(QString name, list_names)
        {
            caseItem = new QTreeWidgetItem(ui->treeWidget);
            caseItem->setText(0, name);
        }
        ui->treeWidget->resizeColumnToContents(0);
        ui->parsePushButton->setEnabled(true);
    }
}

void TestReportViewer::on_parsePushButton_clicked()
{
    ui->treeWidget->clear();
    foreach(QString pathToFile, list_files)
    {
        QDomDocument domDoc;
        QFile file(pathToFile);

        if(file.open(QIODevice::ReadOnly))
        {
            if(domDoc.setContent(&file))
            {
                QDomElement domElement = domDoc.documentElement();
                readReport(domElement);
            }
            file.close();
        }
        else
        {
            qDebug() << "Error reading file!";
            exit (1);
        }
    }

    ui->treeWidget->expandAll();
    QAbstractItemModel* model;
    model = ui->treeWidget->model();
    int cols = model->columnCount();
    for(int i = 0; i < cols; ++i)
        ui->treeWidget->resizeColumnToContents(i);
}

void TestReportViewer::on_clearViewPushButton_clicked()
{
    clearView();
}
