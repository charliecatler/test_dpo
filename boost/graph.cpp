#include <iostream>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/graphviz.hpp>

//edge info
struct EdgeProperties{
    double weight;
};

//declaration of graph type
using Graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    EdgeProperties
>;

template<typename GraphT>
void print_adjacentverties(GraphT const& g){
    using vertex_descriptor = typename boost::graph_traits<GraphT>::vertex_descriptor;
    using edge_iterator = typename boost::graph_traits<GraphT>::out_edge_iterator;

    //circle for vertexes
    for (vertex_descriptor v: boost::make_iterator_range(vertices(g))){
        std::cout << "Vertex " << v << ": ";
        
        for (edge_iterator ei = out_edges(v, g).first; ei != out_edges(v,g).second; ++ei){
            vertex_descriptor target_vertex = boost::target(*ei,g);
            std::cout << target_vertex << "(" << g[*ei].weight << ")";
        }
        std::cout <<std::endl;
    }
}
int main(){
    Graph g;
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);

    add_edge(0,1,EdgeProperties{10}, g);
    add_edge(1,2,EdgeProperties{5}, g);
    add_edge(2,0,EdgeProperties{3}, g);

    print_adjacentverties(g);
    return 0;
}