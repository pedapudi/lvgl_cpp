# Strategic improvement plan: Advancing lvgl_cpp

## 1. Executive summary

This document provides a systemic approach to improving `lvgl_cpp` API coverage from **39.6%** to **75%+** while enhancing usability and type safety. The plan synthesizes analysis from:
- API coverage audit (606/1,529 user-facing functions, 266/423 enums)
- Established design patterns in `lvgl_cpp` (CRTP mixins, fluent API, RAII ownership)
- Architectural gaps identified in previous design documents

### Current state

| Metric | Value | Assessment |
|--------|-------|------------|
| Function coverage | 39.6% (606/1,529) | Core widgets complete, utilities lacking |
| Enum coverage | 62.9% (266/423) | Most event/state enums present |
| Style coverage | 4.1% (6/146) | Critical gap |
| Object coverage | 17.6% (89/505) | Layout and scroll gaps |

### Target state

| Metric | Target | Rationale |
|--------|--------|-----------|
| Function coverage | 75%+ | Cover all documented user-facing APIs |
| Enum coverage | 90%+ | Complete type safety for all constants |
| Style coverage | 70%+ | Fluent styling for common properties |
| Object coverage | 50%+ | Full layout and scroll support |

---

## 2. Priority areas

### 2.1. Style builder API (P0: Critical)

#### Problem statement

The LVGL style system (`lv_style_*`) contains 146 functions, of which only 6 are currently wrapped (4.1%). This forces users to drop down to raw C calls for any non-trivial styling:

```cpp
// Current: Must use raw C API
lv_style_t style;
lv_style_init(&style);
lv_style_set_bg_color(&style, lv_color_hex(0xFF0000));
lv_style_set_bg_opa(&style, LV_OPA_COVER);
lv_style_set_radius(&style, 10);
lv_style_set_border_width(&style, 2);
lv_style_set_border_color(&style, lv_color_hex(0x0000FF));
lv_style_set_pad_all(&style, 10);
lv_obj_add_style(btn.raw(), &style, LV_PART_MAIN);
```

This violates the core design philosophy of `lvgl_cpp`: providing a "Pit of Success" where the idiomatic path is also the safest.

#### Proposed solution

A fluent `Style` class that wraps `lv_style_t` with method chaining:

```cpp
// Proposed: Fluent, type-safe API
auto style = lvgl::Style()
    .bg_color(lvgl::Palette::Red)
    .bg_opa(lvgl::Opacity::Cover)
    .radius(10)
    .border(2, lvgl::Palette::Blue)
    .padding(10);

button.add_style(style, lvgl::Part::Main);
```

#### Implementation breadcrumbs

1. **Create `misc/style.h`** - Header with `Style` class declaration
   - Reference: Existing `misc/enums.h` for enum patterns
   - Reference: `lv_style.h` in LVGL for all `lv_style_set_*` functions

2. **Group setters by category** - For discoverability and documentation:
   ```cpp
   class Style {
    public:
     // === Background ===
     Style& bg_color(Color color);
     Style& bg_opa(Opacity opa);
     Style& bg_grad(GradDir dir, Color start, Color end);
     Style& bg_image(const ImageDescriptor& img);
     
     // === Border ===
     Style& border_color(Color color);
     Style& border_width(int32_t width);
     Style& border_opa(Opacity opa);
     Style& border_side(BorderSide side);
     
     // === Padding ===
     Style& pad_all(int32_t pad);
     Style& pad_hor(int32_t pad);
     Style& pad_ver(int32_t pad);
     Style& pad_left(int32_t left);
     // ... etc
     
     // === Text ===
     Style& text_color(Color color);
     Style& text_font(const Font& font);
     Style& text_align(TextAlign align);
     Style& text_letter_space(int32_t space);
     
     // === Shadow ===
     Style& shadow(int32_t width, Color color);
     Style& shadow_offset(int32_t x, int32_t y);
     
     // === Raw access ===
     lv_style_t* raw();
     const lv_style_t* raw() const;
   };
   ```

