#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <QString>
#include <QFile>
#include "dragwidget.h"

class CodeGenerator
{
    QString dir;
    QFile file;
    QVector<Node*> nodes;
    QVector<DragWidget::Edge> edges;
public:
    CodeGenerator(const QString &idir, const QVector<Node*> & inodes, const QVector<DragWidget::Edge> & iedges);
    ~CodeGenerator();
    void write_begin();
    void write_nodes();
    void write_edges();
    void write_end();
};

#endif // CODEGENERATOR_H
