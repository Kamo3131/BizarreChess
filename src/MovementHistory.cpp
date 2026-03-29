#include "MovementHistory.hpp"

/**
 * @brief Default constructor, initializes current node with nullptr.
 * @param maxNodesSaved initializes m_maxNodesSaved.
 */
MovementHistory::MovementHistory() {}
/**
 * @brief Adds new board to the movement history.
 * @param board is added to m_history.
 */
void MovementHistory::pushNew(const ChessBoard& board){
    std::size_t currentSize = m_history.size();
    if(currentSize > 0 && m_currentIndex < (currentSize - 1)) {
        for(std::size_t i = currentSize - 1; i > m_currentIndex; --i){
            popLast();
        }
    }
    ChessboardData boardData;
    boardData.x = board.getHorizontal();
    boardData.y = board.getVertical();

    for(std::size_t i = 0; i < boardData.x; ++i){
        for(std::size_t j = 0; j < boardData.y; ++j){
            std::unique_ptr<Piece> & piecePtr = board.getPiece(i, j);
            if(piecePtr){
                PieceDataContainer piece;
                piece.m_position = std::make_pair(i, j);
                piece.m_team = piecePtr->getTeam();
                piece.m_type = piecePtr->getType();
                piece.m_move_number = piecePtr->getMoveNumber();
                piece.m_damage = piecePtr->getDamage();
                piece.m_health = piecePtr->getHealth(); 
                boardData.m_pieces.emplace_back(piece);
            }
        }
    }
    m_history.emplace_back(std::move(boardData));
    m_currentIndex = m_history.size() - 1;
}

/**
 * @brief Creates board from the data at current index.
 */
ChessBoard MovementHistory::getBoard() const {
    std::size_t x = m_history.at(m_currentIndex).x;
    std::size_t y = m_history.at(m_currentIndex).y;
    FastVector<PieceDataContainer, 64> * pieces = &m_history.at(m_currentIndex).m_pieces;
    ChessBoard board(x, y);
    std::size_t size = pieces->size();
    for(std::size_t i = 0; i < size; ++i){
        PieceDataContainer & pieceRef = pieces->at(i);
        std::unique_ptr<Piece> piece;
        switch(pieceRef.m_type){
            case Piece::Type::PAWN:
                piece = std::make_unique<Pawn>(pieceRef.m_team);
                break;
            case Piece::Type::BISHOP:
                piece = std::make_unique<Bishop>(pieceRef.m_team);
                break;
            case Piece::Type::ROOK:
                piece = std::make_unique<Rook>(pieceRef.m_team);
                break;
            case Piece::Type::KNIGHT:
                piece = std::make_unique<Knight>(pieceRef.m_team);
                break;
            case Piece::Type::QUEEN:
                piece = std::make_unique<Queen>(pieceRef.m_team);
                break;
            case Piece::Type::KING:
                piece = std::make_unique<King>(pieceRef.m_team);
                break;
        }
        if(piece){
            piece->setMoveNumber(pieceRef.m_move_number);
            piece->setDamage(pieceRef.m_damage);
            piece->setHealth(pieceRef.m_health);
            board.setPiece(pieceRef.m_position.first, pieceRef.m_position.second, std::move(piece));
        }
    }
    return board;
}
/**
 * @brief pops last element of m_history, back.
 */
void MovementHistory::popLast() {
    m_history.pop_back();
}
/**
 * @brief pops first element of m_history.
 */
void MovementHistory::popFirst() {
    m_history.pop_front();
}
/**
 * @brief clears m_history from all elements.
 */
void MovementHistory::clearAll() {
    m_history.clear();
}
/**
 * @brief returns element at given position.
 * @param index is given position. 
 */
ChessboardData& MovementHistory::at(std::size_t index) const {
    return m_history.at(index);
}
/**
 * @brief sets current index.
 * @param new_index
 */
void MovementHistory::setIndex(std::size_t new_index) {
    if (new_index >= m_history.size()) {
        return;
    }
    m_currentIndex = new_index;
}
/**
 * @brief gets current index.
 */
std::size_t MovementHistory::getIndex() const {
    return m_currentIndex;
}
/**
 * @brief returns chessboard to state of board located somewhere in history. 
 * After moving in this state, every next move is overwritten and cannot be redo. 
 * Can't be used if m_currentIndex == 0.
 */
void MovementHistory::undo() {
    if(canUndo()){
        --m_currentIndex;
    }

}
bool MovementHistory::canUndo() const{
    return m_currentIndex > 0;
}
/**
 * @brief goes back to the next index, if current index isn't the current size of m_history.
 */
void MovementHistory::redo() {
    if (canRedo()) {
        ++m_currentIndex;
    }
}
bool MovementHistory::canRedo() const{
    return !m_history.empty() && m_currentIndex < m_history.size() - 1;
}