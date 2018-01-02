#ifndef CONTINOUSNODE_H
#define CONTINOUSNODE_H

#include "node.h"

class ContinousNode : public Node
{
    Q_OBJECT
public:
    explicit ContinousNode(QWidget *parent = nullptr,
                           const QPixmap &map = QPixmap{},
                           const QPoint & p = QPoint{},
                           unsigned int n = 0);
    explicit ContinousNode(QWidget *parent, bool) :  Node{parent, QPixmap{":/icons/continue.png"}, true} { }

    void set_point_in();

};


#endif // CONTINOUSNODE_H
