#include <gtest/gtest.h>
#include "ChessBoard.hpp"
#include "Piece.hpp"
#include "Pawn.hpp"

/**
 * Test the ChessBoard constructor initializes horizontal and vertical sizes to 8.
 */
TEST(ChessBoardTests, DefaultConstructor) {
    ChessBoard board;
    EXPECT_EQ(board.getHorizontal(), 8) << "Default constructor failed to set horizontal size!";
    EXPECT_EQ(board.getVertical(), 8) << "Default constructor failed to set vertical size!";
}

/**
 * Test the ChessBoard constructor initializes horizontal and vertical sizes to specified values.
 */
TEST(ChessBoardTests, ParameterizedConstructor){
    ChessBoard board(10, 12);
    EXPECT_EQ(board.getHorizontal(), 10) << "Parameterized constructor failed to set horizontal size!";
    EXPECT_EQ(board.getVertical(), 12) << "Parameterized constructor failed to set vertical size!";
}

/**
 * Test the ChessBoard constructor does not allow horizontal size to exceed 26.
 */
TEST(ChessBoardTests, ConstructorExceedsHorizontalLimit){
    ChessBoard board(30, 10);
    EXPECT_EQ(board.getHorizontal(), 26) << "Constructor should limit horizontal size to 26!";
    EXPECT_EQ(board.getVertical(), 10) << "Constructor should not limit vertical size to 26!"; 
}

/**
 * Test the ChessBoard constructor does not allow vertical size to exceed 26.
 */
TEST(ChessBoardTests, ConstructorExceedsVerticalLimit){
    ChessBoard board(10, 30);
    EXPECT_EQ(board.getHorizontal(), 10) << "Constructor should not limit horizontal size to 26!";
    EXPECT_EQ(board.getVertical(), 26) << "Constructor should limit vertical size to 26!";
}

/**
 * Test the ChessBoard::init method initializes the board correctly.
 */
TEST(ChessBoardTests, InitMethod){
    ChessBoard board;
    board.init();
    EXPECT_EQ(board.isInitialized(), true) << "Init method failed to set chessboard!";
}

/**
 * Test there is a piece at a specific tile after setting it.
 */
TEST(ChessBoardTests, SetPieceAtTile){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    board.setPiece(0, 0, std::move(pawn1B));
    EXPECT_TRUE(board.isPieceAt(0, 0)) << "Piece should be set at (0, 0)!";
}
/**
 * Test there is no piece at a specific tile after killing it.
 */
TEST(ChessBoardTests, KillPieceAtTile){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    board.setPiece(0, 0, std::move(pawn1B));
    board.killPiece(0, 0);
    EXPECT_FALSE(board.isPieceAt(0, 0)) << "Piece should not be set at (0, 0)!";
}

/**
 * Test the ChessBoard::move method moves a piece correctly.
 */
TEST(ChessBoardTests, MovePiece){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    board.setPiece(0, 0, std::move(pawn1W));
    board.move(0, 0, 0, 1);
    EXPECT_FALSE(board.isPieceAt(0, 0)) << "Piece should not be at (0, 0) after move!";
    EXPECT_TRUE(board.isPieceAt(0, 1)) << "Piece should be at (0, 1) after move!";
}

/**
 * Test the ChessBoard::move method does not move a piece to an occupied square.
 */
TEST(ChessBoardTests, MoveToOccupiedSquare){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    std::unique_ptr<Piece> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    board.setPiece(0, 0, std::move(pawn1B));
    board.setPiece(0, 1, std::move(pawn1W));
    board.move(0, 0, 0, 1); // Attempt to move to occupied square
    EXPECT_TRUE(board.isPieceAt(0, 0)) << "Black pawn should still be at (0, 0) after failed move!";
    EXPECT_TRUE(board.isPieceAt(0, 1)) << "White pawn should still be at (0, 1) after failed move!";
}

/**
 * Test the ChessBoard::move method kills an enemy piece with one health point after moving onto it.
 */
TEST(ChessBoardTests, AttackEnemyPiece){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    std::unique_ptr<Piece> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    pawn1W->setDamage(1); // Set damage to 1 to ensure it can kill
    pawn1B->setHealth(1); // Set health to 1 to ensure it can be killed
    board.setPiece(0, 0, std::move(pawn1W));
    board.setPiece(1, 1, std::move(pawn1B));
    board.move(0, 0, 1, 1);
    EXPECT_FALSE(board.isPieceAt(0, 0)) << "Black pawn should not be at (0, 0) after move!";
    EXPECT_TRUE(board.isPieceAt(1, 1)) << "Black pawn should be at (1, 1) after move!";
}

