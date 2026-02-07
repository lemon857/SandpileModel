#include "SandpileApplication/SandpileApp.h"
#include "SandpileApplication/BitmapRGB.h"

#include "SandpileModel/sandpile.h"

#include <EngineCore/Engine/Application.h>
#include <EngineCore/Engine/Window.h>
#include <EngineCore/Renderer/Renderer.h>
#include <EngineCore/Renderer/Camera.h>
#include <EngineCore/Renderer/Material.h>
#include <EngineCore/Resources/ResourceManager.h>
#include <EngineCore/System/ShadersSettings.h>
#include <EngineCore/System/Input.h>
#include <EngineCore/GUI/GUI_place.h>
#include <EngineCore/GUI/TextRenderer.h>
#include <EngineCore/GUI/ScrollBox.h>
#include <EngineCore/GUI/Button.h>
#include <EngineCore/GUI/Sprite.h>
#include <EngineCore/GUI/InputField.h>
#include <EngineCore/GUI/Font.h>
#include <EngineCore/Sound/SoundEngine.h>

bool SandpileApp::init() {
  is_key_pressed = false;
  is_simulation_running = false;

  current_frame_time = 0;
  move_multiplier = 0;
  update_texture_time = 5;
  coef_sand_place_size = 0.1;

  current_count_sand = 128;

  camera = new Camera();
  camera->set_viewport_size(static_cast<float>(m_pWindow->get_size().x), static_cast<float>(m_pWindow->get_size().y));

  gui_place = new GUI::GUI_place(camera, ResourceManager::getMaterial("default"));

  gui_place->add_element<GUI::Sprite>(-3, ResourceManager::getMaterial("emptySprite"), "default",
    glm::vec2(50.f), glm::vec2(50.f), "picture", true)
    ->set_mouse_down_callback([&]() {
    GUI::Sprite* cur_sprite = gui_place->get_element<GUI::Sprite>("picture");

    glm::vec2 mouse_pos = cur_sprite->get_mouse_click_pos();

    glm::vec2 position = cur_sprite->get_position();
    glm::vec2 scale = cur_sprite->get_scale_p();
    glm::vec2 scale_px = cur_sprite->get_scale();

    glm::vec4 res_shift = glm::vec4((scale_px - position), 0, 0);
    
    glm::mat4 scaleMat(
    50.f / scale.x, 0, 0, 0,
    0, 50.f / scale.y, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1);

    glm::vec4 res = glm::vec4((mouse_pos.x), (mouse_pos.y), 0, 0);

    res = res + res_shift;
    res = res * scaleMat;

    res.x = (int)std::floor(res.x * coef_sand_place_size);
    res.y = (int)std::floor(res.y * coef_sand_place_size);

    LOG_INFO("[click-button-event] Res X: {0}\tRes Y: {1}", res.x, res.y);

    if (res.x >= 0 && res.y >= 0) {
      mat(res.x, res.y) += current_count_sand;
    }
  });

  gui_place->set_active(true);

  set_max_tps(50);

  width = (std::size_t)std::floor(m_window_size.x * coef_sand_place_size);
  height = (std::size_t)std::floor(m_window_size.y * coef_sand_place_size);

  return true;
}

void SandpileApp::init_events() {
m_event_dispather.add_event_listener<EventWindowResize>(
  [&](EventWindowResize& e) {
    camera->set_viewport_size(e.width, e.height);
    width = (std::size_t)std::floor(e.width * coef_sand_place_size);
    height = (std::size_t)std::floor(e.height * coef_sand_place_size);

    if (gui_place) {
      gui_place->on_resize();
    }
  });
m_event_dispather.add_event_listener<EventMouseButtonPressed>([&](EventMouseButtonPressed& e)
  {
    if (gui_place)
    {
      gui_place->on_key_press((KeyCode)e.mouse_button);
      gui_place->on_mouse_press(e.x_pos, e.y_pos);
    }
  });
m_event_dispather.add_event_listener<EventCharSet>([&](EventCharSet& e)
  {
    if (gui_place) gui_place->on_char_set(e.key_char);
  });
m_event_dispather.add_event_listener<EventMouseButtonReleased>([&](EventMouseButtonReleased& e)
  {
    if (gui_place) gui_place->on_mouse_release(e.x_pos, e.y_pos);
  });
m_event_dispather.add_event_listener<EventKeyPressed>([&](EventKeyPressed& e)
  {
    if (gui_place) gui_place->on_key_press(e.key_code);
  });
m_event_dispather.add_event_listener<EventKeyReleased>([&](EventKeyReleased& e)
  {
    is_key_pressed = false;
  });
m_event_dispather.add_event_listener<EventMouseScrolled>([&](EventMouseScrolled& e)
  {
    if (gui_place)
    {
      gui_place->on_mouse_scroll(-e.y_offset);
    }
  });
}


void SandpileApp::terminate() {
  delete gui_place;
  delete camera;
}

void SandpileApp::on_render() {
  RenderEngine::Renderer::setClearColor(kBackgroundColor, kBackgroundColor, kBackgroundColor, 1.f);
  RenderEngine::Renderer::clear();
}

void SandpileApp::on_ui_render() {
  gui_place->on_render();
}

void SandpileApp::on_update(const double delta) {
  if (is_simulation_running) {
    SandpileTick(mat);
  }

  gui_place->on_update(delta);

  if (current_frame_time >= update_texture_time) {
  current_frame_time = 0;

  BitmapRGB bitmap{width, height, {0xAA, 0xAA, 0xAA}};

  index_t left = mat.MaxLeftLine();
  index_t right = mat.MaxRightLine();
  index_t top = mat.CountTop();
  index_t bottom = mat.CountBottom();

    for (int i = -bottom; i < top; ++i) {
      for (int j = -left; j < right; ++j) {
        if (j >= -mat[i].CountLeft() && j <= mat[i].CountRight() && i + bottom < height && j + left < width && i + bottom > 0 && j + left > 0) {
          if (mat(j, i).bv == 0) {
            bitmap.set_color(j + left, i + bottom, {0xAA, 0xAA, 0xAA});

          } else if (mat(j, i).bv == 1) {
            bitmap.set_color(j + left, i + bottom, {0x99, 0x0D, 0x55});

          } else if (mat(j, i).bv == 2) {
            bitmap.set_color(j + left, i + bottom, {0xAA, 0x04, 0x77});

          } else if (mat(j, i).bv == 3) {
            bitmap.set_color(j + left, i + bottom, {0xFF, 0x00, 0xFF});

          } else if (mat(j, i).bv >= 4) {
            bitmap.set_color(j + left, i + bottom, {0xFF, 0xFF, 0x00});
          }
        }
      }
  }
  
  gui_place->get_element<GUI::Sprite>("picture")->get_material()->get_texture_ptr()->updateData(width, height, bitmap.get_data(), 3);

  } else {
    current_frame_time += delta;
  }
}

void SandpileApp::on_key_update(const double delta) {
  if (is_key_pressed) return;

  if (Input::isKeyPressed(KeyCode::KEY_SPACE)) {
    is_simulation_running = !is_simulation_running;
    is_key_pressed = true;
  }
}
