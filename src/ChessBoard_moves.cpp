#include "ChessBoard.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

void ChessBoard::move(const std::size_t o_x, const std::size_t o_y, const int x, const int y){
    // Check if the original position is within bounds
    if(o_x >= _horizontal || o_y >= _vertical){
        std::cerr << "Error: Attempt to invoke move on piece out of bounds at (" << o_x << ", " << o_y << ")." << std::endl;
        return;
    }
    // Check if there is a piece at the original position
    if(pieces[o_x][o_y] == nullptr){
        std::cerr << "Error: No piece at (" << o_x << ", " << o_y << ") to move." << std::endl;
        return;
    }
    // Check if the target square is within bounds
    if(o_x+x >= _horizontal || o_y+y >= _vertical){
        std::cerr << "Error: Attempt to move "<< pieces[o_x][o_y]->getName() << " out of bounds to (" << o_x+x << ", " << o_y+y << ")." << std::endl;
        return;
    }
    //If the piece is a king, it's position has to be updated after it's move
    Piece::Team team;
    if(pieces[o_x][o_y]->getType() == Piece::Type::KING && pieces[o_x][o_y]->getTeam() == Piece::Team::WHITE){
        team = Piece::Team::WHITE;
    } else{
        team = Piece::Team::BLACK;
    }
    //If the piece is a pawn and can perform en passant
    if(enPassantAvailable(o_x, o_y, x, y)){
        std::cout << pieces[o_x][o_y] -> getName() << " performs en passant at (" << o_x+x << ", " << o_y+y << ")." << std::endl;
        enPassant(o_x, o_y, x, y);
        return;
    }
    
    // If the piece can move to the target square and there is no piece
    else if(pieces[o_x][o_y]->move(x, y) && pieces[o_x+x][o_y+y] == nullptr){
        pieces[o_x][o_y]->increaseMoveNumber();
        pieces[o_x+x][o_y+y] = std::move(pieces[o_x][o_y]);
        pieces[o_x][o_y] = nullptr;
    } 
    // If the piece can attack the target square and there is an enemy piece
    else if(pieces[o_x][o_y]->attack(x, y) && pieces[o_x+x][o_y+y] != nullptr && 
    pieces[o_x+x][o_y+y]->getTeam() != pieces[o_x][o_y]->getTeam()){
        std::cout << pieces[o_x][o_y]->getName() << " attacks " << pieces[o_x+x][o_y+y]->getName() << " at (" << o_x+x << ", " << o_y+y << ")." << std::endl;
        damagePiece(o_x+x, o_y+y);
        if(pieces[o_x+x][o_y+y] == nullptr){
            pieces[o_x+x][o_y+y] = std::move(pieces[o_x][o_y]);
            pieces[o_x][o_y] = nullptr;
        }
    } 
    // If castling can be performed
    else if(canCastle(o_x, o_y, x, y)){
        std::cout << pieces[o_x][o_y]->getName() << " is castling with " << pieces[o_x+x][o_y+y]->getName() << " at (" << o_x+x << ", " << o_y+y << ")." << std::endl;
        castling(o_x, o_x+x, team);
        return;
    }
    else {
        std::cout << pieces[o_x][o_y]->getName() << " cannot be moved to this square." << std::endl;
    }
    if(pieces[o_x+x][o_y+y]->getType() == Piece::Type::PAWN && pieces[o_x+x][o_y+y]->getTeam() == Piece::Team::WHITE && (o_y+y == getVertical() - 1)){
        std::cout << pieces[o_x+x][o_y+y]->getName() << " is being promoted at (" << o_x+x << ", " << o_y+y << ")." << std::endl;
        pawnPromotionChoice(o_x+x, o_y+y);
    } else if(pieces[o_x+x][o_y+y]->getType() == Piece::Type::PAWN && pieces[o_x+x][o_y+y]->getTeam() == Piece::Team::BLACK && (o_y+y == 0)){
        std::cout << pieces[o_x+x][o_y+y]->getName() << " is being promoted at (" << o_x+x << ", " << o_y+y << ")." << std::endl;
        pawnPromotionChoice(o_x+x, o_y+y);
    }
    if(pieces[o_x+x][o_y+y]->getType() == Piece::Type::KING && pieces[o_x+x][o_y+y]->getTeam() == team && team == Piece::Team::WHITE){
        setWhiteKingsPosition(o_x+x, o_y+y);
    } else if(pieces[o_x+x][o_y+y]->getType() == Piece::Type::KING && pieces[o_x+x][o_y+y]->getTeam() == team && team == Piece::Team::BLACK){
        setBlackKingsPosition(o_x+x, o_y+y);
    }
}

