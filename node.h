#ifndef NODE_H
#define NODE_H

#include <QLabel>

class Node : public QLabel
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr);
    explicit Node(QWidget *parent, bool menu) :  QLabel{parent} {}

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void node_clicked();
};

#endif // NODE_H
