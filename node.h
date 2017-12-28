#ifndef NODE_H
#define NODE_H

#include <QLabel>

class Node : public QLabel
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr, size_t n=0);
    explicit Node(QWidget *parent, bool) :  QLabel{parent} {}
    ~Node();

    unsigned int id=0;

protected:
    void mouseDoubleClickEvent(QMouseEvent*) override;

signals:
    void node_clicked();
};

#endif // NODE_H
