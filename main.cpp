//Scrabble demo
#include "src/GraphSearch.h"
#include <iostream>
#include <vector>
#include <string.h>

using namespace gaddag;

std::vector< Position > playable_positions;

struct Play {
    unsigned int score;
    std::string word;
    unsigned int direction;
    Position pos;
};

void print_help( ) {
    std::cout << "Usage: play_scrabble <dictionary>" << std::endl;
    exit( 0 );
}

static bool print_board( Board b ) {
    std::cout << "=================================================" << std::endl;
    for( unsigned int y=0; y<BOARDSIZE; y++ ) {
        Position p; p.second=y;
        std::cout << "||";
        for( unsigned int x=0; x<BOARDSIZE; x++ ) {
            p.first=x;
            const char to_print = b.get( p );
            if( !to_print ) {
                std::cout << "   ";
            } else {
                std::cout << " " << to_print << " ";
            }
        }
        std::cout << "||" << std::endl;
    }
    std::cout << "=================================================" << std::endl;
}

unsigned int lay_word( Board& b, Rack*& r, Play p ) {
    bool delimiter_seen = false;
    unsigned int offset = 0;
    unsigned int score = 0;
    unsigned int word_multiplier = 1;
//    Position p1,p2;

    for( size_t t=0; t<p.word.size( ); t++ ) {
        char to_put = p.word.at( t );
        if( to_put == '$' ) {
            delimiter_seen = true;
//            p1 = calculate_position( offset-1, p.direction, p.pos );
            offset = 1;
            continue;
        }
        Position current_pos = calculate_position( offset, p.direction, p.pos );
        playable_positions.push_back( current_pos );

        MultiplierType m = get_multiplier( current_pos );
        char char_on_position = b.get( current_pos );

        if( char_on_position ) {
            //TODO do I count this tile on score?
            if( char_on_position != to_put ) {
                assert( 0 );
                return false;
            }
            score += get_char_value( char_on_position );
        } else { //no tile on position
            unsigned int letter_multiplier = 1;
            switch( m ) {
                case NONE:
                    break;
                case DOUBLELETTER:
                    letter_multiplier = 2;
                    break;
                case DOUBLEWORD:
                    word_multiplier = 2;
                    break;
                case TRIPLELETTER:
                    letter_multiplier = 3;
                    break;
                case TRIPLEWORD:
                    word_multiplier = 3;
                    break;
            }
            if( !remove( to_put, *r ) ) {
                // char is not on rack
                if( !remove( ' ', *r ) ) {
                    //also no blanks... panic
                    assert( 0 );
                } else {
                    b.put( current_pos, ' ' ); //blank
                }
            } else { //char succesfully removed from rack
                b.put( current_pos, to_put );
                score += ( get_char_value( to_put ) * letter_multiplier );
            }
        }
        if( delimiter_seen ) { 
            offset++;
//            p2 = calculate_position( offset, p.direction, p.pos );
        }
        else offset--;
    }
    score *= word_multiplier;
//    playable_positions.push_back( p1 );
//    playable_positions.push_back( p2 );
//    assert( score == p.score );
    return score;
}

bool play_game( Graph& g, bool verbose ) {
    Board b;

    bool game = true;
    bool player0 = true;

    unsigned int player0_score = 0;
    unsigned int player1_score = 0;

    Rack player0_rack;
    Rack player1_rack;

    for( size_t t=0; t<7; t++ ) {
        player0_rack.push_back( b.get_letter_from_pool( ) );
        player1_rack.push_back( b.get_letter_from_pool( ) );
    }

    Position p; p.first = 7; p.second = 7;

    playable_positions.clear( );
    playable_positions.push_back( p );
    
    while( game ) {
        Play best_play;
        best_play.score = 0;

        Rack* r;
        if( player0 ) r = &player0_rack;
        else r = &player1_rack;

        std::vector< Position >::iterator it;

        for( it= playable_positions.begin( ); it!= playable_positions.end( ); it++ ) {
            GraphSearch g_search_h( (*it), 0, b, g );//horizontal
            GraphSearch g_search_v( (*it), 1, b, g );//vertical
            
            g_search_h.generate( 0, "", *r, g.get_initial_node( ) );
            g_search_v.generate( 0, "", *r, g.get_initial_node( ) );

            if( g_search_h.has_plays( ) ) {
                std::pair< std::string, unsigned int > some_play = g_search_h.get_best_play( );
                if( some_play.second > best_play.score ) {
                    best_play.score = some_play.second;
                    best_play.word = some_play.first;
                    best_play.direction = 0;
                    best_play.pos = (*it);
                }
            }
            if( g_search_v.has_plays( ) ) {
                std::pair< std::string, unsigned int > some_play = g_search_v.get_best_play( );
                if( some_play.second > best_play.score ) {
                    best_play.score = some_play.second;
                    best_play.word = some_play.first;
                    best_play.direction = 1;
                    best_play.pos = (*it);
                }
            }
        }
        unsigned int scored = lay_word( b, r, best_play );
        player0 ? player0_score += scored : player1_score += scored ;

        if( verbose ) {
            std::string player_string;
            unsigned int *relevant_score;

            if( player0 ) {
                player_string = "Player 0 ";
                relevant_score = &player0_score;
            } else {
                player_string = "Player 1 ";
                relevant_score = &player1_score;
            }

            std::cout << player_string  << "put down: " << best_play.word
                << " for " << scored << " points." << std::endl;
            std::cout << "Score is now: " << *relevant_score << std::endl;
        }

        while( r->size( ) < 7 && !b.pool_is_empty( ) ) r->push_back( b.get_letter_from_pool( ) );
        player0 = !player0;

        if( b.pool_is_empty( ) ) game = false;
    }

    std::string winner_string, loser_string;
    unsigned int winner_score, loser_score;

    if( player0_score > player1_score ) {
        winner_string = "Player0 ";
        loser_string = "Player1 ";
        winner_score = player0_score;
        loser_score = player1_score;
    } else {
        winner_string = "Player1 ";
        loser_string = "Player0 ";
        winner_score = player1_score;
        loser_score = player0_score;
    }
       
    std::cout << "=============== GAME OVER ===============" << std::endl;
    std::cout << winner_string << "wins with a score of " << winner_score << " points." << std::endl;
    std::cout << loser_string << "has " << loser_score << " points." << std::endl;
    std::cout << std::endl << "The Board looks like this: " << std::endl;
    print_board( b );

    std::cout << std::endl << std::endl;

}


int main( int argc, char* argv[] ) {
    bool verbose = false;
    if( argc == 1 ) print_help( );
    for( size_t t=1; t<argc-1; t++ ){
        if( !strcmp( argv[ t ], "-v" ) ) verbose = true;
    }
    Graph g;
    g.init( argv[ argc-1 ] );

    bool close = false;

    do {
        play_game( g, verbose );
        std::cout << "Play again? Y/N " ;
        char c;
        std::cin >> c;
        if ( c == 'n' || c == 'N' ) close = true;
    } while( !close );

}
