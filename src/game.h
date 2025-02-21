#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "barriers.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height,  std::shared_ptr<BarrierManager> barrierManager);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;

  std::shared_ptr<BarrierManager> barrierManager;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  bool paused = true;
  void PlaceFood();
  void Update();
};

#endif