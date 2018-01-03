#ifndef SOURCENODE_H
#define SOURCENODE_H

#include "node.h"

class SourceNode : public Node
{
    Q_OBJECT
public:
    explicit SourceNode(QWidget *parent, const QPoint &p, unsigned int n);
    explicit SourceNode(QWidget *parent, bool);

    bool connect_node(Node *node) override;
    bool connect_from_out(Node * node) override;
    bool connect_to_in(Node * node) override;

};


#endif // SOURCENODE_H
