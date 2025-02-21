#include "barriers.h"
#include <thread>
#include <utility>
#include <iostream>
#include <algorithm>

Barrier::Barrier(int posx, int posy): _posx(posx), _posy(posy){}

bool Barrier::CheckCollision(int x, int y){
    //std::cout << "(" << x << "," << y << ") checking collision at" <<  "(" << _posx << "," << _posy << ")" <<std::endl;
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
        std::cout << "Placing Barrier #" << numPlaced+1 <<std::endl;
        barrier_ps.push_back(new Barrier(x,y));
        numPlaced++;
    }  
}

bool BarrierManager::CheckCollisions(int x, int y, bool parallel) const{
    //std::cout << "Checking Collisions!" << std::endl;
    //std::lock_guard lock(mtx);
    bool collision = false;
    if (!parallel){
        for (auto barPtr: barrier_ps){
            collision = barPtr->CheckCollision(x,y);
            if (collision) {
                //std::cout << "\n(" << x << "," << y << ") : Collision!!!\n";
                //prms.set_value(true);
                
                return true;
            }
            //else std::cout << "\n(" << x << "," << y << ")";
        }
        return false;
    }
    /*
    else{
        auto vecPtr = &barrier_ps;
        auto num = _numBarriers;
        std::promise<int> prms_x;
        std::future<int> ftr_x;
        ftr_x = prms_x.get_future();
        std::promise<int> prms_y;
        std::future<int> ftr_y;
        ftr_y = prms_y.get_future();
        
        auto findXOverlap = [vecPtr, num](std::promise<int> prms, int x){
        //std::thread([vecPtr, num, x](){
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

        auto findYOverlap = [vecPtr, num](std::promise<int> prms, int y){
            //std::thread([vecPtr, num, x](){
                int i{0};
                for (; i<num; ++i){
                    if (vecPtr->at(i)->CheckY(y)){
                        prms.set_value(i);
                        return;
                    }
                }
                prms.set_value(-1);
                return;
            };

        std::thread tx(findXOverlap, std::move(prms_x), x);
        std::thread ty(findYOverlap, std::move(prms_y), y);

        int xCollideIndex = ftr_x.get();
        int yCollideIndex = ftr_y.get();

        if (xCollideIndex == yCollideIndex) return true;
        tx.join();
        ty.join();
        return false;
    }
    */
   return false;
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