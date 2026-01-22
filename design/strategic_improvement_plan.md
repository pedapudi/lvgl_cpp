# Strategic improvement plan: Advancing lvgl_cpp

## 1. Executive summary

This document provides a systemic approach to improving `lvgl_cpp` API coverage from **39.6%** to **75%+** while enhancing usability and type safety. Based on analysis of:
- Current API coverage (606/1,529 user-facing functions, 266/423 enums)
- Existing design patterns (CRTP mixins, fluent API, RAII)
- Architectural gaps (Style API, Object utilities, deep widget proxies)

### Key metrics

| Metric | Current | Target |
|--------|---------|--------|
| Function coverage | 39.6% | 75% |
| Enum coverage | 62.9% | 90% |
| Style property coverage | 4.1% | 70% |
| lv_obj coverage | 17.6% | 50% |

---

## 2. Strategic priorities

### Priority matrix

| Priority | Impact | Effort | Category |
|----------|--------|--------|----------|
| **P0** | High | Medium | Style Builder API |
| **P0** | High | Low | Core Object Methods |
| **P1** | High | Medium | Deep Widget Proxies |
| **P1** | Medium | Low | Enum Completion |
| **P2** | Medium | High | Event System Extension |
| **P3** | Low | High | Drawing Primitives |

---

## 3. Proposed improvements

### 3.1. Style builder API (P0)

**Problem**: Only 6/146 `lv_style_*` functions wrapped (4.1% coverage).

**Solution**: Fluent `Style` builder with compile-time property validation.

```cpp
// Current (verbose, no type safety)
lv_style_t style;
lv_style_init(&style);
lv_style_set_bg_color(&style, lv_color_hex(0xFF0000));
lv_style_set_radius(&style, 10);

// Proposed (fluent, type-safe)
auto style = lvgl::Style()
    .bg_color(lvgl::Palette::Red)
    .radius(10)
    .bg_opa(lvgl::Opacity::Cover)
    .border(2, lvgl::Palette::Blue)
    .padding(10);

button.add_style(style, lvgl::Part::Main);
```

**Implementation**:
1. Create `misc/style.h` with fluent setters
2. Group related properties: background, border, padding, text, shadow
3. Use scoped enums for all property values
4. Support `LocalStyle` for temporary modifications

**Coverage impact**: +120 functions (+7.8% total)

---

### 3.2. Core object methods (P0)

**Problem**: Only 89/505 `lv_obj_*` functions wrapped (17.6% coverage).

**High-priority missing functions**:

| Category | Functions | Impact |
|----------|-----------|--------|
| Layout | `set_flex_flow`, `set_flex_align`, `set_grid_dsc_array` | Critical for modern layouts |
| Scroll | `scroll_to`, `scroll_by`, `set_scroll_snap_x/y` | Dynamic content |
| Transform | `set_style_transform_*` | Animations |
| State | `add_state`, `remove_state`, `has_state` | Conditional styling |
| Tree | `get_child`, `get_child_by_id`, `get_sibling` | Navigation |

**Implementation approach**:

```cpp
// Layoutable mixin extension
template <typename Derived>
class Layoutable {
 public:
  Derived& flex_flow(FlexFlow flow);
  Derived& flex_align(FlexAlign main, FlexAlign cross, FlexAlign track);
  Derived& flex_grow(uint8_t grow);
  
  Derived& grid_cell(uint8_t col, uint8_t row);
  Derived& grid_span(uint8_t col_span, uint8_t row_span);
};

// Scrollable mixin extension
template <typename Derived>
class Scrollable {
 public:
  Derived& scroll_to(int32_t x, int32_t y, AnimationType anim = AnimationType::On);
  Derived& scroll_by(int32_t dx, int32_t dy, AnimationType anim = AnimationType::On);
  Derived& scroll_snap(Direction dir, ScrollSnap snap);
};
```

**Coverage impact**: +100 functions (+6.5% total)

---

### 3.3. Deep widget proxies (P1)

**Problem**: Complex widgets require raw pointer manipulation.

#### Chart series proxy

```cpp
class ChartSeries {
 public:
  ChartSeries& set_color(Color color);
  ChartSeries& set_next_value(int32_t value);
  ChartSeries& set_values(std::span<const int32_t> values);
  ChartSeries& hide(bool hide);
  lv_chart_series_t* raw() const;
};

class Chart : public Widget<Chart> {
 public:
  ChartSeries add_series(Color color, Axis axis);
  ChartSeries get_series(size_t index);
  Chart& set_cursor_point(ChartCursor cursor, ChartSeries series, uint32_t point);
};
```

#### Scale section proxy

```cpp
class ScaleSection {
 public:
  ScaleSection& set_range(int32_t min, int32_t max);
  ScaleSection& set_style(Part part, const Style& style);
};

class Scale : public Widget<Scale> {
 public:
  ScaleSection add_section();
};
```

**Coverage impact**: +40 functions (+2.6% total)

---

### 3.4. Enum completion (P1)

**Missing enum categories**:

| Category | Current | Missing Values |
|----------|---------|----------------|
| `LV_ANIM_*` | 6.1% | Path callbacks, playback modes |
| `LV_SCALE_*` | 6.7% | Section modes, label positions |
| `LV_GRAD_*` | 0.0% | Gradient directions |
| `LV_TABLE_*` | 0.0% | Cell control flags |

**Implementation**:

