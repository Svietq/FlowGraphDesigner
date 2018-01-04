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

    bool connect_port(Port * port) override;
    bool connect_from_out(Port * port) override;
    bool connect_to_in(Port *) override;

};


#endif // CONTINUOUSNODE_H
