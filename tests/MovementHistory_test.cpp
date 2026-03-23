#include <gtest/gtest.h>
#include "MovementHistory.hpp"
#include "ChessBoard.hpp"
#include <array>


std::array<std::unique_ptr<ChessBoard>, 20> generateChesboardData() {
    std::array<std::unique_ptr<ChessBoard>, 20> boards;
    for(int i = 0; i < 20; ++i){
        std::cout << i << " ";
        boards[i] = std::make_unique<ChessBoard>();
        boards[i]->initPieces();
    }
    std::cout << "\n";
    return boards;
}


TEST(MovementHistoryTests, CorrectIndexSet) {
    MovementHistory history;
    std::array<std::unique_ptr<ChessBoard>, 20> boards = generateChesboardData();
    for(int i = 0; i < 20; ++i) {
        std::cout << i << " ";
        history.pushNew(*boards.at(i));
    }
    history.setIndex(102);
    EXPECT_FALSE(history.getIndex() == 19) << "Index should not be greater than size of m_history!";
    history.setIndex(10);
    EXPECT_TRUE(history.getIndex() == 10) << "Index should be changed!";
}