#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFileSystemModel>
#include <QTreeView>

class FileManager : public QDialog
{
private:
    QLineEdit * line_edit = new QLineEdit("Enter project name here...");
    QPushButton * ok_button = new QPushButton("OK");
    QTreeView * file_explorer = new QTreeView();
    QFileSystemModel * model = new QFileSystemModel(this);
    QString dir;
    QString name;

public:
    FileManager(QWidget * parent);

private slots:
    void set_dir(const QModelIndex & index);

};

#endif // FILEMANAGER_H
