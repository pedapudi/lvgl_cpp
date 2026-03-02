#include "gltf.h"

#if LV_USE_GLTF

#include <optional>

namespace lvgl {

Gltf::Gltf() : Gltf(static_cast<Object*>(nullptr), Ownership::Managed) {}

Gltf::Gltf(Object* parent, Ownership ownership)
    : Widget(lv_gltf_create(parent ? parent->raw() : nullptr), ownership) {}

Gltf::Gltf(Object& parent) : Gltf(&parent) {}

Gltf::Gltf(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

lv_gltf_model_t* Gltf::load_model(const char* path) {
  return raw() ? lv_gltf_load_model_from_file(raw(), path) : nullptr;
}

lv_gltf_model_t* Gltf::load_model(const uint8_t* bytes, size_t len) {
  return raw() ? lv_gltf_load_model_from_bytes(raw(), bytes, len) : nullptr;
}

Gltf& Gltf::recenter(lv_gltf_model_t* model) {
  if (raw()) lv_gltf_recenter(raw(), model);
  return *this;
}

Gltf& Gltf::set_yaw(float yaw) {
  if (raw()) lv_gltf_set_yaw(raw(), yaw);
  return *this;
}

float Gltf::get_yaw() const { return raw() ? lv_gltf_get_yaw(raw()) : 0.0f; }

Gltf& Gltf::set_pitch(float pitch) {
  if (raw()) lv_gltf_set_pitch(raw(), pitch);
  return *this;
}

float Gltf::get_pitch() const {
  return raw() ? lv_gltf_get_pitch(raw()) : 0.0f;
}

Gltf& Gltf::set_distance(float distance) {
  if (raw()) lv_gltf_set_distance(raw(), distance);
  return *this;
}

float Gltf::get_distance() const {
  return raw() ? lv_gltf_get_distance(raw()) : 0.0f;
}

Gltf& Gltf::set_fov(float fov) {
  if (raw()) lv_gltf_set_fov(raw(), fov);
  return *this;
}

float Gltf::get_fov() const { return raw() ? lv_gltf_get_fov(raw()) : 0.0f; }

Gltf& Gltf::set_focal_point(const Point3D& p) {
  if (raw()) {
    lv_gltf_set_focal_x(raw(), p.x);
    lv_gltf_set_focal_y(raw(), p.y);
    lv_gltf_set_focal_z(raw(), p.z);
  }
  return *this;
}

Point3D Gltf::get_focal_point() const {
  if (!raw()) return {0, 0, 0};
  return Point3D(lv_gltf_get_focal_x(raw()), lv_gltf_get_focal_y(raw()),
                 lv_gltf_get_focal_z(raw()));
}

Gltf& Gltf::set_animation_speed(uint32_t speed) {
  if (raw()) lv_gltf_set_animation_speed(raw(), speed);
  return *this;
}

uint32_t Gltf::get_animation_speed() const {
  return raw() ? lv_gltf_get_animation_speed(raw()) : 0;
}

Gltf& Gltf::set_background_mode(BackgroundMode mode) {
  if (raw())
    lv_gltf_set_background_mode(raw(), static_cast<lv_gltf_bg_mode_t>(mode));
  return *this;
}

Gltf::BackgroundMode Gltf::get_background_mode() const {
  return raw() ? static_cast<BackgroundMode>(lv_gltf_get_background_mode(raw()))
               : BackgroundMode::Solid;
}

Gltf& Gltf::set_background_blur(uint32_t blur) {
  if (raw()) lv_gltf_set_background_blur(raw(), blur);
  return *this;
}

uint32_t Gltf::get_background_blur() const {
  return raw() ? lv_gltf_get_background_blur(raw()) : 0;
}

Gltf& Gltf::set_env_brightness(uint32_t brightness) {
  if (raw()) lv_gltf_set_env_brightness(raw(), brightness);
  return *this;
}

uint32_t Gltf::get_env_brightness() const {
  return raw() ? lv_gltf_get_env_brightness(raw()) : 0;
}

Gltf& Gltf::set_exposure(float exposure) {
  if (raw()) lv_gltf_set_image_exposure(raw(), exposure);
  return *this;
}

float Gltf::get_exposure() const {
  return raw() ? lv_gltf_get_image_exposure(raw()) : 1.0f;
}

Gltf& Gltf::set_antialiasing(AntiAliasing mode) {
  if (raw())
    lv_gltf_set_antialiasing_mode(raw(), static_cast<lv_gltf_aa_mode_t>(mode));
  return *this;
}

Gltf::AntiAliasing Gltf::get_antialiasing() const {
  return raw() ? static_cast<AntiAliasing>(lv_gltf_get_antialiasing_mode(raw()))
               : AntiAliasing::Off;
}

Ray3D Gltf::get_ray(const Point& screen_pos) const {
  if (!raw()) return Ray3D({0, 0, 0}, {0, 0, 0});
  lv_point_t p = screen_pos;
  return Ray3D(lv_gltf_get_ray_from_2d_coordinate(raw(), &p));
}

std::optional<Point> Gltf::world_to_screen(const Point3D& world_pos) const {
  if (!raw()) return std::nullopt;
  lv_point_t p;
  if (lv_gltf_world_to_screen(raw(), world_pos, &p) == LV_RESULT_OK) {
    return Point(p);
  }
  return std::nullopt;
}

Plane3D Gltf::get_view_plane(float distance) const {
  if (!raw()) return Plane3D({0, 0, 0}, {0, 1, 0});
  return Plane3D(lv_gltf_get_current_view_plane(raw(), distance));
}

}  // namespace lvgl

#endif  // LV_USE_GLTF
