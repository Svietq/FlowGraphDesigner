#include "dragwidget.h"

#include <QtWidgets>
#include "mainwindow.h"

namespace
{
    void set_node_icons(QWidget *parent, DragWidget::Type type)
    {
        if(type == DragWidget::Type::MenuComputational)
        {
            Node::create(Node::Type::Source, parent, true);
            Node::create(Node::Type::Continuous, parent, true);
            Node::create(Node::Type::Function, parent, true);

        }
        else if(type == DragWidget::Type::MenuJoinSplit)
        {
            Node::create(Node::Type::ReservingJoin, parent, true);
            Node::create(Node::Type::Split, parent, true);
        }
    }

    QDataStream & operator<<( QDataStream & data, const Node & node)
    {
        data << node.id << static_cast<int>(node.type);

        return data;
    }

    QDataStream & operator>>( QDataStream & data, Node & node)
    {
        int type = 0;
        data >> node.id >> type;
        node.type = static_cast<Node::Type>(type);

        return data;
    }

    void find_connection(DragWidget * widget, Node * node)
    {
        if(widget->current_node->type == Node::Type::Split && node->type == Node::Type::ReservingJoin)
        {
            auto it = std::find(widget->current_node->last_port_out->connected_ports.begin(),
                               widget->current_node->last_port_out->connected_ports.end(),
                               node->last_port_in);
            if (it != widget->current_node->last_port_out->connected_ports.end()) { emit widget->deleted_line(); }
        }
        else if(widget->current_node->type == Node::Type::Split)
        {
            auto it =std::find(widget->current_node->last_port_out->connected_ports.begin(),
                               widget->current_node->last_port_out->connected_ports.end(),
                               node->current_port_in);
            if (it != widget->current_node->last_port_out->connected_ports.end()) { emit widget->deleted_line(); }
        }
        else if(node->type == Node::Type::ReservingJoin)
        {
            auto it = std::find(widget->current_node->current_port_out->connected_ports.begin(),
                               widget->current_node->current_port_out->connected_ports.end(),
                               node->last_port_in);
            if (it != widget->current_node->current_port_out->connected_ports.end()) { emit widget->deleted_line(); }
        }
    }

    void delete_line(DragWidget * widget, Node * node)
    {
        find_connection(widget, node);

        if(!node->current_port_in || !widget->current_node->current_port_out) { return; }

        //erase line painted on canvas
        QLine line{widget->current_node->current_port_out->pos, node->current_port_in->pos};
        auto it = std::find(widget->lines.begin(), widget->lines.end(), line);
        if (it != widget->lines.end()) { widget->lines.erase(it); }

        //erase edge
        auto it4 = std::find( widget->edges.begin(), widget->edges.end(), DragWidget::Edge{widget->current_node->current_port_out, node->current_port_in});
        if (it4 != widget->edges.end()) { widget->edges.erase(it4); }

        //erase ports connected to node
        auto it5 = std::find( node->current_port_in->connected_ports.begin(),
                              node->current_port_in->connected_ports.end(),
                              widget->current_node->current_port_out);
        if (it5 != node->current_port_in->connected_ports.end()) { node->current_port_in->connected_ports.erase(it5); }

        //erase ports connected from node
        auto it6 = std::find( widget->current_node->current_port_out->connected_ports.begin(),
                              widget->current_node->current_port_out->connected_ports.end(),
                              node->current_port_in);
        if (it6 != widget->current_node->current_port_out->connected_ports.end()) { widget->current_node->current_port_out->connected_ports.erase(it6); }

    }
}

DragWidget::DragWidget(QWidget *parent, Type itype) : QFrame(parent), type{itype}
{
    setAcceptDrops(true);

    if(itype == Type::MenuComputational || itype == Type::MenuJoinSplit)
    {
        set_node_icons(this, itype);
    }
}

