#ifndef SOURCENODE_H
#define SOURCENODE_H

#include "node.h"

class SourceNode : public Node
{
    Q_OBJECT
public:
    explicit SourceNode(QWidget *parent = nullptr,
                           const QPixmap &map = QPixmap{},
                           const QPoint & p = QPoint{},
                           unsigned int n = 0);
    explicit SourceNode(QWidget *parent, bool) :  Node{parent, QPixmap{":/icons/source.png"}, true} { }

};


#endif // SOURCENODE_H
