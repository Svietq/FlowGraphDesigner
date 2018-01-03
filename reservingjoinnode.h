#ifndef RESERVINGJOINNODE_H
#define RESERVINGJOINNODE_H

#include "node.h"

class ReservingJoinNode : public Node
{
    Q_OBJECT
public:
    explicit ReservingJoinNode(QWidget *parent = nullptr,
                           const QPoint & p = QPoint{},
                           unsigned int n = 0);
    explicit ReservingJoinNode(QWidget *parent, bool);


    bool connect_node(Node * node) override;
    bool connect_from_out(Node *node) override;
    bool connect_to_in(Node *node) override;

};

#endif // RESERVINGJOINNODE_H
