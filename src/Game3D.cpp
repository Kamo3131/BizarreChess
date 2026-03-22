#include "Game3D.hpp"
#include "Chessboard3D.hpp"
#include <string>
#include <sstream>
#include <regex>

Game3D::Game3D() : _turn{1}, _zoom{0.850f}, _chessBoard{8, 8}{
    _chessBoard.initPieces();
}

Game3D::Game3D(const size horizontal, const size vertical) : _turn{1}, _zoom{0.850f}, _chessBoard{horizontal, vertical}{
    _chessBoard.initPieces();
}
Game3D::Game3D(const int turn, ChessBoard&& chessBoard) : _turn{turn}, _zoom{0.850f}, _chessBoard{std::move(chessBoard)}{
    
}
bool Game3D::surrender(){
    return true;
}

void Game3D::enPassantTurnCycle(){
    if(_turn % 2 == 0){
        _chessBoard.enPassantTurnUpdate(Piece::Team::BLACK);
    } else{
        _chessBoard.enPassantTurnUpdate(Piece::Team::WHITE);
    }
}

bool Game3D::quitGame(){
    return true;
}
void Game3D::restart(){
    _chessBoard.reset();
    _turn = 1;
    _possibleMoves.clear();
}

void Game3D::movePiece(){

}
void Game3D::initOpengl() const{
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
}
void Game3D::setupCamera() const{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluPerspective(45.0, static_cast<double>(viewport[2]) / static_cast<double>(viewport[3]), 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10.0/_zoom, 10.0/_zoom, 10.0/_zoom,
                2.0,  -2.0,  2.0,  
                0.0,  1.0,  0.0);
}

