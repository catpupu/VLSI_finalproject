# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <string>
# include <algorithm>
# include <queue>

using namespace std ;

// =============== data structure ==================

struct adjacent {
    string net_name ;
    string direction ;
    int weight ;
} ;

struct Instance {
    string instance_name ;
    vector <adjacent> out ;
} ;

// ==================== head =======================

// graph 

class Graph {
private:
    vector <Instance> vertices ;
    void print_all() ;

public:
    // for outside call to get info.
    bool is_empty() ;
    int size_of() ;
    int find_vertex( string name ) ;
    string find_vertex( int index ) ;
    void push_adjacent ( int vertex_idx, queue <int> & adj ) ; // get adj. list
    void push_adjacent_and_Weight ( int vertex_idx, queue <int> & adj ) ; // get adj. list with weight

    
    // for readfile() call add(or resort) new data
    void append_vertex( string vertex_name ) ;
    void update_direction( string name ) ; 
    void update_AdjacentList( string edge_name, int edge_weight, bool out ) ;
    void sort_adjacent() ;
} ;

void read_file( Graph & circuit_graph ) ;


// graph algorithm

class DFS {
private :
    struct DFS_Vertex {
        string color ;
        int discover_time ;
        int finish_time ;
        int predecessor ;
    } ;

    Graph graph ;
    int time, graph_size ; // dfs time line
    DFS_Vertex * DFS_VertexArr ; // vertex data for DFS, arrange by vector index
    queue <int> DFS_Seq ;
    
    void DFS_Visit( int visit_idx ) ; // DFS recursion
    void Print_DFS_Seq() ;


public :
    // init data then traverse(DFS) and print sequence
    void do_DFS ( Graph circuit_graph ) ; 
} ;

class Dijkstra {
private:
    struct Dijkstra_Vertex {
        int distance ;
        int predecessor ;
        bool popped ; // fake priority queue(min)
    } ;

    Graph graph ;
    int graph_size ;
    Dijkstra_Vertex * Dijkstra_VertexArr ;

public:
    void init ( Graph circuit_graph ) ;
    int fake_pop() ;
    void do_Dijkstra ( Graph circuit_graph, int start_vertex ) ;
    void print_all_shortest () ;
    void clear() ;
} ;

// ================== function =====================

int main() {
    int mode = -1 ;
    Graph circuit_graph ;
    DFS dfs ;
    Dijkstra dij ;

    while( 1 ) {
        cout << "***     VLSI Final Project     **\n" ;
        cout << "* 0. QUIT                       *\n" ;
        cout << "* 1. DFS ( depth first search ) *\n" ;
        cout << "* 2. shotest path ( Dijkstra )  *\n" ;
        cout << "*********************************\n" ;
        cout << "Choose mode (0, 1, 2) : " ;
        cin >> mode ;

        if ( mode == 0 ) break ;
        else if ( mode < 0 || 2 < mode ) cout << "Error : There is no such mode : " << mode << endl ;
        
        else {
            if ( circuit_graph.is_empty() ) read_file( circuit_graph ) ;

            switch ( mode ) {
                case 1 :
                    dfs.do_DFS( circuit_graph ) ;
                    break ;

                case 2 :
                    cout << "\nSingle source shortest path algorithm (Dijkstra)" << endl ;
                    dij.do_Dijkstra(circuit_graph, 0) ; // start from first vertex(S)
                    dij.print_all_shortest() ;
                    dij.clear() ; // free memory
                    break ;

                default :
                    cout << "other mode" << endl ;
                    break ;
            }
        }
    }
}

