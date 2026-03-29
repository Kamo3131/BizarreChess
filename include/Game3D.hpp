#pragma once
#include "GameInterface.hpp"
#include "Chessboard3D.hpp"
#include "RaySelection.hpp"
#include "MovementHistory.hpp"
#include <optional>

class Game3D : public GameInterface {
    public:
    using size = std::size_t;

    /**
     * @brief Default constructor. Initializes _turn with 0 and _chessBoard with 8x8.
     */
    Game3D();
    /**
     * @brief Constructor for game with different chessboard sizes.
     * @param horizontal sets horizontal tile number, should not exceed 26
     * @param vertical sets vertical lenght, should not exceed 26
     */
    Game3D(const size horizontal, const size vertical);
    /**
     * @brief Constructor for game with given turn and chessboard.
     * @param turn sets current turn
     * @param chessBoard sets chessboard
     */
    Game3D(const int turn, ChessBoard&& chessBoard);
    /**
     * @brief main game loop, handles user input and game commands.
     * @details Displays current game state, prompts for commands, and processes them.
     */
    void gameLoop();
    /**
     * @brief Moves a piece on the chessboard.
     */
    void movePiece();
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
    void undo();
    void redo();
    /**
     * @brief Gets the chessboard.
     * @return Reference to the chessboard.
     */
    const ChessBoard& getChessBoard() const;

    /**
     * @brief handles clicking mouse and sending selection ray.
     * @param window basic render target.
     * @param mouseX coordinate X of mouse cursor. 
     * @param mouseY coordinate Y of mouse cursor.
     */
    void handleMouseClick(const Chessboard3D, int mouseX, int mouseY);
    void renderShaderBackground(sf::RenderWindow& window, float time) const;


    private:
    void initOpengl() const;
    /**
     * @brief updates _possibleMoves vector, which contains moves that current, selected piece can do.
     */
    void updatePossibleMoves();
    /**
     * @brief manages the camera settings.
     */
    void setupCamera() const;
    /**
     * @brief renders in-game imgui menu.
     */
    inline void inGameControlsMenu();
    int _turn;
    float _zoom;
    std::optional<sf::Vector2i> _selectedSquare;
    std::vector<sf::Vector2i> _possibleMoves;
    
    RaySelection _raySelection;
    std::unique_ptr<ChessBoard> _chessBoard;
    MovementHistory _movementHistory;
};