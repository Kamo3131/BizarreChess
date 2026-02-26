#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "Piece.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Queen.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"
#include "Pawn.hpp"


class ChessBoard{
    public:
    using size = std::size_t;
    using size_pair = std::pair<std::size_t, std::size_t>;
    /**
     * @brief Default constructor. Initializes _horizontal and _vertical with 8.
     */
    ChessBoard();
    
    /**
     * @brief Constructor for chessboards with different sizes.
     * @param horizontal sets horizontal tile number, should not exceed 26
     * @param vertical sets vertical lenght, should not exceed 26
     */
    ChessBoard(const size horizontal, const size vertical);

    /**
     * @brief Inits chessboard
     */
    void init();
    /**
     * @brief Inits pieces on the board.
     */
    void initPieces();
    /**
     * @brief Resets chessboard.
     */
    void reset();
    /**
     * @brief Displays codes of the squares. Does not take parameters.
     */
    void printSquares(std::ostream& os = std::cout) const;

    /**
     * @brief Displays current state of chessboard.
     */
    void printBoard(std::ostream& os = std::cout) const;

    /**
     * @brief Moves figure by x,y.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @param x shift from original x (positive values to right, negative to left)
     * @param y shift from original y (positive values to top, negative to bottom)
     */
    void move(const std::size_t o_x, const std::size_t o_y, const int x, const int y);
    
    /**
     * @brief Check if piece can move to the target square.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @param x shift from original x (positive values to right, negative to left)
     * @param y shift from original y (positive values to top, negative to bottom)
     * @return true if piece can move to the target square, false otherwise
     */
    bool canMove(const std::size_t o_x, const std::size_t o_y, const int x, const int y) const;
    
    /**
     * @brief Check if piece can attack to the target square.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @param x shift from original x (positive values to right, negative to left)
     * @param y shift from original y (positive values to top, negative to bottom)
     * @return true if piece can move to the target square, false otherwise
     * @note Created mostly for use in inCheck function, so it does not loop itself.
     */
    bool canAttackKing(const std::size_t o_x, const std::size_t o_y, const int x, const int y) const;
    /**
     * @brief Checks if there is any piece on the path from original position to target position.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @param x shift from original x (positive values to right, negative to left)
     * @param y shift from original y (positive values to top, negative to bottom)
     * @return true if there is a piece on the path, false otherwise
     * @note This function assumes that the move is either horizontal, vertical, or diagonal. 
     * It does not handle knight moves. In future possible other pieces with non-linear movement could be added.
     * @note This function does not check the validity of the move itself, only if there are pieces blocking the path.
     */
    bool pieceOnPath(const std::size_t o_x, const std::size_t o_y, const int x, const int y) const;

    /**
     * @brief Checks if castling is possible.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @param x shift from original x (positive values to right, negative to left)
     * @param y shift from original y (positive values to top, negative to bottom)
     * @return true if castling is possible, false otherwise
     */
    bool canCastle(const std::size_t o_x, const std::size_t o_y, const int x, const int y) const;
    /**
     * @brief Moves king by 2 tiles and rook jumps over king.
     * @param k_x king's horizontal position.
     * @param r_x rook's horizontal position.
     * @note This function assumes that the king and rook are on the same row.
     */
    void castling(const std::size_t k_x, const std::size_t r_x);
    /**
     * @brief User chooses which piece pawn will be promoted to.
     * @param o_x original x tile value
     * @param o_y original y tile value
     */
    void pawnPromotionChoice(const std::size_t o_x, const std::size_t o_y);
    /**
     * @brief Promotes pawn at given o(x,y) tile to given type.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @param type type of piece to promote to, defaults to Piece::Type::QUEEN
     */
    void pawnPromotion(const std::size_t o_x, const std::size_t o_y, 
                       Piece::Type type = Piece::Type::QUEEN);

