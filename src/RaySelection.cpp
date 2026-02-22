#include "RaySelection.hpp"
#include <cmath>

RaySelection::RaySelection(){}

void RaySelection::screenToWorld(int mouseX, int mouseY){
    glGetDoublev( GL_MODELVIEW_MATRIX, _modelView ); 
    glGetDoublev( GL_PROJECTION_MATRIX, _projection ); 
    glGetIntegerv( GL_VIEWPORT, _viewport ); 
    GLdouble x1, y1, z1;
    GLdouble x2, y2, z2;
    double winX = static_cast<double>(mouseX); 
    double winY = _viewport[3] - static_cast<double>(mouseY); 
    gluUnProject(winX, winY, 0.0, _modelView, _projection, 
                _viewport, &x1, &y1, &z1); 
    gluUnProject(winX, winY, 1.0, _modelView, _projection, 
                _viewport, &x2, &y2, &z2);
    
    _rayOrigin = sf::Vector3f(x1, y1, z1);
    _rayDirection = sf::Vector3f(x2 - x1, y2 - y1, z2 - z1);

    float len = std::sqrt(
        _rayDirection.x * _rayDirection.x + 
        _rayDirection.y * _rayDirection.y + 
        _rayDirection.z * _rayDirection.z);
    _rayDirection /= len;
}

bool RaySelection::intersectBoard(){
    if (std::abs(_rayDirection.y) < 0.0001f){
        return false;
    }
    float t = -_rayOrigin.y / _rayDirection.y;
    if ( t < 0){
        return false;
    }
    _hitpoint = _rayOrigin + _rayDirection * t;
    return true;
}

std::optional<sf::Vector2i> RaySelection::worldToBoardSquare(const Chessboard3D chess3D, int horizontal, int vertical){
    float tileW = chess3D.getSize().x;
    float tileH = chess3D.getSize().y;

    int render_i = static_cast<int>(std::floor(_hitpoint.x / tileW));
    int render_j = static_cast<int>(std::floor(_hitpoint.z / tileH));


    if (render_i < 0 || render_i >= horizontal || render_j < 0 || render_j >= vertical){
        return std::nullopt;
    }

    constexpr bool mirrorX = false; 
    constexpr bool mirrorY = true;

    const int i = mirrorX ? (horizontal - 1 - render_i) : render_i;
    const int j = mirrorY ? (vertical - 1 - render_j) : render_j;
    return sf::Vector2i(i, j);
}

sf::Vector3f RaySelection::getRayOrigin() const{
    return _rayOrigin;
}
sf::Vector3f RaySelection::getRayDirection() const{
    return _rayDirection;
}
sf::Vector3f RaySelection::getHitPoint() const{
    return _hitpoint;
}