bool ChessBoard::canMove(const std::size_t o_x, const std::size_t o_y, const int x, const int y) const {
    if(o_x >= _horizontal || o_y >= _vertical){
        std::cerr << "Error: Attempt to check move on piece out of bounds at (" << o_x << ", " << o_y << ")." << std::endl;
        return false;
    }
    if(pieces[o_x][o_y] == nullptr){
        // std::cerr << "Error: No piece at (" << o_x << ", " << o_y << ") to check move." << std::endl;
        return false;
    }
    int target_x = static_cast<int>(o_x) + x;
    int target_y = static_cast<int>(o_y) + y;

    if(target_x < 0 || target_x >= static_cast<int>(_horizontal) || target_y < 0 || target_y >= static_cast<int>(_vertical)){
        return false;
    }

    if(pieces[o_x][o_y]->getType() == Piece::Type::KING) {
        if(canCastle(o_x, o_y, x, y)) {
            return true; 
        }
    }
    if(pieces[target_x][target_y] && pieces[target_x][target_y]->getTeam() == pieces[o_x][o_y]->getTeam()){
        // std::cerr << "Error: Cannot move, there is an allied piece on the square: (" << static_cast<int>(o_x)+x << ", " << static_cast<int>(o_y)+y << ")\n";
        return false;
    }
    // if(canCastle(o_x, o_y, x, y)) {
    //     return true;
    // }

    bool isAttack = (pieces[target_x][target_y] != nullptr) || enPassantAvailable(o_x, o_y, x, y);
    bool validPath = false;
    if(isAttack){
        validPath = pieces[o_x][o_y]->attack(x, y);
        if(!validPath && enPassantAvailable(o_x, o_y, x, y)){
            validPath = true;
        }
    } else {
        validPath = pieces[o_x][o_y]->move(x, y);
    }

    if(!validPath){
        return false;
    }
    if(pieceOnPath(o_x, o_y, x, y)){
        // std::cerr << "Error: Cannot move, there is a piece on the path." << std::endl;
        return false;
    }
    // if(pieces[o_x][o_y]->getType() == Piece::Type::KING && inCheck(pieces[o_x][o_y]->getTeam(), std::make_pair(target_x, target_y))){
    if(kingWouldBeInCheck(pieces[o_x][o_y]->getTeam(), std::make_pair(o_x, o_y), std::make_pair(target_x, target_y))){
        // std::cerr << "Error: Cannot move, check on square: (" << static_cast<int>(o_x)+x << ", " << static_cast<int>(o_y)+y << ")\n";
        return false;
    }
    return true;
}