/**
 * Test the ChessBoard::move method damages an enemy piece 
 * with more than one health point after moving onto it, when moved piece has 1 damage.
 */
TEST(ChessBoardTests, DamageEnemyPiece){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    std::unique_ptr<Piece> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    pawn1W->setDamage(1); // Set damage to 1
    pawn1B->setHealth(2); // Set health to 2 to ensure it can be damaged but not killed
    board.setPiece(0, 0, std::move(pawn1W));
    board.setPiece(1, 1, std::move(pawn1B));
    board.move(0, 0, 1, 1);
    EXPECT_TRUE(board.isPieceAt(0, 0)) << "Black pawn should be at (0, 0) after move!";
    EXPECT_TRUE(board.isPieceAt(1, 1)) << "White pawn should be at (1, 1) after move!";
    EXPECT_EQ(board.getHealthOfPiece(1, 1), 1) << "White pawn should have health reduced to 1 after being damaged!";
}

/**
 * Test the ChessBoard::move method does not move a piece out of bounds.
 */
TEST(ChessBoardTests, MoveOutOfBounds){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    board.setPiece(0, 0, std::move(pawn1B));
    board.move(0, 0, 0, 10); // Attempt to move out of bounds
    EXPECT_TRUE(board.isPieceAt(0, 0)) << "Piece should still be at (0, 0) after failed move!";
}
/**
 * Test the ChessBoard::getHealthOfPiece method returns the correct health of a piece.
 */
TEST(ChessBoardTests, GetHealthOfPiece){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    pawn1B->setHealth(5); // Set initial health to 5
    board.setPiece(0, 0, std::move(pawn1B));
    EXPECT_EQ(board.getHealthOfPiece(0, 0), 5) << "Health of piece at (0, 0) should be 5!";
}

/**
 * Test the ChessBoard::getDamageOfPiece method returns the correct damage of a piece.
 */
TEST(ChessBoardTests, GetDamageOfPiece){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    pawn1B->setDamage(3); // Set initial damage to 3
    board.setPiece(0, 0, std::move(pawn1B));
    EXPECT_EQ(board.getDamageOfPiece(0, 0), 3) << "Damage of piece at (0, 0) should be 3!";
}
/**
 * Test the ChessBoard::healPiece method heals a piece correctly.
 */
TEST(ChessBoardTests, HealPiece){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    pawn1B->setHealth(5); // Set initial health to 5
    board.setPiece(0, 0, std::move(pawn1B));
    board.healPiece(0, 0, 3); // Heal by 3
    EXPECT_EQ(board.getHealthOfPiece(0, 0), 8) << "Piece should be healed to 8 health!";
}

/**
 * Test the ChessBoard::damagePiece method with default value damages a piece correctly.
 */
TEST(ChessBoardTests, DamagePieceByDefaultValue){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    pawn1B->setHealth(5); // Set initial health to 5
    board.setPiece(0, 0, std::move(pawn1B));
    board.damagePiece(0, 0); // Damage by 1
    EXPECT_EQ(board.getHealthOfPiece(0, 0), 4) << "Piece should be damaged to 3 health!";
}

/**
 * Test the ChessBoard::damagePiece method with 3 parameters damages a piece correctly.
 */
TEST(ChessBoardTests, DamagePieceByCustomValue){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    pawn1B->setHealth(5); // Set initial health to 5
    board.setPiece(0, 0, std::move(pawn1B));
    board.damagePiece(0, 0, 2); // Damage by 1
    EXPECT_EQ(board.getHealthOfPiece(0, 0), 3) << "Piece should be damaged to 3 health!";
}

/**
 * Test the ChessBoard::damagePiece method with default value kills a piece with 1 health point.
 */
TEST(ChessBoardTests, DamagePieceByDefaultValueKillsWithOneHealth){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    pawn1B->setHealth(1); // Set initial health to 1
    board.setPiece(0, 0, std::move(pawn1B));
    board.damagePiece(0, 0); // Damage by 1
    EXPECT_FALSE(board.isPieceAt(0, 0)) << "Piece should not be at (0, 0) after being killed!";
}

/**
 * Test the ChessBoard::damagePiece method with 3 parameters kills a piece with 2 health point, when dealing 2 damage.
 */
