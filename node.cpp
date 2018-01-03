#include "node.h"
#include "dragwidget.h"
#include "mainwindow.h"
#include "sourcenode.h"
#include "continuousnode.h"
#include "functionnode.h"

Node::Node(QWidget *parent, const QPoint &p, unsigned int n) : QLabel{parent}, id{n}
{
    move(p);
    const auto &top = static_cast<MainWindow*>(this->window());
    QObject::connect( this, &Node::node_double_clicked, top, &MainWindow::show_window );
}

Node *Node::create(Node::Type type, QWidget *parent, const QPoint &p, unsigned int n)
{
    switch (type) {
    case Type::Source:
        return new SourceNode{parent, p, n};
        break;
    case Type::Continuous:
        return new ContinuousNode{parent, p, n};
        break;
    case Type::Function:
        return new FunctionNode{parent, p, n};
        break;
    }
    throw std::invalid_argument("The node does not exist");
}

Node *Node::create(Node::Type type, QWidget *parent, bool)
{
    switch (type) {
    case Type::Source:
        return new SourceNode{parent, true};
        break;
    case Type::Continuous:
        return new ContinuousNode{parent, true};
        break;
    case Type::Function:
        return new FunctionNode{parent, true};
        break;
    }
    throw std::invalid_argument("The node does not exist");
}

void Node::set_point_in()
{
    point_in = this->pos() + QPoint{ 0, pixmap()->height()/2 };
}

void Node::set_point_out()
{
    point_out = this->pos() + QPoint{ pixmap()->width(), pixmap()->height()/2 };
}

void Node::move_node(const QPoint & pos)
{
    move(pos);
    set_point_in();
    set_point_out();
}

bool Node::connect_node(Node *node)
{
    if(!connect_from_out(node)){ return false; }
    if(!node->connect_to_in(this)){ return false;}
    return true;
}

bool Node::connect_from_out(Node *node)
{
    nodes_out.push_back(node);
    return true;
}

bool Node::connect_to_in(Node *node)
{
    nodes_in.push_back(node);
    return true;
}

void Node::mouseDoubleClickEvent(QMouseEvent *)
{
    const auto &top = static_cast<MainWindow*>(this->window());
    top->set_node_id(id);
    emit node_double_clicked();
}