void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat(mime_format))
    {
        if(static_cast<DragWidget*>(event->source())->type == Type::MenuComputational ||
           static_cast<DragWidget*>(event->source())->type == Type::MenuJoinSplit  )
        {
            if( event->source() != this ) //Menu -> Canvas
            {
                event->setDropAction(Qt::CopyAction);
            }
            else                          //Menu -> Menu
            {
                event->ignore();
                return;
            }
        }

        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void DragWidget::mousePressEvent(QMouseEvent *event)
{
    current_node = static_cast<Node*>(childAt(event->pos()));
    if (!current_node)
    {
        return;
    }

    if(is_connecting)
    {
        if(current_node->current_port_out)
        {
            line_begin = current_node->current_port_out->pos;
        }

        is_drawing = true;
    }
    else if(is_disconnecting)
    {
        if(current_node->last_port_out)
        {
            line_begin = current_node->last_port_out->pos;
        }
        else
        {
            line_begin = current_node->current_port_out->pos;
        }

        is_drawing = true;
    }
    else
    {
        start_node_movement(event);
    }
}

void DragWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(mime_format))
    {
        QByteArray itemData = event->mimeData()->data(mime_format);
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPoint offset;
        Node new_node{this};
        dataStream >> new_node >> offset;

        auto source = static_cast<DragWidget*>(event->source());

        if(source->type == Type::MenuComputational || source->type == Type::MenuJoinSplit)
        {
            if( source != this )          //Menu -> Canvas
            {
                auto node = Node::create(new_node.type, this,  event->pos() - offset, highest_node_id++);
                node_list.push_back(node);
                event->setDropAction(Qt::CopyAction);
            }
            else                          //Menu -> Menu
            {
                event->ignore();
                return;
            }
        }
        else if(source->type == Type::Canvas)
        {
            if( source != this )          //Canvas -> Menu
            {
                delete_node(source);
                is_node_dropped = false;
            }
            else                          //Canvas -> Canvas
            {
                current_node->move_node(event->pos() - offset);
                is_node_dropped = true;
                set_lines();
            }
        }

        this->repaint();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void DragWidget::mouseDoubleClickEvent(QMouseEvent * )
{
    emit close_dock_widget();
}

void DragWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(is_connecting || is_disconnecting)
    {
        line_end = event->pos();
        this->repaint();
    }
}

void DragWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(is_connecting || is_disconnecting)
    {
        auto node = static_cast<Node*>(childAt(event->pos()));
        if(node && (node != current_node))
        {            
            if( current_node )
            {
                if(is_connecting)
                {
                    if(!connect_ports(current_node->current_port_out, node->current_port_in))
                    {
                        line_end = line_begin;
                    }
                }
                else if(is_disconnecting)
                {
                    delete_line(this, node);
                    line_end = line_begin;
                }
            }
        }
        else
        {
            line_end = line_begin;
        }
        is_drawing = false;
        this->repaint();
    }
}

void DragWidget::paintEvent(QPaintEvent *)
{
    painter.begin(this);
    painter.setPen(QPen(Qt::black, 5, Qt::SolidLine));

    if(is_connecting)
    {
        current_line = QLine{line_begin, line_end};
        if(line_begin != line_end && line_begin != QPoint{})
        {
            painter.drawLine(current_line);
        }
        painter.drawLines(lines);
    }
    else if(is_disconnecting)
    {
        painter.drawLines(lines);
        painter.setPen(QPen(Qt::red, 5, Qt::SolidLine));
        current_line = QLine{line_begin, line_end};
        if(line_begin != line_end  && line_begin != QPoint{})
        {
            painter.drawLine(current_line);
        }
    }
    else if(is_node_dropped)
    {

        painter.drawLines(lines);
    }

    painter.end();
}

void DragWidget::set_lines()
{
    lines.clear();
    for(const auto & edge : edges)
    {
        QLine line{ edge.first->pos, edge.second->pos };
        lines.push_back(line);
    }
}

void DragWidget::print_node_ids()
{
    node_ids.clear();
    for(const auto & node : node_list)
    {
        node_ids.append(QString::number(node->id));
        node_ids.append(" ");
    }
    window()->setWindowTitle(node_ids);
}

void DragWidget::delete_node(DragWidget *source)
{
    auto obj = source->current_node;

    for(const auto & port : obj->ports_in)
    {
        if(!port.connected_ports.isEmpty())
        {
            return;
        }
    }
    for(const auto & port : obj->ports_out)
    {
        if(!port.connected_ports.isEmpty())
        {
            return;
        }
    }
    auto it = std::find(source->node_list.begin(), source->node_list.end(), obj);
    if (it != source->node_list.end())
    {
        source->node_list.erase(it);
        delete obj;
    }
}

void DragWidget::start_node_movement(QMouseEvent *event)
{
    QPixmap pixmap = *current_node->pixmap();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << *current_node << QPoint(event->pos() - current_node->pos());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(mime_format, itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - current_node->pos());

    if(current_node)
    {
        if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
        {
            current_node->close();
        }
    }
}

bool DragWidget::connect_ports(Port *first, Port *second)
{
    if(!first || !second) { return false; }
    line_end = second->pos;
    auto it = std::find( edges.begin(), edges.end(), Edge{first, second});
    if (it == edges.end())
    {
        if(first->node->connect_port(second))
        {
            edges.push_back(Edge{first, second});
            set_lines();
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}
