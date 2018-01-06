#include "functionnode.h"

FunctionNode::FunctionNode(QWidget *parent, const QPoint &p, unsigned int n) : Node{parent, p, n}
{
    type = Type::Function;
    setPixmap(QPixmap{":/icons/function.png"});
    set_point_in();
    set_point_out();
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}

FunctionNode::FunctionNode(QWidget *parent, bool) :  Node{parent, QPixmap{":/icons/function.png"}, true}
{
    type = Type::Function;
    move(35, 230);
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}


bool FunctionNode::connect_port(Port * port)
{
    return Node::connect_port(port);
}

bool FunctionNode::connect_from_out(Port * port)
{
    return Node::connect_from_out(port);
}

bool FunctionNode::connect_to_in(Port * port)
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
