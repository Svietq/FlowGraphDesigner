#include "filemanager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>


FileManager::FileManager(QWidget *parent) : QDialog{parent}
{
    QHBoxLayout *horizontal_layout = new QHBoxLayout();
    QVBoxLayout *vertical_layout = new QVBoxLayout(this);

    horizontal_layout->addWidget(line_edit);
    horizontal_layout->addWidget(ok_button);
    vertical_layout->addWidget(file_explorer );
    vertical_layout->addLayout(horizontal_layout);
    this->setMinimumSize(500, 300);

    model->setRootPath("/home");
    model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    file_explorer->setModel(model);

}
