#include "Piece.hpp"
#include <iostream>

Piece::Piece() : _name{"unknown"}, _damage{1}, _health{1}{
}
Piece::Piece(std::string name, const int damage, const Team team) : _name{name}, _damage{damage}, 
_team{team}, _health{1}{}
Piece::Piece(std::string name, const int damage, const int health, const Team team) : _name{name}, _damage{damage}, 
_team{team}, _health{health}{}
Piece::~Piece() = default;
std::string Piece::getName() const{
    return _name;
}
int Piece::getHealth() const{
    return _health;
}
int Piece::getDamage() const{
    return _damage;
}
void Piece::setName(const std::string name){
    _name = name;
}
void Piece::setHealth(const int health){
    _health = health;
}
void Piece::setDamage(const int damage){
    _damage = damage;
}

void Piece::increaseMoveNumber(){
    _move_number++;
}

void Piece::setMoveNumber(const int move_number){
    _move_number = move_number;
}
int Piece::getMoveNumber() const{
    return _move_number;
}
Piece::Team Piece::getTeam() const {
    return _team;
}
void Piece::setTeam(const Team team) {
    _team = team;
}
Piece::Type Piece::getType() const {
    return _type;
}
void Piece::setType(const Type type) {
    _type = type;
}