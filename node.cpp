#include "node.h"
#include "dragwidget.h"
#include "mainwindow.h"

Node::Node(QWidget *parent) : QLabel{parent}
{
    const auto &top = static_cast<MainWindow*>(this->window());
    QObject::connect( this, &Node::node_clicked, top, &MainWindow::show_window );
}

void Node::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit node_clicked();
}
