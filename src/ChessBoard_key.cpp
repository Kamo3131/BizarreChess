#include "ChessBoard.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

void ChessBoard::init(){
    for(std::size_t i = 0; i<_horizontal; i++){
            std::vector<std::unique_ptr<Piece>> column;
            for(std::size_t j = 0; j<_vertical; j++){
                column.push_back(nullptr);
            }
            pieces.push_back(std::move(column));
        }
}

void ChessBoard::initPieces(){

    for(int i = 0; i < 8; i++){
        setPiece(i, 1, std::make_unique<Pawn>(Piece::Team::WHITE));
        setPiece(i, 6, std::make_unique<Pawn>(Piece::Team::BLACK));
    }
    // setPiece(0, 2, std::make_unique<Pawn>(Piece::Team::WHITE));
    // setPiece(1, 5, std::make_unique<Pawn>(Piece::Team::BLACK));
    setPiece(0, 0, std::make_unique<Rook>(Piece::Team::WHITE));
    setPiece(7, 0, std::make_unique<Rook>(Piece::Team::WHITE));
    setPiece(0, 7, std::make_unique<Rook>(Piece::Team::BLACK));
    setPiece(7, 7, std::make_unique<Rook>(Piece::Team::BLACK));

    setPiece(1, 0, std::make_unique<Knight>(Piece::Team::WHITE));
    setPiece(6, 0, std::make_unique<Knight>(Piece::Team::WHITE));
    setPiece(1, 7, std::make_unique<Knight>(Piece::Team::BLACK));
    setPiece(6, 7, std::make_unique<Knight>(Piece::Team::BLACK));

    setPiece(2, 0, std::make_unique<Bishop>(Piece::Team::WHITE));
    setPiece(5, 0, std::make_unique<Bishop>(Piece::Team::WHITE));
    setPiece(2, 7, std::make_unique<Bishop>(Piece::Team::BLACK));
    setPiece(5, 7, std::make_unique<Bishop>(Piece::Team::BLACK));

    setPiece(3, 0, std::make_unique<Queen>(Piece::Team::WHITE));
    setPiece(4, 0, std::make_unique<King>(Piece::Team::WHITE));
    setWhiteKingsPosition(4, 0);
    setPiece(3, 7, std::make_unique<Queen>(Piece::Team::BLACK));
    setPiece(4, 7, std::make_unique<King>(Piece::Team::BLACK));
    setBlackKingsPosition(4, 7);
}
void ChessBoard::reset() {
    for(std::size_t i = 0; i<_horizontal; i++){
            for(std::size_t j = 0; j<_vertical; j++){
                if(getPiece(i, j)){
                    killPiece(i, j);
                }
            }
        }
    init();
    initPieces();
    std::cout << "Board has been reset\n"; 
}
ChessBoard::ChessBoard() : _horizontal{8}, _vertical{8}{
    init();
}
ChessBoard::ChessBoard(const size horizontal, const size vertical) : _horizontal{horizontal>26 ? 26 : horizontal}, _vertical{vertical>26 ? 26 : vertical}{
    init();
}
std::size_t ChessBoard::getHorizontal() const{
    return _horizontal;
}
std::size_t ChessBoard::getVertical() const{
    return _vertical;
}
std::pair<int, bool> ChessBoard::getEnPassantStatus(const std::size_t o_x, const std::size_t o_y) const{
    if(o_x > _horizontal || o_y > _vertical){
        std::cout << "Error: Atempt to get enpassant status of piece out of bound at (" << o_x << ", " << o_y << ")\n";
        return {-999, false};
    }
    if(pieces[o_x][o_y]->getType() != Piece::Type::PAWN){
        std::cout << "Error: Enpassant status only available to pawns!";
        return {-999, false};
    }
    Pawn * pawn = dynamic_cast<Pawn*>(pieces[o_x][o_y].get());
    return {pawn->getEnPassant().first, pawn->getEnPassant().second};
}

