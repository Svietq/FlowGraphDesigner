#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QPainter>
#include "node.h"

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class DragWidget : public QFrame
{
    Q_OBJECT

public:
    enum class Type { MenuComputational, MenuJoinSplit, Canvas } type;
    explicit DragWidget(QWidget *parent = nullptr, Type itype = Type::Canvas);
    Node * current_node = nullptr;
    QVector<Node*> node_list;

    bool is_connecting = false;
    bool is_disconnecting = false;
    QVector<QLine> lines;
    using Edge = QPair<Port*, Port*>;
    QVector<Edge> edges;
    QString node_ids;

    void generate_code(const QString &name, const QString &dir);

protected:
    const QString mime_format = "application/x-dnditemdata";

    void dragEnterEvent(QDragEnterEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *) override;


    QPainter painter;
    QPoint line_begin{};
    QPoint line_end{};
    bool is_drawing = false;
    QLine current_line;

    bool is_node_dropped = false;
    void set_lines();

    void print_node_ids();
    unsigned int highest_node_id = 0;
    void delete_node(DragWidget* source);
    void start_node_movement(QMouseEvent *event);
    bool connect_nodes(Node * first, Node * second);
    bool connect_ports(Port *first, Port *second);

signals:
    void close_dock_widget();
    void deleted_line();


};

#endif // DRAGWIDGET_H
