#ifndef BARRIERS_H
#define BARRIERS_H
#include <vector>
#include <random>
#include "SDL.h"
#include <future>
#include <mutex>

class Barrier{
public:
    Barrier(int posx, int posy);
    bool CheckCollision(int x, int y) const;
    SDL_Point getSDL() const;
private:
    const int _posx;
    const int _posy;
};

class BarrierManager{
public:
    BarrierManager(const unsigned int numBarriers, std::size_t grid_width, std::size_t grid_height);
    ~BarrierManager();
    int number() const { return _numBarriers;}
    bool CheckCollisions(int x, int y, bool parallel) const;
    std::vector<SDL_Point> getGraphicsPoints() const;
private:
    std::vector<Barrier*> barrier_ps;
    const unsigned int _numBarriers; 
    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;
};

#endif