#include "GameTerminal.hpp"
#include <string>
#include <sstream>
#include <regex>

GameTerminal::GameTerminal() : _turn{1}, _chessBoard{8, 8}{
    _chessBoard.initPieces();
}

GameTerminal::GameTerminal(const size horizontal, const size vertical) : _turn{1}, _chessBoard{horizontal, vertical}{
    _chessBoard.initPieces();
}
GameTerminal::GameTerminal(const int turn, ChessBoard&& chessBoard) : _turn{turn}, _chessBoard{std::move(chessBoard)}{
    
}
void GameTerminal::printCurrentGameState(std::ostream& os) const{
    _chessBoard.printBoard(os);
    Piece::Team team = _turn % 2 == 0 ? Piece::Team::BLACK : Piece::Team::WHITE;
    os << "Team: " << (_turn % 2 == 0 ? "Black" : "White") << std::endl;
    _chessBoard.kingStatus(team, os);
    os << "Turn: " << _turn << std::endl;
}
bool GameTerminal::surrender(){
    std::string command;
    
    while(true){
        std::cout << "Enter command: ";
        std::cin >> command;
        std::for_each(command.begin(), command.end(), [](char &c) {
                c = toupper(c); // Convert to uppercase
        });
        if(command == "Y" || command == "YES"){
            std::cout << "You have surrendered...\n";
            // exit(EXIT_SUCCESS);
            return false;
        } else if(command == "N" || command == "NO" || command == "NOPE" || command == "NIE" || command == "C" || command == "CANCEL" || command == "CANCEL SURRENDER"){
            std::cout << "You have not surrendered.\n";
            return true;
        } else {
            std::cout << "Invalid command! Try again.\n";
        }
}   
}

void GameTerminal::enPassantTurnCycle(){
    if(_turn % 2 == 0){
        _chessBoard.enPassantTurnUpdate(Piece::Team::BLACK);
    } else{
        _chessBoard.enPassantTurnUpdate(Piece::Team::WHITE);
    }
}

void GameTerminal::restart(){
    _chessBoard.reset();
    _turn = 1;
}
bool GameTerminal::quitGame(){
    std::string command;
    while(true){
        std::cout << "Enter command: ";
        std::cin >> command;
        std::for_each(command.begin(), command.end(), [](char &c) {
                c = toupper(c); // Convert to uppercase
        });
        if(command == "Y" || command == "YES"){
            std::cout << "You have quit the game...\n";
            return false;
        } else if(command == "N" || command == "NO" || command == "NOPE" || command == "NIE" || command == "C" || command == "CANCEL" || command == "CANCEL QUIT"){
            std::cout << "You have not quit the game.\n";
            return true;
        } else {
            std::cout << "Invalid command! Try again.\n";
        }
}
}

void GameTerminal::movePiece(){
    std::string square;
    size o_y, o_x;
    while(true){
        std::cout << "Enter square: ";
        std::cin >> square;
        std::for_each(square.begin(), square.end(), [](char &c) {
            c = toupper(c); // Convert to uppercase
        });
        if(square == "C" || square == "CANCEL" || square == "CANCEL MOVE"){
            std::cout << "Cancelling...\n";
            return;
        }
        std::stringstream ss(square);
        char temp_letter;
        ss >> o_y >> temp_letter;
        o_y = static_cast<size>(o_y - 1); // Convert to 0-based index
        o_x = static_cast<size>(toupper(temp_letter) - 65); // Convert letter to index
        if(o_x >= _chessBoard.getHorizontal() || o_y >= _chessBoard.getVertical()){
            std::cout << "Invalid square! Try again.\n";
            continue;
        }
        if(!_chessBoard.isPieceAt(o_x, o_y)){
            std::cout << "There is no piece at square " << square << "! Try again.\n";
            continue;
        }
        if(_chessBoard.getTeamOfPiece(o_x, o_y) != (_turn % 2 == 0 ? Piece::Team::BLACK : Piece::Team::WHITE)){
            std::cout << "You cannot move a piece that is not yours! Try again.\n";
            continue;
        }
        break;
    }
    std::cout << "Piece to move: " << square << std::endl;
    selectTargetSquare(o_x, o_y);
    return;
}

