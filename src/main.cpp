#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "utils.h"
#include "barriers.h"
#include <memory>

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  /*constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};*/

  auto [kScreenDim, kGridDim, kNumBarr] = parseWindowInfo("../runtimeConfig.txt");
  auto barrierManagerP = std::make_shared<BarrierManager>(kNumBarr, kGridDim, kGridDim);
  Renderer renderer(kScreenDim, kGridDim, barrierManagerP);
  Controller controller;
  Game game(kGridDim, kGridDim, barrierManagerP);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}