void Game3D::updatePossibleMoves(){
    _possibleMoves.clear();
    if(!_selectedSquare){
        return;
    }
    std::size_t width = _chessBoard.getHorizontal();
    std::size_t height = _chessBoard.getVertical();
    if((_selectedSquare->x < 0 || _selectedSquare->x >= width) 
    && (_selectedSquare->y < 0 || _selectedSquare->y >= height)){
        return;
    }

    for(int i = width - 1; i > -1; i--){
        for(int j = height - 1; j > -1; j--){
            float move_x = static_cast<float>(i) - _selectedSquare->x;
            float move_y = static_cast<float>(j) - _selectedSquare->y; 
            // std::cout << move_x << " " << move_y << std::endl;
            if(0 == move_x && 0 == move_y) continue;
            if(_chessBoard.canMove(static_cast<size_t>(_selectedSquare->x), static_cast<size_t>(_selectedSquare->y), move_x, move_y)){
                _possibleMoves.emplace_back(i, j);
            }
        }
    }
}
void Game3D::gameLoop(){
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 2;
    settings.minorVersion = 0;
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Chess Project - OpenGL 2.0", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window)) return;
    sf::Clock deltaClock;
    float totalTime = 0.0f;
    Chessboard3D chess3D(sf::Vector2f(1.0f, 1.0f));
    int horizontal = _chessBoard.getHorizontal();
    int vertical = _chessBoard.getVertical();
    initOpengl();
    bool isCheckmate = false, isStalemate = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) window.close();
            
            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                handleMouseClick(chess3D, event.mouseButton.x, event.mouseButton.y);
            }
        }
        if (_selectedSquare) {
            if (_selectedSquare->x < 0 ||
                _selectedSquare->y < 0 ||
                _selectedSquare->x >= _chessBoard.getHorizontal() ||
                _selectedSquare->y >= _chessBoard.getVertical() ||
                !_chessBoard.getPiece(_selectedSquare->x,
                                    _selectedSquare->y)) {
                _selectedSquare.reset();
                _possibleMoves.clear();
            }
        }
        sf::Time deltaTime = deltaClock.restart();
        ImGui::SFML::Update(window, deltaTime);
        totalTime += deltaTime.asSeconds();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderShaderBackground(window, totalTime);
        setupCamera();
        bool mirrorX = false; 
        bool mirrorY = true;


        for(int i = horizontal - 1; i > -1; i--){
            for(int j = vertical - 1; j > -1; j--){
                int render_i = mirrorX ? (horizontal - i - 1) : i;
                int render_j = mirrorY ? (vertical - j - 1) : j;
                float draw_x = render_i * chess3D.getSize().x;
                float draw_y = render_j * chess3D.getSize().y;
                sf::Vector2f drawPos(draw_x, draw_y);
                if(i%2 == j%2){
                    chess3D.drawSquareTile(Piece::Team::BLACK, drawPos);
                } else {
                    chess3D.drawSquareTile(Piece::Team::WHITE, drawPos);
                }
                if(0 == render_i) chess3D.drawChessboardWall(drawPos, Chessboard3D::Direction::WEST);
                if((horizontal - 1) == render_i) chess3D.drawChessboardWall(drawPos, Chessboard3D::Direction::EAST);
                if(0 == render_j) chess3D.drawChessboardWall(drawPos, Chessboard3D::Direction::NORTH);
                if((vertical - 1) == render_j) chess3D.drawChessboardWall(drawPos, Chessboard3D::Direction::SOUTH);
                
                if(_chessBoard.getPiece(static_cast<size_t>(i), static_cast<size_t>(j))){
                    std::unique_ptr<Piece> &piece = _chessBoard.getPiece(static_cast<size_t>(i), static_cast<size_t>(j));
                    chess3D.drawPiece(piece->getType(), piece->getTeam(), sf::Vector2i(static_cast<int>(draw_x),
                    static_cast<int>(draw_y)));
                    if(_selectedSquare && i == _selectedSquare->x && j == _selectedSquare->y){
                        chess3D.highlightSelectedPiece(drawPos);
                    }

                }
            }
        }
        glDisable(GL_LIGHTING);
        if(_selectedSquare){
            for(const sf::Vector2i& move : _possibleMoves){
                int render_move_x = mirrorX ? (horizontal - 1 - move.x) : move.x;
                int render_move_y = mirrorY ? (vertical - 1 - move.y) : move.y;
                chess3D.highlightPossibleSquare(sf::Vector2f(render_move_x * chess3D.getSize().x, render_move_y * chess3D.getSize().y));
            }
        }
        if(_selectedSquare && _chessBoard.getPiece(_selectedSquare->x, _selectedSquare->y)){
            for(int i = horizontal - 1; i > -1; i--){
                for(int j = vertical - 1; j > -1; j--){
                    float move_x = i - _selectedSquare->x;
                    float move_y = j - _selectedSquare->y; 
                    // std::cout << move_x << " " << move_y << std::endl;
                    if((_selectedSquare->x + move_x >= horizontal) || (_selectedSquare->x + move_x < 0)){
                        std::cout << "Cannot move out of bonds!\n";
                        continue;
                    }
                    if((_selectedSquare->y + move_y >= vertical) || (_selectedSquare->y + move_y < 0)){
                        std::cout << "Cannot move out of bonds!\n";
                        continue;
                    }
                    if(_chessBoard.canMove(_selectedSquare->x, _selectedSquare->y, move_x, move_y)){
                        int render_i = mirrorX ? (horizontal - 1 - i) : i;
                        int render_j = mirrorY ? (vertical - 1 - j) : j;
                        chess3D.highlightPossibleSquare(sf::Vector2f(render_i * chess3D.getSize().x, render_j * chess3D.getSize().y));
                    }
                }
            }
        }
        glEnable(GL_LIGHTING);


        ImGui::Begin("Debug Menu");
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::SliderFloat("Zoom", &_zoom, 0.5f, 4.0f);

            if(ImGui::Button("Quit")){
                if(quitGame()){
                    return;
                }
            }
            if(ImGui::Button("Surrender")){
                if(surrender()){
                    return;
                }
            }
            if(ImGui::Button("Restart")){
                restart();
            }
        ImGui::End();
        ImGui::Begin("Game Status");
        ImGui::SeparatorText("Selection");
        if (_selectedSquare->x >= 0 && _selectedSquare->x < 8 && _selectedSquare->y >= 0 && _selectedSquare->y < 8) {
            ImGui::Text("Square: (%d, %d)", _selectedSquare->x, _selectedSquare->y);
            std::unique_ptr<Piece>& piece = _chessBoard.getPiece(_selectedSquare->x, _selectedSquare->y);
            if (piece) {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[%s]", piece->getName().c_str());
            } else {
                ImGui::SameLine();
                ImGui::TextDisabled("[Empty]");
            }
        } else {
            ImGui::TextDisabled("No square selected");
        }
        ImGui::Spacing();
        ImGui::SeparatorText("King Status");

        // Helper lambda to draw status text
        auto DrawKingStatus = [&](const char* label, Piece::Team team, std::pair<int, int> kingPos) {
            isCheckmate = _chessBoard.inCheckmate(team);
            bool isCheck = false;
                        
            ImGui::Text("%s:", label);
            ImGui::SameLine();
            if(!isCheckmate){
                isStalemate = _chessBoard.inStalemate(team);
            } else {
                ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "CHECKMATED!");
                return; 
            }
            if(!isStalemate){
                isCheck = _chessBoard.inCheck(team, kingPos);
            } else {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.2f, 1.0f), "IT'S A STALEMATE!");
                return; 
            }
            
            if (isCheck) {
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "IN CHECK!");
                return; 
            } else {
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Safe");
                return;
            }
        };
        std::pair<int, int> wPos = _chessBoard.getWhiteKingsPosition();
        DrawKingStatus("White King", Piece::Team::WHITE, wPos);
        std::pair<int, int> bPos = _chessBoard.getBlackKingsPosition();
        DrawKingStatus("Black King", Piece::Team::BLACK, bPos);
        ImGui::SeparatorText("Turn");
        ImGui::Text("%s: ", "Number");
        ImGui::SameLine();
        ImGui::Text("%i", _turn);
        if(_turn%2 == 1){
            ImGui::Text("%s: %s", "Team", "white");
        } else {
            ImGui::Text("%s: %s", "Team", "black");
        }
        ImGui::End();

        window.pushGLStates();
        ImGui::SFML::Render(window);
        window.popGLStates();

        window.display();
    }

    ImGui::SFML::Shutdown();
}
void Game3D::nextTurn(){
    _turn++;
}
void Game3D::previousTurn(){
    _turn--;
}
int Game3D::getTurn() const {
    return _turn;
}
const ChessBoard& Game3D::getChessBoard() const {
    return _chessBoard;
}

