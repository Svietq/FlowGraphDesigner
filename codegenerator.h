#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include "dragwidget.h"

class CodeGenerator
{
    QFile file_cpp;
    QFile file_h;
    QVector<Node*> nodes;
    QVector<DragWidget::Edge> edges;
public:
    CodeGenerator(const QString &name, const QString &idir, const QVector<Node*> & inodes, const QVector<DragWidget::Edge> & iedges);
    ~CodeGenerator();

    void generate();

private:
    void write_begin();
    void write_nodes();
    void write_edges();
    void write_end();

    void write_node(Node *node);
    void write_source_node(Node *node);
    void write_continuous_node(Node *node);
    void write_function_node(Node *node);
    void write_reserving_join_node(Node *node);
    void write_split_node(Node *node);

};

#endif // CODEGENERATOR_H
