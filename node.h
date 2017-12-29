#ifndef NODE_H
#define NODE_H

#include <QLabel>

class Node : public QLabel
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr, const QPixmap &map = QPixmap{}, const QPoint & p = QPoint{}, size_t n=0);
    explicit Node(QWidget *parent, const QPixmap & map, bool) :  QLabel{parent} { setPixmap(map);}
    ~Node();

    unsigned int id=0;
    QPoint point_in;
    QPoint point_out;

    QVector<Node*> nodes_in;
    QVector<Node*> nodes_out;

protected:
    void mouseDoubleClickEvent(QMouseEvent*) override;

signals:
    void node_clicked();
};

#endif // NODE_H
