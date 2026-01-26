#ifndef LVGL_CPP_MISC_FILE_SYSTEM_H_
#define LVGL_CPP_MISC_FILE_SYSTEM_H_

#include <cstdint>
#include <string>
#include <vector>

#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

/**
 * @brief Wrapper for file operations (lv_fs_file_t)
 */
class File {
 public:
  File();
  explicit File(const std::string& path, lv_fs_mode_t mode = LV_FS_MODE_RD);
  ~File();

  // Disable verify by default to prevent accidental closing
  File(const File&) = delete;
  File& operator=(const File&) = delete;
  File(File&& other) noexcept;
  File& operator=(File&& other) noexcept;

  /**
   * @brief Open a file.
   * @param path Path to the file.
   * @param mode Access mode (e.g. `LV_FS_MODE_RD`).
   * @return `LV_FS_RES_OK` on success, or an error code.
   */
  lv_fs_res_t open(const std::string& path, lv_fs_mode_t mode);

  /**
   * @brief Close the file.
   * @return `LV_FS_RES_OK` on success.
   */
  lv_fs_res_t close();

  /**
   * @brief Read from the file.
   * @param buf Buffer to read into.
   * @param btr Bytes to read.
   * @param br Pointer to store the number of bytes read.
   * @return `LV_FS_RES_OK` on success.
   */
  lv_fs_res_t read(void* buf, uint32_t btr, uint32_t* br = nullptr);

  /**
   * @brief Load the entire file into a buffer.
   * @param path The path of the file.
   * @return Vector containing file contents. Empty on error.
   */
  static std::vector<uint8_t> load_to_buffer(const std::string& path);

  /**
   * @brief Write to the file.
   * @param buf Buffer to write from.
   * @param btw Bytes to write.
   * @param bw Pointer to store the number of bytes written.
   * @return `LV_FS_RES_OK` on success.
   */
  lv_fs_res_t write(const void* buf, uint32_t btw, uint32_t* bw = nullptr);

  /**
   * @brief Seek to a position in the file.
   * @param pos Position offset.
   * @param whence Reference position (`LV_FS_SEEK_SET`, `LV_FS_SEEK_CUR`,
   * `LV_FS_SEEK_END`).
   * @return `LV_FS_RES_OK` on success.
   */
  lv_fs_res_t seek(uint32_t pos, lv_fs_whence_t whence);

  /**
   * @brief Get the current position.
   * @param pos Pointer to store the position.
   * @return `LV_FS_RES_OK` on success.
   */
  lv_fs_res_t tell(uint32_t* pos);

  /**
   * @brief Get the file size.
   * @return File size in bytes.
   */
  uint32_t size();  // Convenience wrapper around lv_fs_get_size

  /**
   * @brief Check if the file is currently open.
   * @return true if open.
   */
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
  explicit Directory(const std::string& path);
  ~Directory();

  Directory(const Directory&) = delete;
  Directory& operator=(const Directory&) = delete;
  Directory(Directory&& other) noexcept;
  Directory& operator=(Directory&& other) noexcept;

  /**
   * @brief Open a directory.
   * @param path Path to the directory.
   * @return `LV_FS_RES_OK` on success.
   */
  lv_fs_res_t open(const std::string& path);

  /**
   * @brief Close the directory.
   * @return `LV_FS_RES_OK` on success.
   */
  lv_fs_res_t close();

  /**
   * @brief Read next entry.
   * @param fn Buffer to store filename.
   * @return LV_FS_RES_OK on success. if fn is empty, end of dir.
   */
  lv_fs_res_t read(std::string& fn);

  bool is_open() const;

 private:
  lv_fs_dir_t dir_;
  bool is_opened_ = false;
};

/**
 * @brief Global filesystem utilities
 */
class FileSystem {
 public:
  /**
   * @brief Get available driver letters.
   * @return String containing all registered driver letters (e.g. "PS").
   */
  static std::string get_letters();

  /**
   * @brief Get the extension of a filename.
   * @param path The filename or path.
   * @return Extension (e.g. "txt") or empty string.
   */
  static std::string get_extension(const std::string& path);

  /**
   * @brief Get the filename from a path (e.g. "S:/foo/bar.txt" -> "bar.txt").
   * @param path The full path.
   * @return The filename component.
   */
  static std::string get_filename(const std::string& path);

  /**
   * @brief Step up one level in the path (get parent directory).
   * @param path The path to truncate.
   * @return Parent directory path.
   */
  static std::string up(const std::string& path);

  /**
   * @brief Join two path components with appropriate separators.
   * @param base The base path.
   * @param part The component to append.
   * @return Joined path.
   */
  static std::string join_path(const std::string& base,
                               const std::string& part);

  /**
   * @brief Check if a file or directory exists.
   * @param path The path to check.
   * @return true if it exists.
   */
  static bool exists(const std::string& path);

  /**
   * @brief Get the size of a file at the given path.
   * @param path The path to the file.
   * @return File size in bytes, or 0 if not found/error.
   */
  static uint32_t get_size(const std::string& path);

  /**
   * @brief Check if a driver is ready.
   * @param letter The driver letter.
   * @return true if ready.
   */
  static bool is_ready(char letter);
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_FILE_SYSTEM_H_