    /**
     * @brief Checks if en passant is available for the piece at o(x,y) tile.
     * @param o_x original x tile value 
     * @param o_y original y tile value
     * @param x shift from original x (positive values to right, negative to left)
     * @param y shift from original y (positive values to top, negative to bottom)
     * @return true if en passant is available, false otherwise
     */
    bool enPassantAvailable(const std::size_t o_x, const std::size_t o_y, const int x, const int y) const;
    /**
     * @brief Performs en passant for the piece at o(x,y) tile.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @param x shift from original x (positive values to right, negative to left)
     * @param y shift from original y (positive values to top, negative to bottom)
     * @note Should be called only if en passant is available.
     */
    void enPassant(const std::size_t o_x, const std::size_t o_y, const int x, const int y);
    /**
     * @brief Updates en passant status for all pawns on the board each turn.
     * Decreases the turns left for en passant and resets the ability to capture if necessary.
     * @param team The team whose pawns' en passant status should be updated.
     */
    void enPassantTurnUpdate(const Piece::Team team);

    /**
     * @brief gets white king's position on the chessboard.
     * @returns white king's position.
     */
    size_pair getWhiteKingsPosition() const;

    /**
     * @brief gets black king's position on the chessboard.
     * @returns black king's position.
     */
    size_pair getBlackKingsPosition() const;

    /**
     * @brief sets black king's current position.
     * @param x x tile value
     * @param y y tile value
     */
    void setBlackKingsPosition(const size x, const size y);

    /**
     * @brief sets white king's current position.
     * @param x x tile value
     * @param y y tile value
     */
    void setWhiteKingsPosition(const size x, const size y);
    /**
     * @brief Checks and updates the king's status (check, checkmate, stalemate, nothing) for the given team.
     * @param team The team to check for check status.
     */
    void kingStatus(const Piece::Team team, std::ostream& os = std::cout) const;
    /**
     * @brief Checks if king can move to any adjacent square.
     * @param team The team to check for king's possible moves.
     * @returns true if king can move to any adjacent square, false otherwise.
     */
    bool kingCanMove(const Piece::Team team) const;

    /**
     * @brief Checks if pawn can move to any adjacent square.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @param team The team to check for pawn's possible moves.
     * @returns true if pawn can move to any adjacent square, false otherwise.
     */
    bool pawnCanMove(const std::size_t o_x, const std::size_t o_y, const Piece::Team team) const;
    /**
     * @brief Checks if rook can move to any adjacent square.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @returns true if rook can move to any adjacent square, false otherwise.
     */
    bool rookCanMove(const std::size_t o_x, const std::size_t o_y) const; 
    /**
     * @brief Checks if bishop can move to any adjacent square.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @returns true if bishop can move to any adjacent square, false otherwise.
     */
    bool bishopCanMove(const std::size_t o_x, const std::size_t o_y) const;
    /**
     * @brief Checks if knight can move to any adjacent square.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @returns true if knight can move to any adjacent square, false otherwise.
     */
    bool knightCanMove(const std::size_t o_x, const std::size_t o_y) const;
    /**
     * @brief Checks if queen can move to any adjacent square.
     * @param o_x original x tile value
     * @param o_y original y tile value
     * @returns true if queen can move to any adjacent square, false otherwise.
     */
    bool queenCanMove(const std::size_t o_x, const std::size_t o_y) const;

    /**
     * @brief Checks if pieces can move anywhere.
     * @param team The team to check for possible moves.
     * @returns true if any piece can move, false otherwise.
     */
    bool piecesCanMove(const Piece::Team team) const;
    
    /**
     * @brief Checks if the given team is in check.
     * @param team The team to check for check status.
     * @returns true if king in check, false if not.
     * @note This function will be used in kingStatusUpdate(), 
     * and will result in only king and pieces able to defend him being possible to move.
     */
    bool inCheck(const Piece::Team team) const;

    /**
     * @brief Checks if the given team is in check.
     * @param position The position on which should stand the king (for testing purposes mostly)
     * @returns true if king in check, false if not.
     * @note This function will be used in kingStatusUpdate(), 
     * and will result in only king and pieces able to defend him being possible to move.
     */
    bool inCheck(const Piece::Team team, const size_pair position) const;

    
    /**
     * @brief Checks if the given team is in checkmate.
     * @param team The team to check for checkmate status.
     * @returns true if king in checkmate, false if not.
     * @note This function will be used in kingStatusUpdate() and will result in game over if true.
     */
    bool inCheckmate(const Piece::Team team) const;
    /**
     * @brief Checks if the given team is in stalemate.
     * @param team The team to check for stalemate status.
     * @returns true if king in stalemate, false if not.
     * @note This function will be used in kingStatusUpdate() and will result in game over if true.
     */
    bool inStalemate(const Piece::Team team) const;
    /**
     * @brief Return hotizpntal length
     * @return horizontal lenght
     */
    size getHorizontal() const;

