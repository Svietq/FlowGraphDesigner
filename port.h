#ifndef PORT_H
#define PORT_H

#include <QPoint>
#include <QVector>
//#include "node.h"
class Node;

class Port
{
public:
    Port(Node * inode = nullptr) : node{inode} {}
    QPoint pos;
    Node * node;
    QVector<Port*> connected_ports;
};

#endif // PORT_H
