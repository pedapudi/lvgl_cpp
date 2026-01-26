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
  lv_fs_path_join(buf, sizeof(buf), base.c_str(), part.c_str());
  return std::string(buf);
}

bool FileSystem::is_ready(char letter) { return lv_fs_is_ready(letter); }

bool FileSystem::exists(const std::string& path) {
  uint32_t size = 0;
  return lv_fs_path_get_size(path.c_str(), &size) == LV_FS_RES_OK;
}

uint32_t FileSystem::get_size(const std::string& path) {
  uint32_t size = 0;
  if (lv_fs_path_get_size(path.c_str(), &size) != LV_FS_RES_OK) {
    return 0;
  }
  return size;
}

std::vector<uint8_t> File::load_to_buffer(const std::string& path) {
  uint32_t size = 0;
  if (lv_fs_path_get_size(path.c_str(), &size) != LV_FS_RES_OK) {
    return {};
  }

  std::vector<uint8_t> buf(size);
  // Using lv_fs_load_to_buf if available, or manual read
  // lv_fs_load_to_buf signature: lv_fs_res_t lv_fs_load_to_buf(void * buf,
  // uint32_t buf_size, const char * path)
  if (lv_fs_load_to_buf(buf.data(), size, path.c_str()) != LV_FS_RES_OK) {
    return {};
  }

  return buf;
}

}  // namespace lvgl