3. **Implement in `misc/style.cpp`** - Each method wraps one `lv_style_set_*` call:
   ```cpp
   Style& Style::bg_color(Color color) {
     lv_style_set_bg_color(&style_, color.raw());
     return *this;
   }
   ```

4. **Add missing enums to `misc/enums.h`**:
   - `GradDir` (gradient direction)
   - `BorderSide` (which borders to render)
   - `TextDecor` (underline, strikethrough)

5. **Update `Object` and `Widget<T>`** to accept `Style`:
   ```cpp
   // In core/mixins/stylable.h
   Derived& add_style(const Style& style, Part part = Part::Main);
   Derived& remove_style(const Style& style, Part part = Part::Main);
   ```

6. **Create unit tests** in `tests/test_style.cpp`:
   - Test fluent chaining
   - Test all property categories
   - Verify memory safety (Style owns lv_style_t)

#### Why this approach

| Consideration | Decision | Rationale |
|--------------|----------|-----------|
| Ownership | `Style` owns `lv_style_t` | RAII pattern consistent with rest of library |
| Method grouping | By visual category | IDE autocomplete shows related properties together |
| Return type | `Style&` for chaining | Matches fluent API pattern used elsewhere |
| Enum usage | Scoped enums for values | Type safety, no namespace pollution |

#### Alternatives considered

**Alternative A: Property proxies**
```cpp
button.style(Part::Main).bg_color(Color::Red);
```
- **Rejected**: Requires storing style state in widget, increases memory overhead
- **Intent**: Zero per-widget overhead is a core constraint

**Alternative B: Style templates**
```cpp
auto redButton = StyleTemplate::RedButton();
```
- **Not rejected, but separate**: Useful as convenience layer on top of Style builder
- **Implementation**: Add as `styles/templates.h` after core Style is complete

**Alternative C: CSS-like string parsing**
```cpp
button.set_style("background: red; border: 2px blue;");
```
- **Rejected**: Runtime parsing overhead, no compile-time type checking
- **Intent**: Compile-time safety is higher priority than familiar syntax

#### Coverage impact

- Functions: +80-100 style setters → +5-7% total coverage
- Enums: +40 constants (gradients, borders, text) → +9% enum coverage

---

### 2.2. Core object methods (P0: Critical)

#### Problem statement

The `lv_obj_*` API surface contains 505 functions, of which only 89 are wrapped (17.6%). Critical gaps include:

| Category | Examples | Impact |
|----------|----------|--------|
| Flex layout | `set_flex_flow`, `set_flex_grow` | Cannot use Flex without raw C |
| Grid layout | `set_grid_cell`, `set_grid_dsc` | Cannot use Grid without raw C |
| Scrolling | `scroll_to`, `set_scroll_snap` | No programmatic scroll control |
| State | `add_state`, `has_state` | Manual flag management |
| Tree navigation | `get_child`, `get_child_cnt` | Must track children manually |

#### Proposed solution

Extend existing CRTP mixins to cover these APIs:

```cpp
// Expanded Layoutable mixin
template <typename Derived>
class Layoutable {
 public:
  // === Flex ===
  Derived& flex_flow(FlexFlow flow);
  Derived& flex_align(FlexAlign main, FlexAlign cross, FlexAlign track);
  Derived& flex_grow(uint8_t grow);
  
  // === Grid ===
  Derived& grid_cell(uint8_t col, uint8_t row);
  Derived& grid_cell(uint8_t col, uint8_t row, GridAlign col_align, GridAlign row_align);
  Derived& grid_span(uint8_t col_span, uint8_t row_span);
  
  // === General ===
  Derived& layout(LayoutType type);
  void update_layout();
};

// Expanded Scrollable mixin
template <typename Derived>
class Scrollable {
 public:
  Derived& scroll_to(int32_t x, int32_t y, AnimEnable anim = AnimEnable::On);
  Derived& scroll_to_view(AnimEnable anim = AnimEnable::On);
  Derived& scroll_by(int32_t dx, int32_t dy, AnimEnable anim = AnimEnable::On);
  Derived& scroll_snap(Direction dir, ScrollSnap snap);
  
  Point get_scroll_pos() const;
  Area get_scroll_area() const;
};

// State management (new methods in Object or Stylable)
Derived& add_state(State state);
Derived& remove_state(State state);
bool has_state(State state) const;

// Tree navigation (new methods in Object)
Object get_parent() const;
Object get_child(int32_t index) const;
size_t get_child_count() const;
Object get_sibling(int32_t offset) const;
```

