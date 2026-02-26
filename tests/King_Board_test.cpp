#include <gtest/gtest.h>
#include "ChessBoard.hpp"
#include "Piece.hpp"
#include "Pawn.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "King.hpp"

/**
 * Test the Black King position is correctly set on the board after move.
 */
TEST(KingBoardTests, BlackKingCorrectPositionAfterMove){
    ChessBoard chessBoard;
    std::unique_ptr<Piece> king1B = std::make_unique<King>(Piece::Team::BLACK);
    chessBoard.setPiece(4, 4, std::move(king1B));
    chessBoard.setBlackKingsPosition(4, 4);
    chessBoard.move(4, 4, 1, 0); // Move king right by 1
    EXPECT_TRUE(chessBoard.getBlackKingsPosition().first == 5 && chessBoard.getBlackKingsPosition().second == 4) << "King position should be updated to (5, 4)!";
}

/**
 * Test the White King position is correctly set on the board after move.
 */
TEST(KingBoardTests, WhiteKingCorrectPositionAfterMove){
    ChessBoard chessBoard;
    std::unique_ptr<Piece> king1W = std::make_unique<King>(Piece::Team::WHITE);
    chessBoard.setPiece(4, 4, std::move(king1W));
    chessBoard.setWhiteKingsPosition(4, 4);
    chessBoard.move(4, 4, 1, 0); // Move king right by 1
    EXPECT_TRUE(chessBoard.getWhiteKingsPosition().first == 5 && chessBoard.getWhiteKingsPosition().second == 4) << "King position should be updated to (5, 4)!";
}

/**
 * Test the ChessBoard::inCheck method detects check correctly for white king. Outer square no check scenario.
 */
TEST(KingBoardTests, OuterSquareNoCheckVer1White){
    ChessBoard board;
    board.setPiece(3, 2, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(3, 2);
    board.setPiece(0, 0, std::make_unique<Rook>(Piece::Team::BLACK));
    
    EXPECT_FALSE(board.inCheck(Piece::Team::WHITE)) << "White King should not be in check!";
}

TEST(KingBoardTests, OuterSquareNoCheckVer2White){
    ChessBoard board;
    board.setPiece(5, 5, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(5, 5);
    board.setPiece(0, 0, std::make_unique<Rook>(Piece::Team::BLACK));
    
    EXPECT_FALSE(board.inCheck(Piece::Team::WHITE)) << "White King should not be in check!";
}


/**
 * Test the ChessBoard::inCheck method detects check correctly for white king. Inner square no check scenario.
 */
TEST(KingBoardTests, InnerSquareNoCheckWhite){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(6, 4, std::make_unique<Knight>(Piece::Team::BLACK));
    
    EXPECT_FALSE(board.inCheck(Piece::Team::WHITE)) << "White King should not be in check!";
}

/**
 * Test the ChessBoard::inCheck method detects check correctly for black king. Outer square no check scenario.
 */
TEST(KingBoardTests, OuterSquareNoCheckVer1Black){
    ChessBoard board;
    board.setPiece(3, 2, std::make_unique<King>(Piece::Team::BLACK));
    board.setBlackKingsPosition(3, 2);
    board.setPiece(0, 0, std::make_unique<Rook>(Piece::Team::WHITE));
    
    EXPECT_FALSE(board.inCheck(Piece::Team::BLACK)) << "Black King should not be in check!";
}

TEST(KingBoardTests, OuterSquareNoCheckVer2Black){
    ChessBoard board;
    board.setPiece(5, 5, std::make_unique<King>(Piece::Team::BLACK));
    board.setBlackKingsPosition(5, 5);
    board.setPiece(0, 0, std::make_unique<Rook>(Piece::Team::WHITE));
    
    EXPECT_FALSE(board.inCheck(Piece::Team::BLACK)) << "Black King should not be in check!";
}


/**
 * Test the ChessBoard::inCheck method detects check correctly for black king. Inner square no check scenario.
 */
TEST(KingBoardTests, InnerSquareNoCheckBlack){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::BLACK));
    board.setBlackKingsPosition(4, 4);
    board.setPiece(6, 4, std::make_unique<Knight>(Piece::Team::WHITE));
    
    EXPECT_FALSE(board.inCheck(Piece::Team::BLACK)) << "Black King should not be in check!";
}

