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
    stream_cpp << " #include \"" << file_h.fileName() << "\" "             << '\n' << '\n';
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

//    stream_h << " static void spin_for( double weight ) { " << '\n';
//    stream_h << "     tick_count t1, t0 = tick_count::now(); " << '\n' << '\n';
//    stream_h << "     const double weight_multiple = 10e-6; " << '\n';
//    stream_h << "     do { " << '\n';
//    stream_h << "         t1 = tick_count::now(); " << '\n';
//    stream_h << "     } while ( (t1-t0).seconds() < end_time ); " << '\n';
//    stream_h << " } " << '\n' << '\n';
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
        write_port(edge.first, stream_cpp, false);
        stream_cpp << ", ";
        write_port(edge.second, stream_cpp, true);
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
    case Node::Type::QueueingJoin:
        write_queueing_join_node(node);
        break;
    case Node::Type::Split:
        write_split_node(node);
        break;
    case Node::Type::Sequencer:
        write_sequencer_node(node);
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
    stream_cpp << "     source_node< " << node->output_type << " > source_node_" << id << "(graph_g0, " ;

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
    stream_h << "     bool operator()( " << node->output_type << " &output ) {" << '\n';
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
    stream_h << " };" << '\n' << '\n';

}

void CodeGenerator::write_continuous_node(Node *node)
{
    QString id = QString::number(node->id);

    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     continue_node< " << node->output_type
               << " > continue_node_" << id << "(graph_g0, ";

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
    stream_h << " };" << '\n' << '\n';
}

void CodeGenerator::write_function_node(Node *node)
{
    QString id = QString::number(node->id);

    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     function_node< " << node->input_type << " , " << node->output_type
               << " > function_node_" << id << "(graph_g0, 1, ";

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
    stream_h << "     bool operator()( const " << node->input_type << " & /*input*/ ) {" << '\n';
    stream_h << "         //" << '\n';
    stream_h << "         // ADD USER BODY HERE" << '\n';
    stream_h << "         // " << '\n';
    stream_h << "         return int();" << '\n';
    stream_h << "     }" << '\n';
    stream_h << " };" << '\n' << '\n';

}

void CodeGenerator::write_reserving_join_node(Node *node)
{
    QString id = QString::number(node->id);
    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     join_node< " << node->output_type << ", reserving > join_node_" << id << "(graph_g0); " << '\n';
}

void CodeGenerator::write_queueing_join_node(Node *node)
{
    QString id = QString::number(node->id);
    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     join_node< " << node->output_type << ", queueing > join_node_" << id << "(graph_g0); " << '\n';
}

void CodeGenerator::write_split_node(Node *node)
{
    QString id = QString::number(node->id);
    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     split_node< " << node->input_type << " > split_node_" << id << "(graph_g0); " << '\n';
}

void CodeGenerator::write_sequencer_node(Node *node)
{
    QString id = QString::number(node->id);

    //write to .cpp file:
    QTextStream stream_cpp( &file_cpp );
    stream_cpp << "     sequencer_node< " << node->input_type << " > sequencer_node_" << id << "(graph_g0, ";

    if(node->function == "")
    {
        stream_cpp << "sequencer_node_" << id << "_body()";
    }
    else
    {
        stream_cpp << node->function;
    }
    stream_cpp << " ); " << '\n';

    //write to .h file:
    QTextStream stream_h( &file_h );
    stream_h << " class sequencer_node_" << id << "_body {" << '\n';
    stream_h << "     atomic< size_t > next_val;" << '\n';
    stream_h << " public:" << '\n';
    stream_h << "     sequencer_node_" << id << "_body() { next_val = 0; } " << '\n';
    stream_h << "     size_t operator()( const " << node->input_type << " & /*input*/ ) {" << '\n';
    stream_h << "         //" << '\n';
    stream_h << "         // ADD USER BODY HERE" << '\n';
    stream_h << "         // " << '\n';
    stream_h << "         return next_val.fetch_and_increment();" << '\n';
    stream_h << "     }" << '\n';
    stream_h << " };" << '\n' << '\n';
}

void CodeGenerator::write_port(Port *port, QTextStream &stream_cpp, bool is_second)
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
        write_reserving_join_port(port, stream_cpp, is_second);
        break;
    case Node::Type::QueueingJoin:
        write_queueing_join_port(port, stream_cpp, is_second);
        break;
    case Node::Type::Split:
        write_split_port(port, stream_cpp, is_second);
        break;
    case Node::Type::Sequencer:
        write_sequencer_port(port, stream_cpp);
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
    stream_cpp << "continue_node_" << id;
}

void CodeGenerator::write_function_port(Port *port, QTextStream &stream_cpp)
{
    QString id = QString::number(port->node->id);
    //write to .cpp file:
    stream_cpp << "function_node_" << id;
}

void CodeGenerator::write_reserving_join_port(Port *port, QTextStream & stream_cpp, bool is_second)
{
    QString node_id = QString::number(port->node->id);
    QString port_id = QString::number(port->id);
    //write to .cpp file:
    if(is_second)
    {
        stream_cpp << "get< " << port_id << " >( join_node_" << node_id << ".input_ports() )";
    }
    else
    {
        stream_cpp << "join_node_" << node_id << " ";
    }
}

void CodeGenerator::write_queueing_join_port(Port *port, QTextStream &stream_cpp, bool is_second)
{
    QString node_id = QString::number(port->node->id);
    QString port_id = QString::number(port->id);
    //write to .cpp file:
    if(is_second)
    {
        stream_cpp << "get< " << port_id << " >( join_node_" << node_id << ".input_ports() )";
    }
    else
    {
        stream_cpp << "join_node_" << node_id << " ";
    }
}

void CodeGenerator::write_split_port(Port *port, QTextStream &stream_cpp, bool is_second)
{
    QString node_id = QString::number(port->node->id);
    QString port_id = QString::number(port->id);
    //write to .cpp file:
    if(is_second)
    {
        stream_cpp << "split_node_" << node_id << " ";
    }
    else
    {
        stream_cpp << "get< " << port_id << " >( split_node_" << node_id << ".output_ports() )";
    }
}

void CodeGenerator::write_sequencer_port(Port *port, QTextStream &stream_cpp)
{
    QString id = QString::number(port->node->id);
    //write to .cpp file:
    stream_cpp << "sequencer_node_" << id;
}



















