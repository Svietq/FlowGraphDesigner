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


bool ContinuousNode::connect_node(Node * node)
{
    Node::connect_node(node);
}

bool ContinuousNode::connect_from_out(Node *node)
{
    nodes_out.push_back(node);
    return true;
}

bool ContinuousNode::connect_to_in(Node *node)
{
    if(nodes_in.isEmpty())
    {
        nodes_in.push_back(node);
        return true;
    }
    else
    {
        return false;
    }
}
