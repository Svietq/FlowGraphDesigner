#include "reservingjoinnode.h"

ReservingJoinNode::ReservingJoinNode(QWidget *parent, const QPoint &p, unsigned int n) : Node{parent, p, n}
{
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


bool ReservingJoinNode::connect_node(Node * node)
{
    return Node::connect_node(node);
}

bool ReservingJoinNode::connect_from_out(Node *node)
{
    return Node::connect_from_out(node);
}

bool ReservingJoinNode::connect_to_in(Node *node)
{
    return Node::connect_to_in(node);
}
