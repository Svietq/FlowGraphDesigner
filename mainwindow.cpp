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
    QObject::connect( this, &MainWindow::connect_button_toggled, this->ui->canvas, &DragWidget::start_connecting);
    QObject::connect( this, &MainWindow::connect_button_not_toggled, this->ui->canvas, &DragWidget::stop_connecting);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_node_id(std::size_t id)
{
    static_cast<QLabel*>(ui->dockWidgetContents_2->children()[0])->setText(QString::number(id));
}

void MainWindow::show_window()
{
    ui->dockWidget_2->show();
}

void MainWindow::on_toolButton_4_toggled(bool checked)
{
    if(checked)
    {
        emit connect_button_toggled();
    }
    else
    {
        emit connect_button_not_toggled();
    }
}
