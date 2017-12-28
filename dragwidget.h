#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>
#include <QFrame>
#include "node.h"

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class DragWidget : public QFrame
{

public:
    enum class Type { Menu, Canvas } type;
    explicit DragWidget(QWidget *parent = nullptr, Type itype = Type::Canvas);
    Node * current_block = nullptr;

protected:
    const QString mime_format = "application/x-dnditemdata";

    void dragEnterEvent(QDragEnterEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void dropEvent(QDropEvent *event) override;

signals:
    void node_pressed();

};

#endif // DRAGWIDGET_H
