# Layout system design

## 1. Overview
The Layouts design focuses on declaration helpers for Flex and Grid layouts.

## 2. Flex layout
```cpp
struct Flex {
    static const Flex Row() { return Flex(LV_FLEX_FLOW_ROW); }
    static const Flex Column() { return Flex(LV_FLEX_FLOW_COLUMN); }
    
    Flex& gap(int32_t row_gap, int32_t col_gap);
    Flex& align(lv_flex_align_t main, lv_flex_align_t cross, lv_flex_align_t track);
};

// Usage
obj.layout(Flex::Row().gap(10).center());
```

## 3. Grid layout
Grids require static arrays for track descriptions. This is tricky in C++.

**Proposal**: A helper that manages `static` storage for simple grids or wraps `std::vector` for dynamic ones (aware of lifetime issues).

```cpp
// Static Grid Helper
struct Grid {
    template <size_t C, size_t R>
    static void apply(Object& obj, const int32_t (&cols)[C], const int32_t (&rows)[R]);
};
```
