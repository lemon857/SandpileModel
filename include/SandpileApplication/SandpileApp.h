#pragma once

#include "SandpileModel/utils.h"

#include <EngineCore/Engine/Application.h>
#include <EngineCore/GUI/GUI_place.h>

class SandpileApp : public Application {
private:
  const float kBackgroundColor = 0x77 / 255.f;

  GUI::GUI_place* gui_place;
  Camera* camera;

  udmatrix64_t mat;

  double coef_sand_place_size;
  double current_frame_time;
  double move_multiplier;
  double update_texture_time;

  int current_count_sand;

  size_t width;
  size_t height;

  bool is_simulation_running;
  bool is_key_pressed;

public:
  bool init() override;
  void init_events() override;

  void terminate() override;

  void on_render() override;
  void on_ui_render() override;

  void on_update(const double delta) override;
  void on_key_update(const double delta) override;
};
