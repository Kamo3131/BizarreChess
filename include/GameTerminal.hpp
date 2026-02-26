#pragma once
#include "GameInterface.hpp"
class GameTerminal : public GameInterface{
    public:
    using size = std::size_t;
    /**
     * @brief Default constructor. Initializes _turn with 0 and _chessBoard with 8x8.
     */
    GameTerminal();
    /**
     * @brief Constructor for game with different chessboard sizes.
     * @param horizontal sets horizontal tile number, should not exceed 26
     * @param vertical sets vertical lenght, should not exceed 26
     */
    GameTerminal(const size horizontal, const size vertical);
    /**
     * @brief Constructor for game with given turn and chessboard.
     * @param turn sets current turn
     * @param chessBoard sets chessboard
     */
    GameTerminal(const int turn, ChessBoard&& chessBoard);
    /**
     * @brief Prints current game state, including chessboard state and turn number.
     */
    void printCurrentGameState(std::ostream& os = std::cout) const;
    /**
     * @brief main game loop, handles user input and game commands.
     * @details Displays current game state, prompts for commands, and processes them.
     */
    void gameLoop();
    /**
     * @brief Moves a piece on the chessboard.
     * @details Prompts user to select a piece and a target square, validates input, and performs the move.
     */
    void movePiece();
    /**
     * @brief Moves a piece on the chessboard.
     * @param square The square to move the piece from, in the format "x y" (e.g., "2 A").
     * @details Validates the input square, checks if a piece exists at that square,
     */
    void movePiece(std::string square); // Yet to be implemented
    /**
     * @brief Selects a target square for the piece to move to.
     * @param o_x Original x coordinate of the piece.
     * @param o_y Original y coordinate of the piece.
     * @details Prompts user to select a target square, validates input, and performs the move.
     * @note This method assumes is intended to be used after a piece has been selected with movePiece() or it's alternatives.
     */
    void selectTargetSquare(std::size_t o_x, std::size_t o_y);
    /**
     * @brief Surrenders the game.
     * @return false if the game is surrendered, true otherwise
     */
    bool surrender();
    /**
     * @brief Restarts the game.
     */
    void restart();
    /**
     * @brief Quits the game.
     * @return false if the game is quit, true otherwise
     */
    bool quitGame();
    /**
     * @brief Advances to the next turn.
     */
    void nextTurn();
    /**
     * @brief Goes back to the previous turn.
     * @details This function is a placeholder for future implementation.
     */
    void previousTurn();
    /**
     * @brief Decreases enpassant turn status for all pieces of playing team.
     */
    void enPassantTurnCycle();
    /**
     * @brief Gets the current turn number.
     * @return Current turn number.
     */
    int getTurn() const;
    /**
     * @brief Gets the chessboard.
     * @return Reference to the chessboard.
     */
    const ChessBoard& getChessBoard() const;
    private:
    int _turn;
    ChessBoard _chessBoard;
};


