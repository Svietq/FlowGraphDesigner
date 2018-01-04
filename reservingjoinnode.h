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

    void set_point_in() override;

    bool connect_port(Port * port) override;
    bool connect_from_out(Port * port) override;
    bool connect_to_in(Port *port) override;

protected:
    const unsigned int no_of_ports = 4;
    bool are_ports_in_full = false;
    void set_ports();

private slots:
    void set_current_port_in();

};

#endif // RESERVINGJOINNODE_H
