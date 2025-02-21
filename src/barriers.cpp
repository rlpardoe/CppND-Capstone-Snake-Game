#include "barriers.h"
#include <thread>
#include <utility>
#include <iostream>
#include <algorithm>

Barrier::Barrier(int posx, int posy): _posx(posx), _posy(posy){}

bool Barrier::CheckCollision(int x, int y) const{
    return ((x == _posx ) && (y == _posy));
}

SDL_Point Barrier::getSDL() const{
    SDL_Point point{_posx, _posy};
    return point;
}

BarrierManager::BarrierManager(const unsigned int numBarriers, std::size_t grid_width, std::size_t grid_height): 
                engine(dev()), random_w(0, static_cast<int>(grid_width-1)), random_h(0, static_cast<int>(grid_width-1)),
                _numBarriers(numBarriers){
    // use rand dist to generate random positions;
    int numPlaced{0};
    int x, y;
    while (numPlaced < numBarriers){
        x = random_w(engine);
        y = random_h(engine);
        // Dont place multipl barriers at the same location;
        if (CheckCollisions(x,y, false)) continue;
        std::cout << "Placing Barrier #" << numPlaced+1 <<std::endl;
        barrier_ps.push_back(new Barrier(x,y));
        numPlaced++;
    }  
}

bool BarrierManager::CheckCollisions(int x, int y, bool parallel) const{
    bool collision = false;
    for (auto barPtr: barrier_ps){
        collision = barPtr->CheckCollision(x,y);
        if (collision) return true;
    }
    return false;
    
}

BarrierManager::~BarrierManager(){
    for (auto barPtr : barrier_ps){
        delete barPtr;
    }
}

std::vector<SDL_Point> BarrierManager::getGraphicsPoints() const{
    std::vector<SDL_Point> points;
    points.reserve(_numBarriers);
    for (auto p : barrier_ps){
        points.push_back(p->getSDL());
    }
    return points;
}