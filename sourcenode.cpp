#include "sourcenode.h"

SourceNode::SourceNode(QWidget *parent, const QPoint &p, unsigned int n) : Node{parent, p, n}
{
    type = Type::Source;
    setPixmap(QPixmap{":/icons/source.png"});
    set_point_in();
    set_point_out();
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}

SourceNode::SourceNode(QWidget *parent, bool) :  Node{parent, QPixmap{":/icons/source.png"}, true}
{
    type = Type::Source;
    move(35, 10);
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}

bool SourceNode::connect_node(Node * node)
{
    return Node::connect_node(node);
}

bool SourceNode::connect_from_out(Node *node)
{
    nodes_out.push_back(node);
    return true;
}

bool SourceNode::connect_to_in(Node *)
{
    return false;
}
