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

