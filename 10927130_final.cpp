# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <string>

using namespace std ;

struct adjacent {
    string net_name ;
    string direction ;
    int weight ;
} ;

struct Instance {
    string instance_name ;
    vector <adjacent> out ;
} ;

class Graph {
private:
    vector <Instance> vertices ;

public:
    bool is_empty() {
        if ( vertices.empty() ) return true ;
        else return false ;
    }

    int find_vertex( string name ) {
        for ( int i = 0 ; i < vertices.size() ; i++ ) 
            if ( name == vertices[i].instance_name ) return i ;
        
        return -1 ; // not found
    }

    void print_all() {
        for ( int i = 0 ; i < vertices.size() ; i++ ) {
            cout << "INSTANCE" << vertices[i].instance_name << endl ;

            for ( int j = 0 ; j < vertices[i].out.size() ; j++ ) {
                cout << "\t" << vertices[i].out[j].net_name << " : " ;
                cout << vertices[i].out[j].weight << ", " << vertices[i].out[j].direction ;
            }

            cout << endl ;
        }
    }

    void append_vertex( string vertex_name ) {
        Instance push_in ;
        push_in.instance_name = vertex_name ;
        vertices.push_back( push_in ) ;
    }

    // find edge and update direction data( adjacent list )
    void update_direction( string name ) {
        for ( int i = 0 ; i < vertices.size() ; i++ ) {
            for ( int j = 0 ; j < vertices[i].out.size() ; j++ ) {
                // find where the net start
                if ( name == vertices[i].out[j].net_name )
                    // and the direction is last vertex( current process instance )
                    vertices[i].out[j].direction = vertices[vertices.size() - 1].instance_name ;
            }
        }
    }

    void update_AdjacentList( string edge_name, int edge_weight, bool out ) {
        // bug : ( the same net ) out first(out->in) is fine, but in first can't find where the net from
        if ( out ) {
            adjacent push_in ;
            push_in.net_name = edge_name ;
            push_in.weight = edge_weight ;
            vertices[vertices.size() - 1].out.push_back( push_in ) ;
        }

        else update_direction( edge_name ) ;
    }
} ;



void read_file( Graph & circuit_graph ) ;



int main() {
    int mode = -1 ;
    Graph circuit_graph ;

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
                    // dfs
                    break ;

                case 2 :
                    // dijkstra
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
            if ( line == "ENDCIRCUIT" ) break ;

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

        circuit_graph.print_all() ;
    }
}