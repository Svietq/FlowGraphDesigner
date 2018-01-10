#ifndef SEQUENCERNODE_H
#define SEQUENCERNODE_H

#include "node.h"

class SequencerNode : public Node
{
    Q_OBJECT
public:
    explicit SequencerNode(QWidget *parent = nullptr,
                           const QPoint & p = QPoint{},
                           unsigned int n = 0);
    explicit SequencerNode(QWidget *parent, bool);


    bool connect_port(Port * port) override;
    bool connect_from_out(Port * port) override;
    bool connect_to_in(Port *) override;

};

#endif // SEQUENCERNODE_H