#### Implementation breadcrumbs

1. **Expand `core/mixins/layoutable.h`**:
   - Add Flex methods wrapping `lv_obj_set_flex_*`
   - Add Grid methods wrapping `lv_obj_set_grid_*`
   - Reference: LVGL's `lv_obj_style_gen.h` for layout-related functions

2. **Expand `core/mixins/scrollable.h`**:
   - Add scroll control wrapping `lv_obj_scroll_*`
   - Reference: LVGL's `lv_obj_scroll.h`

3. **Add enums to `misc/enums.h`**:
   ```cpp
   enum class FlexFlow : uint8_t {
     Row = LV_FLEX_FLOW_ROW,
     Column = LV_FLEX_FLOW_COLUMN,
     RowWrap = LV_FLEX_FLOW_ROW_WRAP,
     ColumnWrap = LV_FLEX_FLOW_COLUMN_WRAP,
     RowReverse = LV_FLEX_FLOW_ROW_REVERSE,
     ColumnReverse = LV_FLEX_FLOW_COLUMN_REVERSE,
   };
   
   enum class FlexAlign : uint8_t {
     Start = LV_FLEX_ALIGN_START,
     End = LV_FLEX_ALIGN_END,
     Center = LV_FLEX_ALIGN_CENTER,
     SpaceEvenly = LV_FLEX_ALIGN_SPACE_EVENLY,
     SpaceAround = LV_FLEX_ALIGN_SPACE_AROUND,
     SpaceBetween = LV_FLEX_ALIGN_SPACE_BETWEEN,
   };
   
   enum class ScrollSnap : uint8_t {
     None = LV_SCROLL_SNAP_NONE,
     Start = LV_SCROLL_SNAP_START,
     End = LV_SCROLL_SNAP_END,
     Center = LV_SCROLL_SNAP_CENTER,
   };
   ```

4. **Add state methods to `core/object.h` or `core/mixins/stylable.h`**:
   ```cpp
   Object& Object::add_state(State state) {
     lv_obj_add_state(obj_, static_cast<lv_state_t>(state));
     return *this;
   }
   ```

5. **Add tree navigation to `core/object.h`**:
   ```cpp
   Object Object::get_child(int32_t index) const {
     lv_obj_t* child = lv_obj_get_child(obj_, index);
     return Object(child, false);  // Unmanaged wrapper
   }
   ```
   - **Note**: Return unmanaged `Object` to avoid ownership confusion

6. **Create tests** in `tests/test_layout.cpp` and `tests/test_scroll.cpp`

#### Why this approach

| Consideration | Decision | Rationale |
|--------------|----------|-----------|
| Mixin placement | Extend existing | Maintains CRTP architecture, no new classes |
| State API | Methods not operator overloads | Explicit is better than implicit |
| Tree navigation | Return unmanaged Object | Prevents double-free, matches C API semantics |
| Scroll animation | Default to animated | Matches LVGL default behavior |

#### Alternatives considered

**Alternative A: Separate Layout class**
```cpp
auto layout = Flex(button).flow(FlexFlow::Row).grow(1);
```
- **Rejected**: Adds unnecessary object, increases complexity
- **Intent**: Layout is a property of the object, not a separate entity

**Alternative B: CSS-like layout strings**
```cpp
button.set_layout("display: flex; flex-direction: row;");
```
- **Rejected**: Runtime parsing, no compile-time checking
- **Intent**: Type-safe API is higher priority

