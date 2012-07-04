#include "Board.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace gaddag {

struct Node;
struct Edge {
   
    char value;
    Node* to_node;
    Node* from_node;
};

struct Node {

    Node* follow( char c ) const {
        std::unordered_map<char,Edge*>::const_iterator it = out_edges.find( c );
        if( it != out_edges.end() ) return out_edges.at( c )->to_node;
        else return 0;
    }

    bool contains_final_char( char c ) const {
        std::vector<char>::const_iterator it;
        for( it = final_char.begin( ); it != final_char.end( ); it++ ) {
            if( (*it) == c ) {
                return true;
            }
        }
        return false;
    }

    Edge* contains_out_edge( char c ) const { 
        std::unordered_map<char,Edge*>::const_iterator it = out_edges.find( c );
        if( it!= out_edges.end( ) ) return out_edges.at( c );
        else return 0;
    }

    std::vector<char> final_char;
    std::unordered_map<char, Edge*> out_edges;

};

class Graph {

    public:
        bool init( std::string lexicon );
        Node* get_initial_node( ) const;

        Node* add_arc( Node* st, char c );
        Node* add_final_arc( Node* st, char c, char a );
        bool force_arc( Node* st, char c, Node* final_state );

    private:
        Node* m_initial_node;

};

};
