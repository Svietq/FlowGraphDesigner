#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dragwidget.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->addItem(new DragWidget(nullptr, DragWidget::Type::MenuComputational), "Computational");
    ui->tabWidget->addItem(new DragWidget(nullptr, DragWidget::Type::MenuJoinSplit), "Join/split");
    ui->tabWidget->addItem(new DragWidget(nullptr, DragWidget::Type::MenuBuffer), "Buffer");
    ui->frame->hide();
    QObject::connect( this->ui->canvas, &DragWidget::close_dock_widget, [=](){ ui->frame->hide(); } );
    QObject::connect( this, &MainWindow::connect_button_toggled, [this](){ this->ui->canvas->is_connecting = true; });
    QObject::connect( this, &MainWindow::connect_button_not_toggled, [this](){ this->ui->canvas->is_connecting = false; });
    QObject::connect( this, &MainWindow::disconnect_button_toggled, [this](){ this->ui->canvas->is_disconnecting = true; });
    QObject::connect( this, &MainWindow::disconnect_button_not_toggled, [this](){ this->ui->canvas->is_disconnecting = false; });
    QObject::connect( &pop_up, &FileManager::ok_button_pressed, [this](){ ui->canvas->generate_code(pop_up.get_name(), pop_up.get_dir()); } );
    auto button = static_cast<QPushButton*>(ui->frame->children()[2]);
    QObject::connect( button, &QPushButton::clicked, this, &MainWindow::set_function );
    QObject::connect( button, &QPushButton::clicked, this, &MainWindow::set_types );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_dock_widget_contents(std::size_t id,
                                          const QString & function,
                                          const QString & input_type,
                                          const QString & output_type)
{
    auto id_label = static_cast<QLabel*>(ui->frame->children()[1]);
    id_label->setText( "     Node id:   " + QString::number(id));

    auto function_line = static_cast<QLineEdit*>(ui->frame->children()[8]);
    function_line->setText(function);

    auto input_line = static_cast<QLineEdit*>(ui->frame->children()[4]);
    input_line->setText(input_type);

    auto output_line = static_cast<QLineEdit*>(ui->frame->children()[6]);
    output_line->setText(output_type);

}

void MainWindow::show_window()
{
    ui->frame->show();
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

void MainWindow::set_function()
{
    auto function_line = static_cast<QLineEdit*>(ui->frame->children()[8]);
    ui->canvas->current_node->function = function_line->text();
}

void MainWindow::set_types()
{
    auto input_line = static_cast<QLineEdit*>(ui->frame->children()[4]);
    ui->canvas->current_node->input_type = input_line->text();

    auto output_line = static_cast<QLineEdit*>(ui->frame->children()[6]);
    ui->canvas->current_node->output_type = output_line->text();
}



















