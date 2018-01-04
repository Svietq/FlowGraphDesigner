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

//    void set_point_in() override;

protected:
    QVector<QPoint> ports;
    int current_port = 0;
    const unsigned int no_of_ports = 4;
    void set_ports();

};

#endif // RESERVINGJOINNODE_H
