#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dragwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(new DragWidget(nullptr, DragWidget::Type::MenuComputational), "Computational");
    ui->tabWidget->addTab(new DragWidget(nullptr, DragWidget::Type::MenuJoinSplit), "Join/split");
    ui->dockWidget_2->hide();
    QObject::connect( this->ui->canvas, &DragWidget::close_dock_widget, [=](){ ui->dockWidget_2->hide(); } );
    QObject::connect( this, &MainWindow::connect_button_toggled, [this](){ this->ui->canvas->is_connecting = true; });
    QObject::connect( this, &MainWindow::connect_button_not_toggled, [this](){ this->ui->canvas->is_connecting = false; });
    QObject::connect( this, &MainWindow::disconnect_button_toggled, [this](){ this->ui->canvas->is_disconnecting = true; });
    QObject::connect( this, &MainWindow::disconnect_button_not_toggled, [this](){ this->ui->canvas->is_disconnecting = false; });
    QObject::connect( &pop_up, &FileManager::ok_button_pressed, [this](){ ui->canvas->generate_code(pop_up.get_name(), pop_up.get_dir()); } );

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
        ui->toolButton_3->setChecked(false);
        emit connect_button_toggled();
    }
    else
    {
        emit connect_button_not_toggled();
    }
}

void MainWindow::on_toolButton_3_toggled(bool checked)
{
    if(checked)
    {
        ui->toolButton_4->setChecked(false);
        emit disconnect_button_toggled();
    }
    else
    {
        emit disconnect_button_not_toggled();
    }
}

void MainWindow::on_toolButton_2_clicked()
{
    pop_up.show();
}



















