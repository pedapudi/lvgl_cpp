#include "file_system.h"

#include <cstring>  // for memset

namespace lvgl {

// --- File ---

File::File() {
  // lv_fs_file_t has no invalid state defined, but usually zeroed out is safe
  // We track state with is_opened_
  std::memset(&file_, 0, sizeof(file_));
}

File::File(const std::string& path, lv_fs_mode_t mode) : File() {
  open(path, mode);
}

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

lv_fs_res_t File::open(const std::string& path, lv_fs_mode_t mode) {
  close();  // Ensure previous is closed
  lv_fs_res_t res = lv_fs_open(&file_, path.c_str(), mode);
  if (res == LV_FS_RES_OK) {
    is_opened_ = true;
  }
  return res;
}

lv_fs_res_t File::close() {
  if (is_opened_) {
    lv_fs_res_t res = lv_fs_close(&file_);
    is_opened_ = false;
    return res;
  }
  return LV_FS_RES_OK;
}

lv_fs_res_t File::read(void* buf, uint32_t btr, uint32_t* br) {
  if (!is_opened_) return LV_FS_RES_NOT_EX;
  return lv_fs_read(&file_, buf, btr, br);
}

lv_fs_res_t File::write(const void* buf, uint32_t btw, uint32_t* bw) {
  if (!is_opened_) return LV_FS_RES_NOT_EX;
  return lv_fs_write(&file_, buf, btw, bw);
}

lv_fs_res_t File::seek(uint32_t pos, lv_fs_whence_t whence) {
  if (!is_opened_) return LV_FS_RES_NOT_EX;
  return lv_fs_seek(&file_, pos, whence);
}

lv_fs_res_t File::tell(uint32_t* pos) {
  if (!is_opened_) return LV_FS_RES_NOT_EX;
  return lv_fs_tell(&file_, pos);
}

uint32_t File::size() {
  if (!is_opened_) return 0;
  uint32_t s = 0;
  lv_fs_get_size(&file_, &s);
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

lv_fs_res_t Directory::open(const std::string& path) {
  close();
  lv_fs_res_t res = lv_fs_dir_open(&dir_, path.c_str());
  if (res == LV_FS_RES_OK) {
    is_opened_ = true;
  }
  return res;
}

lv_fs_res_t Directory::close() {
  if (is_opened_) {
    lv_fs_res_t res = lv_fs_dir_close(&dir_);
    is_opened_ = false;
    return res;
  }
  return LV_FS_RES_OK;
}

lv_fs_res_t Directory::read(std::string& fn) {
  if (!is_opened_) return LV_FS_RES_NOT_EX;
  char buf[256];  // Max path length
  lv_fs_res_t res = lv_fs_dir_read(&dir_, buf, sizeof(buf));
  if (res == LV_FS_RES_OK) {
    fn = buf;
  }
  return res;
}

bool Directory::is_open() const { return is_opened_; }

}  // namespace lvgl
