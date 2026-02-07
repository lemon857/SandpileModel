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
