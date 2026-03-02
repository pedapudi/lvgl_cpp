#ifndef LVGL_CPP_WIDGETS_GLTF_H_
#define LVGL_CPP_WIDGETS_GLTF_H_

#include "../core/traits.h"
#include "../core/widget.h"
#include "../misc/math_3d.h"
#include "lvgl.h"

#if LV_USE_GLTF

namespace lvgl {

class Gltf;

template <>
struct class_traits<Gltf> {
  static const lv_obj_class_t* get() { return &lv_gltf_class; }
};

/**
 * @brief Wrapper for lv_gltf.
 *
 * Gltf allows viewing and interacting with 3D models in glTF format.
 */
class Gltf : public Widget<Gltf> {
 public:
  enum class AntiAliasing : uint8_t {
    Off = LV_GLTF_AA_MODE_OFF,
    On = LV_GLTF_AA_MODE_ON,
    Dynamic = LV_GLTF_AA_MODE_DYNAMIC,
  };

  enum class BackgroundMode : uint8_t {
    Solid = LV_GLTF_BG_MODE_SOLID,
    Environment = LV_GLTF_BG_MODE_ENVIRONMENT,
  };

  Gltf();
  explicit Gltf(Object* parent, Ownership ownership = Ownership::Default);
  explicit Gltf(Object& parent);
  explicit Gltf(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Load a glTF model from a file.
   * @param path File path to the .gltf or .glb file.
   * @return Pointer to the raw lv_gltf_model_t.
   */
  lv_gltf_model_t* load_model(const char* path);

  /**
   * @brief Load a glTF model from memory.
   * @param bytes Pointer to the raw data.
   * @param len Length of the data in bytes.
   * @return Pointer to the raw lv_gltf_model_t.
   */
  lv_gltf_model_t* load_model(const uint8_t* bytes, size_t len);

  /**
   * @brief Recenter the camera on the model.
   * @param model Model to recenter on (nullptr for primary).
   */
  Gltf& recenter(lv_gltf_model_t* model = nullptr);

  // --- Camera Control ---

  Gltf& set_yaw(float yaw);
  float get_yaw() const;

  Gltf& set_pitch(float pitch);
  float get_pitch() const;

  Gltf& set_distance(float distance);
  float get_distance() const;

  Gltf& set_fov(float fov);
  float get_fov() const;

  // --- Focal Point ---

  Gltf& set_focal_point(const Point3D& p);
  Point3D get_focal_point() const;

  // --- Visual Settings ---

  Gltf& set_animation_speed(uint32_t speed);
  uint32_t get_animation_speed() const;

  Gltf& set_background_mode(BackgroundMode mode);
  BackgroundMode get_background_mode() const;

  Gltf& set_background_blur(uint32_t blur);
  uint32_t get_background_blur() const;

  Gltf& set_env_brightness(uint32_t brightness);
  uint32_t get_env_brightness() const;

  Gltf& set_exposure(float exposure);
  float get_exposure() const;

  Gltf& set_antialiasing(AntiAliasing mode);
  AntiAliasing get_antialiasing() const;

  // --- Interaction / Raycasting ---

  /**
   * @brief Get a ray from a 2D screen coordinate (e.g. mouse click).
   */
  Ray3D get_ray(const Point& screen_pos) const;

  /**
   * @brief Project a world point to screen coordinates.
   */
  std::optional<Point> world_to_screen(const Point3D& world_pos) const;

  /**
   * @brief Get a camera-facing plane at a certain distance.
   */
  Plane3D get_view_plane(float distance) const;
};

}  // namespace lvgl

#endif  // LV_USE_GLTF
#endif  // LVGL_CPP_WIDGETS_GLTF_H_
