#include "mainwindow.h"
#include <QApplication>
#include "dragwidget.h"
#include "QHBoxLayout"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;

    QHBoxLayout *horizontalLayout = new QHBoxLayout(&w);
    QTabWidget *tabWidget = new QTabWidget();

    tabWidget->addTab(new DragWidget(false), "First tab");

    horizontalLayout->addWidget(tabWidget);
    horizontalLayout->addWidget(new DragWidget(true, 900, 500));

    w.show();

    return a.exec();
}
