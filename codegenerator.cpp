#include "codegenerator.h"
#include <QDebug>

CodeGenerator::CodeGenerator(const QString &idir, const QVector<Node *> &inodes, const QVector<DragWidget::Edge> &iedges) :
    file{idir}, nodes{inodes}, edges{iedges}
{
    file.open(QIODevice::WriteOnly);
}

CodeGenerator::~CodeGenerator()
{
    file.close();
}

void CodeGenerator::generate()
{
    write_begin();
    write_nodes();
    write_edges();
    write_end();
}

void CodeGenerator::write_begin()
{
    QTextStream stream( &file );
    stream << " #include \"graph_stubs.h\" "             << '\n' << '\n';
    stream << " int main( int argc, char *argv[] ) { "   << '\n' << '\n';
    stream << "     graph graph_g0; "                    << '\n';
}

void CodeGenerator::write_nodes()
{
    qDebug() << "CodeGenerator::write_nodes";
    for(const auto & node : nodes)
    {
        qDebug() << "CodeGenerator::write_nodes - 1";
        write_node(node);
    }
}

void CodeGenerator::write_edges()
{

}

void CodeGenerator::write_end()
{
    QTextStream stream( &file );
    stream << "     graph_g0.wait_for_all(); "           << '\n' << '\n';
    stream << "     return 0; "                          << '\n' << '\n';
    stream << "} "                                       << '\n';

}

void CodeGenerator::write_node(Node * node)
{
    switch (node->type) {
    case Node::Type::Source:
        write_source_node(node);
        break;
    case Node::Type::Continuous:
        write_continous_node(node);
        break;
    case Node::Type::Function:
        write_function_node(node);
        break;
    case Node::Type::ReservingJoin:
        write_reserving_join_node(node);
        break;
    case Node::Type::Split:
        write_split_node(node);
        break;
    default:
        throw std::invalid_argument("node does not exist");
        break;
    }
}

void CodeGenerator::write_source_node(Node *node)
{
    QString id = QString::number(node->id);
    QTextStream stream( &file );
    stream << "     source_node< int > source_node_" << id << "(graph_g0, source_node_" << id << "_body(), false ); " << '\n';
}

void CodeGenerator::write_continous_node(Node *node)
{
    QString id = QString::number(node->id);
    QTextStream stream( &file );
    stream << "     continous_node< int, int > continous_node_" << id << "(graph_g0, 1, continous_node_" << id << "_body() ); \\\\??" << '\n';
}

void CodeGenerator::write_function_node(Node *node)
{
    QString id = QString::number(node->id);
    QTextStream stream( &file );
    stream << "     function_node< int, int > function_node_" << id << "(graph_g0, 1, function_node_" << id << "_body() ); " << '\n';
}

void CodeGenerator::write_reserving_join_node(Node *node)
{
    QString id = QString::number(node->id);
    QTextStream stream( &file );
    stream << "     join_node< flow::tuple< int, int >, reserving > join_node_" << id << "(graph_g0); " << '\n';
}

void CodeGenerator::write_split_node(Node *node)
{
    QString id = QString::number(node->id);
    QTextStream stream( &file );
    stream << "     split_node< flow::tuple< int, int > > split_node_" << id << "(graph_g0); " << '\n';
}



















