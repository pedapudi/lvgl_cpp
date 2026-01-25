# Systemic Proxy Migration Matrix

## 1. Style Domain
**Goal**: Move all styling to `StyleProxy`.
**Target Class**: `core/style_proxy.h`

| Legacy Method (Object/Stylable) | Proxy Equivalent | Notes |
| :--- | :--- | :--- |
| `set_style_bg_color` | `style(part).bg_color(color)` | |
| `set_style_bg_opa` | `style(part).bg_opa(opa)` | |
| `set_style_bg_image_src` | `style(part).bg_image_src(src)` | |
| `set_style_border_width` | `style(part).border_width(w)` | |
| `set_style_border_color` | `style(part).border_color(c)` | |
| `set_style_border_side` | `style(part).border_side(s)` | |
| `set_style_outline_width` | `style(part).outline_width(w)` | |
| `set_style_outline_color` | `style(part).outline_color(c)` | |
| `set_style_outline_pad` | `style(part).outline_pad(p)` | |
| `set_style_text_color` | `style(part).text_color(c)` | |
| `set_style_text_align` | `style(part).text_align(a)` | |
| `set_style_text_font` | `style(part).text_font(f)` | |
| `set_style_text_opa` | `style(part).text_opa(o)` | |
| `set_style_radius` | `style(part).radius(r)` | |
| `set_style_pad_all` | `style(part).pad_all(p)` | |
| `set_style_pad_hor` | `style(part).pad_hor(p)` | |
| `set_style_pad_ver` | `style(part).pad_ver(p)` | |
| `set_style_pad_gap` | `style(part).pad_gap(p)` | |
| `set_style_width` | `style(part).width(w)` | Explicit size override |
| `set_style_height` | `style(part).height(h)` | Explicit size override |

## 2. Layout Domain
**Goal**: Move all flex/grid logic to `LayoutProxy`.
**Target Class**: `core/layout_proxy.h`

| Legacy Method (Object/Layoutable) | Proxy Equivalent | Notes |
| :--- | :--- | :--- |
| `set_flex_flow` | `layout().flex_flow(f)` | |
| `set_flex_align` | `layout().flex_align(...)` | |
| `set_flex_grow` | `layout().flex_grow(g)` | |
| `set_grid_dsc_array` | `layout().grid_dsc(...)` | |
| `set_grid_align` | `layout().grid_align(...)` | |
| `set_grid_cell` | `layout().grid_cell(...)` | |
| `update_layout` | `update_layout()` | **Keep on Object** (Frequent) |

## 3. Scroll Domain
**Goal**: Move scrolling logic to `ScrollProxy`.
**Target Class**: `core/scroll_proxy.h`

| Legacy Method (Object) | Proxy Equivalent | Notes |
| :--- | :--- | :--- |
| `set_scrollbar_mode` | `scroll().mode(m)` | |
| `set_scroll_snap_x/y` | `scroll().snap_x/y(s)` | |
| `scroll_to`, `scroll_by` | `scroll().to(...)` | |
| `scroll_to_view` | `scroll().to_view(...)` | |
| `set_scroll_dir` | `scroll().dir(d)` | |

## 4. Widget Cleanups
**Goal**: Remove manual forwarding code from `core/widget.h`.
- Remove inheritance from `Stylable`, `Layoutable`.
- Remove `lines 43-160` (approx) in `widget.h`.
