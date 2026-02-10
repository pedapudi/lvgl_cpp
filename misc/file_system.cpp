#include "file_system.h"

#include <cstring>  // for memset

#include "../core/compatibility.h"

namespace lvgl {

#if !LVGL_CPP_HAS_NEW_FS_HELPERS

static lv_fs_res_t fallback_fs_get_size(lv_fs_file_t* file, uint32_t* size) {
  uint32_t cur = 0;
  lv_fs_res_t res = lv_fs_tell(file, &cur);
  if (res != LV_FS_RES_OK) return res;

  res = lv_fs_seek(file, 0, LV_FS_SEEK_END);
  if (res != LV_FS_RES_OK) return res;

  res = lv_fs_tell(file, size);
  if (res != LV_FS_RES_OK) return res;

  return lv_fs_seek(file, cur, LV_FS_SEEK_SET);
}

static lv_fs_res_t fallback_fs_path_get_size(const char* path, uint32_t* size) {
  lv_fs_file_t f;
  lv_fs_res_t res = lv_fs_open(&f, path, LV_FS_MODE_RD);
  if (res != LV_FS_RES_OK) return res;
  fallback_fs_get_size(&f, size);
  lv_fs_close(&f);
  return LV_FS_RES_OK;
}

static lv_fs_res_t fallback_fs_load_to_buf(void* buf, uint32_t size,
                                           const char* path) {
  lv_fs_file_t f;
  lv_fs_res_t res = lv_fs_open(&f, path, LV_FS_MODE_RD);
  if (res != LV_FS_RES_OK) return res;
  uint32_t br;
  res = lv_fs_read(&f, buf, size, &br);
  lv_fs_close(&f);
  return res;
}

static void fallback_fs_path_join(char* buf, uint32_t buf_len, const char* path,
                                  const char* part) {
  (void)buf_len;
  size_t path_len = std::strlen(path);
  std::strcpy(buf, path);
  if (path_len > 0 && path[path_len - 1] != '/' && path[path_len - 1] != '\\') {
    std::strcat(buf, "/");
  }
  std::strcat(buf, part);
}

#endif  // !LVGL_CPP_HAS_NEW_FS_HELPERS

// --- File ---

File::File() {
  // lv_fs_file_t has no invalid state defined, but usually zeroed out is safe
  // We track state with is_opened_
  std::memset(&file_, 0, sizeof(file_));
}

File::File(const std::string& path, FsMode mode) : File() { open(path, mode); }

File::~File() { close(); }

File::File(File&& other) noexcept
    : file_(other.file_), is_opened_(other.is_opened_) {
  other.is_opened_ = false;
  std::memset(&other.file_, 0, sizeof(other.file_));
}

File& File::operator=(File&& other) noexcept {
  if (this != &other) {
    close();
    file_ = other.file_;
    is_opened_ = other.is_opened_;
    other.is_opened_ = false;
    std::memset(&other.file_, 0, sizeof(other.file_));
  }
  return *this;
}

FsRes File::open(const std::string& path, FsMode mode) {
  close();  // Ensure previous is closed
  lv_fs_res_t res =
      lv_fs_open(&file_, path.c_str(), static_cast<lv_fs_mode_t>(mode));
  if (res == LV_FS_RES_OK) {
    is_opened_ = true;
  }
  return static_cast<FsRes>(res);
}

FsRes File::close() {
  if (is_opened_) {
    lv_fs_res_t res = lv_fs_close(&file_);
    is_opened_ = false;
    return static_cast<FsRes>(res);
  }
  return FsRes::Ok;
}

FsRes File::read(void* buf, uint32_t btr, uint32_t* br) {
  if (!is_opened_) return FsRes::NotEx;
  return static_cast<FsRes>(lv_fs_read(&file_, buf, btr, br));
}

FsRes File::write(const void* buf, uint32_t btw, uint32_t* bw) {
  if (!is_opened_) return FsRes::NotEx;
  return static_cast<FsRes>(lv_fs_write(&file_, buf, btw, bw));
}

FsRes File::seek(uint32_t pos, FsWhence whence) {
  if (!is_opened_) return FsRes::NotEx;
  return static_cast<FsRes>(
      lv_fs_seek(&file_, pos, static_cast<lv_fs_whence_t>(whence)));
}

FsRes File::tell(uint32_t* pos) {
  if (!is_opened_) return FsRes::NotEx;
  return static_cast<FsRes>(lv_fs_tell(&file_, pos));
}

uint32_t File::size() {
  if (!is_opened_) return 0;
  uint32_t s = 0;
#if LVGL_CPP_HAS_NEW_FS_HELPERS
  lv_fs_get_size(&file_, &s);
#else
  fallback_fs_get_size(&file_, &s);
#endif
  return s;
}

bool File::is_open() const { return is_opened_; }

// --- Directory ---

Directory::Directory() { std::memset(&dir_, 0, sizeof(dir_)); }

Directory::Directory(const std::string& path) : Directory() { open(path); }

Directory::~Directory() { close(); }

Directory::Directory(Directory&& other) noexcept
    : dir_(other.dir_), is_opened_(other.is_opened_) {
  other.is_opened_ = false;
  std::memset(&other.dir_, 0, sizeof(other.dir_));
}

Directory& Directory::operator=(Directory&& other) noexcept {
  if (this != &other) {
    close();
    dir_ = other.dir_;
    is_opened_ = other.is_opened_;
    other.is_opened_ = false;
    std::memset(&other.dir_, 0, sizeof(other.dir_));
  }
  return *this;
}

FsRes Directory::open(const std::string& path) {
  close();
  lv_fs_res_t res = lv_fs_dir_open(&dir_, path.c_str());
  if (res == LV_FS_RES_OK) {
    is_opened_ = true;
  }
  return static_cast<FsRes>(res);
}

FsRes Directory::close() {
  if (is_opened_) {
    lv_fs_res_t res = lv_fs_dir_close(&dir_);
    is_opened_ = false;
    return static_cast<FsRes>(res);
  }
  return FsRes::Ok;
}

FsRes Directory::read(std::string& fn) {
  if (!is_opened_) return FsRes::NotEx;
  char buf[256];  // Max path length
  lv_fs_res_t res = lv_fs_dir_read(&dir_, buf, sizeof(buf));
  if (res == LV_FS_RES_OK) {
    fn = buf;
  }
  return static_cast<FsRes>(res);
}

bool Directory::is_open() const { return is_opened_; }

// --- FileSystem Utilities ---

std::string FileSystem::get_letters() {
  char buf[32];  // Enough for 26 letters + terminator
  lv_fs_get_letters(buf);
  return std::string(buf);
}

std::string FileSystem::get_extension(const std::string& path) {
  const char* ext = lv_fs_get_ext(path.c_str());
  if (!ext) return "";
  return std::string(ext);
}

std::string FileSystem::get_filename(const std::string& path) {
  const char* fn = lv_fs_get_last(path.c_str());
  if (!fn) return "";
  return std::string(fn);
}

std::string FileSystem::up(const std::string& path) {
  // lv_fs_up modifies the buffer in place.
  // We need a mutable buffer.
  std::vector<char> buf(path.begin(), path.end());
  buf.push_back('\0');  // Ensure null-terminated

  lv_fs_up(buf.data());

  return std::string(buf.data());
}

std::string FileSystem::join_path(const std::string& base,
                                  const std::string& part) {
  char buf[LV_FS_MAX_PATH_LENGTH];
#if LVGL_CPP_HAS_NEW_FS_HELPERS
  lv_fs_path_join(buf, sizeof(buf), base.c_str(), part.c_str());
#else
  fallback_fs_path_join(buf, sizeof(buf), base.c_str(), part.c_str());
#endif
  return std::string(buf);
}

bool FileSystem::is_ready(char letter) { return lv_fs_is_ready(letter); }

bool FileSystem::exists(const std::string& path) {
  uint32_t size = 0;
#if LVGL_CPP_HAS_NEW_FS_HELPERS
  return lv_fs_path_get_size(path.c_str(), &size) == LV_FS_RES_OK;
#else
  return fallback_fs_path_get_size(path.c_str(), &size) == LV_FS_RES_OK;
#endif
}

uint32_t FileSystem::get_size(const std::string& path) {
  uint32_t size = 0;
#if LVGL_CPP_HAS_NEW_FS_HELPERS
  if (lv_fs_path_get_size(path.c_str(), &size) != LV_FS_RES_OK) {
    return 0;
  }
#else
  if (fallback_fs_path_get_size(path.c_str(), &size) != LV_FS_RES_OK) {
    return 0;
  }
#endif
  return size;
}

std::vector<uint8_t> File::load_to_buffer(const std::string& path) {
  // Use get_size logic (which we just defined to use helpers)
  uint32_t size = FileSystem::get_size(path);
  if (size == 0) return {};

  std::vector<uint8_t> buf(size);
#if LVGL_CPP_HAS_NEW_FS_HELPERS
  if (lv_fs_load_to_buf(buf.data(), size, path.c_str()) != LV_FS_RES_OK) {
    return {};
  }
#else
  if (fallback_fs_load_to_buf(buf.data(), size, path.c_str()) != LV_FS_RES_OK) {
    return {};
  }
#endif
  return buf;
}

}  // namespace lvgl
