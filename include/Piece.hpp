#pragma once
#include <string>

/**
 * @brief Represents a generic game piece with fundamental properties and actions.
 *
 * This is an abstract base class, meaning it cannot be instantiated directly.
 * It provides common attributes like name, damage, health, and move count,
 * along with pure virtual functions for `move` and `attack` that must be
 * implemented by derived concrete classes.
 */
struct Piece{
    
    /**
     * @brief Enum representing the team to which a piece belongs.
     * Can be used for game logic, such as determining valid moves or attacks.
     */
    enum Team{
        WHITE,
        BLACK
    };
    /**
     * @brief Enum representing the type of piece.
     * This can be used for identification or logic within the game.
     */
    enum Type{
        KING,
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT,
        PAWN
    };
    /**
     * @brief Default constructor. Initializes a piece with default values:
     * name "unknown", damage 1, and health 1.
     */
    Piece();

    /**
     * @brief Constructor that initializes a piece with a name and damage.
     * Health is defaulted to 1.
     * @param name The name of the piece.
     * @param damage The damage value of the piece.
     * @param team The team to which the piece belongs.
     */
    Piece(std::string name, const int damage, const Team team);

    /**
     * @brief Constructor that initializes a piece with a name, damage, and health.
     * @param name The name of the piece.
     * @param damage The damage value of the piece.
     * @param health The health value of the piece.
     * @param team The team to which the piece belongs.
     */
    Piece(std::string name, const int damage, const int health, const Team team);
    /**
     * @brief Virtual destructor. Ensures proper cleanup of derived class objects.
     */
    virtual ~Piece();

    /**
     * @brief Pure virtual function to define the movement behavior of the piece.
     * Must be implemented by derived classes.
     * @param horizontal The horizontal displacement for the move.
     * @param vertical The vertical displacement for the move.
     * @return True if the move is successful, false otherwise.
     */
    virtual bool move(const int horizontal, const int vertical) = 0;

    /**
     * @brief Pure virtual function to define the attack behavior of the piece.
     * Must be implemented by derived classes.
     * @param horizontal The horizontal target for the attack.
     * @param vertical The vertical target for the attack.
     * @return True if the attack is successful, false otherwise.
     */
    virtual bool attack(const int horizontal, const int vertical) = 0;
    /**
     * @brief Gets the name of the piece.
     * @return The name of the piece.
     */
    std::string getName() const;

    /**
     * @brief Gets the current health of the piece.
     * @return The health of the piece.
     */
    int getHealth() const;

    /**
     * @brief Gets the number of moves made by the piece.
     * @return The move number.
     */
    int getMoveNumber() const;

    /**
     * @brief Gets the damage value of the piece.
     * @return The damage value.
     */
    int getDamage() const;

    /**
     * @brief Sets the name of the piece.
     * @param name The new name for the piece.
     */
    void setName(const std::string name);

    /**
     * @brief Sets the health of the piece.
     * @param health The new health value for the piece.
     */
    void setHealth(const int health);

    /**
     * @brief Sets the damage of the piece.
     * @param damage The new damage value for the piece.
     */
    void setDamage(const int damage);

    /**
     * @brief Increments the move counter for the piece.
     */
    void increaseMoveNumber();
    /**
     * @brief Sets move number.
     */
    void setMoveNumber(const int move_number);
    /**
     * @brief Gets the team of the piece.
     * @return The team to which the piece belongs.
     */
    Team getTeam() const;
    /**
     * @brief Sets the team of the piece.
     * @param team The team to which the piece belongs.
     */
    void setTeam(const Team team);
    /**
     * @brief Gets the type of the piece.
     * @return The type of the piece, which can be used for identification or logic.
     */
    Type getType() const;
    /**
     * @brief Sets the type of the piece.
     * @param type The type of the piece, which can be used for identification or logic.
     */
    void setType(const Type type);
    private:
    std::string _name; //The name of the piece
    int _move_number = 0; // Tracks the number of moves made by the piece
    /**
     * I don't know if it will be used.
     */
    int _damage; // Default damage value, can be overridden by derived classes
    Team _team; // The team to which the piece belongs, can be used for game logic
    int _health; // Current health value, can be overridden by derived classes
    Type _type; // Type of the piece, can be used for identification or logic
};