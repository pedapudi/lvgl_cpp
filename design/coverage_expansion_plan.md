# Coverage Expansion Plan: Color & Font

This plan details the steps required to bring `lv_color` to Platinum status and `lv_font` to Gold status.

## 1. `lv_color` (Platinum Roadmap)

Current Status: **41.4%**. The missing APIs are primarily advanced color math and 16-bit specific optimizations.

### Proposed Changes
- [ ] **Color Math Overloads**: Implement `operator*`, `operator+`, and `mix` methods in the `Color` class.
- [ ] **Luminance & Brightness**: Expose `lv_color_luminance` and `lv_color_brightness` as member functions.
- [ ] **HSV Integration**: Improve the `HSV` struct and provide seamless conversion to/from `Color`.
- [ ] **16-bit Utilities**: Expose `swap_16` and `mix_16` for memory-constrained embedded environments.

### Definition of Done
- 100% of non-internal `lv_color_` functions wrapped or logically verified.
- Unit tests for color mixing and formatting.

---

## 2. `lv_font` (Gold Roadmap)

Current Status: **16.7%**. The target is high-priority usage patterns for embedded systems.

### Proposed Changes
- [ ] **Font Manager**: Create a static `Font::get_default()` helper.
- [ ] **Custom Loading**: Implement `Font::load_from_memory(void* data)` to support fonts embedded as binary blobs.
- [ ] **Kerning Control**: Expose `lv_font_set_kerning` for fine-grained typography control.
- [ ] **Asset Integration**: Link with the `ImageDescriptor` pattern to allow fonts to be treated as managed assets.

### Definition of Done
- > 80% coverage of core `lv_font_` functions.
- Successful demonstration of loading a non-standard font in a test case.

## 3. Timeline

| Subsystem | Target | Deadline |
| :--- | :--- | :--- |
| **lv_color** | Platinum | End of Phase 8 |
| **lv_font** | Gold | End of Phase 8 |
