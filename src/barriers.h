#include <vector>
#include <random>

class Barrier{
public:
    Barrier(int posx, int posy);
    bool CheckCollision(int x, int y);
    bool CheckX(int x);
    bool CheckY(int y);
private:
    const int _posx;
    const int _posy;
};

class BarrierManager{
    public:
    BarrierManager(const int numBarriers, std::size_t grid_width, std::size_t grid_height);
    ~BarrierManager();
    bool CheckCollisions(int x, int y);
    bool CheckCollisions(int x, int y, bool parallel);
    private:
        std::vector<Barrier*> barrier_ps;
        unsigned int _numBarriers; 
        std::random_device dev;
        std::mt19937 engine;
        std::uniform_int_distribution<int> random_w;
        std::uniform_int_distribution<int> random_h;
};