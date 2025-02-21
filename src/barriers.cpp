#include "barriers.h"
#include <thread>
#include <future>
#include <utility>
#include <iostream>
#include <algorithm>

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
SDL_Point Barrier::getSDL(){
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
        // check if same as food, if it 'continue and regenerate'
        std::cout << "Placing Barrier #" << numPlaced+1 <<std::endl;
        barrier_ps.push_back(new Barrier(x,y));
        numPlaced++;
    }  
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
        std::promise<int> prms_x;
        std::future<int> ftr_x;
        ftr_x = prms_x.get_future();
        std::promise<int> prms_y;
        std::future<int> ftr_y;
        ftr_y = prms_y.get_future();
        
        auto findOverlap = [vecPtr, num](std::promise<int> prms, int x){
        /*std::thread([vecPtr, num, x](){*/
            int i{0};
            for (; i<num; ++i){
                if (vecPtr->at(i)->CheckX(x)){
                    prms.set_value(i);
                    return;
                }
            }
            prms.set_value(-1);
            return;
        };
        std::thread tx(findOverlap, std::move(prms_x), x);
        std::thread ty(findOverlap, std::move(prms_y), y);

        int xCollideIndex = ftr_x.get();
        int yCollideIndex = ftr_y.get();

        if (xCollideIndex == yCollideIndex) return true;
        tx.join();
        ty.join();
        return false;
    }
}

BarrierManager::~BarrierManager(){
    for (auto barPtr : barrier_ps){
        delete barPtr;
    }
}

std::vector<SDL_Point> BarrierManager::getGraphicsPoints(){
    std::vector<SDL_Point> points;
    points.reserve(_numBarriers);
    for (auto p : barrier_ps){
        points.push_back(p->getSDL());
    }
    return points;
}