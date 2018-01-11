#ifndef QUEUEINGJOINNODE_H
#define QUEUEINGJOINNODE_H

#include "node.h"

class QueueingJoinNode : public Node
{
    Q_OBJECT
public:
    explicit QueueingJoinNode(QWidget *parent = nullptr,
                              const QPoint & p = QPoint{},
                              unsigned int n = 0);
    explicit QueueingJoinNode(QWidget *parent, bool);

    void set_point_in() override;

    bool connect_port(Port * port) override;
    bool connect_from_out(Port * port) override;
    bool connect_to_in(Port *port) override;

protected:
    const unsigned int no_of_ports = 4;
    bool are_ports_in_full = false;

private slots:
    void set_current_port_in();

};


#endif // QUEUEINGJOINNODE_H
