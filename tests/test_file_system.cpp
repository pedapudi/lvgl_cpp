#include <cstring>
#include <iostream>
#include <vector>

#include "../display/display.h"
#include "../misc/file_system.h"
#include "../lvgl_cpp.h"

void test_filesystem_write_read() {
  std::cout << "Testing Filesystem Write/Read..." << std::endl;

  // Use a temporary file path. Since we use STDIO driver with letter 'A',
  // and no prefix path, we might need to be careful where it writes.
  // For tests running in build dir, writing to "test.bin" usually works for
  // STDIO. But we need the driver prefix.
  std::string path = "A:test_fs.bin";

  {
    lvgl::File f;
    lvgl::FsRes res = f.open(path, lvgl::FsMode::Write);
    if (res != lvgl::FsRes::Ok) {
      std::cerr << "FAIL: Could not open file for writing: " << path
                << " res=" << static_cast<int>(res) << std::endl;
      exit(1);
    }

    const char* data = "Hello LVGL Filesystem!";
    uint32_t len = strlen(data);
    uint32_t bw = 0;
    res = f.write(data, len, &bw);
    if (res != lvgl::FsRes::Ok || bw != len) {
      std::cerr << "FAIL: Write failed. res=" << static_cast<int>(res)
                << " bw=" << bw << std::endl;
      exit(1);
    }
    f.close();
  }

  {
    lvgl::File f(path, lvgl::FsMode::Read);
    if (!f.is_open()) {
      std::cerr << "FAIL: Could not open file for reading." << std::endl;
      exit(1);
    }

    char buf[100];
    uint32_t br = 0;
    lvgl::FsRes res = f.read(buf, sizeof(buf) - 1, &br);
    if (res != lvgl::FsRes::Ok) {
      std::cerr << "FAIL: Read failed. res=" << static_cast<int>(res)
                << std::endl;
      exit(1);
    }
    buf[br] = '\0';

    if (std::string(buf) == "Hello LVGL Filesystem!") {
      std::cout << "PASS: Read/Write content matches." << std::endl;
    } else {
      std::cerr << "FAIL: Content mismatch. Got: '" << buf << "'" << std::endl;
      exit(1);
    }
  }

  // Test tell/seek
  {
    lvgl::File f(path, lvgl::FsMode::Read);
    f.seek(6, lvgl::FsWhence::Set);
    uint32_t pos;
    f.tell(&pos);
    if (pos == 6) {
      std::cout << "PASS: Seek/Tell works." << std::endl;
    } else {
      std::cerr << "FAIL: Seek/Tell failed. pos=" << pos << std::endl;
      exit(1);
    }

    char buf[100];
    uint32_t br;
    f.read(buf, 4, &br);
    buf[br] = 0;
    if (std::string(buf) == "LVGL") {
      std::cout << "PASS: Read after seek works." << std::endl;
    } else {
      std::cerr << "FAIL: Read after seek mismatch. Got: '" << buf << "'"
                << std::endl;
      exit(1);
    }
  }

  // Test Utility functions
  {
    if (lvgl::FileSystem::exists(path)) {
      std::cout << "PASS: FileSystem::exists(path) works." << std::endl;
    } else {
      std::cerr << "FAIL: FileSystem::exists(path) failed for existing file."
                << std::endl;
      exit(1);
    }

    uint32_t size = lvgl::FileSystem::get_size(path);
    if (size > 0) {
      std::cout << "PASS: FileSystem::get_size(path) returns " << size << "."
                << std::endl;
    } else {
      std::cerr << "FAIL: FileSystem::get_size(path) returned 0." << std::endl;
      exit(1);
    }

    if (!lvgl::FileSystem::exists("A:missing_file.txt")) {
      std::cout
          << "PASS: FileSystem::exists() correctly identifies missing file."
          << std::endl;
    } else {
      std::cerr << "FAIL: FileSystem::exists() incorrectly found missing file."
                << std::endl;
      exit(1);
    }
  }
}

void test_directory() {
  std::cout << "Testing Directory..." << std::endl;
  // Open current directory
  lvgl::Directory dir("A:.");  // . usually works for current dir

  // Note: iterating directories in pure STDIO driver might vary depending on
  // implementation. If it fails we might skip, but let's see.

  // Just check if it opens or handles gracefully.
  // Standard LVGL stdio driver usually supports opendir on '.'

  // Actually, let's just create a dummy file and assume we are in it.
}

int main() {
  lv_init();

  // Creating/deleting objects requires memory management to be initialized
  // Depending on lv_conf.h, lv_init might be sufficient.
  // We need a display for active screen to work in widgets.
  lvgl::Display display = lvgl::Display::create(800, 480);

  test_filesystem_write_read();

  return 0;
}
