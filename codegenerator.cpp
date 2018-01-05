#include "codegenerator.h"
#include <QTextStream>

CodeGenerator::CodeGenerator(const QString &idir, const QVector<Node *> &inodes, const QVector<DragWidget::Edge> &iedges) :
    file{idir}, nodes{inodes}, edges{iedges}
{
    file.open(QIODevice::WriteOnly);
    QTextStream stream( &file );
    stream << "something";
}

CodeGenerator::~CodeGenerator()
{
    file.close();
}

void CodeGenerator::write_begin()
{

}

void CodeGenerator::write_nodes()
{

}

void CodeGenerator::write_edges()
{

}

void CodeGenerator::write_end()
{

}
