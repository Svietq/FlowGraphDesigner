#include "node.h"
#include "dragwidget.h"
#include "mainwindow.h"

Node::Node(QWidget *parent, const QPixmap & map, const QPoint &p, size_t n) : QLabel{parent}, id{n}
{
    setPixmap(map);
    move(p);
    const auto &top = static_cast<MainWindow*>(this->window());
    QObject::connect( this, &Node::node_double_clicked, top, &MainWindow::show_window );
    set_point_in();
    set_point_out();
}

Node::~Node()
{

}

void Node::set_point_in()
{
    point_in = this->pos() + QPoint{ 0, pixmap()->height()/2 };
}

void Node::set_point_out()
{
    point_out = this->pos() + QPoint{ pixmap()->width(), pixmap()->height()/2 };
}

void Node::mouseDoubleClickEvent(QMouseEvent *)
{
    const auto &top = static_cast<MainWindow*>(this->window());
    top->set_node_id(nodes_out.size());
    emit node_double_clicked();
}
