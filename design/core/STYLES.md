# Style System Design

## 1. Overview
The Style System focuses on replacing raw enums and untyped arrays with strongly typed, readable builders.

## 2. Style Builder
A fluent builder for `lv_style_t`.

```cpp
class Style {
public:
    Style(); // inits lv_style_t
    
    Style& bg_color(Color c);
    Style& bg_opa(Opacity o);
    Style& border_width(int32_t w);
    Style& border_color(Color c);
    Style& radius(int32_t r);
    
    // ... all 100+ properties
    
    const lv_style_t* raw() const;
};
```

## 3. Local Styles
Helpers for setting local styles on objects without creating a separate `lv_style_t`.

```cpp
// Returns a specialized proxy
auto style_proxy = obj.style(); 
style_proxy.bg_color(Palette::Red);
```
