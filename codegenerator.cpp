#include "codegenerator.h"
#include <QDebug>

CodeGenerator::CodeGenerator(const QString &name, const QString &idir, const QVector<Node *> &inodes, const QVector<DragWidget::Edge> &iedges) :
    file_cpp{idir+name+".cpp"}, file_h{idir+name+".h"}, nodes{inodes}, edges{iedges}
{
    file_cpp.open(QIODevice::WriteOnly);
    file_h.open(QIODevice::WriteOnly);
}

CodeGenerator::~CodeGenerator()
{
    file_cpp.close();
    file_h.close();
}

void CodeGenerator::generate()
{
    write_begin();
    write_nodes();
    write_edges();
    write_source_activate();
    write_end();
}

void CodeGenerator::write_begin()
{
    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << " #include \"graph_stubs.h\" "             << '\n' << '\n';
    stream_cpp << " int main( int argc, char *argv[] ) { "   << '\n' << '\n';
    stream_cpp << "     graph graph_g0; "                    << '\n';

    //write to .h file:
    QTextStream stream_h( &file_h );
    stream_h << " #pragma once "             << '\n';
    stream_h << " #define TBB_PREVIEW_GRAPH_NODES 1 "   << '\n';
    stream_h << " #include \"tbb/flow_graph.h\" "                    << '\n';
    stream_h << " #include \"tbb/atomic.h\" "             << '\n';
    stream_h << " #include \"tbb/tick_count.h\" "   << '\n' << '\n';
    stream_h << " using namespace std; "                    << '\n';
    stream_h << " using namespace tbb; "             << '\n';
    stream_h << " using namespace tbb::flow; "   << '\n' << '\n';

    stream_h << " static void spin_for( double weight ) { " << '\n';
    stream_h << "     tick_count t1, t0 = tick_count::now(); " << '\n' << '\n';
    stream_h << "     const double weight_multiple = 10e-6; " << '\n';
    stream_h << "     do { " << '\n';
    stream_h << "         t1 = tick_count::now(); " << '\n';
    stream_h << "     } while ( (t1-t0).seconds() < end_time ); " << '\n';
    stream_h << " } " << '\n' << '\n';
}

void CodeGenerator::write_nodes()
{
    for(const auto & node : nodes)
    {
        write_node(node);
        if(node->type == Node::Type::Source) { source_nodes.push_back(node); }
    }
}

void CodeGenerator::write_edges()
{
    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    for(const auto & edge : edges)
    {
        stream_cpp << "     make_edge( ";
        write_port(edge.first, stream_cpp);
        stream_cpp << ", ";
        write_port(edge.second, stream_cpp);
        stream_cpp << ");" << '\n';
    }
}

void CodeGenerator::write_source_activate()
{
    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    for(const auto & source : source_nodes)
    {
        QString id = QString::number(source->id);
        stream_cpp << "     source_node_" << id << ".activate();" << '\n';
    }
}

void CodeGenerator::write_end()
{
    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     graph_g0.wait_for_all(); "           << '\n' << '\n';
    stream_cpp << "     return 0; "                          << '\n' << '\n';
    stream_cpp << "} "                                       << '\n';

}

