#include "queueingjoinnode.h"

QueueingJoinNode::QueueingJoinNode(QWidget *parent, const QPoint &p, unsigned int n) : Node{parent, p, n}
{
    ports_in.push_back(Port{this, 1});
    ports_in.push_back(Port{this, 2});
    ports_in.push_back(Port{this, 3});
    current_port_in = &ports_in[0];
    current_port_out = &ports_out[0];
    type = Type::QueueingJoin;
    setPixmap(QPixmap{":/icons/queue_join.png"});
    set_point_in();
    set_point_out();
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}

QueueingJoinNode::QueueingJoinNode(QWidget *parent, bool) :  Node{parent, QPixmap{":/icons/queue_join.png"}, true}
{
    type = Type::QueueingJoin;
    move(40, 250);
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}

void QueueingJoinNode::set_point_in()
{
    int h = pixmap()->height()/(no_of_ports+1);
    for(auto & port : ports_in)
    {
        port.pos = this->pos() + QPoint{ 0, h };
        h += pixmap()->height()/(no_of_ports+1);
    }
}

void QueueingJoinNode::set_current_port_in()
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

bool QueueingJoinNode::connect_port(Port *port)
{
    return Node::connect_port(port);
}

bool QueueingJoinNode::connect_from_out(Port *port)
{
    return Node::connect_from_out(port);
}

bool QueueingJoinNode::connect_to_in(Port *port)
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
