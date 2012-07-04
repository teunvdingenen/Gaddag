#include "GraphSearch.h"

namespace gaddag {

void
GraphSearch::generate( int offset, std::string word, Rack r, Node* n ) {
    Position p = calculate_position( offset, m_direction, m_start );
    if( m_board.has_letter( p ) ) {
        const char c = m_board.get( p );
        proceed( offset, c, word, r, n->follow( c ), n );
    } else {
        Rack::iterator it;
        for( it = r.begin( ); it!= r.end( ); it++ ) {
            if( (*it) == ' ' ) {
                for( size_t t = 0; t < 26; t++ ) {
                    if( allowed( ALPHABET[ t ], p ) ) {
                        char c = ALPHABET[ t ];
                        Rack new_rack = r;
                        remove( ' ', new_rack );
                        proceed( offset, c, word, new_rack, n->follow( c ), n );
                    }
                }
            } else if ( allowed( *it, p ) ) {
                char c = *it;
                Rack new_rack = r;
                remove( c, new_rack );
                proceed( offset, c, word, new_rack, n->follow( c ), n );
            }
        }
    }
}

void
GraphSearch::proceed( int offset, char c, std::string word, Rack r, Node* new_node, Node* old_node ) {
    if( offset <= 0 ) {             //moving :: left or up
        word.append( 1, c );
        if( old_node->contains_final_char( c ) && 
                !m_board.has_letter( calculate_position( offset-1, m_direction, m_start ) ) ) { 
            record_play( word );
        } if( new_node ) {
            Position p = calculate_position( offset - 1, m_direction, m_start ); // position to the left / up
            if( m_board.has_room( p ) ) generate( offset-1, word, r, new_node );
            new_node = new_node->follow( '$' );
            if( new_node && !m_board.has_letter( p ) && 
                    m_board.has_room( calculate_position( 1, m_direction, m_start ) ) ) {
                word.append( 1, '$' );
                generate( 1, word, r, new_node );
            }
        } 
    } else { // if offset > 0
        word.append( 1, c );
        if( old_node->contains_final_char( c ) && 
                !m_board.has_letter( calculate_position( offset+1, m_direction, m_start ) ) ) {
            record_play( word );
        } if( new_node && m_board.has_room( calculate_position( offset+1, m_direction, m_start ) ) ) {
            generate( offset+1, word, r, new_node );
        }
    }
}

bool
GraphSearch::check( const char c, const Node* last, Node* current, 
                        Position p, int offset, size_t direction ) const {
    if( !current ) return false;
    char next = m_board.get( calculate_position( offset, direction, p ) );
    if( offset < 0 ) {
        if( next ) {
            return check( next, current, current->follow( next ), p, offset-1, direction );
        } else {
            return check( c, current, current->follow( '$' ), p, 1, direction );
        }
    } else if ( offset > 0 ) {
        if( next ) {
            return check( c, current, current->follow( c ), p, offset+1, direction );
        } else {
            return last->contains_final_char( c );
        }
    } else {
        //this can't happen
        assert( 0 );
        return false;
    }
}

bool
GraphSearch::allowed( const char c, Position p ) const {
//    Not added because shouldn't be a problem here
//    if( p.first >= BOARDSIZE || p.second >= BOARDSIZE ) return false;

    const Node* n = m_graph.get_initial_node( );
    if( m_direction ) { //laying word vertically, check horizonal
        if( m_board.has_letter( calculate_position( -1, 0, p ) ) ) { // left
            return check( c, n, n->follow( c ), p, -1, 0 );
        } else if( m_board.has_letter( calculate_position( 1, 0, p ) ) ) { // right
            return check( c, n, n->follow( c ), p, 1, 0 );
        } else {
            return true;
        }
    } else {
        if( m_board.has_letter( calculate_position( 1, 1, p ) ) ) { // above
            return check( c, n, n->follow( c ), p, -1, 1 );    
        } else if( m_board.has_letter( calculate_position( -1, 1, p ) ) ) { // below
            return check( c, n, n->follow( c ), p, 1, 1 );
        } else {
            return true;
        }
    }
}

bool 
GraphSearch::record_play( std::string word ) {
    //TODO identify blanks; score+=0
    bool negate = true;
    size_t offset = 0;
    size_t word_multiplier = 1;
    size_t score = 0;

    for( size_t t=0; t< word.length( ); t++ ) {
        if( word.at( t ) == '$' ) {
            negate = false;
            offset = 1;
        } else {
            Position p = calculate_position( offset, m_direction, m_start );
            size_t ch_val = get_char_value( word.at( t ) );
            if( !m_board.get( p ) ) { // letter is not already on board
                size_t mult = multiplier_matrix[ p.first ][ p.second ];
                switch (mult) {
                    case 0:
                        break;
                    case 1:
                        ch_val *= 2;
                        break;
                    case 2:
                        word_multiplier = 2;
                        break;
                    case 3:
                        ch_val *= 3;
                        break;
                    case 4:
                        word_multiplier = 3;
                        break;
                }        
            }
            score += ch_val;
            if( negate ) offset -= 1;
            else offset += 1;
        }
    }
    score *= word_multiplier;
    std::pair< std::string, unsigned int > my_play;
    my_play.first = word;
    my_play.second = score;
        
    std::vector< std::pair< std::string, unsigned int > >::iterator it = plays.begin( );
    while( it != plays.end( ) ) {
        if( (*it).second < score ) { 
            plays.insert( it, my_play );
            return true;
        }
        it++;
    }
    plays.insert( plays.end( ), my_play );
    return true;
}

bool
GraphSearch::has_plays( ) {
    return plays.size( ) > 0;
}

std::pair< std::string, unsigned int >
GraphSearch::get_best_play( ) {
    if( plays.size( ) == 0 ) assert( 0 );
    return plays[ 0 ];
}

};
