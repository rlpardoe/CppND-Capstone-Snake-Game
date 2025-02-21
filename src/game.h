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
  std::mutex pause_mtx;
  void PlaceFood();
  void Update();

  void BonusFoodTimer();
  //void PlaceBonusFood();

  //bool already_appeared;
  bool is_bonus_food_active;
  //SDL_Point bonus_add_food;
  std::thread bonusFoodThread;
  std::mutex mutex;
  std::condition_variable conditon_var;
};

#endif