bool ChessBoard::canAttackKing(const std::size_t o_x, const std::size_t o_y, const int x, const int y) const {
    if(o_x >= _horizontal || o_y >= _vertical){
        std::cerr << "Error: Attempt to check move on piece out of bounds at (" << o_x << ", " << o_y << ")." << std::endl;
        return false;
    }
    if(pieces[o_x][o_y] == nullptr){
        std::cerr << "Error: No piece at (" << o_x << ", " << o_y << ") to check move." << std::endl;
        return false;
    }
    if(pieceOnPath(o_x, o_y, x, y)){
        // std::cout << "Cannot move, there is a piece on the path." << std::endl;
        return false;
    }
    if(pieces[o_x+x][o_y+y] && pieces[o_x+x][o_y+y]->getTeam() == pieces[o_x][o_y]->getTeam()){
        // std::cout << "Error: Cannot move, there is an allied piece on the square.\n";
        return false;
    }
    if(pieces[o_x+x][o_y+y]){
        return pieces[o_x][o_y]->attack(x, y);
    }
    return pieces[o_x][o_y]->move(x, y);
}
bool ChessBoard::canCastle(const std::size_t o_x, const std::size_t o_y, const int x, const int y) const {
    if(!pieces[o_x+x][o_y+y]) {
        return false;
    }
    if(pieces[o_x][o_y]->getType() != Piece::Type::KING || pieces[o_x+x][o_y+y]->getType() != Piece::Type::ROOK){
        // std::cerr << "Error: Castling can only be performed with a King and a Rook." << std::endl;
        return false;
    }
    if(pieces[o_x][o_y]->getTeam() != pieces[o_x+x][o_y+y]->getTeam()) {
        return false;
    }
    if(pieces[o_x+x][o_y+y]->getMoveNumber() > 0 || pieces[o_x][o_y]->getMoveNumber() > 0){
        // std::cerr << "Error: Castling cannot be performed as either the King or the Rook has already moved." << std::endl;
        return false;
    }
    bool direction = (x > 0) ? true : false;
    int start_x = static_cast<int>(o_x) + (direction ? 1 : -1);
    int end_x = static_cast<int>(o_x) + x;
    for(; direction ? start_x < end_x : start_x > end_x; start_x += direction ? 1 : -1){
        if(pieces[start_x][o_y] != nullptr){
            // std::cout << "Cannot castle, there is a piece at (" << start_x << ", " << o_y << ")." << std::endl;
            return false;
        }
    }
    return true;
}
void ChessBoard::castling(const std::size_t k_x, const std::size_t r_x, const Piece::Team team){
    size_t y = Piece::Team::WHITE == team ? 0 : 7; 
    if(r_x < k_x){
        pieces[k_x][y] -> increaseMoveNumber();
        pieces[k_x-2][y] = std::move(pieces[k_x][y]);
        // pieces[k_x][y] == nullptr;
        pieces[k_x-1][y] = std::move(pieces[r_x][y]);
        // pieces[r_x][y] == nullptr;
    } else if(r_x > k_x){
        pieces[k_x][y] -> increaseMoveNumber();
        pieces[k_x+2][y] = std::move(pieces[k_x][y]);
        // pieces[k_x][y] == nullptr;
        pieces[k_x+1][y] = std::move(pieces[r_x][y]);
        // pieces[r_x][y] == nullptr;
    } else {
        std::cerr << "Error: Invalid castling move." << std::endl;
    }

}

void ChessBoard::pawnPromotionChoice(const std::size_t o_x, const std::size_t o_y) {
    std::cout << "Choose a piece to promote your pawn to (Q for Queen, R for Rook, B for Bishop, N for Knight): ";
    char choice;
    std::cin >> choice;
    switch(toupper(choice)) {
        case 'Q':
            pawnPromotion(o_x, o_y, Piece::Type::QUEEN);
            break;
        case 'R':
            pawnPromotion(o_x, o_y, Piece::Type::ROOK);
            break;
        case 'B':
            pawnPromotion(o_x, o_y, Piece::Type::BISHOP);
            break;
        case 'N':
            pawnPromotion(o_x, o_y, Piece::Type::KNIGHT);
            break;
        default:
            std::cout << "Invalid choice. Promoting to Queen by default." << std::endl;
            pawnPromotion(o_x, o_y, Piece::Type::QUEEN);
    }
}

void ChessBoard::pawnPromotion(const std::size_t o_x, const std::size_t o_y, Piece::Type type) {
    std::unique_ptr<Piece> promoted_piece;
    switch(type) {
        case Piece::Type::QUEEN:
            promoted_piece = std::make_unique<Queen>(pieces[o_x][o_y]->getTeam());
            break;
        case Piece::Type::ROOK:
            promoted_piece = std::make_unique<Rook>(pieces[o_x][o_y]->getTeam());
            break;
        case Piece::Type::BISHOP:
            promoted_piece = std::make_unique<Bishop>(pieces[o_x][o_y]->getTeam());
            break;
        case Piece::Type::KNIGHT:
            promoted_piece = std::make_unique<Knight>(pieces[o_x][o_y]->getTeam());
            break;
        default:
            std::cerr << "Invalid promotion type. Defaulting to Queen." << std::endl;
            promoted_piece = std::make_unique<Queen>(pieces[o_x][o_y]->getTeam());
    }
    pieces[o_x][o_y] = nullptr;
    pieces[o_x][o_y] = std::move(promoted_piece);
    std::cout << "Pawn at (" << o_x << ", " << o_y << ") has been promoted to " 
              << pieces[o_x][o_y]->getName() << "." << std::endl;
}


