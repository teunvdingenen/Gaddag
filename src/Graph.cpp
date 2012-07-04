#include "Graph.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <assert.h>
#include <memory>

#define PRINT_PROGRESS 1

namespace gaddag { 
bool
Graph::init( std::string lexicon ) {
    m_initial_node = new Node( );

    std::ifstream infile;
    infile.open( lexicon.c_str() );
    char word[64];


#if PRINT_PROGRESS
    size_t word_count=0;
    std::cout << "Initializing GADDAG, this may take a while" << std::endl;
    std::cout << "[ ";
#endif

    if( infile.is_open( ) ) {
        while( !infile.eof( ) ) {
#if PRINT_PROGRESS
            if( word_count % 2000 == 0 ) std::cout << "-";
            word_count++;
#endif
            Node *current_state = m_initial_node;
            infile.getline( word, 64 );
            if( strlen( word ) < 2 ) continue;

            for( int i=strlen( word )-1; i >= 2; i-- ) {
                current_state = add_arc( current_state, word[ i ] );
            }
            current_state = add_final_arc( current_state, word[ 1 ], word[ 0 ] );


            current_state = m_initial_node; // reset state to initial node;

            for ( int i = strlen( word )-2; i >= 0; i-- ) {
                current_state = add_arc( current_state, word[ i ] );
            }
            current_state = add_final_arc( current_state, '$', word[ strlen( word ) - 1 ] );


            for ( int j = strlen( word )-3; j >= 0; j-- ) {
                Node* force_state = current_state;
                current_state = m_initial_node; //reset initial to initial_node
                for ( int i = j; i >= 0; i-- ) {
                    current_state = add_arc( current_state, word[ i ] );
                }
                current_state = add_arc( current_state, '$');
                force_arc( current_state, word[ j+1 ], force_state );
            }

        }
        infile.close( );
    } else {
        assert( 0 );
        return false;
    }
#if PRINT_PROGRESS
    std::cout << " ] " << std::endl;
    std::cout << "Finished loading: " << word_count << " words." << std::endl;
#endif
    return true;
}

inline Node* create_arc( Node* from, char c ) {
    std::auto_ptr<Edge> new_edge( new Edge( ) );
    std::auto_ptr<Node> new_node( new Node( ) );
    new_edge->value = c;
    new_edge->from_node = from;
    new_edge->to_node = new_node.release( );
    Node* new_state = new_edge->to_node;
    from->out_edges[ c ] = new_edge.release( );
    return new_state;
}

//Add arc from st for char c (if it does not already exist)
// reset st to the node it leads to
Node*
Graph::add_arc( Node* st, char c ) {
    Edge* some_edge = st->contains_out_edge( c );
    if( some_edge ) {
        st = some_edge->to_node;
    } else {
        st = create_arc( st, c );
    }
    return st;
}

//adds an arc from st for c ( if it does not already exist ) and adds
// a to the letter set of this arc ????
Node*
Graph::add_final_arc( Node* st, char c, char a ) {
    Edge* some_edge = st->contains_out_edge( c );
    if( !some_edge ) {
        st = create_arc( st, c );    
    } else {
        st = some_edge->to_node;
    }
    if( st->contains_final_char( a ) ) {
        //This can't happen??
        assert( 0 );
        return 0;
    } else 
        st->final_char.push_back( a );
    return st;
}

//add an arc from st for char c to fst ( error if the arc exists to any
// other state
bool
Graph::force_arc( Node* st, char c, Node* force_state ) {
    Edge* some_edge = st->contains_out_edge( c );
    if( some_edge ) {
        if( some_edge->to_node != force_state ) { //has at least one letters on the letter set
            assert( 0 );
            return false;
        }
    } else {
        std::auto_ptr<Edge> new_edge( new Edge( ) );
        new_edge->value = c;
        new_edge->from_node = st;
        new_edge->to_node = force_state;
        st->out_edges[ c ] = new_edge.release( );
    }
    return true;
}

Node*
Graph::get_initial_node( ) const {
    return m_initial_node;
}

} //end namespace