    /**
     * @brief Return vertical lenght
     * @return vertical lenght
     */
    size getVertical() const;

    /**
     * @brief Return enpassant status of piece at given o(x,y) tile
     * @param o_x x tile value
     * @param o_y y tile value
     * @return pair of int and bool representing enpassant status, (-1, false) if there is no piece at given o(x,y) tile
     */
    std::pair<int, bool> getEnPassantStatus(const std::size_t o_x, const std::size_t o_y) const;
    /**
     * @brief Checks if container has been created but not set.
     * @return true if container has been created but not set, false otherwise
     */
    bool isInitialized() const;

    /**
     * @brief Sets given figure at o(x,y)
     * @param o_x x tile value
     * @param o_y y tile value
     * @param piece figure to set
     */
    void setPiece(const std::size_t o_x, const std::size_t o_y, std::unique_ptr<Piece> piece);

    /**
     * @brief Gets piece at given tile o(x, y)
     * @param o_x x tile value
     * @param o_y y tile value
     */

    std::unique_ptr<Piece>& getPiece(const std::size_t o_x, const std::size_t o_y) const;

    /**
     * @brief Gets if there is a piece at given o(x,y) tile
     * @param o_x x tile value
     * @param o_y y tile value
     * @return true if there is a piece at given o(x,y) tile, false otherwise
     */
    bool isPieceAt(const std::size_t o_x, const std::size_t o_y) const;
    /**
     * @brief Destroyes figure at given o(x,y) tile
     * @param o_x x tile value
     * @param o_y y tile value
     */
    void killPiece(const std::size_t o_x, const std::size_t o_y);

    /**
     * @brief Increases figure's health value at o(x,y) tile for given health
     * @param o_x x tile value
     * @param o_y y tile value
     * @param health heals for given value
     */
    void healPiece(const std::size_t o_x, const std::size_t o_y, const int health);

    /**
     * @brief Gets health of piece at given o(x,y) tile
     * @param o_x x tile value
     * @param o_y y tile value
     * @return health of piece at given o(x,y) tile, -1 if there is no piece at given o(x,y) tile
     */
    int getHealthOfPiece(const std::size_t o_x, const std::size_t o_y) const;
    
    /**
     * @brief Damages figure at given o(x,y) for default value
     * @param o_x x tile value
     * @param o_y y tile value
     */
    void damagePiece(const std::size_t o_x, const std::size_t o_y);

    /**
     * @brief Damages figure at o(x,y) tile for given damage value
     * @param o_x x tile value
     * @param o_y y tile value
     * @param damage value of damage
     */
    void damagePiece(const std::size_t o_x, const std::size_t o_y, const int damage);
    /**
     * @brief Gets damage of piece at given o(x,y) tile
     * @param o_x x tile value
     * @param o_y y tile value
     * @return damage of piece at given o(x,y) tile, -1 if there is no piece at given o(x,y) tile
     */
    int getDamageOfPiece(const std::size_t o_x, const std::size_t o_y) const;
    /**
     * @brief Gets team of piece at given o(x,y) tile
     * @param o_x x tile value
     * @param o_y y tile value
     * @return team of piece at given o(x,y) tile, Piece::Team::WHITE if there is no piece at given o(x,y) tile
     */
    Piece::Team getTeamOfPiece(const std::size_t o_x, const std::size_t o_y) const;
    private:
    /**
     * @brief Simulates king moving to another square and checks if he would be in check.
     * @param team The team to which the king belongs.
     * @param starting_position The position at which the king starts.
     * @param shift_position The position to which the king is to be moved.
     * @returns true if king would be in check, false otherwise.
     */
    bool kingWouldBeInCheck(const Piece::Team team, const size_pair starting_position, const size_pair shift_position) const;
    const size _horizontal; //height
    const size _vertical;   //width
    mutable std::vector<std::vector<std::unique_ptr<Piece>>> pieces; //stores figures
    size_pair _whiteKingsPosition;
    size_pair _blackKingsPosition;

};

