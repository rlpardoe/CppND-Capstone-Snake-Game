#include "game.h"
#include <iostream>
#include "SDL.h"
#include <thread>
#include <future>

Game::Game(std::size_t grid_width, std::size_t grid_height, std::shared_ptr<BarrierManager> barrierManager)
    : snake(grid_width, grid_height),
      barrierManager(barrierManager),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, paused, snake);
    Update();

    std::promise<bool> prms;
    std::future<bool> ftr;
    ftr = prms.get_future();

    //Start thread to check if snake has hit a barrier while rendering and waiting for frame time
    auto f = [](std::shared_ptr<BarrierManager> barMptr, int x, int y, std::promise<bool> prms){
      prms.set_value(barMptr->CheckCollisions(x,y,false));
    };
    std::thread t1(f, barrierManager, static_cast<int>(snake.head_x), static_cast<int>(snake.head_y), std::move(prms));

    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }

    //get info back from thread on wether snake hit barrier and is dead before next frame
    bool collision = ftr.get();
    snake.alive = snake.alive && !collision;
    t1.join();
  }
  
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food. Also Check that it is not occupied by a barrier
    if (!snake.SnakeCell(x, y) && !barrierManager->CheckCollisions(x, y, false)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {

  if (!snake.alive || paused) return;

  snake.Update();
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
 


  //bool hitBarrier = barrierManager->CheckCollisions(new_x, new_y, false);
  //snake.alive = snake.alive && !hitBarrier;

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.01;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }