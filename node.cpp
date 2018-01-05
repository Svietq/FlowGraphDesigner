#include "node.h"
#include "dragwidget.h"
#include "mainwindow.h"
#include "sourcenode.h"
#include "continuousnode.h"
#include "functionnode.h"
#include "reservingjoinnode.h"
#include "splitnode.h"
#include "ui_mainwindow.h"
#include <QDebug>

Node::Node(QWidget *parent, const QPoint &p, unsigned int n) : QLabel{parent}, id{n}
{
    ports_in.push_back(Port{this, 0});
    ports_out.push_back(Port{this, 0});
    current_port_in = &ports_in[0];
    current_port_out = &ports_out[0];
    move(p);
    const auto &top = static_cast<MainWindow*>(this->window());
    QObject::connect( this, &Node::node_double_clicked, top, &MainWindow::show_window );
    QObject::connect(top->ui->canvas, &DragWidget::deleted_line, this, &SplitNode::set_ports);
}

Node *Node::create(Node::Type type, QWidget *parent, const QPoint &p, unsigned int n)
{
    switch (type) {
    case Type::Source:
        return new SourceNode{parent, p, n};
    case Type::Continuous:
        return new ContinuousNode{parent, p, n};
    case Type::Function:
        return new FunctionNode{parent, p, n};
    case Type::ReservingJoin:
        return new ReservingJoinNode{parent, p, n};
    case Type::Split:
        return new SplitNode{parent, p, n};
    }
    throw std::invalid_argument("The node does not exist");
}

Node *Node::create(Node::Type type, QWidget *parent, bool)
{
    switch (type) {
    case Type::Source:
        return new SourceNode{parent, true};
    case Type::Continuous:
        return new ContinuousNode{parent, true};
    case Type::Function:
        return new FunctionNode{parent, true};
    case Type::ReservingJoin:
        return new ReservingJoinNode{parent, true};
    case Type::Split:
        return new SplitNode{parent, true};
    }
    throw std::invalid_argument("The node does not exist");
}

void Node::set_point_in()
{
    current_port_in->pos = this->pos() + QPoint{ 0, pixmap()->height()/2 };
}

void Node::set_point_out()
{
    current_port_out->pos = this->pos() + QPoint{ pixmap()->width(), pixmap()->height()/2 };
}

void Node::move_node(const QPoint & pos)
{
    move(pos);
    set_point_in();
    set_point_out();
    raise();
}

bool Node::connect_port(Port *port)
{
    if(!port) return false;
    if(!connect_from_out(port)){ return false; }
    if(!port->node->connect_to_in(this->last_port_out)){ return false; }
    return true;
}

bool Node::connect_from_out(Port *port)
{
    if(current_port_out < ports_out.end())
    {
        current_port_out->connected_ports.push_back(port);
        return true;
    }
    else
    {
        return false;
    }
}

bool Node::connect_to_in(Port * port)
{
    if(current_port_in < ports_in.end())
    {
        current_port_in->connected_ports.push_back(port);
        return true;
    }
    else
    {
        return false;
    }
}

void Node::mouseDoubleClickEvent(QMouseEvent *)
{
    const auto &top = static_cast<MainWindow*>(this->window());
    top->set_node_id(id);
    emit node_double_clicked();
}

void Node::inc_last_port(Port * &last_port, QVector<Port> & ports)
{
    if(!last_port)
    {
        last_port = ports.begin();
    }
    else if(last_port < &ports.back())
    {
        last_port++;
    }
}

void Node::dec_last_port(Port * &last_port, QVector<Port> &ports)
{
    if(last_port)
    {
        if( last_port > ports.begin())
        {
            --last_port;
        }
        else if( last_port == ports.begin())
        {
            last_port = nullptr;
        }
    }
}

void Node::inc_curr_port(Port * &curr_port, QVector<Port> &ports)
{
    if(curr_port)
    {
        if( curr_port < &ports.back())
        {
            curr_port++;
        }
        else if( curr_port == &ports.back())
        {
            curr_port = nullptr;
        }
    }
}

void Node::dec_curr_port(Port *&curr_port, QVector<Port> &ports)
{
    if(!curr_port)
    {
        curr_port = &ports.back();
    }
    else if(curr_port > ports.begin())
    {
        curr_port--;
    }
}

void Node::set_ports()
{
    dec_curr_port(current_port_in, ports_in);
    dec_curr_port(current_port_out, ports_out);
    dec_last_port(last_port_in, ports_in);
    dec_last_port(last_port_out, ports_out);
}
