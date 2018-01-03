#ifndef CONTINUOUSNODE_H
#define CONTINUOUSNODE_H

#include "node.h"

class ContinuousNode : public Node
{
    Q_OBJECT
public:
    explicit ContinuousNode(QWidget *parent = nullptr,
                           const QPoint & p = QPoint{},
                           unsigned int n = 0);
    explicit ContinuousNode(QWidget *parent, bool);

};


#endif // CONTINUOUSNODE_H
