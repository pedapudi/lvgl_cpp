# Design: Incremental API Coverage and Consistency

## 1. Executive Summary

`lvgl_cpp` already possesses a robust foundation with Type-Safe Enums (`misc/enums.h`) and a Fluent Style System (`Style` / `StyleProxy`). However, audit data indicates ~50% API coverage.

This plan focuses on **incremental improvements** to fill specific gaps in Type Safety, Layout Management, and Object introspection, building directly upon the existing architecture.

## 2. Gap Analysis & Strategy

### 2.1. Existing Foundation (Do Not Reinvent)
*   **Enums**: `Align`, `Dir`, `State`, `Part`, `Color` etc. are already well-defined in `misc/enums.h`.
*   **Styles**: `Style` (reusable) and `StyleProxy` (local) already provide type-safe setters (e.g., `bg_color(Color)`).

### 2.2. Identified Gaps

#### Gap 1: Layout Descriptors (Type Safety)
*   **Current State**: Flex/Grid layouts often require raw C arrays (e.g. `lv_anim_timeline_wrapper_t`) or raw `int32_t*` for Grid definitions.
*   **Plan**: Implement `FlexLayout` and `GridLayout` helper classes.
    *   `GridLayout::Builder` to construct the `int32_t*` column/row descriptors using C++ constants.
    *   Wrap `LV_GRID_TEMPLATE_*` macros into a `GridTemplate` class/enum.

#### Gap 2: Global Constants vs Enums (Reconciliation Support)
*   **Problem**: `misc/enums.h` contains strict `enum class` wrappers. However, LVGL also uses macro-based "constants" (like `LV_SIZE_CONTENT` or `LV_PCT`) which are not enums.
*   **Plan**: Establish clear separation of concerns:
    *   `misc/enums.h`: Strictly for `enum class` definitions that map to `lv_<name>_t` enums.
    *   `misc/constants.h`: A new header for static constant helpers and magic values.
        *   `lvgl::Size::Content` (wraps `LV_SIZE_CONTENT`)
        *   `lvgl::Size::Percent(x)` (wraps `LV_PCT(x)`)
        *   `lvgl::Radius::Circle` (wraps `LV_RADIUS_CIRCLE`)

#### Gap 3: Object Introspection & Type Safety
*   **Problem**: `lv_obj_check_type` is useful, but putting `is<T>()` type-checking logic inside `Object` creates circular dependency issues (where `Object` needs to know about every derived widget type).
*   **Plan**: Implement **Type Traits / Cast** header (`core/cast.h`) with free functions:
    *   `lvgl::is<T>(obj)`: Free function to check if an object is of type `T` (wraps `lv_obj_check_type`).
    *   `lvgl::cast<T>(obj)`: Safe downcast (returns `T*` or `nullptr`).
    *   `WidgetClass` wrapper: Encapsulates `lv_obj_class_t` metadata, separate from `Object`.

### 2.3. Architecture: Enums vs Constants (Reconciliation)
To resolve the mix of macros and enums in LVGL, we define this strict separation:
*   **`misc/enums.h`**:
    *   **Content**: Only `enum class` definitions that map 1:1 to C-style enums (e.g., `lv_align_t` -> `Align`).
    *   **Rule**: Must be distinct types.
*   **`misc/constants.h`**:
    *   **Content**: `constexpr` values and static helper classes for pseudo-constants (macros).
    *   **Rule**: Use namespaces to group them.
    *   *Examples*:
        *   `lvgl::Size::Content` (was `LV_SIZE_CONTENT`)
        *   `lvgl::Size::Percent(x)` (was `LV_PCT(x)`)
        *   `lvgl::Radius::Circle` (was `LV_RADIUS_CIRCLE`)
        *   `lvgl::Grid::Template::Content` (was `LV_GRID_TEMPLATE_CONTENT`)

#### Gap 4: Style Getters
*   **Current State**: `StyleProxy` allows *setting* styles fluently, but reading them requires `lv_obj_get_style_prop` with raw IDs.
*   **Plan**: Add `get_style_<prop>()` wrappers or a generic type-safe accessor to `Object`.

## 3. Implementation Phases

### Phase 6: Core Consistency (Immediate)
*   **Constants**: Implement `misc/constants.h` (`Size::Content`, `Grid::Template`).
*   **Type Checks**: Implement free functions `lvgl::is<T>()` and `lvgl::cast<T>()` in `core/cast.h`.
*   **Enums**: Add any missing core enums (like `GridTemplate`).

### Phase 7: Layout Helpers (High Value)
*   **GridLayout**: Create a builder class to replace `static int32_t col_dsc[] = {...}`.
    ```cpp
    auto grid = GridLayout()
        .add_column(Grid::Template::Max)
        .add_column(100)
        .add_row(Grid::Template::Content);
    obj.set_grid_dsc_array(grid);
    ```

### Phase 8: Coverage Sweep
*   **Manual Audit**: Systematically wrap the remaining "Low Hanging Fruit" functions identified in the `lv_obj` audit.

## 4. Design Decisions

### 4.1. C compatibility
We retain the C-type overloads in `Style` (e.g. `bg_color(lv_color_t)`) for interoperability, but strictly promote the C++ versions in documentation/examples.

### 4.2. No "User Data" Wrapper
We explicitly **do not** wrap `lv_obj_set_user_data`. C++ users should use inheritance (`class MyWidget : public lvgl::Button`) or capture state in `std::function` callbacks, rather than relying on `void*` pointers.