bool ChessBoard::isInitialized() const{
        for(std::size_t i = 0; i < _horizontal; i++){
            for(std::size_t j = 0; j < _vertical; j++){
                if(pieces[i][j] != nullptr){
                    return false;
                }
            }
        }
        return true;
}
void ChessBoard::printSquares(std::ostream& os) const{
    const char char_horizontal = static_cast<char>(_horizontal+65);
    bool condition = _vertical > 9 ? true : false;
    for(int i = _vertical; i>0; i--){
        for(char c = 'A'; c < char_horizontal && c<='Z'; c++){
                std::ostringstream ss;
                if(true==condition){
                    if(i<10){
                        ss << c << '0' << i;
                    } else{
                        ss << c << i;
                    }
                    os << std::setw(4) << ss.str() << " ";
                } else{
                    ss << c << i;
                    os << ss.str() << " ";
                }
            }
            os << std::endl;
        }
    }
void ChessBoard::printBoard(std::ostream& os) const{
        for(int i = _vertical-1; i>-2; i--){
            for(int j = -1; j < static_cast<int>(_horizontal); j++){
                std::ostringstream ss;
                if(-1==j && -1!=i){
                    if(_vertical > 9 && 9>i){
                        ss << '0' << i+1 << "|";
                    } else{
                        ss << i+1 << "|";
                    }
                } else if(-1==j && -1==i){
                    ss << "  ";
                } else if(-1!=j && -1==i){
                    ss << " "<< static_cast<char>(j+65) << " ";
                } else if(pieces[j][i]==nullptr){
                    ss << "  |";
                } else if(pieces[j][i]!=nullptr){
                    // Check if the piece is a Knight
                    if(pieces[j][i]->getName().substr(0, 6) != "Knight"){
                        // If it is not a Knight, display the first letter of the name
                        ss << (pieces[j][i]->getName())[0];
                        if(pieces[j][i]->getTeam() == Piece::Team::WHITE){
                            ss << "W|";
                        } else if(pieces[j][i]->getTeam() == Piece::Team::BLACK){
                            ss << "B|";
                        } else {
                            ss << "?|"; // Unknown team
                        };
                        // If it is a Knight, display 'N' instead
                    } else{
                        ss << "N";
                        if(pieces[j][i]->getTeam() == Piece::Team::WHITE){
                            ss << "W|";
                        } else if(pieces[j][i]->getTeam() == Piece::Team::BLACK ){
                            ss << "B|";
                        } else {
                            ss << "?|"; // Unknown team
                        };
                    }
                }
                os << ss.str();
            }
            os << std::endl;
        }
        os << std::endl;
    }

void ChessBoard::setPiece(const std::size_t o_x, const std::size_t o_y, std::unique_ptr<Piece> piece){
    if(o_x >= _horizontal || o_y >= _vertical){
        std::cerr << "Error: Attempt to set piece out of bounds at (" << o_x << ", " << o_y << ")." << std::endl;
        return;
    }
    if(pieces[o_x][o_y] != nullptr){
        std::cerr << "Error: Square (" << o_x << ", " << o_y << ") is already occupied by " 
                    << pieces[o_x][o_y]->getName() << ". Cannot set new piece." << std::endl;
        return;
    }
    if(piece == nullptr){
        std::cerr << "Error: Attempt to set a null piece at (" << o_x << ", " << o_y << ")." << std::endl;
        return;
    }
    // std::cout << "Setting piece " << piece->getName() << " at (" << o_x << ", " << o_y << ")." << std::endl;
    pieces[o_x][o_y] = std::move(piece);
}

std::unique_ptr<Piece>& ChessBoard::getPiece(const std::size_t o_x, const std::size_t o_y) const{
    if(o_x >= _horizontal || o_y >= _vertical){
        std::cerr << "Error: Attempt to get piece out of bounds at (" << o_x << ", " << o_y << ")." << std::endl;
        // Return a reference to a static null unique_ptr to avoid dangling reference
        static std::unique_ptr<Piece> nullPtr = nullptr;
        return nullPtr;
    }
    return pieces[o_x][o_y];
}

