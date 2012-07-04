// Scrabble Board
#include <string>
#include "defines.h"

namespace gaddag {


class Board {

    public:
        Board( );

//        const char** get_board( ) const;

        const bool has_letter( Position p ) const;

        const bool has_room( Position p ) const;

        const char get( Position p ) const;

        const char get_letter_from_pool( );

        bool put( Position p, char c );

        bool pool_is_empty( ) const {
            return m_pool.size( ) == 0;
        }

    private:
        bool fill_pool( );
        
    private:
        char m_board[BOARDSIZE][BOARDSIZE];
        std::vector< char > m_pool;
    };
};
