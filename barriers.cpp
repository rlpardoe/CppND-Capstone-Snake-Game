#include "barriers.h"
Barrier::Barrier(int posx, int posy): _posx(posx), _posy(posy){}

bool Barrier::CheckCollision(int x, int y){
    return ((x == _posx ) && (y == _posy));
}

BarrierManager::BarrierManager(int numBarriers){
    // use rand dist to generate random positions;
    barrier_ps.push_back(new Barrier(1,2));
}