void GameTerminal::movePiece(std::string square){
    std::for_each(square.begin(), square.end(), [](char &c) {
        c = toupper(c); // Convert to uppercase
    });
    std::stringstream ss(square);
    char temp_letter;
    size o_x, o_y;
    ss >> o_y >> temp_letter;
    o_y = static_cast<size>(o_y - 1); // Convert to 0-based index
    o_x = static_cast<size>(toupper(temp_letter) - 65); // Convert letter to index
    if(o_x >= _chessBoard.getHorizontal() || o_y >= _chessBoard.getVertical()){
        std::cout << "Invalid square! Try again.\n";
        movePiece();
        return;        
    }
    if(!_chessBoard.isPieceAt(o_x, o_y)){
        std::cout << "There is no piece at square " << square << "! Try again.\n";
        movePiece();
        return;
    }
    if(_chessBoard.getTeamOfPiece(o_x, o_y) != (_turn % 2 == 0 ? Piece::Team::BLACK : Piece::Team::WHITE)){
        std::cout << "You cannot move a piece that is not yours! Try again.\n";
        movePiece();
        return;
    }
    std::cout << "Piece to move: " << square << std::endl;
    selectTargetSquare(o_x, o_y);
    return;
}

void GameTerminal::selectTargetSquare(std::size_t o_x, std::size_t o_y){
    std::cout << "Now, choose where should this piece move to!\n";
    int x, y;
    std::string target_square;
    while(true){
        std::cout << "Enter square: ";
        std::cin >> target_square;
        std::for_each(target_square.begin(), target_square.end(), [](char &c) {
            c = toupper(c); // Convert to uppercase
        });
        if(target_square == "C" || target_square == "CANCEL" || target_square == "CANCEL MOVE"){
            std::cout << "Cancelling...\n";
            return;
        }
        std::stringstream ss_target(target_square);
        char temp_letter;
        ss_target >> y >> temp_letter;
        int target_y = target_square[0] - '1';
        int target_x = toupper(target_square[1]) - 'A';

        y = target_y - static_cast<int>(o_y);
        x = target_x - static_cast<int>(o_x);
        if(o_x+x >= _chessBoard.getHorizontal() || o_y+y >= _chessBoard.getVertical()){
            std::cout << "Invalid square! Try again.\n";
            continue;
        }
        if(_chessBoard.canMove(o_x, o_y, x, y)){
            std::cout << "Moving piece from (" << o_x << ", " << o_y << ") to (" << o_x+x << ", " << o_y+y << ").\n";
            break;
        } else {
            std::cout << "Piece cannot be moved to this square! Try again.\n";
            continue;
        }
        break;
    }
    // std::cout << o_x << " " << o_y << " " << x << " " << y << std::endl;
    _chessBoard.move(o_x, o_y, x, y);
    nextTurn();
}
void GameTerminal::gameLoop(){
    bool temp = true;
    while(temp){
        enPassantTurnCycle();
        printCurrentGameState();

        std::string command;
        std::cout << "Choose next command:\n"<<
        "s) Surrender\t   m) Move\t  q) Quit\n";

        std::cin >> command;
        std::for_each(command.begin(), command.end(), [](char &c) {
            c = toupper(c); // Convert to uppercase
        });
        std::regex pattern{"^(?:[0-9]|1[0-9]|2[0-5])[a-zA-Z]$"}; 
        if(command == "S" || command == "SURRENDER" || command == "SURENDER"){
            std::cout << "Are you sure you want to surrender?\n"<<
            "y) Yes\t   n) No\n";
            temp = surrender();
        }
        else if(command == "Q" || command == "QUIT"){
            std::cout << "Are you sure you want to quit the game?\n"<<
            "y) Yes\t   n) No\n";
            temp = quitGame();
        }
        else if(std::regex_match(command, pattern)){
            movePiece(command);
        }
        else if(command == "M" || command == "MOVE"){
            std::cout << "Choose piece you want to move! Write code of the square it's standing at!\n";
            std::cout << "Example of squares: 1A, 03B, 6D. Make sure it's not out of bounds!\n";
            movePiece();
        } 
        else if(command == "R" || command == "RESTART"){
            restart();
        } 
        else{
            std::cout << "Invalid command! Try again.\n";
        }
    }
}
void GameTerminal::nextTurn(){
    _turn++;
}
void GameTerminal::previousTurn(){
    _turn--;
}
int GameTerminal::getTurn() const {
    return _turn;
}
const ChessBoard& GameTerminal::getChessBoard() const {
    return _chessBoard;
}