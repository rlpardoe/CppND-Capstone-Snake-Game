#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include <mutex>

class Controller {
 public:
  void HandleInput(bool &running, bool &paused, std::mutex& p_mtx, Snake &snake) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
  void TogglePause(bool &paused) const;
};

#endif