#### Coverage impact

- Functions: +80-100 object methods → +5-7% total coverage
- Enums: +20 constants (flex, grid, scroll) → +5% enum coverage

---

### 2.3. Deep widget proxies (P1: Important)

#### Problem statement

Complex widgets require managing opaque handles directly:

```cpp
// Current: Raw pointer management
lv_chart_series_t* ser = lv_chart_add_series(chart.raw(), lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
lv_chart_set_next_value(chart.raw(), ser, 50);
lv_chart_set_series_color(chart.raw(), ser, lv_palette_main(LV_PALETTE_BLUE));
```

This exposes users to:
- Use-after-free if series is removed
- Double-free if series handle is mismanaged
- No IDE autocomplete for series-specific methods

#### Proposed solution: Chart series proxy

```cpp
// widgets/chart.h
class ChartSeries {
 public:
  ChartSeries(Chart* chart, lv_chart_series_t* series);
  
  // Fluent setters
  ChartSeries& set_color(Color color);
  ChartSeries& set_next_value(int32_t value);
  ChartSeries& set_all_values(int32_t value);
  ChartSeries& set_values(std::span<const int32_t> values);
  ChartSeries& hide(bool hidden = true);
  
  // Getters
  int32_t get_value(uint32_t index) const;
  uint32_t get_point_count() const;
  
  // Raw access
  lv_chart_series_t* raw() const;
  
 private:
  Chart* chart_;
  lv_chart_series_t* series_;
};

class Chart : public Widget<Chart> {
 public:
  // Factory methods
  ChartSeries add_series(Color color, Axis axis = Axis::PrimaryY);
  
  // Access
  ChartSeries get_series(size_t index);
  size_t get_series_count() const;
  
  // Remove (invalidates ChartSeries handles)
  void remove_series(ChartSeries& series);
  void remove_all_series();
};
```

#### Implementation breadcrumbs

1. **Define `ChartSeries` in `widgets/chart.h`**:
   - Store pointer to parent `Chart` and raw `lv_chart_series_t*`
   - No ownership - Chart owns the series

2. **Implement in `widgets/chart.cpp`**:
   ```cpp
   ChartSeries& ChartSeries::set_color(Color color) {
     lv_chart_set_series_color(chart_->raw(), series_, color.raw());
     return *this;
   }
   
   ChartSeries& ChartSeries::set_next_value(int32_t value) {
     lv_chart_set_next_value(chart_->raw(), series_, value);
     return *this;
   }
   ```

3. **Add factory method to Chart**:
   ```cpp
   ChartSeries Chart::add_series(Color color, Axis axis) {
     lv_chart_series_t* ser = lv_chart_add_series(
       raw(), color.raw(), static_cast<lv_chart_axis_t>(axis));
     return ChartSeries(this, ser);
   }
   ```

4. **Create similar proxy for Scale sections**:
   ```cpp
   class ScaleSection {
    public:
     ScaleSection& set_range(int32_t min, int32_t max);
     ScaleSection& set_style(Part part, const Style& style);
   };
   ```

5. **Add tests** in `tests/test_chart_series.cpp`

#### Why this approach

| Consideration | Decision | Rationale |
|--------------|----------|-----------|
| Ownership | Chart owns series | Matches LVGL semantics exactly |
| Proxy lifetime | Not managed | User must not use after removal |
| Parent reference | Store Chart* | Needed to call lv_chart_* functions |
| Factory pattern | `add_series` returns proxy | Fluent, type-safe creation |

#### Alternatives considered

**Alternative A: RAII series wrapper with ref-counting**
```cpp
auto series = chart.create_series(...);  // shared_ptr internally
```
- **Rejected**: LVGL doesn't support ref-counting on series
- **Issue**: No way to prevent LVGL from freeing series when chart is deleted

**Alternative B: Index-based access only**
```cpp
chart.set_series_color(0, Color::Red);
```
- **Rejected**: Less discoverable, no method grouping
- **Intent**: Proxy provides semantic grouping of series operations

