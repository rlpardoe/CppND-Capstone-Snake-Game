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
    controller.HandleInput(running, paused, pause_mtx, snake);
    Update();

    std::promise<bool> prms;
    std::future<bool> ftr;
    ftr = prms.get_future();

    //Start thread to check if snake has hit a barrier while rendering and waiting for frame time
    auto f = [](std::shared_ptr<BarrierManager> barMptr, int x, int y, std::promise<bool> prms){
      prms.set_value(barMptr->CheckCollisions(x,y));
    };
    std::thread t1(f, barrierManager, static_cast<int>(snake.head_x), static_cast<int>(snake.head_y), std::move(prms));

    renderer.Render(snake, food, is_bonus_food_active, mutex);

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
    if (!snake.SnakeCell(x, y) && !barrierManager->CheckCollisions(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  std::unique_lock<std::mutex> plock(pause_mtx);
  if (!snake.alive || paused) return;
  plock.unlock();
  snake.Update();
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
 
  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;

    // Handle Bonus food
    std::unique_lock<std::mutex> lock(mutex);
    if (!is_bonus_food_active){ // food timer expired (in which case thread already exited)
      snake.speed += 0.01; //increase speed of snake

      //restart bonus food and its timer
      is_bonus_food_active = true; 
      lock.unlock(); // if old thread expired start new
      bonusFoodThread = std::thread(&Game::BonusFoodTimer, this);
      bonusFoodThread.detach();
    }
    else{ // if thread hadn't expired
      //no speed increase !!

      //Close old thread
      is_bonus_food_active = false;
      conditon_var.notify_one(); 
      lock.unlock();
      //sleep to allow thread to check condition and close
      std::this_thread::sleep_for(std::chrono::milliseconds(2));

      //Start new thread and detach
      lock.lock();
      is_bonus_food_active = true;
      lock.unlock();
      bonusFoodThread = std::thread(&Game::BonusFoodTimer, this);
      bonusFoodThread.detach();
    }

    //place new food (will start as bonus)
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::BonusFoodTimer()
{
  long bonusMilliSeconds = 15000;
  //std::chrono::milliseconds bonus_ms(15000);
  auto startTime = std::chrono::high_resolution_clock::now();
  std::unique_lock<std::mutex> lock(mutex);
  while (is_bonus_food_active)
  {
    lock.unlock();
    
    auto current_Time = std::chrono::high_resolution_clock::now();
    auto elapsed_Seconds = std::chrono::duration_cast<std::chrono::milliseconds>(current_Time - startTime).count();
    //if (paused) bonusSeconds = std::chrono::high_resolution_clock::now();
    if (elapsed_Seconds >= bonusMilliSeconds)
    {
      // Bonus food time is up
      lock.lock(); // will unlock at break
      is_bonus_food_active = false;
      break;
    }
    lock.lock();
    // Wait for a short interval or until the condition_variable is notified
    conditon_var.wait_for(lock, std::chrono::milliseconds(500));
    //std::cout << "bonus is currently : " << is_bonus_food_active << std::endl;

    // if time is paused increase real time where bonus food is active equally
    std::lock_guard<std::mutex> plock(pause_mtx); // falls out of scoper at end of loop
    if (paused) bonusMilliSeconds += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - current_Time).count();
  }
}