void read_file( Graph & circuit_graph ) {
    ifstream ifs ;
    stringstream ss, stoi ;

    string line ; // for getline
    string instance_name ; // for instance format
    string net, net_name, net_weight_str ; // for net format
    int net_weight_int ;

    ifs.open("input.txt") ;
    if ( !ifs.is_open() ) {
        cout << "## input.txt does not exist!! ##\n" ;
        return ;
    }

    else {
        getline( ifs, line ) ; // circuit name

        // two lines in a group until "endcircuit"
        while( getline( ifs, line ) ) {
            if ( line.find("ENDCIRCUIT") != string::npos ) break ;

            // ss already output all char no needs to ss.str("")
            // ""EOF ---(clear)---> ""(reset success)
            // reset
            ss.clear() ;

            ss << line ;
            if (  ss.str().size() > 1 ) {
                ss >> instance_name >> instance_name ;
                circuit_graph.append_vertex( instance_name ) ;
            }

            else {
                cout << "ss size error\n" ;
                return ;
            }

            // reset
            ss.str("") ;
            ss.clear() ;

            getline( ifs, line ) ;
            ss << line ;

            while( ss >> net ) {
                // split  ex:N9(2,i)
                size_t delimiter_up = net.find("(") ;
                size_t delimiter_comma = net.find(",") ;
                net_name = net.substr( 0, delimiter_up ) ;
                net_weight_str = net.substr( delimiter_up + 1, delimiter_comma - delimiter_up - 1 ) ;

                // convert string to int
                // because g++ 4.8.5 20150623 can't use std::stoi(), at least c++11
                stoi.clear() ;
                stoi << net_weight_str ;
                stoi >> net_weight_int ;

                // store net
                if ( net[delimiter_comma + 1] == 'i' )
                    circuit_graph.update_AdjacentList( net_name, net_weight_int, false ) ;
                else 
                    circuit_graph.update_AdjacentList( net_name, net_weight_int, true ) ;
            }
        }

        ifs.close() ;

        // resort adjacent list by vertex name order ( Originally base on input file order )
        circuit_graph.sort_adjacent() ;
    }
}


void Graph::print_all() {
    for ( int i = 0 ; i < vertices.size() ; i++ ) {
        cout << "INSTANCE " << vertices[i].instance_name << endl ;

        for ( int j = 0 ; j < vertices[i].out.size() ; j++ ) {
            cout << "\t" << vertices[i].out[j].net_name << " : " ;
            cout << vertices[i].out[j].weight << ", " << vertices[i].out[j].direction ;
        }

        cout << endl ;
    }
}


bool Graph::is_empty() {
    if ( vertices.empty() ) return true ;
    else return false ;
}

int Graph::size_of() {
    return vertices.size() ;
}

int Graph::find_vertex( string name ) {
    for ( int i = 0 ; i < vertices.size() ; i++ ) 
        if ( name == vertices[i].instance_name ) return i ;
    
    return -1 ; // not found
}

string Graph::find_vertex( int index ) {
    return vertices[index].instance_name ;
}

void Graph::push_adjacent ( int vertex_idx, queue <int> & adj ) {
    string dir_vertex_name ;

    for ( int i = 0 ; i < vertices[ vertex_idx ].out.size() ; i++ ) {
        dir_vertex_name = vertices[ vertex_idx ].out[i].direction ;
        adj.push( find_vertex( dir_vertex_name ) ) ;
    }
}

void Graph::push_adjacent_and_Weight ( int vertex_idx, queue <int> & adj ) {
    string dir_vertex_name ;
    int dir_vertex_weight ;

    for ( int i = 0 ; i < vertices[ vertex_idx ].out.size() ; i++ ) {
        dir_vertex_name = vertices[ vertex_idx ].out[i].direction ;
        dir_vertex_weight = vertices[ vertex_idx ].out[i].weight ;
        adj.push( find_vertex( dir_vertex_name ) ) ;
        adj.push( dir_vertex_weight ) ;
    }
}

void Graph::append_vertex( string vertex_name ) {
    Instance push_in ;
    push_in.instance_name = vertex_name ;
    vertices.push_back( push_in ) ;
}

// find edge and update direction data( adjacent list )
void Graph::update_direction( string name ) {
    for ( int i = 0 ; i < vertices.size() ; i++ ) {
        for ( int j = 0 ; j < vertices[i].out.size() ; j++ ) {
            // find where the net start
            if ( name == vertices[i].out[j].net_name )
                // and the direction is last vertex( current process instance )
                vertices[i].out[j].direction = vertices[vertices.size() - 1].instance_name ;
        }
    }
}

void Graph::update_AdjacentList( string edge_name, int edge_weight, bool out ) {
    // bug : ( the same net ) out first(out->in) is fine, but in first can't find where the net from
    if ( out ) {
        adjacent push_in ;
        push_in.net_name = edge_name ;
        push_in.weight = edge_weight ;
        vertices[vertices.size() - 1].out.push_back( push_in ) ;
    }

    else update_direction( edge_name ) ;
}

// n^3 hahaha (12*2*2)
void Graph::sort_adjacent() {
    for ( int i = 0 ; i < vertices.size() ; i++ ) {
        for ( int j = 0 ; j < vertices[i].out.size() ; j++ ) {
            for ( int k = j + 1 ; k < vertices[i].out.size() ; k++ ) {
                if ( vertices[i].out[j].direction > vertices[i].out[k].direction ) {
                    swap( vertices[i].out[j], vertices[i].out[k] ) ;
                }
            }
        }
    }
}
    