**Alternative C: Copying series data into C++ container**
```cpp
class ChartSeries {
  std::vector<int32_t> cached_values_;
};
```
- **Rejected**: Memory overhead, sync complexity
- **Intent**: Keep C++ wrapper stateless

#### Coverage impact

- Functions: +30-40 chart/scale methods → +2-3% total coverage

---

### 2.4. Enum design taxonomy (P1: Important)

#### Problem statement

Several enum categories have low coverage, forcing users to use raw `LV_*` constants:

| Category | Current Coverage | Issue |
|----------|-----------------|-------|
| `LV_ANIM_*` | 6.1% (2/33) | Animation paths missing |
| `LV_SCALE_*` | 6.7% (1/15) | Scale modes missing |
| `LV_GRAD_*` | 0.0% (0/15) | No gradient support |
| `LV_TABLE_*` | 0.0% (0/9) | No cell control flags |
| `LV_MENU_*` | 0.0% (0/6) | No menu modes |

The original proposal to consolidate all enums in `misc/enums.h` conflicts with the design intent of placing constants close to their usage.

#### Revised design: Enum taxonomy

Enums should be categorized and placed according to their semantic scope:

| Category | Placement | Examples |
|----------|-----------|----------|
| **Global** | `misc/enums.h` | Align, State, Part, Dir, Key, EventCode |
| **Widget-scoped** | Inside widget class | Arc::Mode, Chart::Type, Label::LongMode |
| **Factory pattern** | Struct with static methods | Animation::Path, Animation::Exec |

##### Decision criteria

1. **Global enums** (`misc/enums.h`): Constants used across multiple widgets or subsystems
   - Example: `lvgl::Align::Center` (used by all positioning)
   - Example: `lvgl::Part::Main` (used by styling on any widget)

2. **Widget-scoped enums**: Constants specific to one widget's behavior
   - **Current pattern (deprecated)**: `lvgl::ArcMode::Normal`
   - **Proposed pattern**: `lvgl::Arc::Mode::Normal`

3. **Factory pattern**: When the "constant" is actually a callback or complex object
   - **Example**: `lvgl::Animation::Path::EaseOut()` returns `PathCallback`
   - **Rationale**: These are not simple enum values but factory methods

#### Proposed solution: Widget-scoped enums

Migrate widget-specific enums from `misc/enums.h` into their respective widget classes:

```cpp
// BEFORE (misc/enums.h)
enum class ArcMode : uint8_t {
  Normal = LV_ARC_MODE_NORMAL,
  // ...
};

// AFTER (widgets/arc.h)
class Arc : public Widget<Arc> {
 public:
  enum class Mode : uint8_t {
    Normal = LV_ARC_MODE_NORMAL,
    Symmetrical = LV_ARC_MODE_SYMMETRICAL,
    Reverse = LV_ARC_MODE_REVERSE,
  };
  
  Arc& set_mode(Mode mode);
  Mode get_mode() const;
  
  // ...
};

// Usage
arc.set_mode(lvgl::Arc::Mode::Normal);
```

##### Widgets to refactor

| Widget | Current Enum | Proposed Scoped Enum |
|--------|--------------|----------------------|
| `Arc` | `ArcMode` | `Arc::Mode` |
| `Bar` | `BarMode` | `Bar::Mode` |
| `Slider` | `SliderMode` | `Slider::Mode` |
| `Label` | `LabelLongMode` | `Label::LongMode` |
| `Keyboard` | `KeyboardMode` | `Keyboard::Mode` |
| `Chart` | `ChartType`, `ChartAxis`, `ChartUpdateMode` | `Chart::Type`, `Chart::Axis`, `Chart::UpdateMode` |
| `Roller` | `RollerMode` | `Roller::Mode` |
| `Table` | (new) | `Table::CellCtrl` |
| `Scale` | (new) | `Scale::Mode` |
| `Menu` | (new) | `Menu::Mode`, `Menu::HeaderMode` |

