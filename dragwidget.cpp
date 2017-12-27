#include "dragwidget.h"
#include <QLabel>
#include <QtWidgets>

namespace
{
    void set_node_icons(QWidget *parent)
    {
        QLabel *node_icon = new QLabel(parent);
        node_icon->setPixmap(QPixmap(":/icons/node.png"));
        node_icon->move(10, 10);
        node_icon->show();
        node_icon->setAttribute(Qt::WA_DeleteOnClose);
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
                event->accept();
            }
            else                         //Menu -> Menu
            {
                event->ignore();
            }
        }
        else if(static_cast<DragWidget*>(event->source())->type == Type::Canvas)
        {
            if( event->source() != this ) //Canvas -> Menu
            {
                event->accept();
            }
            else                          //Canvas -> Canvas
            {
                event->setDropAction(Qt::MoveAction);
                event->accept();
            }
        }
    }
    else
    {
        event->ignore();
    }
}

void DragWidget::mousePressEvent(QMouseEvent *event)
{
    current_block = static_cast<QLabel*>(childAt(event->pos()));
    if (!current_block)
    {
        return;
    }

    QPixmap pixmap = *current_block->pixmap();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - current_block->pos());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(mime_format, itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - current_block->pos());

    if(current_block)
    {
        if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
        {
            current_block->close();
        }
    }
}

void DragWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(mime_format))
    {
        QByteArray itemData = event->mimeData()->data(mime_format);
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QPoint offset;
        dataStream >> pixmap >> offset;

        if( !( (static_cast<DragWidget*>(event->source())->type == Type::Canvas) && (event->source() != this) ))
        {
            QLabel *newIcon = new QLabel(this);
            newIcon->setPixmap(pixmap);
            newIcon->move(event->pos() - offset);
            newIcon->show();
            newIcon->setAttribute(Qt::WA_DeleteOnClose);
        }
        else
        {
            auto obj = static_cast<DragWidget*>(event->source())->current_block;
            delete obj;
        }

        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
}


