#ifndef SOURCENODE_H
#define SOURCENODE_H

#include "node.h"

class SourceNode : public Node
{
    Q_OBJECT
public:
    explicit SourceNode(QWidget *parent, const QPoint &p, unsigned int n);
    explicit SourceNode(QWidget *parent, bool);

};


#endif // SOURCENODE_H
