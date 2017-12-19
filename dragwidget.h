#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>

class DragWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DragWidget(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // DRAGWIDGET_H
