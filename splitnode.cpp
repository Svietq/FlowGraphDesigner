#include "splitnode.h"
#include <QDebug>
#include "mainwindow.h"
#include "dragwidget.h"
#include "ui_mainwindow.h"

SplitNode::SplitNode(QWidget *parent, const QPoint &p, unsigned int n) : Node{parent, p, n}
{
    ports_out.push_back(Port{this, 1});
    ports_out.push_back(Port{this, 2});
    ports_out.push_back(Port{this, 3});
    current_port_in = &ports_in[0];
    current_port_out = &ports_out[0];
    type = Type::Split;
    setPixmap(QPixmap{":/icons/split.png"});
    set_point_in();
    set_point_out();
    show();
    setAttribute(Qt::WA_DeleteOnClose);
    input_type = "flow::tuple< int, int >";
}

SplitNode::SplitNode(QWidget *parent, bool) :  Node{parent, QPixmap{":/icons/split.png"}, true}
{
    type = Type::Split;
    move(40, 130);
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}

void SplitNode::set_point_out()
{
    int h = pixmap()->height()/(no_of_ports+1);
    for(auto & port : ports_out)
    {
        port.pos = this->pos() + QPoint{ pixmap()->width(), h };
        h += pixmap()->height()/(no_of_ports+1);
    }
}

bool SplitNode::connect_port(Port *port)
{
    if(!port) return false;
    if(!connect_from_out(port)){ return false; }
    if(!port->node->connect_to_in(this->last_port_out)){ return false; }
    return true;
}

bool SplitNode::connect_from_out(Port *port)
{
    if(current_port_out < ports_out.end())
    {
        bool val = Node::connect_from_out(port);
        inc_curr_port(current_port_out, ports_out);
        inc_last_port(last_port_out, ports_out);
        return val;
    }
    else
    {
        port->connected_ports.pop_back();
        return false;
    }
}

bool SplitNode::connect_to_in(Port *port)
{
    return Node::connect_to_in(port);
}



