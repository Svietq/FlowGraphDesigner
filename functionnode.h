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


    bool connect_port(Port * port) override;
    bool connect_from_out(Port * port) override;
    bool connect_to_in(Port *) override;

};

#endif // FUNCTIONNODE_H