void Game3D::handleMouseClick(const Chessboard3D chess3D, int mouseX, int mouseY){
    setupCamera();
    _raySelection.screenToWorld(mouseX, mouseY);
    if( !_raySelection.intersectBoard()){
        return;
    }
    std::optional<sf::Vector2i> square = _raySelection.worldToBoardSquare(chess3D, 
        _chessBoard.getHorizontal(), _chessBoard.getVertical());

    if(!square){
        std::cout << "No square selected!\n";
        return; 
    }
    std::unique_ptr<Piece>& targetPiece = _chessBoard.getPiece(square->x, square->y);
    std::unique_ptr<Piece>& piece = _chessBoard.getPiece(_selectedSquare->x, _selectedSquare->y);

    bool targetIsFriendly = targetPiece && ((targetPiece->getTeam() == Piece::Team::WHITE && getTurn() % 2 == 1) || (targetPiece->getTeam() == Piece::Team::BLACK && getTurn() % 2 == 0));
    //Player should be able to choose if they want to castle with the ROOK
    bool choiceToCastle = true;
    if (piece && piece->getType() == Piece::KING && targetIsFriendly && targetPiece->getType() == Piece::ROOK && choiceToCastle) {
        sf::Vector2i moveVector = sf::Vector2i(square->x - _selectedSquare->x, square->y - _selectedSquare->y);
        std::cout << "Castling with Rook on square (" << square->x << ", " << square->y << ")!\n"; 
        if(_chessBoard.canMove(_selectedSquare->x, _selectedSquare->y, moveVector.x, moveVector.y)){
            std::cout << "Piece can move!\n";
            enPassantTurnCycle();
            std::cout << "Turn cycle\n";
            _chessBoard.move(_selectedSquare->x, _selectedSquare->y, moveVector.x, moveVector.y);
            std::cout << "Move complete\n";
            nextTurn();
            _selectedSquare.reset();
            _possibleMoves.clear();
        } else {
            std::cout << "Piece can't move to this square!\n";
            _selectedSquare.reset();
            _possibleMoves.clear();
        }
        return;
    }
    else if(!_selectedSquare || targetIsFriendly){
        if(targetIsFriendly){
            std::cout << "Square with piece (" << square->x << ", " << square->y << ") selected!\n"; 
            _selectedSquare = square;
            updatePossibleMoves();
            // std::cout << "Possible moves updated!" << std::endl;
        }
        return;
    }
    else {
        sf::Vector2i moveVector = sf::Vector2i(square->x - _selectedSquare->x, square->y - _selectedSquare->y);
        std::cout << "Square (" << square->x << ", " << square->y << ") selected!\n"; 
        if(piece){

            if(_chessBoard.canMove(_selectedSquare->x, _selectedSquare->y, moveVector.x, moveVector.y)){
                std::cout << "Piece can move!\n";
                enPassantTurnCycle();
                _chessBoard.move(_selectedSquare->x, _selectedSquare->y, moveVector.x, moveVector.y);
                nextTurn();
                _selectedSquare.reset();
                _possibleMoves.clear();
            } else {
                std::cout << "Piece can't move to this square!\n";
                _selectedSquare.reset();
                _possibleMoves.clear();
            }
        }
        return;
    }
}

