#ifndef SPLITNODE_H
#define SPLITNODE_H

#include "node.h"

class SplitNode : public Node
{
    Q_OBJECT
public:
    explicit SplitNode(QWidget *parent = nullptr,
                           const QPoint & p = QPoint{},
                           unsigned int n = 0);
    explicit SplitNode(QWidget *parent, bool);

    void set_point_out() override;

    bool connect_port(Port * port) override;
    bool connect_from_out(Port * port) override;
    bool connect_to_in(Port *port) override;

protected:
    const unsigned int no_of_ports = 4;

};

#endif // SPLITNODE_H
