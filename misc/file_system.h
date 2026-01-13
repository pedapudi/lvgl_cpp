#ifndef LVGL_CPP_MISC_FILE_SYSTEM_H_
#define LVGL_CPP_MISC_FILE_SYSTEM_H_

#include "lvgl.h" // IWYU pragma: export
#include <string>
#include <vector>

namespace lvgl {

/**
 * @brief Wrapper for file operations (lv_fs_file_t)
 */
class File {
public:
  File();
  explicit File(const std::string &path, lv_fs_mode_t mode = LV_FS_MODE_RD);
  ~File();

  // Disable verify by default to prevent accidental closing
  File(const File &) = delete;
  File& operator=(const File &) = delete;
  File(File &&other) noexcept;
  File& operator=(File &&other) noexcept;

  lv_fs_res_t open(const std::string &path, lv_fs_mode_t mode);
  lv_fs_res_t close();

  lv_fs_res_t read(void* buf, uint32_t btr, uint32_t* br = nullptr);
  lv_fs_res_t write(const void* buf, uint32_t btw, uint32_t* bw = nullptr);
  lv_fs_res_t seek(uint32_t pos, lv_fs_whence_t whence);
  lv_fs_res_t tell(uint32_t* pos);

  uint32_t size(); // Convenience wrapper around lv_fs_get_size

  bool is_open() const;

private:
  lv_fs_file_t file_;
  bool is_opened_ = false;
};

/**
 * @brief Wrapper for directory operations (lv_fs_dir_t)
 */
class Directory {
public:
  Directory();
  explicit Directory(const std::string &path);
  ~Directory();

  Directory(const Directory &) = delete;
  Directory& operator=(const Directory &) = delete;
  Directory(Directory &&other) noexcept;
  Directory& operator=(Directory &&other) noexcept;

  lv_fs_res_t open(const std::string &path);
  lv_fs_res_t close();

  /**
   * @brief Read next entry.
   * @param fn Buffer to store filename.
   * @return LV_FS_RES_OK on success. if fn is empty, end of dir.
   */
  lv_fs_res_t read(std::string &fn);

  bool is_open() const;

private:
  lv_fs_dir_t dir_;
  bool is_opened_ = false;
};

} // namespace lvgl

#endif // LVGL_CPP_MISC_FILE_SYSTEM_H_
