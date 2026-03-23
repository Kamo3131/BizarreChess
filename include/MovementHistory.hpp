#include <iostream>
#include <vector>
#include "Piece.hpp"
#include "FastVector.hpp"
#include "ChessBoard.hpp"


struct PieceDataContainer {
    std::pair<std::size_t, std::size_t> m_position = {0, 0};
    Piece::Team m_team = Piece::Team::WHITE;
    Piece::Type m_type = Piece::Type::PAWN;
    int m_move_number = 0;
    int m_damage = 1;
    int m_health = 1;    
};
struct ChessboardData {
    FastVector<PieceDataContainer, 64> m_pieces;
};
/**
 * @brief Movement history of fixed number of turns, that happened before current turn.
 */
class MovementHistory {
    public:
    /**
     * @brief Default constructor, initializes current node with nullptr.
     * @param maxNodesSaved initializes m_maxNodesSaved.
     */
    MovementHistory();
    /**
     * @brief Adds new board to the movement history.
     * @param board is added to m_history.
     */
    void pushNew(const ChessBoard & board);
    /**
     * @brief pops last element of m_history.
     */
    void popLast();
    /**
     * @brief pops first element of m_history.
     */
    void popFirst();
    /**
     * @brief clears m_history from all elements.
     */
    void clearAll();
    /**
     * @brief returns element at given position.
     * @param index is given position. 
     */
    ChessboardData& at(std::size_t index) const;
    /**
     * @brief sets current index.
     * @param new_index
     */
    void setIndex(std::size_t new_index);
    /**
     * @brief gets current index.
     */
    std::size_t getIndex() const;
    /**
     * @brief returns chessboard to state of board located somewhere in history. 
     * After moving in this state, every next move is overwritten and cannot be redo. 
     * Can't be used if m_currentIndex == 0.
     */
    void undo();
    /**
     * @brief goes back to the next index, if current index isn't the current size of m_history.
     */
    void redo();
    private:
    FastVector<ChessboardData, 500> m_history;
    std::size_t m_currentIndex = 0;
};