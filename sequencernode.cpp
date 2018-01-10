#include "sequencernode.h"

SequencerNode::SequencerNode(QWidget *parent, const QPoint &p, unsigned int n) : Node{parent, p, n}
{
    type = Type::Sequencer;
    setPixmap(QPixmap{":/icons/sequencer_node.png"});
    set_point_in();
    set_point_out();
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}

SequencerNode::SequencerNode(QWidget *parent, bool) :  Node{parent, QPixmap{":/icons/sequencer_node.png"}, true}
{
    type = Type::Sequencer;
    move(50, 10);
    show();
    setAttribute(Qt::WA_DeleteOnClose);
}


bool SequencerNode::connect_port(Port * port)
{
    return Node::connect_port(port);
}

bool SequencerNode::connect_from_out(Port * port)
{
//    return Node::connect_from_out(port);
    if(current_port_out->connected_ports.isEmpty())
    {
        return Node::connect_from_out(port);
    }
    else
    {
//        port->connected_ports.pop_back();
        return false;
    }
}

bool SequencerNode::connect_to_in(Port * port)
{
    return Node::connect_to_in(port);
}
