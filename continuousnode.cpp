#include "continuousnode.h"

ContinuousNode::ContinuousNode(QWidget *parent, const QPoint &p, unsigned int n) : Node{parent, p, n}
{
    type = Type::Continuous;
    setPixmap(QPixmap{":/icons/continue.png"});
    set_point_in();
    set_point_out();
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}

ContinuousNode::ContinuousNode(QWidget *parent, bool) :  Node{parent, QPixmap{":/icons/continue.png"}, true}
{
    type = Type::Continuous;
    move(10, 120);
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}


bool ContinuousNode::connect_port(Port * port)
{
    return Node::connect_port(port);
}

bool ContinuousNode::connect_from_out(Port * port)
{
    return Node::connect_from_out(port);
}

bool ContinuousNode::connect_to_in(Port * port)
{
    if(current_port_in->connected_ports.isEmpty())
    {
        return Node::connect_to_in(port);
    }
    else
    {
        port->connected_ports.pop_back();
        return false;
    }
}
