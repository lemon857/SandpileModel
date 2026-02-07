#include "SandpileApplication/SandpileApp.h"

#include <EngineCore/Resources/ResourceManager.h>

int main(int argc, char** argv) {

  glm::ivec2 windowSize(800, 600);

  ResourceManager::setExecutablePath(argv[0]);

  SandpileApp* app = new SandpileApp();

  app->start(windowSize, "SandpileModel", "systemres/resources.json", "SandpileModel.ini");

  app->stop();

  delete app;

  return 0;
}

/*
HOTKEYS
Q - increase update picture
A - decrease
R - reset sand
E - reset position
space - start/stop simulation
left ctrl - press for add more speed to actions (move and etc.)
arrows for move
+ and - at numpad for scale
*/
