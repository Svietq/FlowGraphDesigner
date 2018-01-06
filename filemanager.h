#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFileSystemModel>
#include <QTreeView>

class FileManager : public QDialog
{
    Q_OBJECT
private:
    QLineEdit * dir_line = new QLineEdit();
    QLineEdit * name_line = new QLineEdit("Enter project name here...");
    QPushButton * ok_button = new QPushButton("OK");
    QTreeView * file_explorer = new QTreeView();
    QFileSystemModel * model = new QFileSystemModel(this);
    QString dir;
    QString name;

public:
    FileManager(QWidget * parent);
    const QString & get_dir()  const { return dir; }
    const QString & get_name() const { return name; }

private slots:
    void set_dir(const QModelIndex & index);
    void exit_window();

signals:
    void ok_button_pressed();

};

#endif // FILEMANAGER_H
