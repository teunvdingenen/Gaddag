#include "Graph.h"

namespace gaddag {

class GraphSearch {

public:

    GraphSearch( Position p, size_t direction, const Board& b, const Graph& g )
        : 
        m_start( p ),
        m_direction( direction ),
        m_board( b ),
        m_graph( g )
        {}

    void generate( int offset, std::string word, Rack r, Node* n );

    bool has_plays( );

    std::pair< std::string, unsigned int > get_best_play( );

private:

    bool allowed( const char c, Position p ) const;

    void proceed( int offset, char c, std::string word, Rack r, 
                Node* new_node, Node* old_node );

    bool check( const char c, const Node* last, Node* current, Position p, int offset, 
                        size_t direction) const ;
    
    bool record_play( std::string word );

    const Position m_start;
    const size_t m_direction; // 0 = horizontal, 1 = vertical
    std::vector< std::pair< std::string, unsigned int > > plays;
    const Board& m_board;
    const Graph& m_graph;

};


}//end namespace
