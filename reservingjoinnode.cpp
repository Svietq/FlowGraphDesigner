#include "reservingjoinnode.h"
#include <QDebug>
#include "mainwindow.h"
#include "dragwidget.h"
#include "ui_mainwindow.h"

ReservingJoinNode::ReservingJoinNode(QWidget *parent, const QPoint &p, unsigned int n) : Node{parent, p, n}
{
    ports_in.push_back(Port{this});
    ports_in.push_back(Port{this});
    ports_in.push_back(Port{this});
    current_port_in = &ports_in[0];
    current_port_out = &ports_out[0];
    type = Type::ReservingJoin;
    setPixmap(QPixmap{":/icons/reserving_join.png"});
    set_point_in();
    set_point_out();
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}

ReservingJoinNode::ReservingJoinNode(QWidget *parent, bool) :  Node{parent, QPixmap{":/icons/reserving_join.png"}, true}
{
    type = Type::ReservingJoin;
    move(35, 10);
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}

void ReservingJoinNode::set_point_in()
{
    int h = pixmap()->height()/(no_of_ports+1);
    for(auto & port : ports_in)
    {
        port.pos = this->pos() + QPoint{ 0, h };
        h += pixmap()->height()/(no_of_ports+1);
    }
}

void ReservingJoinNode::set_current_port_in()
{
    if(!current_port_in)
    {
        current_port_in = &ports_in.back();
    }
    else if(current_port_in > &ports_in[0] )
    {
        --current_port_in;
    }
}

bool ReservingJoinNode::connect_port(Port *port)
{
    return Node::connect_port(port);
}

bool ReservingJoinNode::connect_from_out(Port *port)
{
    return Node::connect_from_out(port);
}

bool ReservingJoinNode::connect_to_in(Port *port)
{
    if(current_port_in < ports_in.end())
    {
        bool val = Node::connect_to_in(port);
        inc_curr_port(current_port_in, ports_in);
        inc_last_port(last_port_in, ports_in);
        return val;
    }
    else
    {
        port->connected_ports.pop_back();
        return false;
    }

}





