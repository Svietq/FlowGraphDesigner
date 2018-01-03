#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "node.h"

class FunctionNode : public Node
{
    Q_OBJECT
public:
    explicit FunctionNode(QWidget *parent = nullptr,
                           const QPoint & p = QPoint{},
                           unsigned int n = 0);
    explicit FunctionNode(QWidget *parent, bool);


    bool connect_node(Node * node) override;
    bool connect_from_out(Node *node) override;
    bool connect_to_in(Node *node) override;

};

#endif // FUNCTIONNODE_H
