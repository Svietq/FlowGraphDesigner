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
    move(15, 230);
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}


bool FunctionNode::connect_node(Node * node)
{
    return Node::connect_node(node);
}

bool FunctionNode::connect_from_out(Node *node)
{
    nodes_out.push_back(node);
    return true;
}

bool FunctionNode::connect_to_in(Node *node)
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
