#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void foo(){setWindowTitle("dziala");}

private:
    Ui::MainWindow *ui;

public slots:
    void show_window();
};

#endif // MAINWINDOW_H
