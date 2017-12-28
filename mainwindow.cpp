#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dragwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(new DragWidget(nullptr, DragWidget::Type::Menu), "First tab");
    ui->dockWidget_2->hide();
    QObject::connect( this->ui->canvas, &DragWidget::close_dock_widget, [=](){ui->dockWidget_2->hide();} );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_window()
{
    ui->dockWidget_2->show();
}
