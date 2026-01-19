#include <cstring>
#include <iostream>
#include <vector>

#include "../misc/file_system.h"
#include "lvgl.h"

void test_filesystem_write_read() {
  std::cout << "Testing Filesystem Write/Read..." << std::endl;

  // Use a temporary file path. Since we use STDIO driver with letter 'A',
  // and no prefix path, we might need to be careful where it writes.
  // For tests running in build dir, writing to "test.bin" usually works for
  // STDIO. But we need the driver prefix.
  std::string path = "A:test_fs.bin";

  {
    lvgl::File f;
    lv_fs_res_t res = f.open(path, LV_FS_MODE_WR);
    if (res != LV_FS_RES_OK) {
      std::cerr << "FAIL: Could not open file for writing: " << path
                << " res=" << res << std::endl;
      exit(1);
    }

    const char* data = "Hello LVGL Filesystem!";
    uint32_t len = strlen(data);
    uint32_t bw = 0;
    res = f.write(data, len, &bw);
    if (res != LV_FS_RES_OK || bw != len) {
      std::cerr << "FAIL: Write failed. res=" << res << " bw=" << bw
                << std::endl;
      exit(1);
    }
    f.close();
  }

  {
    lvgl::File f(path, LV_FS_MODE_RD);
    if (!f.is_open()) {
      std::cerr << "FAIL: Could not open file for reading." << std::endl;
      exit(1);
    }

    char buf[100];
    uint32_t br = 0;
    lv_fs_res_t res = f.read(buf, sizeof(buf) - 1, &br);
    if (res != LV_FS_RES_OK) {
      std::cerr << "FAIL: Read failed. res=" << res << std::endl;
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
    lvgl::File f(path, LV_FS_MODE_RD);
    f.seek(6, LV_FS_SEEK_SET);
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

  // We need to ensure the driver is ready.
  if (!lvgl::FileSystem::is_ready('A')) {
    std::cout << "WARNING: Driver 'A' not ready. Is LV_USE_FS_STDIO enabled?"
              << std::endl;
    // Proceed anyway, maybe it just needs first access
  }

  test_filesystem_write_read();

  return 0;
}