TEST(ChessBoardTests, DamagePieceByCustomValueKillsWithTwoHealth){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    pawn1B->setHealth(2); // Set initial health to 2
    board.setPiece(0, 0, std::move(pawn1B));
    board.damagePiece(0, 0, 2); // Damage by 2
    EXPECT_FALSE(board.isPieceAt(0, 0)) << "Piece should not be at (0, 0) after being killed!";
}

/**
 * Test the ChessBoard::castling method performs castling correctly to the right.
 */
TEST(ChessBoardTests, CastlingRight) {
    ChessBoard board;
    constexpr Piece::Team team = Piece::Team::WHITE;
    std::unique_ptr<Piece> king = std::make_unique<King>(team);
    std::unique_ptr<Piece> rook = std::make_unique<Rook>(team);
    board.setPiece(4, 0, std::move(king)); // Place king at e1
    board.setPiece(7, 0, std::move(rook)); // Place rook at h1
    board.castling(4, 7, team); // Perform castling
    EXPECT_FALSE(board.isPieceAt(4, 0)) << "King should not be at e1 after castling!";
    EXPECT_TRUE(board.isPieceAt(6, 0)) << "King should be at g1 after castling!";
    EXPECT_FALSE(board.isPieceAt(7, 0)) << "Rook should not be at h1 after castling!";
    EXPECT_TRUE(board.isPieceAt(5, 0)) << "Rook should be at f1 after castling!";
}
/**
 * Test the ChessBoard::castling method performs castling correctly to the left.
 */
TEST(ChessBoardTests, CastlingLeft) {
    ChessBoard board;
    constexpr Piece::Team team = Piece::Team::WHITE;
    std::unique_ptr<Piece> king = std::make_unique<King>(team);
    std::unique_ptr<Piece> rook = std::make_unique<Rook>(team);
    board.setPiece(4, 0, std::move(king)); // Place king at e1
    board.setPiece(0, 0, std::move(rook)); // Place rook at a1
    board.castling(4, 0, team); // Perform castling
    EXPECT_FALSE(board.isPieceAt(4, 0)) << "King should not be at e1 after castling!";
    EXPECT_TRUE(board.isPieceAt(2, 0)) << "King should be at c1 after castling!";
    EXPECT_FALSE(board.isPieceAt(0, 0)) << "Rook should not be at a1 after castling!";
    EXPECT_TRUE(board.isPieceAt(3, 0)) << "Rook should be at d1 after castling!";
}

/**
 * Test the ChessBoard::castling method does not allow castling if there are pieces in between.
 */
TEST(ChessBoardTests, CastlingWithPiecesInBetween) {
    ChessBoard board;
    std::unique_ptr<Piece> king = std::make_unique<King>(Piece::Team::WHITE);
    std::unique_ptr<Piece> rook = std::make_unique<Rook>(Piece::Team::WHITE);
    std::unique_ptr<Piece> pawn = std::make_unique<Pawn>(Piece::Team::WHITE);
    board.setPiece(4, 0, std::move(king)); // Place king at e1
    board.setPiece(7, 0, std::move(rook)); // Place rook at h1
    board.setPiece(5, 0, std::move(pawn)); // Place pawn at f1
    board.move(4, 0, 7, 0); // Attempt to perform castling
    EXPECT_TRUE(board.isPieceAt(4, 0)) << "King should still be at e1 after failed castling!";
    EXPECT_TRUE(board.isPieceAt(7, 0)) << "Rook should still be at h1 after failed castling!";
    EXPECT_TRUE(board.isPieceAt(5, 0)) << "Pawn should still be at f1 after failed castling!";
}

/**
 * Tests the ChessBoard::enPassant method allows a pawn to perform en passant capture.
 */
TEST(ChessBoardTests, EnPassantCapture) {
    ChessBoard board;
    std::unique_ptr<Piece> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    std::unique_ptr<Piece> pawn2B = std::make_unique<Pawn>(Piece::Team::BLACK);
    
    // Set up the pawns on the board
    pawn1W->setDamage(1);
    pawn2B->setHealth(1);
    board.setPiece(4, 4, std::move(pawn1W));
    board.setPiece(5, 6, std::move(pawn2B));
    
    
    // Move the black pawn two squares forward to set up en passant
    board.move(5, 6, 0, -2); // Black pawn moves to f5
    EXPECT_TRUE(board.isPieceAt(5, 4)) << "Black pawn should be at f5 after move!";
    
    // Perform en passant capture
    board.move(4, 4, 1, 1); // White pawn captures en passant
    EXPECT_FALSE(board.isPieceAt(5, 4)) << "Black pawn should not be at f5 after en passant!";
    EXPECT_TRUE(board.isPieceAt(5, 5)) << "White pawn should be at f6 after en passant!";
}

