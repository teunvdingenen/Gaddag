#include <assert.h>
#include <vector>
#include <stdlib.h>

namespace gaddag {

const static unsigned int BOARDSIZE = 15;

typedef std::pair < unsigned int, unsigned int > Position;

const static char ALPHABET[] = 
    {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

typedef std::vector< char > Rack; 

static bool remove( char c, Rack& r ) {
    std::vector< char >::iterator it;
    for( it = r.begin( ); it != r.end( ); it++ ) {
        if( (*it) == c ) {
            r.erase( it );
            return true;
        }
    }
    return false;
}

static inline bool is_edge( Position p ) {
    return ( p.first == 14  || p.second == 14 || p.first == 0 || p.second == 0 );
}

static inline Position above( Position p ) { 
    Position r = p;
    r.second = r.second - 1;
    return r;
}
static inline Position below( Position p ) { 
    Position r = p;
    r.second = r.second + 1;
    return r;
}

static inline Position left( Position p ) { 
    Position r = p;
    r.first = r.first - 1;
    return r;
}

static inline Position right( Position p ) { 
    Position r = p;
    r.first = r.first - 1;
    return r;
}

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum MultiplierType {
        NONE = 0,
        DOUBLELETTER = 1,
        DOUBLEWORD = 2,
        TRIPLELETTER = 3,
        TRIPLEWORD = 4
};
const static int multiplier_matrix[BOARDSIZE][BOARDSIZE] = 
{ 
{ 4,0,0,1,0,0,0,4,0,0,0,1,0,0,4 },
{ 0,2,0,0,0,3,0,0,0,3,0,0,0,2,0 },
{ 0,0,2,0,0,0,1,0,1,0,0,0,2,0,0 },
{ 1,0,0,2,0,0,0,1,0,0,0,2,0,0,1 },
{ 0,0,0,0,2,0,0,0,0,0,2,0,0,0,0 },
{ 0,3,0,0,0,3,0,0,0,3,0,0,0,3,0 },
{ 0,0,1,0,0,0,1,0,1,0,0,0,1,0,0 },
{ 4,0,0,1,0,0,0,0,0,0,0,1,0,0,4 },
{ 0,0,1,0,0,0,1,0,1,0,0,0,1,0,0 },
{ 0,3,0,0,0,3,0,0,0,3,0,0,0,3,0 },
{ 0,0,0,0,2,0,0,0,0,0,2,0,0,0,0 },
{ 1,0,0,2,0,0,0,1,0,0,0,2,0,0,1 },
{ 0,0,2,0,0,0,1,0,1,0,0,0,2,0,0 },
{ 0,2,0,0,0,3,0,0,0,3,0,0,0,2,0 },
{ 4,0,0,1,0,0,0,4,0,0,0,1,0,0,4 }
};

static const MultiplierType
get_multiplier( Position p ) {
    switch( multiplier_matrix[ p.first ][ p.second ] ) {
        case 0:
            return NONE;
        case 1:
            return DOUBLELETTER;
        case 2:
            return DOUBLEWORD;
        case 3:
            return TRIPLELETTER;
        case 4:
            return TRIPLEWORD;
        default:
            assert( 0 );
    }
}


static unsigned int
get_char_value( const char c ) {
    switch( c ) {
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'N':
        case 'R':
        case 'T':
        case 'L':
        case 'S':
        case 'U':
            return 1;
            break;
        case 'D':
        case 'G':
            return 2;
            break;
        case 'B':
        case 'C':
        case 'M':
        case 'P':
            return 3;
            break;
        case 'F':
        case 'H':
        case 'V':
        case 'W':
        case 'Y':
            return 4;
            break;
        case 'K':
            return 5;
            break;
        case 'J':
        case 'X':
            return 8;
            break;
        case 'Q':
        case 'Z':
            return 10;
            break;
        case ' ':
            return 0;
            break;
        default:
            assert( 0 );
            return 0;
            break;
    }
}

static Position calculate_position( int offset, size_t direction, Position s ) {
    Position r = s;
    if( direction ) { // vertical
        r.second += offset;
    } else {            // horizontal
        r.first += offset;
    }
    return r;
}

/* Letter distritubtion
A: 9
B: 2
C: 2
D: 4
E: 12
F: 2
G: 3
H: 2
I: 9
J: 1
K: 1
L: 4
M: 2
N: 6
O: 8
P: 2
Q: 1
R: 6
S: 4
T: 6
U: 4
V: 2
W: 2
X: 1
Y: 2
Z: 1
@: 2
*/

/*English:
    1 point:
        E 12x
        A 9x
        I 9x
        O 8x
        N 6x
        R 6x
        T 6x
        L 4x
        S 4x
        U 4x
    2 points:
        D 4x
        G 3x
    3 points:
        B 2x
        C 2x
        M 2x
        P 2x
    4 points:
        F 2x
        H 2x
        V 2x
        W 2x
        Y 2x
    5 points:
        K 1x
    8 points:
        J 1x
        X 1x
    10 points:
        Q 1x
        Z 1x
*/
};
