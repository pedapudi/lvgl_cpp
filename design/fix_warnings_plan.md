
- [ ] **Fix Compilation Errors in Image Widget**
  - Restore the `set_pivot(int32_t x, int32_t y)` declaration in `widgets/image.h` (marked as deprecated) to match the implementation in `widgets/image.cpp`.
  - This resolves the "no declaration matches" error.

- [ ] **Fix Deprecation Warnings in Canvas Widget**
  - Refactor `Canvas::set_buffer` and `Canvas::set_px` implementations to avoid calling deprecated overloads internally.
  - Implement the new typed overloads (taking `ColorFormat`, `Opacity`, etc.) to directly call the underlying C API (`lv_canvas_set_buffer`, etc.).
  - Make the deprecated legacy overloads forward to the new typed overloads or duplicate the C API call to break the dependency cycle.

- [ ] **Fix Deprecation Warnings in Scale, Roller, Keyboard Widgets**
  - These warnings stem from using deprecated enum types in function signatures.
  - Update `Scale::set_mode`, `Roller::set_options`, and `Keyboard::set_mode` signatures to use the modern scoped enums (`Scale::Mode`, `Roller::Mode`, `Keyboard::Mode`) as the primary interface.
  - Retain legacy signatures using the deprecated types if backward compatibility is strict, but ensure the implementation doesn't trigger warnings (e.g., using `#pragma GCC diagnostic push` or direct implementation).

- [ ] **Verify Build**
  - Run the build again to ensure all errors and warnings are resolved.

- [ ] **Verify Tests**
  - Run existing tests to ensure no regressions were introduced by API changes.
