#include <vector>
class Barrier{\
public:
    Barrier(int posx, int posy);
    bool CheckCollision(int x, int y);
private:
    const int _posx;
    const int _posy;
};

class BarrierManager{
    public:
    BarrierManager(int numBarriers);
    ~BarrierManager();
    bool CheckCollisions(int x, int y);
    private:
        std::vector<Barrier*> barrier_ps;
        unsigned int _numBarriers; 


};