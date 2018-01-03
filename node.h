#ifndef NODE_H
#define NODE_H

#include <QLabel>

class Node : public QLabel
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr, const QPoint & p = QPoint{}, unsigned int n = 0);
    explicit Node(QWidget *parent, const QPixmap & map, bool) :  QLabel{parent} { setPixmap(map); }
    explicit Node(QWidget *parent, bool) :  QLabel{parent} { }

    enum class Type {Source, Continuous} type;
    static Node* create(Type type, QWidget *parent, const QPoint &p, unsigned int n);
    static Node* create(Type type, QWidget *parent, bool);

    unsigned int id = 0;
    QPoint point_in;
    QPoint point_out;

    QVector<Node*> nodes_in;
    QVector<Node*> nodes_out;

    void set_point_in();
    void set_point_out();

    void move_node(const QPoint &pos);
    virtual bool connect_node(Node * node);
    virtual bool connect_from_out(Node * node);
    virtual bool connect_to_in(Node * node);

protected:
    void mouseDoubleClickEvent(QMouseEvent*) override;

signals:
    void node_double_clicked();
};

#endif // NODE_H