/**
 * Test the ChessBoard::inCheck method does not detect check from allied piece for white king. Inner square scenario.
 */
TEST(KingBoardTests, InnerSquareCheckAlliedWhite){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(6, 4, std::make_unique<Rook>(Piece::Team::WHITE));
    
    EXPECT_FALSE(board.inCheck(Piece::Team::WHITE)) << "White King should not be in check from allied piece!";
}

/**
 * Test the ChessBoard::inCheck method does not detect check from allied piece for black king. Inner square scenario.
 */
TEST(KingBoardTests, InnerSquareCheckAlliedBlack){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::BLACK));
    board.setBlackKingsPosition(4, 4);
    board.setPiece(6, 4, std::make_unique<Rook>(Piece::Team::BLACK));
    
    EXPECT_FALSE(board.inCheck(Piece::Team::BLACK)) << "Black King should not be in check from allied piece!";
}

/**
 * Test the ChessBoard::inCheck method detects check correctly for white king. Inner square check scenario.
 */
TEST(KingBoardTests, InnerSquareCheckWhite){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(6, 4, std::make_unique<Rook>(Piece::Team::BLACK));
    
    EXPECT_TRUE(board.inCheck(Piece::Team::WHITE)) << "White King should be in check!";
}
/**
 * Test the ChessBoard::inCheck method detects check correctly for black king. Inner square check scenario.
 */
TEST(KingBoardTests, InnerSquareCheckBlack){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::BLACK));
    board.setBlackKingsPosition(4, 4);
    board.setPiece(6, 4, std::make_unique<Rook>(Piece::Team::WHITE));
    
    EXPECT_TRUE(board.inCheck(Piece::Team::BLACK)) << "Black King should be in check!";
}
/**
 * Test the ChessBoard::inCheck method detects check correctly for white king. Outer square check scenario.
 */
TEST(KingBoardTests, OuterSquareCheckVer1White){
    ChessBoard board;
    board.setPiece(3, 2, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(3, 2);
    board.setPiece(3, 0, std::make_unique<Rook>(Piece::Team::BLACK));
    
    EXPECT_TRUE(board.inCheck(Piece::Team::WHITE)) << "White King should be in check!";
}

TEST(KingBoardTests, OuterSquareCheckVer2White){
    ChessBoard board;
    board.setPiece(5, 5, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(5, 5);
    board.setPiece(1, 1, std::make_unique<Bishop>(Piece::Team::BLACK));
    
    EXPECT_TRUE(board.inCheck(Piece::Team::WHITE)) << "White King should be in check!";
}
/**
 * Test the ChessBoard::inCheck method detects check correctly for black king. Outer square check scenario.
 */
TEST(KingBoardTests, OuterSquareCheckVer1Black){
    ChessBoard board;
    board.setPiece(3, 2, std::make_unique<King>(Piece::Team::BLACK));
    board.setBlackKingsPosition(3, 2);
    board.setPiece(3, 0, std::make_unique<Rook>(Piece::Team::WHITE));
    
    EXPECT_TRUE(board.inCheck(Piece::Team::BLACK)) << "Black King should be in check!";
}

TEST(KingBoardTests, OuterSquareCheckVer2Black){
    ChessBoard board;
    board.setPiece(5, 5, std::make_unique<King>(Piece::Team::BLACK));
    board.setBlackKingsPosition(5, 5);
    board.setPiece(1, 1, std::make_unique<Bishop>(Piece::Team::WHITE));
    
    EXPECT_TRUE(board.inCheck(Piece::Team::BLACK)) << "Black King should be in check!";
}

/**
 * Test the ChessBoard::inCheck method does not detect check if allied piece is blocking the path for white king. Inner square scenario.
 */

TEST(KingBoardTests, InnerSquareCheckAllyOnWayWhite){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(5, 4, std::make_unique<Pawn>(Piece::Team::WHITE));
    board.setPiece(6, 4, std::make_unique<Rook>(Piece::Team::BLACK));
    
    EXPECT_FALSE(board.inCheck(Piece::Team::WHITE)) << "White King should not be in check!";
}
/**
 * Test the ChessBoard::inCheck method does not detect check if allied piece is blocking the path for black king. Inner square scenario.
 */

TEST(KingBoardTests, InnerSquareCheckAllyOnWayBlack){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::BLACK));
    board.setBlackKingsPosition(4, 4);
    board.setPiece(5, 4, std::make_unique<Pawn>(Piece::Team::BLACK));
    board.setPiece(6, 4, std::make_unique<Rook>(Piece::Team::WHITE));
    
    EXPECT_FALSE(board.inCheck(Piece::Team::BLACK)) << "Black King should not be in check!";
}

