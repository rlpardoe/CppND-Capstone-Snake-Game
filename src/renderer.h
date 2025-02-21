#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include <memory>
#include "barriers.h"
#include <mutex>

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height,
           std::shared_ptr<BarrierManager> barrierManager);
  Renderer(const std::size_t screen_dim,
            const std::size_t gridDim,
            std::shared_ptr<BarrierManager> barrierManager);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food, bool &is_bonus_food_active, std::mutex& bonus_mtx);
  void UpdateWindowTitle(int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  std::shared_ptr<BarrierManager> barrierManager;
  std::vector<SDL_Point> barriers;
  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif