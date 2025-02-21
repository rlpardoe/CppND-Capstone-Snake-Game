#include "barriers.h"
#include <thread>
Barrier::Barrier(int posx, int posy): _posx(posx), _posy(posy){}

bool Barrier::CheckCollision(int x, int y){
    return ((x == _posx ) && (y == _posy));
}
bool Barrier::CheckX(int x){
    return (x==_posx);
}
bool Barrier::CheckY(int y){
    return (y==_posy);
}

BarrierManager::BarrierManager(const int numBarriers, std::size_t grid_width, std::size_t grid_height): 
                engine(dev()), random_w(0, static_cast<int>(grid_width-1)), random_h(0, static_cast<int>(grid_width-1)){
    // use rand dist to generate random positions;
    int numPlaced{};
    int x, y;
    while (numPlaced <= numBarriers){
        x = random_w(engine);
        y = random_h(engine);
        // check if same as food, if it 'continue and regenerate'
        barrier_ps.push_back(new Barrier(x,y));
    }  
}

bool BarrierManager::CheckCollisions(int x, int y){
    
}

bool BarrierManager::CheckCollisions(int x, int y, bool parallel){
    bool collision = false;
    if (!parallel){
        for (auto barPtr: barrier_ps){
            if (collision = barPtr->CheckCollision(x,y)) break;
        }
        return collision;
    }
    else{
        auto vecPtr = &barrier_ps;
        auto num = _numBarriers;
        std::thread
        auto fx = [vecPtr, num, x](){
            for (int i{0}; i<num; ++i){
                if (vecPtr->at(i)->CheckX(x)) return i;
            }
            return -1;
        }

    }
}

BarrierManager::~BarrierManager(){
    for (auto barPtr : barrier_ps){
        delete barPtr;
    }
}
