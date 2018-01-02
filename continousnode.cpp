#include "continousnode.h"

ContinousNode::ContinousNode(QWidget *parent, const QPixmap & map, const QPoint &p, unsigned int n) : Node{parent, map, p, n}
{
}

void ContinousNode::set_point_in()
{
    point_in = this->pos();// + QPoint{ 0, pixmap()->height()/2 };
}
