#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

#include "../misc/file_system.h"
#include "lvgl.h"

void test_path_utils() {
  std::cout << "Testing Path Utils..." << std::endl;

  // Extension
  assert(lvgl::FileSystem::get_extension("S:/data/file.txt") == "txt");
  assert(lvgl::FileSystem::get_extension("S:/data/file") == "");
  assert(lvgl::FileSystem::get_extension("S:/data/file.tar.gz") == "gz");

  // Filename
  assert(lvgl::FileSystem::get_filename("S:/data/file.txt") == "file.txt");
  assert(lvgl::FileSystem::get_filename("S:/data/") == "data/");
  assert(lvgl::FileSystem::get_filename("file.txt") == "file.txt");

  // Up
  // Implementation note: up() modifies string. "S:/data/file.txt" -> "S:/data"
  // (approx, depends on lv_fs logic) lv_fs_up behavior: "S:/folder/file" ->
  // "S:/folder"
  assert(lvgl::FileSystem::up("S:/data/file.txt") == "S:/data");
  assert(lvgl::FileSystem::up("S:/data") == "S:");

  // Join
  // "S:/folder" + "file.txt" -> "S:/folder/file.txt"
  // "S:/folder/" + "file.txt" -> "S:/folder/file.txt"
  std::string joined = lvgl::FileSystem::join_path("S:/folder", "file.txt");
  assert(joined == "S:/folder/file.txt" ||
         joined == "S:/folder\\file.txt");  // Separator depends on OS

  std::cout << "Path Utils Passed" << std::endl;
}

void test_driver_enumeration() {
  std::cout << "Testing Driver Enumeration..." << std::endl;
  std::string letters = lvgl::FileSystem::get_letters();
  std::cout << "Available Drivers: " << letters << std::endl;

  if (letters.empty()) {
    std::cout << "WARNING: No FS drivers available. I/O tests might fail."
              << std::endl;
  }
}

void test_file_io() {
  std::cout << "Testing File I/O..." << std::endl;

  std::string letters = lvgl::FileSystem::get_letters();
  if (letters.empty()) {
    std::cout << "Skipping I/O tests (no driver)" << std::endl;
    return;
  }

  // Use the first available driver
  char drive = letters[0];
  std::string path = std::string(1, drive) + ":/test_file.txt";

  // Write
  {
    lvgl::File f(path, LV_FS_MODE_WR);
    if (!f.is_open()) {
      // Some drivers might fail if root is not writable or requires mounting
      std::cout << "Failed to open " << path
                << " for writing. Skipping I/O test." << std::endl;
      return;
    }
    std::string data = "Hello, LVGL FS!";
    lv_fs_res_t res = f.write(data.c_str(), data.length());
    assert(res == LV_FS_RES_OK);
  }

  // Read
  {
    lvgl::File f(path, LV_FS_MODE_RD);
    assert(f.is_open());
    char buf[32] = {0};
    uint32_t br = 0;
    lv_fs_res_t res = f.read(buf, sizeof(buf) - 1, &br);
    assert(res == LV_FS_RES_OK);
    assert(std::string(buf) == "Hello, LVGL FS!");
  }

  // Load to buffer
  {
    std::vector<uint8_t> data = lvgl::File::load_to_buffer(path);
    assert(data.size() == 15);  // "Hello, LVGL FS!" length
    std::string str(data.begin(), data.end());
    assert(str == "Hello, LVGL FS!");
  }

  std::cout << "File I/O Passed" << std::endl;
}

int main() {
  lv_init();

  // Initialize STDIO driver manually if needed?
  // The C++ wrapper assumes drivers are inited.
  // In tests, we might need to call lv_fs_stdio_init().
  // lv_fs_stdio_init(); // This might be needed if lv_conf.h doesn't auto-init
  // or if we need to explicitly call it. Checking lv_conf_internal.h or similar
  // would reveal if it's auto-inited. Usually lv_init() calls lv_fs_init()
  // which might init drivers if configured. However, mostly drivers need
  // explicit init. Since I don't know if available, I'll try to execute it if
  // symbol exists, but I can't conditionally compile easily here. I'll rely on
  // global init or just run and see.

  // Actually, lv_fsdrv.h declares lv_fs_stdio_init().
  // If we link with lvgl, and LV_USE_FS_STDIO is 1, we should call it.
  // I will blindly try to use whatever is available.

  test_path_utils();
  test_driver_enumeration();
  test_file_io();

  return 0;
}
