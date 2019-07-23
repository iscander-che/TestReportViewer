#include "testreportviewer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestReportViewer w;
    w.show();

    return a.exec();
}