```cpp
// misc/enums.h additions
enum class AnimPath : uint8_t {
  Linear = LV_ANIM_PATH_LINEAR,
  EaseIn = LV_ANIM_PATH_EASE_IN,
  EaseOut = LV_ANIM_PATH_EASE_OUT,
  EaseInOut = LV_ANIM_PATH_EASE_IN_OUT,
  Bounce = LV_ANIM_PATH_BOUNCE,
  Overshoot = LV_ANIM_PATH_OVERSHOOT,
  Step = LV_ANIM_PATH_STEP,
};

enum class GradDir : uint8_t {
  None = LV_GRAD_DIR_NONE,
  Vertical = LV_GRAD_DIR_VER,
  Horizontal = LV_GRAD_DIR_HOR,
};
```

**Coverage impact**: +100 enums (+23.7% enum coverage)

---

### 3.5. Event system extension (P2)

**Problem**: Only 5/41 `lv_event_*` functions wrapped (12.2% coverage).

**Missing capabilities**:
- Custom event codes
- Event bubbling control
- Event parameter extraction

```cpp
// Event class extensions
class Event {
 public:
  // Existing
  EventCode get_code() const;
  Object* get_target() const;
  
  // New
  template<typename T>
  T* get_param() const;
  
  void stop_bubbling();
  void stop_processing();
  
  Point get_indev_point() const;
  Key get_key() const;
  GestureDir get_gesture_dir() const;
};

// Custom event registration
class EventRegistry {
 public:
  static EventCode register_custom(const char* name);
  static void send(Object& target, EventCode code, void* param = nullptr);
};
```

**Coverage impact**: +25 functions (+1.6% total)

---

## 4. Implementation plan

### Phase 1: Foundation (Weeks 1-2)

| Task | Files | Est. Hours |
|------|-------|------------|
| Style builder API | `misc/style.h`, `misc/style.cpp` | 16h |
| Add missing enums | `misc/enums.h` | 4h |
| Unit tests for Style | `tests/test_style.cpp` | 6h |

**Deliverables**:
- Fluent Style API with 60+ property setters
- Complete enum coverage for animations, gradients, tables

### Phase 2: Object expansion (Weeks 3-4)

| Task | Files | Est. Hours |
|------|-------|------------|
| Layoutable mixin extension | `core/mixins/layoutable.h` | 8h |
| Scrollable mixin extension | `core/mixins/scrollable.h` | 6h |
| Object tree navigation | `core/object.h` | 4h |
| Unit tests | `tests/test_layout.cpp` | 6h |

**Deliverables**:
- Full Flex and Grid layout support via mixins
- Scroll control with snap and animation
- Child/parent/sibling navigation

### Phase 3: Widget proxies (Weeks 5-6)

| Task | Files | Est. Hours |
|------|-------|------------|
| ChartSeries proxy | `widgets/chart.h`, `widgets/chart.cpp` | 8h |
| ChartCursor proxy | `widgets/chart.h` | 4h |
| ScaleSection proxy | `widgets/scale.h` | 4h |
| Unit tests | `tests/test_chart_advanced.cpp` | 6h |

**Deliverables**:
- Type-safe series and cursor manipulation
- Section-based styling for Scale widget

### Phase 4: Event system (Week 7)

| Task | Files | Est. Hours |
|------|-------|------------|
| Event parameter extraction | `core/event.h` | 4h |
| Custom event registration | `core/event.h` | 4h |
| Bubble/processing control | `core/event.h` | 2h |
| Unit tests | `tests/test_event_advanced.cpp` | 4h |

**Deliverables**:
- Type-safe event parameter access
- Custom event code registration

### Phase 5: Documentation (Week 8)

| Task | Files | Est. Hours |
|------|-------|------------|
| Update MIGRATION_GUIDE.md | `docs/` | 4h |
| API examples | `examples/` | 6h |
| Update design docs | `design/` | 4h |

---

## 5. Success criteria

### Coverage targets

| Metric | Before | After | Change |
|--------|--------|-------|--------|
| Function coverage | 39.6% | 75%+ | +35% |
| Enum coverage | 62.9% | 90%+ | +27% |
| Style coverage | 4.1% | 70%+ | +66% |
| Object coverage | 17.6% | 50%+ | +32% |

### Quality gates

1. All new code follows Google C++ Style Guide
2. Unit test coverage >80% for new code
3. No memory leaks (validated by ASAN)
4. Zero internal API usage violations
5. Documentation updated for all public APIs

---

## 6. Risk mitigation

| Risk | Impact | Mitigation |
|------|--------|------------|
| LVGL API changes | High | Pin to v9.4, track upstream |
| Style API complexity | Medium | Start with most-used properties |
| Breaking changes | Medium | Deprecation warnings before removal |
| Scope creep | Medium | Strict phase boundaries |

---

## 7. Appendix: Coverage breakdown

### Functions by priority

**P0 (Must have)**:
- `lv_style_set_*` (120 functions)
- `lv_obj_set_flex_*` (8 functions)
- `lv_obj_set_grid_*` (6 functions)
- `lv_obj_scroll_*` (10 functions)

**P1 (Should have)**:
- `lv_chart_*` series methods (26 functions)
- `lv_scale_*` section methods (14 functions)
- `lv_event_*` utilities (25 functions)

**P2 (Nice to have)**:
- `lv_anim_*` advanced (42 functions)
- `lv_indev_*` utilities (46 functions)

### Enums by priority

**P0**: `LV_GRAD_*`, `LV_TABLE_*`, `LV_ANIM_*` paths
**P1**: `LV_SCALE_*`, `LV_MENU_*`, `LV_DROPDOWN_*`