#### Proposed solution: Animation constants

Animation paths and exec callbacks use the **factory pattern** (already implemented):

```cpp
// Already exists in misc/animation.h
class Animation {
 public:
  struct Path {
    static PathCallback Linear();
    static PathCallback EaseIn();
    static PathCallback EaseOut();
    static PathCallback EaseInOut();
    static PathCallback Overshoot();
    static PathCallback Bounce();
    static PathCallback Step();
  };
  
  struct Exec {
    static ExecCallback X();
    static ExecCallback Y();
    static ExecCallback Width();
    static ExecCallback Height();
    static ExecCallback Opacity();
  };
};

// Usage
anim.set_path_cb(lvgl::Animation::Path::EaseOut());
```

**No changes needed** for Animation. The factory pattern is preferred because:
1. It returns typed callbacks, not raw enum values
2. It encapsulates the `lv_anim_path_*` function pointers
3. IDE autocomplete shows available options contextually

#### Implementation breadcrumbs

1. **Phase A: Refactor existing widget enums** (non-breaking with aliases)
   ```cpp
   // In widgets/arc.h
   class Arc : public Widget<Arc> {
    public:
     enum class Mode : uint8_t { ... };
   };
   
   // In misc/enums.h (deprecated alias)
   [[deprecated("Use Arc::Mode instead")]]
   using ArcMode = Arc::Mode;
   ```

2. **Phase B: Add new widget-scoped enums**
   - `Table::CellCtrl`
   - `Scale::Mode`
   - `Menu::Mode`
   - `Dropdown::Dir`

3. **Phase C: Add global enums to misc/enums.h**
   - `GradDir` (used by Style for gradients)
   - `TextDecor` (used by Style for text)

4. **Update audit script** to detect class-scoped enums:
   ```python
   # Patterns to match:
   # - lvgl::Arc::Mode::Normal
   # - lvgl::Animation::Path::EaseOut
   ```

#### Why this approach

| Consideration | Decision | Rationale |
|--------------|----------|-----------|
| Widget modes | Class-scoped | Discoverability: `arc.set_mode(Arc::` triggers IDE completion |
| Global alignment | Namespace-level | Used by all widgets, no single owner |
| Callbacks | Factory pattern | Type safety, encapsulates C function pointers |
| Migration | Deprecation aliases | Non-breaking for existing code |

#### Alternatives considered

**Alternative A: All enums in misc/enums.h (original proposal)**
- **Rejected**: Poor discoverability, doesn't scale
- **Conflict**: `lvgl::Animation::Path::EaseOut()` already violates this pattern

**Alternative B: Separate enum files per widget**
```
widgets/arc_enums.h
widgets/chart_enums.h
```
- **Rejected**: Increases file count, breaks convention of single header per widget

**Alternative C: Nested namespaces**
```cpp
namespace lvgl::arc { enum class Mode : uint8_t { ... }; }
```
- **Rejected**: Inconsistent with class-based widget design

#### Coverage impact

- Widget enums: Migrate existing 10 enums to class-scoped
- New enums: +30-40 values (Table, Scale, Menu, Dropdown)
- Global enums: +10-15 values (gradients, text decorations)

---

### 2.5. Event system extension (P2: Nice to have)

#### Problem statement

Only 5/41 `lv_event_*` functions are wrapped (12.2%). Missing capabilities:

- Custom event code registration
- Event parameter extraction (key, gesture, etc.)
- Bubble/processing control

#### Proposed solution

```cpp
// Extended Event class
class Event {
 public:
  // Existing
  EventCode get_code() const;
  Object* get_target() const;
  void* get_user_data() const;
  
  // New: Typed parameter extraction
  template<typename T>
  T* get_param() const {
    return static_cast<T*>(lv_event_get_param(raw()));
  }
  
  Key get_key() const;
  Point get_indev_point() const;
  GestureDir get_gesture_dir() const;
  
  // New: Flow control
  void stop_bubbling();
  void stop_processing();
  
  // Raw
  lv_event_t* raw() const;
};

// Custom event registration
class EventRegistry {
 public:
  // Register a custom event code
  static EventCode register_custom(const char* name = nullptr);
  
  // Send custom event
  static void send(Object& target, EventCode code, void* param = nullptr);
  static void send(lv_obj_t* target, EventCode code, void* param = nullptr);
};
```