void Game3D::renderShaderBackground(sf::RenderWindow& window, float time) const
{
    static sf::Shader shader;
    static sf::VertexArray quad;
    static bool isInitialized = false;

    if (!isInitialized) {
        const std::string vertexShader = R"(
            #version 120
            attribute vec2 aPos;
            varying vec2 vUV;
            void main() {
                vUV = aPos * 0.5 + 0.5;
                gl_Position = vec4(aPos, 0.0, 1.0);
            }
        )";

        const std::string fragmentShader = R"(
          #version 120
          // Inputs from vertex shader and host
          varying vec2 vUV;
          uniform vec2 u_resolution;
          uniform float u_time;

          void main() {
              vec2 uv = vUV - 0.5;
              uv.y *= u_resolution.y / u_resolution.x;

              vec3 finalColor = vec3(0.0);
              
              const float numberOfRibbons = 4.0;
              for(float i = 1.0; i <= numberOfRibbons; i++) {
                  float t_offset = u_time * 0.5 + i * 2.3;

                  vec2 point = vec2(
                      sin(t_offset * 1.1) * 0.4,
                      cos(t_offset * 1.4) * 0.3
                  );

                  // Calculate distance from the current pixel to this moving point
                  float dist = length(uv - point);

                  // --- The Core "Neon" Effect ---
                  // We want high brightness when distance is near zero.
                  // 1.0 / dist gives a glow, but it's too wide.
                  // pow(..., 2.0) sharpens the glow into a thin line.
                  float intensity = 0.001 / pow(abs(dist - 0.02), 1.8);
                  
                  // Smooth out the extreme bright spots
                  intensity = clamp(intensity, 0.0, 1.0);

                  // Give each ribbon a slightly different color palette
                  // Varying colors over time:
                  float r = sin(u_time * 0.5 + i) * 0.5 + 0.5;
                  float g = sin(u_time * 0.7 + i) * 0.5 + 0.5;
                  // Keep blue high for that "neon" look
                  vec3 ribbonColor = vec3(r * 0.4, g * 0.6, 1.0); 

                  // Add this ribbon's contribution to the final pixel color
                  finalColor += ribbonColor * intensity;
              }
              // Final color with a dark background tone
              gl_FragColor = vec4(finalColor * 0.6, 1.0);
          }
        )";

        if (!shader.loadFromMemory(vertexShader, fragmentShader)) {
            std::cerr << "Failed to load shader\n";
            return;
        }
        quad.setPrimitiveType(sf::Triangles);
        quad.resize(6);

        quad[0].position = sf::Vector2f(-1.f, -1.f);
        quad[1].position = sf::Vector2f( 1.f, -1.f);
        quad[2].position = sf::Vector2f( 1.f,  1.f);
        quad[3].position = sf::Vector2f(-1.f, -1.f);
        quad[4].position = sf::Vector2f( 1.f,  1.f);
        quad[5].position = sf::Vector2f(-1.f,  1.f);

        isInitialized = true;
    }

    sf::Vector2f windowResolution(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    shader.setUniform("u_resolution", windowResolution);
    shader.setUniform("u_time", time);
    
    window.pushGLStates();
    sf::RenderStates states;
    states.shader = &shader;
    window.draw(quad, states);
    window.popGLStates();
}