/**
 * Test the ChessBoard::canMove method returns true for a valid move.
 */
TEST(ChessBoardTests, CanMoveValid) {
    ChessBoard board;
    std::unique_ptr<Piece> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    board.setPiece(0, 1, std::move(pawn1W));
    EXPECT_TRUE(board.canMove(0, 1, 0, 1)) << "Pawn should be able to move forward by 1!";
    
}
/**
 * Test the ChessBoard::canMove method returns true for a valid attack.
 */
TEST(ChessBoardTests, CanMoveAttackValid) {
    ChessBoard board;
    std::unique_ptr<Piece> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    board.setPiece(0, 1, std::move(pawn1W));
    board.setPiece(1, 2, std::make_unique<Pawn>(Piece::Team::BLACK));
    EXPECT_TRUE(board.canMove(0, 1, 1, 1)) << "Pawn should be able to attack diagonally!";
}

/**
 * Test the ChessBoard::canMove method returns true for a valid en passant move.
 */
TEST(ChessBoardTests, CanMoveEnPassantValid) {
    ChessBoard board;
    std::unique_ptr<Piece> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    board.setPiece(0, 1, std::move(pawn1W));
    std::unique_ptr<Pawn> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    pawn1B->setEnPassant(1, true); // Ensure en passant is possible
    board.setPiece(1, 1, std::move(pawn1B));
    EXPECT_TRUE(board.canMove(0, 1, 1, 1)) << "Pawn should be able to perform en passant when possible!";
}
/**
 * Test the ChessBoard::canMove method returns false for an invalid move.
 */
TEST(ChessBoardTests, CanMoveInvalid) {
    ChessBoard board;
    std::unique_ptr<Piece> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    board.setPiece(0, 1, std::move(pawn1W));
    EXPECT_FALSE(board.canMove(0, 1, 0, 0)) << "Pawn should not be able to move backwards!";
}
/**
 * Test the ChessBoard::canMove method returns false for an invalid attack.
 */
TEST(ChessBoardTests, CanMoveAttackInvalid) {
    ChessBoard board;
    std::unique_ptr<Piece> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    board.setPiece(0, 1, std::move(pawn1W));
    board.setPiece(1, 3, std::make_unique<Pawn>(Piece::Team::BLACK));
    EXPECT_FALSE(board.canMove(0, 1, 1, 2)) << "Pawn should not be able to attack non-diagonally!";
}
/**
 * Test the ChessBoard::canMove method returns false if en passant is not possible.
 */
TEST(ChessBoardTests, CanMoveEnPassantInvalid) {
    ChessBoard board;
    std::unique_ptr<Piece> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    board.setPiece(0, 1, std::move(pawn1W));
    std::unique_ptr<Pawn> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    pawn1B->setEnPassant(0, false); // Ensure en passant is not possible
    board.setPiece(1, 1, std::move(pawn1B));
    EXPECT_FALSE(board.canMove(0, 1, 1, 1)) << "Pawn should not be able to perform en passant when not possible!";
}

/**
 * Test the ChessBoard::pieceOnPath method detects an allied piece on the path correctly.
 */
TEST(ChessBoardTests, AlliedPieceOnPath){
    ChessBoard board;
    board.setPiece(0, 1, std::make_unique<Rook>(Piece::Team::WHITE));
    board.setPiece(0, 4, std::make_unique<Pawn>(Piece::Team::WHITE));
    
    EXPECT_TRUE(board.pieceOnPath(0, 1, 0, 5)) << "Rook should not be able to move through the Pawn!";
}
/**
 * Test the ChessBoard::pieceOnPath method detects an enemy piece on the path correctly.
 */
TEST(ChessBoardTests, EnemyPieceOnPath){
    ChessBoard board;
    board.setPiece(0, 1, std::make_unique<Rook>(Piece::Team::WHITE));
    board.setPiece(0, 4, std::make_unique<Pawn>(Piece::Team::BLACK));
    
    EXPECT_TRUE(board.pieceOnPath(0, 1, 0, 5)) << "Rook should not be able to move through the Pawn!";
}

/**
 * Test the ChessBoard::pieceOnPath method detects there is no piece on the path correctly.
 */
TEST(ChessBoardTests, NoPieceOnPath){
    ChessBoard board;
    board.setPiece(0, 1, std::make_unique<Rook>(Piece::Team::WHITE));
    
    EXPECT_FALSE(board.pieceOnPath(0, 1, 0, 5)) << "There is no piece on the path!";
}