bool ChessBoard::enPassantAvailable(const std::size_t o_x, const std::size_t o_y, const int x, const int y) const{
    // Only pawns can perform en passant
    if(pieces[o_x][o_y] && pieces[o_x][o_y]->getType() == Piece::Type::PAWN){
        // Check if the move is diagonal and one square forward
        if(pieces[o_x+x][o_y+y] == nullptr && abs(x) == 1 
        && ((pieces[o_x][o_y]->getTeam() == Piece::Team::WHITE 
        && y == 1) || (pieces[o_x][o_y]->getTeam() == Piece::Team::BLACK && y == -1))){
            // Check if the pawn can be captured en passant
            if(pieces[o_x+x][o_y] && pieces[o_x+x][o_y]->getType() == Piece::Type::PAWN && 
               pieces[o_x+x][o_y]->getTeam() != pieces[o_x][o_y]->getTeam()){
                Pawn* adjacentPawn = dynamic_cast<Pawn*>(pieces[o_x+x][o_y].get());
                return (adjacentPawn->getEnPassant().second && adjacentPawn->getEnPassant().first > 0);
            }
    }
}
return false;
}

bool ChessBoard::pieceOnPath(const std::size_t o_x, const std::size_t o_y, const int x, const int y) const{
    if(pieces[o_x][o_y]->getType() == Piece::Type::KNIGHT){
        return false;
    }
    if(o_x == static_cast<std::size_t>(x) && o_y == static_cast<std::size_t>(y)){
        return false;
    }
    int temp_x;
    if(x > 0){
        temp_x = 1;
    } else if(x < 0){
        temp_x = -1;
    } else{
        temp_x = 0;
    }
    int temp_y;
    if(y > 0){
        temp_y = 1;
    } else if(y < 0){
        temp_y = -1;
    } else {
        temp_y = 0;
    }
    int start_x = static_cast<int>(o_x) + temp_x, start_y = static_cast<int>(o_y) + temp_y;
    int end_x = static_cast<int>(o_x) + x, end_y = static_cast<int>(o_y) + y;
    if(start_x == end_x) temp_x = 0;
    if(start_y == end_y) temp_y = 0;
    if(!temp_x){
        // std::cout << "x = "<< temp_x << std::endl;
        for(int j = start_y; temp_y == 1 ? j < end_y : j > end_y; j+=temp_y){
            // std::cout << "y = "<< j << "; x = " << start_x << std::endl;
            if(pieces[start_x][j]){
                return true;
            }
        }
        return false;
    }
    if(!temp_y){
        // std::cout << "y = " << temp_y << std::endl;
        for(int i = start_x; temp_x == 1 ? i < end_x : i > end_x; i+=temp_x){
            // std::cout << "x = " << i << "; y = " << start_y << std::endl;
            if(pieces[i][start_y]){
                return true;
            }
        }
        return false;
    }
    for(int i = start_x, j = start_y; (temp_x == 1 ? i < end_x : i > end_x) && (temp_y == 1 ? j < end_y : j > end_y); i+=temp_x, j+=temp_y){
            // std::cout << "x = " << i << "; y = " << j << std::endl;
            if(i < 0 || i >= static_cast<int>(_horizontal) || j < 0 || j >= static_cast<int>(_vertical)){
                return true;
            }
            if(pieces[i][j]){
                return true;
            }
    }
    return false;
}

void ChessBoard::enPassant(const std::size_t o_x, const std::size_t o_y, const int x, const int y){
    if(abs(x) == 1 && abs(y) == 1){
    // Assuming pieces[o_x+x][o_y] is a Pawn and can be captured en passant
        damagePiece(o_x+x, o_y);
        if(pieces[o_x+x][o_y] != nullptr){
            Pawn* pawn = dynamic_cast<Pawn*>(pieces[o_x+x][o_y].get());
            pawn->setEnPassant(0, false); // Reset en passant status
        }
        pieces[o_x+x][o_y+y] = std::move(pieces[o_x][o_y]);
        pieces[o_x][o_y] = nullptr; // Clear the original position
    }
}