/**
 * Test the ChessBoard::canMove detects if king can move to a square with check.
 */
TEST(KingBoardTests, CantMoveKingToASquareWithCheck){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(5, 5, std::make_unique<Rook>(Piece::Team::BLACK));
    EXPECT_FALSE(board.canMove(4, 4, 1, 0)) << "King should not be able to move to a square with check!";
}

/**
 * Test the ChessBoard::kingCanMove detects if king has any valid moves.
 */

 TEST(KingBoardTests, KingHasNoValidMoves){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(3, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(5, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(7, 5, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(7, 3, std::make_unique<Rook>(Piece::Team::BLACK));

    EXPECT_FALSE(board.kingCanMove(Piece::Team::WHITE)) << "King should not have any valid moves!";
 }

 TEST(KingBoardTests, KingHasValidMoves){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(3, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(5, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(7, 5, std::make_unique<Rook>(Piece::Team::BLACK));

    EXPECT_TRUE(board.kingCanMove(Piece::Team::WHITE)) << "King should have one valid move!";
}

/**
 * Test the ChessBoard::inCheckmate method detects checkmate correctly.
 */

 TEST(KingBoardTests, InCheckmate){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(3, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(4, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(5, 7, std::make_unique<Rook>(Piece::Team::BLACK));

    EXPECT_TRUE(board.inCheckmate(Piece::Team::WHITE)) << "White King should be in checkmate!";
 }

 TEST(KingBoardTests, NotInCheckmate){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(3, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(5, 7, std::make_unique<Rook>(Piece::Team::BLACK));

    EXPECT_FALSE(board.inCheckmate(Piece::Team::WHITE)) << "White King should not be in checkmate!";
}
/**
 * Test the ChessBoard::inStalemate method detects stalemate correctly.
 */
TEST(KingBoardTests, InStalemate){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(3, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(5, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(7, 5, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(7, 3, std::make_unique<Rook>(Piece::Team::BLACK));

    EXPECT_TRUE(board.inStalemate(Piece::Team::WHITE)) << "King should be in stalemate!";
}

TEST(KingBoardTests, InStalemateVer2){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(1, 1, std::make_unique<Pawn>(Piece::Team::WHITE));
    board.setPiece(1, 2, std::make_unique<Pawn>(Piece::Team::BLACK));
    board.setPiece(3, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(5, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(7, 5, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(7, 3, std::make_unique<Rook>(Piece::Team::BLACK));

    EXPECT_TRUE(board.inStalemate(Piece::Team::WHITE)) << "King and pawn should be in stalemate!";
}

TEST(KingBoardTests, NotInStalemate){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(3, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(5, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(7, 5, std::make_unique<Rook>(Piece::Team::BLACK));

    EXPECT_FALSE(board.inStalemate(Piece::Team::WHITE)) << "King should not be in stalemate!";
}

TEST(KingBoardTests, NotInStalemateVer2){
    ChessBoard board;
    board.setPiece(4, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 4);
    board.setPiece(1, 1, std::make_unique<Pawn>(Piece::Team::WHITE));
    board.setPiece(3, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(5, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(7, 5, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(7, 3, std::make_unique<Rook>(Piece::Team::BLACK));

    EXPECT_FALSE(board.inStalemate(Piece::Team::WHITE)) << "King and pawn should be in stalemate!";
}

/**
 * Test if king surrounded by allied pieces or edges of board and one enemy Queen  gets checkmated.
 * @note Doesn't work right now, needs fixing.
 */
TEST(KingBoardTests, SurroundedWhiteKingCheckmatedByQueenOnTheEdge){
    ChessBoard board;
    board.setPiece(0, 0, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(0, 0);
    board.setPiece(0, 1, std::make_unique<Pawn>(Piece::Team::WHITE));
    board.setPiece(1, 0, std::make_unique<Queen>(Piece::Team::BLACK));
    board.setPiece(2, 0, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(1, 1, std::make_unique<Pawn>(Piece::Team::WHITE));
    board.setPiece(4, 2, std::make_unique<King>(Piece::Team::BLACK));
    board.setBlackKingsPosition(4, 2);  
    EXPECT_TRUE(board.inCheck(Piece::Team::WHITE)) << "White King should be in checkmate by Black Queen!";
    EXPECT_TRUE(board.inCheckmate(Piece::Team::WHITE)) << "White King should be in checkmate by Black Queen!";
}

TEST(KingBoardTests, SurroundedWhiteKingCheckmatedByQueen){
    ChessBoard board;
    board.setPiece(3, 4, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(3, 4);
    board.setPiece(2, 3, std::make_unique<Pawn>(Piece::Team::WHITE));
    board.setPiece(2, 4, std::make_unique<Pawn>(Piece::Team::WHITE));
    board.setPiece(2, 5, std::make_unique<Pawn>(Piece::Team::WHITE));
    board.setPiece(4, 3, std::make_unique<Pawn>(Piece::Team::WHITE));
    board.setPiece(4, 4, std::make_unique<Pawn>(Piece::Team::WHITE));
    board.setPiece(4, 5, std::make_unique<Pawn>(Piece::Team::WHITE));
    board.setPiece(3, 5, std::make_unique<Pawn>(Piece::Team::WHITE));
    // board.setPiece(2, 1, std::make_unique<Queen>(Piece::Team::BLACK));
    board.setPiece(3, 3, std::make_unique<Queen>(Piece::Team::BLACK));
    board.setPiece(3, 2, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(6, 6, std::make_unique<King>(Piece::Team::BLACK));
    board.setBlackKingsPosition(6, 6);  
    EXPECT_TRUE(board.inCheck(Piece::Team::WHITE)) << "White King should be in checkmate by Black Queen!";
    EXPECT_TRUE(board.inCheckmate(Piece::Team::WHITE)) << "White King should be in checkmate by Black Queen!";
}

TEST(KingBoardTests, SurroundedBlackKingCheckmatedByQueenOnTheEdge){
    ChessBoard board;
    for(int i = 0; i < 8; i++){
        board.setPiece(i, 1, std::make_unique<Pawn>(Piece::Team::WHITE));
        board.setPiece(i, 6, std::make_unique<Pawn>(Piece::Team::BLACK));
    }
    // board.setPiece(0, 2, std::make_unique<Pawn>(Piece::Team::WHITE));
    // board.setPiece(1, 5, std::make_unique<Pawn>(Piece::Team::BLACK));
    board.setPiece(0, 0, std::make_unique<Rook>(Piece::Team::WHITE));
    board.setPiece(7, 0, std::make_unique<Rook>(Piece::Team::WHITE));
    board.setPiece(0, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    board.setPiece(7, 7, std::make_unique<Rook>(Piece::Team::BLACK));

    board.setPiece(1, 0, std::make_unique<Knight>(Piece::Team::WHITE));
    board.setPiece(2, 3, std::make_unique<Knight>(Piece::Team::WHITE));
    board.setPiece(1, 7, std::make_unique<Knight>(Piece::Team::BLACK));
    board.setPiece(6, 7, std::make_unique<Knight>(Piece::Team::BLACK));

    board.setPiece(2, 0, std::make_unique<Bishop>(Piece::Team::WHITE));
    board.setPiece(5, 0, std::make_unique<Bishop>(Piece::Team::WHITE));
    board.setPiece(2, 7, std::make_unique<Bishop>(Piece::Team::BLACK));
    board.setPiece(5, 7, std::make_unique<Bishop>(Piece::Team::BLACK));

    board.setPiece(5, 2, std::make_unique<Queen>(Piece::Team::WHITE));
    board.setPiece(4, 0, std::make_unique<King>(Piece::Team::WHITE));
    board.setWhiteKingsPosition(4, 0);
    board.setPiece(3, 7, std::make_unique<Queen>(Piece::Team::BLACK));
    board.setPiece(4, 7, std::make_unique<King>(Piece::Team::BLACK));
    board.setBlackKingsPosition(4, 7);

    board.move(5, 2, 0, 4);

    EXPECT_TRUE(board.inCheck(Piece::Team::BLACK)) << "Black King should be in checkmate by Black Queen!";
    EXPECT_TRUE(board.inCheckmate(Piece::Team::BLACK)) << "Black King should be in checkmate by Black Queen!";
}