#### Implementation breadcrumbs

1. **Extend `core/event.h`** with new methods
2. **Add typed parameter getters**:
   ```cpp
   Key Event::get_key() const {
     return static_cast<Key>(lv_event_get_key(raw()));
   }
   ```
3. **Implement EventRegistry** as static methods
4. **Add tests** for custom events and parameter extraction

#### Why this approach

| Consideration | Decision | Rationale |
|--------------|----------|-----------|
| Custom events | Static registry | No per-object overhead |
| Parameters | Template getter | Flexible for any type |
| Flow control | Methods not return value | Explicit action |

#### Coverage impact

- Functions: +20-25 event methods → +1-2% total coverage

---

### 2.6. Core system enhancements (P1: Important)

#### Problem statement

While widgets are well-covered, the core system APIs that glue the UI together have significant gaps, limiting the ability to build advanced interaction models:

| Subsystem | Coverage | Key Missing Capabilities |
|-----------|----------|--------------------------|
| **Input Devices (`lv_indev_`)** | 31.3% | Gesture limits, scroll throw/inertia, cursor management |
| **Groups (`lv_group_`)** | 48.1% | Focus/Edge callbacks, wrap policies, search |
| **Animations (`lv_anim_`)** | 39.1% | Advanced duration/speed conversion, custom paths |

#### Proposed solution

Broaden the `Display`, `InputDevice`, and `Group` classes to cover the full functional surface of LVGL v9, prioritizing interaction logic and accessibility.

```cpp
// indev.h extension
class InputDevice {
 public:
  InputDevice& set_scroll_throw(uint8_t throw_v);
  InputDevice& set_gesture_limit(uint8_t limit);
  InputDevice& set_gesture_min_velocity(uint8_t velocity);
  InputDevice& set_cursor(const Object& obj);
  
  Point get_point() const;
  Key get_key() const;
  Object get_target() const;
};

// group.h extension
class Group {
 public:
  using FocusCallback = std::function<void(Group&, Object&, Object&)>;
  
  Group& set_focus_cb(FocusCallback cb);
  Group& set_wrap(bool en);
  Group& set_refocus_policy(bool en);
  
  void focus_next();
  void focus_prev();
  void focus_obj(Object& obj);
  
  Object get_focused() const;
  size_t get_count() const;
};
```

#### Implementation breadcrumbs

1. **Expand `indev/input_device.h`**:
   - Add setters for scroll/gesture limits wrapping `lv_indev_set_*`
   - Implement getters for current interaction state
   - Reference: `lv_indev.h` in LVGL

2. **Expand `core/group.h`**:
   - Add focus management methods wrapping `lv_group_*`
   - Implement C++ callback bridge for focus events (mirroring the `EventSource` pattern)
   - Reference: `lv_group.h` in LVGL

3. **Enhance `display/display.h`**:
   - Add display-level event handling
   - Wrap screen loading with advanced transition parameters
   - Reference: `lv_display.h` in LVGL

#### Why this approach

| Consideration | Decision | Rationale |
|--------------|----------|-----------|
| Accessibility | Complete Group API | Essential for keyboard/encoder-only navigation |
| Performance | Inlined getters | Zero cost access to input state (X/Y, key) |
| Safety | Unmanaged Object returns | Core handles (Parent, Screen) should not be deleted by wrapper |

#### Alternatives considered

**Alternative A: Global input state hooks**
```cpp
auto point = Input::get_active_point();
```
- **Rejected**: LVGL supports multiple input devices; global state is ambiguous
- **Intent**: Stick to the object-oriented `InputDevice` instance model