void CodeGenerator::write_node(Node * node)
{
    switch (node->type) {
    case Node::Type::Source:
        write_source_node(node);
        break;
    case Node::Type::Continuous:
        write_continuous_node(node);
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

    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     source_node< int > source_node_" << id << "(graph_g0, " ;

    if(node->function == "" )
    {
        stream_cpp << "source_node_" << id << "_body()";
    }
    else
    {
        stream_cpp << node->function;
    }
    stream_cpp << ", false ); " << '\n';

    //write to .h file:
    QTextStream stream_h( &file_h );
    stream_h << " class source_node_" << id << "_body {" << '\n';
    stream_h << " public:" << '\n';
    stream_h << "     bool operator()( int &output ) {" << '\n';
    stream_h << "         //" << '\n';
    stream_h << "         // ADD USER BODY HERE" << '\n';
    stream_h << "         // " << '\n';
    stream_h << "         static int i = 0;" << '\n';
    stream_h << "         if ( i++ == 0 ) {" << '\n';
    stream_h << "             output = int();" << '\n';
    stream_h << "             return true;" << '\n';
    stream_h << "         }" << '\n';
    stream_h << "         return false;" << '\n';
    stream_h << "     }" << '\n';
    stream_h << " }" << '\n' << '\n';

}

void CodeGenerator::write_continuous_node(Node *node)
{
    QString id = QString::number(node->id);

    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     continue_node< int, int > continue_node_" << id << "(graph_g0, 1, ";

    if(node->function == "")
    {
        stream_cpp << " continue_node_" << id << "_body()";
    }
    else
    {
        stream_cpp << node->function;
    }
    stream_cpp << " ); " << '\n';

    //write to .h file:
    QTextStream stream_h( &file_h );
    stream_h << " class continue_node_" << id << "_body {" << '\n';
    stream_h << " public:" << '\n';
    stream_h << "     bool operator()( const continue_msg & ) {" << '\n';
    stream_h << "         //" << '\n';
    stream_h << "         // ADD USER BODY HERE" << '\n';
    stream_h << "         // " << '\n';
    stream_h << "         return int();" << '\n';
    stream_h << "     }" << '\n';
    stream_h << " }" << '\n' << '\n';
}

void CodeGenerator::write_function_node(Node *node)
{
    QString id = QString::number(node->id);

    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     function_node< int, int > function_node_" << id << "(graph_g0, 1, ";

    if(node->function == "")
    {
        stream_cpp << "function_node_" << id << "_body()";
    }
    else
    {
        stream_cpp << node->function;
    }
    stream_cpp << " ); " << '\n';

    //write to .h file:
    QTextStream stream_h( &file_h );
    stream_h << " class function_node_" << id << "_body {" << '\n';
    stream_h << " public:" << '\n';
    stream_h << "     bool operator()( const int & /*input*/ ) {" << '\n';
    stream_h << "         //" << '\n';
    stream_h << "         // ADD USER BODY HERE" << '\n';
    stream_h << "         // " << '\n';
    stream_h << "         return int();" << '\n';
    stream_h << "     }" << '\n';
    stream_h << " }" << '\n' << '\n';

}

void CodeGenerator::write_reserving_join_node(Node *node)
{
    QString id = QString::number(node->id);
    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     join_node< flow::tuple< int, int >, reserving > join_node_" << id << "(graph_g0); " << '\n';
}

void CodeGenerator::write_split_node(Node *node)
{
    QString id = QString::number(node->id);
    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     split_node< flow::tuple< int, int > > split_node_" << id << "(graph_g0); " << '\n';
}

void CodeGenerator::write_port(Port *port, QTextStream &stream_cpp)
{
    switch (port->node->type) {
    case Node::Type::Source:
        write_source_port(port, stream_cpp);
        break;
    case Node::Type::Continuous:
        write_continuous_port(port, stream_cpp);
        break;
    case Node::Type::Function:
        write_function_port(port, stream_cpp);
        break;
    case Node::Type::ReservingJoin:
        write_reserving_join_port(port, stream_cpp);
        break;
    case Node::Type::Split:
        write_split_port(port, stream_cpp);
        break;
    default:
        throw std::invalid_argument("node does not exist");
        break;
    }
}

void CodeGenerator::write_source_port(Port *port, QTextStream & stream_cpp)
{
    QString id = QString::number(port->node->id);
    //write to .cpp file:
    stream_cpp << "source_node_" << id;
}

void CodeGenerator::write_continuous_port(Port *port, QTextStream &stream_cpp)
{
    QString id = QString::number(port->node->id);
    //write to .cpp file:
    stream_cpp << "continuous_node_" << id;
}

void CodeGenerator::write_function_port(Port *port, QTextStream &stream_cpp)
{
    QString id = QString::number(port->node->id);
    //write to .cpp file:
    stream_cpp << "function_node_" << id;
}

void CodeGenerator::write_reserving_join_port(Port *port, QTextStream & stream_cpp)
{
    QString node_id = QString::number(port->node->id);
    QString port_id = QString::number(port->id);
    //write to .cpp file:
    stream_cpp << "input_port< " << port_id << " >( join_node_" << node_id << " )";
}

void CodeGenerator::write_split_port(Port *port, QTextStream &stream_cpp)
{
    QString node_id = QString::number(port->node->id);
    QString port_id = QString::number(port->id);
    //write to .cpp file:
    stream_cpp << "output_port< " << port_id << " >( split_node_" << node_id << " )";
}



















