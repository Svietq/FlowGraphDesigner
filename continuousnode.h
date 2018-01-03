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


    bool connect_node(Node * node) override;
    bool connect_from_out(Node *node) override;
    bool connect_to_in(Node *node) override;

};


#endif // CONTINUOUSNODE_H
