#ifndef NODE_H
#define NODE_H

#include <QLabel>
#include "port.h"

class Node : public QLabel
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr, const QPoint & p = QPoint{}, unsigned int n = 0);
    explicit Node(QWidget *parent, const QPixmap & map, bool) :  QLabel{parent} { setPixmap(map); }
    explicit Node(QWidget *parent, bool) :  QLabel{parent} { }

    enum class Type {Source, Continuous, Function, ReservingJoin} type;
    static Node* create(Type type, QWidget *parent, const QPoint &p, unsigned int n);
    static Node* create(Type type, QWidget *parent, bool);

    unsigned int id = 0;

    QVector<Port> ports_in;
    QVector<Port> ports_out;
    Port* current_port_in;
    Port* current_port_out;

    virtual void set_point_in();
    virtual void set_point_out();

    void move_node(const QPoint &pos);
    virtual bool connect_port(Port * port);
    virtual bool connect_from_out(Port * port);
    virtual bool connect_to_in(Port * port);

protected:
    void mouseDoubleClickEvent(QMouseEvent*) override;

signals:
    void node_double_clicked();
};

#endif // NODE_H
