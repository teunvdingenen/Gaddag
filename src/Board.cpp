//Scrabble board
#include "Board.h"
#include <stdlib.h>
#include <time.h>

namespace gaddag {

Board::Board( ) {
    for( unsigned int i=0; i<BOARDSIZE; i++ ) {
        for( unsigned int j=0; j<BOARDSIZE; j++ ) {
            m_board[i][j]=0;
        }
    }
    fill_pool( );
    // initialize random seed (for grabbing from pool)
    srand( time(NULL) );
}
/*
const char**
Board::get_board( ) const {
    return &m_board;
}
*/

bool
Board::fill_pool( ) {
    for( size_t t = 0; t < 9; t++ ) { 
        m_pool.push_back( 'A' ); m_pool.push_back( 'I' ); 
    }
    for( size_t t = 0; t < 2; t++ ) { 
        m_pool.push_back( 'B' ); m_pool.push_back( 'C' ); m_pool.push_back( 'F' ); m_pool.push_back( 'H' ); 
        m_pool.push_back( 'M' ); m_pool.push_back( 'P' ); m_pool.push_back( 'V'); m_pool.push_back( 'W' ); 
        m_pool.push_back( 'Y' ); m_pool.push_back( ' ' );
    }
    for( size_t t = 0; t < 4; t++ ) { 
        m_pool.push_back( 'D' ); m_pool.push_back( 'L' ); m_pool.push_back( 'S' );m_pool.push_back( 'U' ); 
    }
    for( size_t t = 0; t < 12; t++ ) m_pool.push_back( 'E' );
    for( size_t t = 0; t < 3; t++ ) m_pool.push_back( 'G' );
    for( size_t t = 0; t < 6; t++ ) { m_pool.push_back( 'N' ); m_pool.push_back( 'R' ); m_pool.push_back( 'T' ); }
    for( size_t t = 0; t < 8; t++ ) m_pool.push_back( 'O' );
    m_pool.push_back( 'J' );
    m_pool.push_back( 'K' );
    m_pool.push_back( 'Q' );
    m_pool.push_back( 'X' );
    m_pool.push_back( 'Z' );

    assert( m_pool.size( ) == 100 );
    return m_pool.size( ) == 100;
}

const char
Board::get_letter_from_pool( ) {
    size_t t =  rand( ) % (m_pool.size( ));
    const char c = m_pool.at( t );
    m_pool.erase( m_pool.begin( ) + t );
    return c;
}

const bool
Board::has_letter( Position p ) const {
    if( p.first > BOARDSIZE || p.second > BOARDSIZE ) return false;
    return !(m_board[p.first][p.second] == 0 );
}

const bool
Board::has_room( Position p ) const {
    if( p.first >= BOARDSIZE || p.second >= BOARDSIZE ) return false;
    return (m_board[p.first][p.second] == 0 );
}

const char
Board::get( Position p ) const {
    if( p.first > BOARDSIZE - 1 || p.second > BOARDSIZE -1 ) return 0;
    return m_board[p.first][p.second];
}

bool
Board::put( Position p, char c ) {
    if( !has_room( p ) ) return false;
    m_board[p.first][p.second] = c;
    return true;
}

} //end namespace
