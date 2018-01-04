#include "reservingjoinnode.h"

ReservingJoinNode::ReservingJoinNode(QWidget *parent, const QPoint &p, unsigned int n) : Node{parent, p, n}
{
    type = Type::ReservingJoin;
    setPixmap(QPixmap{":/icons/reserving_join.png"});
//    set_ports();
    Node::set_point_in();
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

//void ReservingJoinNode::set_point_in()
//{
//    if(current_port < ports.size())
//    {
//        point_in = ports[current_port++];
//    }
//}

void ReservingJoinNode::set_ports()
{
    ports.resize(no_of_ports);
    int h = pixmap()->height()/no_of_ports;
    for(auto & port : ports)
    {
        port = this->pos() + QPoint{ 0, h };
        h += pixmap()->height()/no_of_ports;
    }
}




