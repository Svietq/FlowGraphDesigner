#include "node.h"
#include "dragwidget.h"
#include "mainwindow.h"

Node::Node(QWidget *parent, size_t n) : QLabel{parent}, id{n}
{
    const auto &top = static_cast<MainWindow*>(this->window());
    QObject::connect( this, &Node::node_clicked, top, &MainWindow::show_window );
}

Node::~Node()
{

}

void Node::mouseDoubleClickEvent(QMouseEvent *)
{
    const auto &top = static_cast<MainWindow*>(this->window());
    top->set_node_id(id);
    emit node_clicked();
}