**Alternative B: Group indices instead of Objects**
```cpp
group.focus(5);
```
- **Rejected**: Brittle, doesn't handle dynamic UI well
- **Intent**: Use typed `Object` references for focus control

#### Coverage impact

- Functions: +40-50 system methods → +3-4% total coverage

---

## 3. Implementation phases

### Phase 1: Style API and enum foundation

**Goal**: Enable fluent styling without raw C calls.

**Tasks**:
1. Create `misc/style.h` and `misc/style.cpp`
2. Implement property setters grouped by category
3. Add missing enums (gradients, borders, text decorations)
4. Update `Stylable` mixin to accept `Style`
5. Create comprehensive tests

**Dependencies**: None

**Verification**:
- All style tests pass
- No raw `lv_style_*` calls needed for common styling

---

### Phase 2: Core object expansion

**Goal**: Full layout and scroll control.

**Tasks**:
1. Extend `Layoutable` mixin with Flex and Grid methods
2. Extend `Scrollable` mixin with scroll control
3. Add state management methods
4. Add tree navigation methods
5. Create layout and scroll tests

**Dependencies**: Phase 1 (for scroll-related styling)

**Verification**:
- Flex and Grid layouts work without raw C
- Programmatic scrolling works

---

### Phase 3: Widget proxies

**Goal**: Type-safe manipulation of complex widget internals.

**Tasks**:
1. Implement `ChartSeries` proxy
2. Implement `ChartCursor` proxy
3. Implement `ScaleSection` proxy
4. Update Chart and Scale classes with factory methods
5. Create proxy tests

**Dependencies**: None (can run in parallel with Phases 1-2)

**Verification**:
- Chart series can be created and modified fluently
- Scale sections can be styled without raw pointers

---

### Phase 4: Core systems and events

**Goal**: Complete interaction model and custom event handling.

**Tasks**:
1. Extend `InputDevice` with scroll and gesture controls
2. Extend `Group` with focus management and callbacks
3. Extend `Event` class with parameter extraction
4. Add `EventRegistry` for custom events
5. Update MIGRATION_GUIDE.md and design docs

**Dependencies**: Phases 1-3 complete

**Verification**:
- Keyboard/Encoder navigation works fluously
- Custom gestures and events validated in integration tests

---

## 4. Success criteria

### Coverage targets

| Metric | Before | After | Method |
|--------|--------|-------|--------|
| Function coverage | 39.6% | 75%+ | `scripts/audit_api_coverage.py` |
| Enum coverage | 62.9% | 90%+ | Same script |
| Style coverage | 4.1% | 70%+ | Manual audit |
| Object coverage | 17.6% | 50%+ | Same script |

### Quality gates

1. **Style compliance**: Google C++ Style Guide
2. **Test coverage**: >80% for new code
3. **Memory safety**: Validated by AddressSanitizer
4. **API compliance**: Zero internal API usage
5. **Documentation**: All public APIs documented

---

## 5. Appendix: Function coverage by priority

### P0: Must implement

| Subsystem | Count | Key Functions |
|-----------|-------|---------------|
| `lv_style_set_*` | ~80 | All background, border, padding, text properties |
| `lv_obj_set_flex_*` | 8 | flow, align, grow |
| `lv_obj_set_grid_*` | 6 | cell, span, align |
| `lv_obj_scroll_*` | 10 | to, by, snap |
| `lv_obj_*_state` | 4 | add, remove, has, get |

### P1: Should implement

| Subsystem | Count | Key Functions |
|-----------|-------|---------------|
| `lv_chart_*` series | 20 | add, remove, set_color, set_values |
| `lv_scale_section_*` | 8 | add, set_range, set_style |
| `lv_event_*` | 15 | get_param, stop_bubbling, send |

### P2: Nice to have

| Subsystem | Count | Key Functions |
|-----------|-------|---------------|
| `lv_anim_*` advanced | 30 | paths, callbacks, timeline integration |
| `lv_indev_*` utilities | 20 | gesture, scroll inertia |
