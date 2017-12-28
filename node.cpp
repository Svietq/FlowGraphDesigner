#include "node.h"
#include "dragwidget.h"
#include "mainwindow.h"

Node::Node(QWidget *parent) : QLabel{parent}
{
//    const auto &obj  = static_cast<DragWidget*>( parentWidget() );
//    const auto &obj2 = static_cast<QWidget*>( obj->parentWidget() );
//    const auto &obj3 = static_cast<MainWindow*>( obj2->parentWidget() );
//    obj3->show_window();
    const auto &top = static_cast<MainWindow*>(this->window());
//    if(parent->)
    QObject::connect( this, &Node::node_clicked, top, &MainWindow::show_window );
//    this->window()->setWindowTitle("NOde created");
//    if(this->window() == this->parent()->parent()->parent()->)
//    {
//        this->window()->setWindowTitle("is the same");
//    }

    top->foo();

}

void Node::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit node_clicked();
}
