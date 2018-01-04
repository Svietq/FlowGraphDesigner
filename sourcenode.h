#ifndef SOURCENODE_H
#define SOURCENODE_H

#include "node.h"

class SourceNode : public Node
{
    Q_OBJECT
public:
    explicit SourceNode(QWidget *parent, const QPoint &p, unsigned int n);
    explicit SourceNode(QWidget *parent, bool);

    bool connect_port(Port * port) override;
    bool connect_from_out(Port * port) override;
    bool connect_to_in(Port *port) override;

};


#endif // SOURCENODE_H
