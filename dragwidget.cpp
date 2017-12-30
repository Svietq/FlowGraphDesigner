#include "dragwidget.h"

#include <QtWidgets>

namespace
{
    void set_node_icons(QWidget *parent)
    {
        Node *node = new Node(parent, QPixmap(":/icons/node.png"), true);
        node->move(10, 10);
        node->show();
        node->setAttribute(Qt::WA_DeleteOnClose);
    }

    Node* create_node(QWidget *parent, const QPoint & pos, const QPixmap & pixmap, std::size_t id)
    {
        Node *newNode = new Node(parent, pixmap, pos, id);
        newNode->show();
        newNode->setAttribute(Qt::WA_DeleteOnClose);

        return newNode;
    }

    void move_node(Node & node, const QPoint & pos)
    {
        node.move(pos);
        node.set_point_in();
        node.set_point_out();
    }

    QDataStream & operator<<( QDataStream & data, const Node & node)
    {
        data << node.id << *node.pixmap();

        return data;
    }

    QDataStream & operator>>( QDataStream & data, Node & node)
    {
        QPixmap pixmap;
        data >> node.id >> pixmap;
        node.setPixmap(pixmap);

        return data;
    }

    QString print_lines(const QVector<QLine> & vec)
    {
        QString title;
        for(const auto & x : vec)
        {
            title.append( QString::number(x.x1()) );
            title.append(" ");
            title.append( QString::number(x.x2()) );
            title.append(" ");
            title.append( QString::number(x.y1()) );
            title.append(" ");
            title.append( QString::number(x.y2()) );
            title.append(" | ");
        }

        return title;
    }

    void delete_line(DragWidget * widget, Node * node)
    {
        QLine line{widget->current_node->point_out, node->point_in};
        auto it = std::find(widget->lines.begin(), widget->lines.end(), line);
        if (it != widget->lines.end()) { widget->lines.erase(it); }
        auto it2 = std::find(widget->current_node->nodes_out.begin(), widget->current_node->nodes_out.end(), node);
        if (it2 != widget->current_node->nodes_out.end()) { widget->current_node->nodes_out.erase(it2); }
        it2 = std::find( node->nodes_in.begin(), node->nodes_in.end(), widget->current_node);
        if (it2 != node->nodes_in.end()) { node->nodes_in.erase(it2); }
    }
}

DragWidget::DragWidget(QWidget *parent, Type itype) : QFrame(parent), type{itype}
{
    setAcceptDrops(true);

    if(itype == Type::Menu)
    {
        set_node_icons(this);
    }
}

void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat(mime_format))
    {
        if(static_cast<DragWidget*>(event->source())->type == Type::Menu)
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

    if(is_connecting || is_disconnecting)
    {
        line_begin = current_node->point_out;
        is_drawing = true;
    }
    else
    {
        //node moving
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

        if(static_cast<DragWidget*>(event->source())->type == Type::Menu)
        {
            if( event->source() != this ) //Menu -> Canvas
            {
                auto node = create_node(this, event->pos() - offset, *new_node.pixmap(), node_list.size());
                node_list.push_back(node);
                event->setDropAction(Qt::CopyAction);
            }
            else                          //Menu -> Menu
            {
                event->ignore();
                return;
            }
        }
        else if(static_cast<DragWidget*>(event->source())->type == Type::Canvas)
        {
            if( event->source() != this ) //Canvas -> Menu
            {
                auto obj = static_cast<DragWidget*>(event->source())->current_node;
                auto it = std::find(node_list.begin(), node_list.end(), obj);
                if (it != node_list.end()) { node_list.erase(it); }
                delete obj;
                is_node_dropped = false;
            }
            else                          //Canvas -> Canvas
            {
                move_node(*current_node, event->pos() - offset);
                is_node_dropped = true;
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

void DragWidget::mouseDoubleClickEvent(QMouseEvent * event)
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
                    line_end = node->point_in;
                    current_node->nodes_out.push_back(node);
                    node->nodes_in.push_back(current_node);
                    set_lines();
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

void DragWidget::paintEvent(QPaintEvent *event)
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
        set_lines();
        painter.drawLines(lines);
    }

    painter.end();
}

void DragWidget::set_lines()
{
    lines.clear();
    for(const auto & node : node_list)
    {
        for(const auto & in : node->nodes_in)
        {
            QLine line{ in->point_out, node->point_in};
            lines.push_back(line);
        }
    }
}


