# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <string>

using namespace std ;

struct adjacent {
    string net_name ;
    int weight ;
} ;

struct Instance {
    string instance_name ;
    vector <adjacent> out ;
} ;

class Graph {
private:
    vector <Instance> edges ;

public:
    bool is_empty() {
        if ( edges.empty() ) return true ;
        else return false ;
    }

    int find_instance( string name ) {
        for ( int i = 0 ; i < edges.size() ; i++ ) 
            if ( name == edges[i].instance_name ) return i ;
        
        return -1 ; // not found
    }

    void append_edge( string edge_name ) {
        Instance push_in ;
        push_in.instance_name = edge_name ;
        edges.push_back( push_in ) ;
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
        }
    }
}

void read_file( Graph & circuit_graph ) {
    ifstream ifs ;
    stringstream ss ;
    string line, instance_name, net ;

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
                circuit_graph.append_edge( instance_name ) ;
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
                // split (,)
                // store net
            }
        }
    }
}