#ifndef PORT_H
#define PORT_H

#include <QPoint>
#include <QVector>

class Node;

class Port
{
public:
    Port(Node * inode = nullptr, int n = 0) : node{inode}, id{n} {}
    QPoint pos;
    Node * node;
    QVector<Port*> connected_ports;
    int id = 0;
};

#endif // PORT_H