/**
 * Test the ChessBoard::canMove method detects an allied piece on the path correctly.
 */
TEST(ChessBoardTests, CanMoveAlliedPieceOnPath){
    ChessBoard board;
    board.setPiece(0, 1, std::make_unique<Rook>(Piece::Team::WHITE));
    board.setPiece(0, 4, std::make_unique<Pawn>(Piece::Team::WHITE));
    
    EXPECT_FALSE(board.canMove(0, 1, 0, 5)) << "Rook should not be able to move through the Pawn!";
}

/**
 * Test the ChessBoard::canMove method detects an enemy piece on the path correctly.
 */
TEST(ChessBoardTests, CanMoveEnemyPieceOnPath){
    ChessBoard board;
    board.setPiece(0, 1, std::make_unique<Rook>(Piece::Team::WHITE));
    board.setPiece(0, 4, std::make_unique<Pawn>(Piece::Team::BLACK));
    
    EXPECT_FALSE(board.canMove(0, 1, 0, 5)) << "Rook should not be able to move through the Pawn!";
}

/**
 * Test the ChessBoard::canMove method detects there is no piece on the path correctly.
 */
TEST(ChessBoardTests, CanMoveNoPieceOnPath){
    ChessBoard board;
    board.setPiece(0, 1, std::make_unique<Rook>(Piece::Team::WHITE));
    
    EXPECT_TRUE(board.canMove(0, 1, 0, 5)) << "There is no piece on the path!";
}

/**
 * Test the ChessBoard::enPassantTurnUpdate method decreases the en passant turn counter correctly.
 */
TEST(ChessBoardTests, EnPassantTurnUpdate) {
    ChessBoard board;
    std::unique_ptr<Pawn> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    pawn1B->setEnPassant(2, true); // Set en passant turns to 2
    board.setPiece(0, 6, std::move(pawn1B));
    
    board.enPassantTurnUpdate(Piece::Team::BLACK); // Decrease turns by 1
    EXPECT_EQ(board.getEnPassantStatus(0, 6).first, 1) << "En passant turns should decrease to 1!";
    
    board.enPassantTurnUpdate(Piece::Team::BLACK); // Decrease turns by 1
    EXPECT_EQ(board.getEnPassantStatus(0, 6).first, 0) << "En passant turns should decrease to 0!";
    EXPECT_FALSE(board.getEnPassantStatus(0, 6).second) << "En passant should no longer be possible!";

    std::unique_ptr<Pawn> pawn1W = std::make_unique<Pawn>(Piece::Team::WHITE);
    pawn1W->setEnPassant(2, true); // Set en passant turns to 2
    board.setPiece(0, 1, std::move(pawn1W));
    
    board.enPassantTurnUpdate(Piece::Team::WHITE); // Decrease turns by 1
    EXPECT_EQ(board.getEnPassantStatus(0, 1).first, 1) << "En passant turns should decrease to 1!";
    
    board.enPassantTurnUpdate(Piece::Team::WHITE); // Decrease turns by 1
    EXPECT_EQ(board.getEnPassantStatus(0, 1).first, 0) << "En passant turns should decrease to 0!";
    EXPECT_FALSE(board.getEnPassantStatus(0, 1).second) << "En passant should no longer be possible!";
}

/**
 * Test the ChessBoard::getPiece method returns the correct piece at a given tile.
 */
TEST(ChessBoardTests, GetPieceAtTile){
    ChessBoard board;
    std::unique_ptr<Piece> pawn1B = std::make_unique<Pawn>(Piece::Team::BLACK);
    Piece::Team team = pawn1B->getTeam();
    int health = pawn1B->getHealth();
    int damage = pawn1B->getDamage();
    board.setPiece(0, 0, std::move(pawn1B));
    EXPECT_TRUE(board.getPiece(0, 0) != nullptr) << "There should be a piece at (0, 0)!";
    EXPECT_TRUE(board.getPiece(1, 1) == nullptr) << "There should be no piece at (1, 1)!";

    EXPECT_TRUE(board.getPiece(0, 0)->getTeam() == team) << "Team should be the same as in pawn declared first!";
    EXPECT_TRUE(board.getPiece(0, 0)->getHealth() == health) << "Healt should be the same as in pawn declared first!";
    EXPECT_TRUE(board.getPiece(0, 0)->getDamage() == damage) << "Damage should be the same as in pawn declared first!";
}