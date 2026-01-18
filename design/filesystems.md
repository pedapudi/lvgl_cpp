# Research Report: LVGL Filesystem in C++

## Executive Summary

LVGL provides a unified File System interface (`lv_fs`) that abstracts underlying storage drivers (FATFS, SD Card, POSIX, etc.) using a drive letter mechanism (e.g., `S:/path/to/file`). The `lvgl_cpp` library currently lacks a wrapper for this subsystem.

This report outlines the design for a C++ `FileSystem` module that provides:
1.  **RAII wrappers** for files and directories (automatic closing).
2.  **Idiomatic C++ API** mimicking `std::fstream` and `std::filesystem` where appropriate.
3.  **Safety** against common resource leaks.

## 1. LVGL Filesystem Mechanics (`lv_fs`)

### 1.1 Drivers and Paths
- **Drivers**: Identified by a single uppercase letter (A-Z).
- **Paths**: Must start with the driver letter (e.g., `"P:/data/logs.txt"`).
- **Registration**: Drivers are usually registered at startup via `lv_fs_drv_register`.

### 1.2 Resource Management
- **Files**: Opened with `lv_fs_open`, must be closed with `lv_fs_close`.
- **Directories**: Opened with `lv_fs_dir_open`, must be closed with `lv_fs_dir_close`.
- **Error Handling**: Functions return `lv_fs_res_t` enum.

## 2. Existing `lvgl_cpp` Implementation

An existing wrapper was found in `misc/file_system.h`. It covers the core functionality (`File` and `Directory` classes with RAII) effectively.

### 2.1 Current Capabilities
-   **RAII**: Class destructors automatically close handles.
-   **Move Semantics**: Properly implemented to prevent double-free.
-   **Core I/O**: Read, Write, Seek, Tell supported.

### 2.2 Missing Features
The current implementation lacks wrappers for several useful `lv_fs` utility functions:
-   `lv_fs_get_letters`: Enumerate available drive letters.
-   **Path Utilities**: `lv_fs_up` (parent dir), `lv_fs_get_last` (filename), `lv_fs_path_join`.
-   **Extension Helper**: `lv_fs_get_ext`.
-   **Static Loaders**: `lv_fs_load_to_buf` (helper for reading entire files).

## 3. Driver Initialization (`fsdrv`)

The `lv_fsdrv` module (standard drivers) acts as the backend.
-   **Initialization**: The C++ wrapper assumes the underlying driver is already initialized (e.g., `lv_fs_stdio_init()` called during app setup).
-   **Configuration**: Drivers are typically enabled via `lv_conf.h` (e.g., `LV_USE_FS_STDIO 1`).
-   **Recommendation**: The C++ library should likely distinct itself from *driver configuration* but clearly document that drivers must be active.

## 4. Recommendations

1.  **Add Unit Tests**: No tests were found for `file_system.h`. We should add `tests/test_file_system.cpp` that mocks a driver or uses the STDIO driver (letter 'P'/'S' usually) to verify file operations.
2.  **Add Utility Wrappers**: Add a `FileSystem` namespace or static class to expose:
    *   `static std::string get_extension(const std::string& path);`
    *   `static std::string get_filename(const std::string& path);`
    *   `static std::string join_path(const std::string& base, const std::string& part);`
3.  **Documentation**: Add comments to `file_system.h` clarifying that `ensure_driver_init()` (or similar) is a user responsibility, or provide a helper if appropriate.


## 5. References

1.  **LVGL Documentation - File System**: [https://docs.lvgl.io/master/details/main-modules/fs.html](https://docs.lvgl.io/master/details/main-modules/fs.html)
2.  **Header**: `src/misc/lv_fs.h`
