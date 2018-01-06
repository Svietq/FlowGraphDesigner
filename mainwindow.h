#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void set_node_id(std::size_t id);

    Ui::MainWindow *ui;

public slots:
    void show_window();

signals:
    void connect_button_toggled();
    void connect_button_not_toggled();
    void disconnect_button_toggled();
    void disconnect_button_not_toggled();

private slots:
    void on_toolButton_4_toggled(bool checked);
    void on_toolButton_3_toggled(bool checked);
    void on_toolButton_2_clicked();

private:
    FileManager pop_up{this};
    void set_pop_up();
};

#endif // MAINWINDOW_H