void DFS::DFS_Visit( int visit_idx ) {
    queue <int> go_to ;
    int curr_go_to ;

    // discover this vertex
    DFS_VertexArr[visit_idx].color = "gray" ;
    DFS_VertexArr[visit_idx].discover_time = ++time ;
    DFS_Seq.push( visit_idx ) ;

    // get adjacent
    graph.push_adjacent( visit_idx, go_to ) ;

    // visit all
    while( !go_to.empty() ) {
        curr_go_to = go_to.front() ;
        go_to.pop() ;

        if ( DFS_VertexArr[curr_go_to].color == "white" ) {
            DFS_VertexArr[curr_go_to].predecessor = visit_idx ;
            DFS_Visit( curr_go_to ) ;
        }
    }

    // finish this vertex
    DFS_VertexArr[visit_idx].color = "black" ;
    DFS_VertexArr[visit_idx].finish_time = ++time ;
}

void DFS::Print_DFS_Seq() {
    bool end = false ;
    
    cout << endl ;
    while ( !DFS_Seq.empty() ) {
        if ( end ) 
            cout << graph.find_vertex( DFS_Seq.front() ) << endl << endl << endl ;
            
        else {
            cout << graph.find_vertex( DFS_Seq.front() ) << " >> " ;
            if ( DFS_Seq.size() == 2 ) end = true ;
        }
        
        DFS_Seq.pop() ;
    }
}

void DFS::do_DFS( Graph circuit_graph ) {
    // initial
    time = 0 ;
    graph = circuit_graph ;
    graph_size = graph.size_of() ;
    DFS_VertexArr = new DFS_Vertex[ graph_size ] ;

    for ( int i = 0 ; i < graph_size ; i++ ) 
        DFS_VertexArr[i].color = "white" ;


    // call resursion to traverse verticescd
    for ( int i = 0 ; i < graph_size ; i++ ) {
        if ( DFS_VertexArr[i].color == "white" ) {
            DFS_Visit( i ) ;
        }
    }

    Print_DFS_Seq() ;
    delete[] DFS_VertexArr ;
}

void Dijkstra::init( Graph circuit_graph ) {
    graph = circuit_graph ;
    graph_size = graph.size_of() ;
    Dijkstra_VertexArr = new Dijkstra_Vertex[graph_size] ;

    for ( int i = 0 ; i < graph_size ; i++ ) {
        Dijkstra_VertexArr[i].predecessor = -1 ;
        Dijkstra_VertexArr[i].distance = INT32_MAX ;
        Dijkstra_VertexArr[i].popped = false ;
    }
}

int Dijkstra::fake_pop() {
    int min = -1, min_dis = INT32_MAX ;

    for ( int i = 0 ; i < graph_size ; i++ ) {
        if ( !Dijkstra_VertexArr[i].popped && Dijkstra_VertexArr[i].distance < min_dis ) {
            min = i ;
            min_dis = Dijkstra_VertexArr[i].distance ;
        }
    }
    
    if ( min != -1 ) {
        Dijkstra_VertexArr[min].popped = true ;
    }

    return min ;
}

void Dijkstra::do_Dijkstra( Graph circuit_graph, int start_vertex ) {
    int min_dis_vertex, adj_vertex, adj_weight ;
    queue <int> adj_list ;

    init( circuit_graph ) ;
    Dijkstra_VertexArr[start_vertex].distance = 0 ;
    
    while ( 1 ) {
        min_dis_vertex = fake_pop() ;
        if ( min_dis_vertex == -1 ) break ;

        graph.push_adjacent_and_Weight( min_dis_vertex, adj_list ) ;

        while( !adj_list.empty() ) {
            adj_vertex = adj_list.front() ;
            adj_list.pop() ;

            adj_weight = adj_list.front() ;
            adj_list.pop() ;

            if ( ( Dijkstra_VertexArr[min_dis_vertex].distance + adj_weight )
            < Dijkstra_VertexArr[adj_vertex].distance ) {
                Dijkstra_VertexArr[adj_vertex].predecessor = min_dis_vertex ;
                Dijkstra_VertexArr[adj_vertex].distance = Dijkstra_VertexArr[min_dis_vertex].distance + adj_weight ;
            }
        }
    }
}

void Dijkstra::print_all_shortest() {
    for ( int i = 0 ; i < graph_size ; i++ ) {
        cout << "Vertex " << graph.find_vertex( i ) << " distance = " ;
        cout << Dijkstra_VertexArr[i].distance << endl ;
    }

    cout << endl << endl ;
}

void Dijkstra::clear() {
    delete[] Dijkstra_VertexArr ;
}