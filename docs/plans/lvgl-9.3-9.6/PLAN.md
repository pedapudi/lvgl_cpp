# lvgl_cpp 2.0: native LVGL 9.3 + 9.6 support and a sound core

Status: proposed, not started. Written 2026-10-31 against lvgl_cpp `main` at
`8cacda4`.

## Goal

lvgl_cpp 2.0 builds and passes its tests against exactly two LVGL versions:

| Tier | LVGL source | Why |
|---|---|---|
| **9.3** | Only the snapshot that ZMK and Zephyr v4.1 ship, not the 9.3.0 release: `zmkfirmware/lvgl` `f1db87ee` (LVGL master `1ed1ddd8`, 2025-01-28; `lv_version.h` reports 9.3.0 with `LVGL_VERSION_INFO "dev"`) | Every ZMK keyboard, including wbws-keypad and Paw60, builds against it. ZMK draft PR #3387 would move to 9.5, but nothing in ZMK targets 9.6. |
| **9.6** | LVGL `v9.6.0` (2026-09-16), the final v9 release | Current LVGL, shipped by Zephyr v4.5 and Zephyr main. |

The same work fixes the confirmed correctness bugs and reshapes the interfaces
that let callers write those bugs, so that 2.0 is well organized, sound, and
hard to misuse.

Non-goals:

- The LVGL 9.3.0 release, which Zephyr v4.2 and v4.3 ship, is not a target.
  It differs from the ZMK snapshot in two function signatures:
  `lv_draw_add_task` and `lv_dropdown_set_selected`.
- LVGL 9.4 and 9.5 are not CI targets. Code paths are chosen by feature, not
  by version, so 9.4 and 9.5 will mostly work, but nothing guarantees it.
- Source compatibility with lvgl_cpp 1.x. 2.0 is a major version, and
  `MIGRATION_2_0.md` documents every break.
- LVGL Pro's XML engine, which left the open repository in 9.5.

## Where things stand

The facts below come from a read-only survey on 2026-10-31. Its probe
programs and API-diff tables are saved next to this plan, in `probes/` and
`apidiff/`.

- **LVGL 9.3 does not build.** `main` fails against the ZMK snapshot with 1592
  errors (273 unique). The only build that works on 9.3 is the uncommitted
  patch carried in wbws-paw60 (`patches/lvgl_cpp-zmk-compat.patch`) on top of
  `e207c77`. Some of that patch is legitimate version handling, some is plain
  bug fixes, and some is a ZMK-specific hack that changes behaviour, such as
  turning `set_text_vfmt` into a silent no-op.
- **LVGL 9.6 builds, with warnings.** The library and tests compile, and
  43 of 45 tests pass. The build emits 60 deprecation warnings, because
  lvgl_cpp uses APIs that LVGL v10 will remove. Making it build at all needed
  three configuration workarounds.
- **The repo disagrees with itself about which LVGL it targets.** The README
  says 9.0+, `idf_component.yml` says `^9.4`, the code is guarded for 9.5, and
  the CMake build uses an unpinned sibling `../lvgl`.
- **The core has memory-safety bugs.** Each one below was reproduced with an
  ASan/UBSan probe:

  | # | Bug | Probe |
  |---|---|---|
  | 1 | Moving an `Object` leaves LVGL pointing at callback nodes the moved-from wrapper frees: heap-use-after-free | p4 |
  | 2 | Ownership depends on the type: `Object(&parent)` is deleted at scope exit, but `Button(parent)` and `Label(parent)` are not | p6 |
  | 3 | Moving an `Observer` leaves LVGL pointing at the dead temporary: undefined behaviour, and the binding is silently lost | p5 |
  | 4 | Moving a `Style` copies `lv_style_t` by value, so two styles share one property array: heap corruption | p9 |
  | 5 | `release()` drops every event callback registered through the wrapper | p7 |
  | 6 | Every wrapper, temporaries included, adds an LVGL event descriptor | p8 |
  | 7 | `set_id`, `get_id` and `find_by_id` are declared but never defined: link error | p1 |
  | 8 | The README's `Style` example does not compile, and `StyleProxy` lacks the blur and drop-shadow setters | p2, p3 |
  | 9 | Proxies dereference null on 9.3; 9.6 survives only because its argument checking is on by default | p10 |

  The survey also found these problems, without probes:
  - `ImageDescriptor` is never zero-initialized, and its stride is never set.
  - `Display::get_default` returns a pointer to one static object that every
    call overwrites.
  - `Display::set_flush_cb` overwrites Zephyr's display user data.
  - Some feature guards are misspelled (`LV_CPP_HAS_PROPERTIES`,
    `LV_USE_I18N`), so code behind them is dead.
  - The display code has two enums for the same screen-load animations.
  - Debug output is left in: `printf` and `<iostream>`.
  - About 46 getters that should be `const` are not.
- **Build and test hygiene is weak.**
  - Nothing runs in CI.
  - Nine test files are never compiled, and one registered test has no
    executable behind it.
  - There is no test framework.
  - Using lvgl_cpp as a CMake subdirectory always builds its tests and
    benchmarks, and requires Python.
  - The build depends on hard-coded `../lvgl` and `src/` include paths, and on
    private LVGL headers. One of those headers, `lv_draw_mask_private.h`, is
    gone in 9.6.
  - There is no Zephyr module, so ZMK users list 23 source files by hand.
- **lvgl_cpp silently requires LVGL options to be enabled.** It assumes
  `LV_USE_OBSERVER`, `LV_USE_FLEX`, `LV_USE_GRID`, `LV_USE_OBJ_PROPERTY`,
  `LV_USE_FLOAT` and `LV_USE_OBJ_NAME`. Under Zephyr, `LV_USE_OBSERVER` is 0
  unless `CONFIG_LV_USE_OBSERVER` is set, even though LVGL's own Kconfig
  defaults it to y.

## Design decisions

### D1. Choose code paths by feature, and keep version branches in one place

- One internal header, `lvgl_cpp/detail/lvgl_api.h`, includes LVGL correctly
  for each version. 9.6 moved its public headers to `include/lvgl/`, and
  including anything under `src/` now raises a warning.
- The same header defines `LVGL_CPP_LVGL_9_6`, a single version switch. Code
  outside `detail/` never compares version numbers.
- `detail/compat.h` holds one inline shim for each API that differs between
  the two versions:
  - `lv_draw_add_task`: 2 arguments on 9.3, 3 on 9.6.
  - `lv_dropdown_set_selected`: 3 arguments on 9.3, 2 on 9.6.
  - `lv_draw_vector` takes a different descriptor type.
  - `lv_display_add_event_cb` and `lv_indev_add_event_cb` return different
    types.
  - `lv_obj_invalidate` returns a value on 9.6 only.
  - Screen-load animation constants are `LV_SCR_*` on 9.3 and `LV_SCREEN_*`
    on 9.6.
  - Label long modes are `LV_LABEL_LONG_*` on 9.3 and
    `LV_LABEL_LONG_MODE_*` on 9.6.
  - Default font: `lv_font_default` on 9.3, `lv_font_get_default` on 9.6.
  - Observer removal: `lv_observer_remove` / `lv_observer_delete`.
  - String subjects: `lv_subject_copy_string` / `lv_subject_set_string`.
  - Tabview: `lv_tabview_rename_tab` / `lv_tabview_set_tab_text`.
  - Calendar header constructors.
- When an API is old on one version and new on the other, the shim calls the
  9.6 name on 9.6. The 9.6 CI job therefore builds with
  `LV_DISABLE_API_MAPPING` and with deprecation warnings treated as errors.
  That keeps 2.0 ready for LVGL v10.
- Each optional LVGL module is gated by its own `LV_USE_*` macro, never by the
  LVGL version: observer, flex, grid, object properties, float, object names,
  translation, and each widget. Features that exist only on 9.6 are gated by
  `LVGL_CPP_HAS_<FEATURE>`, which the same header defines.
- **Never fail silently.** A feature is either really implemented, or it is
  absent at compile time, so calling it is a compile error. Several of the
  9.3 gaps can be implemented properly:
  - `Label::set_text_vfmt`: format with `lv_vsnprintf` into a buffer, then
    set the text.
  - `Image::src_width`: read it through `lv_image_decoder_get_info`.
  - The flag and state helpers: build them on the older APIs.

  Where that is impossible, the feature does not exist on that version.
  Indev timestamps are one example: they are absent on 9.3.
- Only public LVGL headers are included. The private-header includes and the
  hard-coded relative paths in `misc/mask.h`, `widgets/lottie.h` and
  `draw.cpp` all go.

### D2. Ownership: LVGL's object tree owns objects, and C++ owns only roots

The 1.x Managed/Unmanaged flag is a runtime property, so ownership depends on
which constructor ran (bug 2). 2.0 makes ownership part of the type:

- **`Object`, `Button`, `Label` and the other widget types are non-owning
  handles.** Each wraps one `lv_obj_t*` and nothing else, so it is trivially
  copyable and its destructor does nothing. That matches LVGL's own model,
  where a parent deletes its children.
  - `Button::create(parent)` returns a `Button`.
  - The handle stays valid until LVGL deletes the object.
  - Handles hold no state, so temporaries are free and moves cannot go wrong.
    This fixes bugs 1, 5 and 6.
- **`Owned<T>` is the only owning type.**
  - It is move-only.
  - Its destructor calls `lv_obj_delete`.
  - `release()` returns the raw `lv_obj_t*`, as ZMK's status screen needs.
  - Moving it transfers the pointer and nothing else.
  - Screens, and objects created without a parent, come back as `Owned<T>`.
    `Screen::create()` replaces the 1.x behaviour where a default-constructed
    `Object()` silently created a screen.
- **Nullability is explicit.** A handle is never null. Lookups that can fail
  return `std::optional<Handle>`: `parent()`, `child(i)`, `find_by_name`, and
  similar.
  - Debug builds assert with `LV_ASSERT_NULL` on every raw-pointer adoption.
    This matters on 9.3, which does not check arguments the way 9.6 does.
  - `Handle::from_raw(lv_obj_t*)` is the single explicit escape hatch.

### D3. Callbacks live with the LVGL object, not the wrapper

- Each `on(event, fn)` registration allocates a small closure block with
  `lv_malloc`. The block is registered as the event's `user_data`, through
  one static trampoline function.
- The closure is freed when the event descriptor is removed or the object is
  deleted:
  - **9.6:** `lv_obj_add_delete_cb`, so there are no extra event descriptors.
  - **9.3:** one `LV_EVENT_DELETE` handler per object. It is installed on the
    first closure, and later closures find it by its function pointer.
- `on()` returns an `EventConnection` token. Calling `disconnect()` on it
  removes the callback; dropping the token without calling it keeps the
  callback alive. Disconnecting tolerates the object already being gone,
  because the token checks the descriptor on 9.6 and is told by the delete
  hook on 9.3.
- The default callable is `std::function`. Whether a small fixed-size callable
  type is worth adding instead depends on the code-size budget in Phase 0,
  where `std::function` is measured inside ZMK.

### D4. Styles, subjects and observers live at stable addresses

- **`Style`** keeps its `lv_style_t` in a heap block, so moving a `Style` only
  moves a pointer.
  - It is move-only, and its destructor calls `lv_style_reset`.
  - A style must outlive every object that uses it. Debug builds check this
    with a reference count.
  - Fluent setters return `Style&`, and the documentation shows only patterns
    that compile, such as `static Style s; s.bg_color(...)` and
    `Style::make().bg_color(...)`, which returns by value.
- **`Subject<T>`** stores its subject on the heap, so it can be moved. On 9.6
  it uses `lv_subject_create` and `lv_subject_delete`; on 9.3 it allocates an
  `lv_subject_t` with `lv_malloc` and initializes it in place.
- **`Observer`** owns its `lv_observer_t*` and a closure block, so moving it
  transfers both and LVGL still points at the right place.
  - Standalone observers are removed when the `Observer` is destroyed.
  - Bindings to an object (`obj.bind_checked(subject)` and similar) return
    nothing to destroy, because LVGL removes them when the object is deleted.
  - `add_observer` no longer returns a raw `new Observer`.
  - `set_formatted` sizes its buffer to fit instead of truncating at 256
    bytes.

### D5. One way to do each thing

- Proxies (`obj.style()`, `.scroll()`, `.layout()`, `.events()`, `.state()`)
  become small value types holding a non-null `lv_obj_t*`, and never an
  `Object*` that can dangle.
- Each operation has one canonical spelling. These duplicates go:
  - `Object::scroll_to`, `ScrollProxy::to` and `Widget::scroll_by` collapse
    into the scroll proxy.
  - `set_flex_flow` duplicates the layout proxy.
  - `add_state` duplicates the state proxy.
  - `get_parent` duplicates the tree proxy.
  - `add_event_cb`, `on_clicked` and `EventProxy::on` collapse into one `on()`.
- **Generate the style property setters.** One table, generated from the style
  properties of both LVGL versions, produces both `Style` and `StyleProxy`,
  each with its availability guard. That fixes the missing blur, drop-shadow,
  colorkey, layout, text_decor and text_leading_trim setters, and stops the
  two classes drifting apart again.
- **Generate the enum wrappers** from the same tables. That replaces the
  single 700-line `misc/enums.h` and removes the duplicate `LoadAnim` and
  `ScreenLoadAnim` enums.
- Apply modern C++ conventions throughout:
  - `const` getters.
  - `[[nodiscard]]` on functions that create or look up objects.
  - `noexcept` moves.
  - `std::string_view` overloads that copy their text. The `set_text_static`
    variants keep taking `const char*`, to make the lifetime requirement
    explicit.
- The library does not use exceptions or RTTI. ZMK builds without both, and CI
  enforces this with `-fno-exceptions -fno-rtti`.

### D6. Layout of the code

- **Headers:**
  - Public headers move to `include/lvgl_cpp/...`.
  - Implementation goes in `src/`.
  - Internal helpers go in `include/lvgl_cpp/detail/`.
  - `lvgl_cpp.h` remains as the umbrella header.
- `object.h` stops including every proxy, the whole style setter list and all
  the enums, so each source file pays only for what it uses. Widget code that
  is identical for every widget stays out of the CRTP templates, so it is not
  compiled once per widget type.
- Each widget has its own header and source file, both gated by
  `LV_USE_<WIDGET>`.
- **`List`, `Win` and `Menu` become lvgl_cpp's own composite widgets.** LVGL
  9.6 deprecates `lv_list`, `lv_win` and `lv_menu`, and says to rebuild them
  from `lv_obj` with flex layouts:
  - a list is a column flex container (`lv_example_flex_list`)
  - a window is a flex column with a header bar and a content area
    (`lv_example_flex_win`)
  - a menu is pages of plain objects plus a back button that switches pages
    (`lv_example_menu_navigation`)

  lvgl_cpp implements each of those patterns as a C++ widget class. Callers
  still write `lvgl::List`, `lvgl::Win` and `lvgl::Menu`; only the
  implementation underneath changes. Rules for these composite widgets:
  - **Same behaviour on both versions.** The deprecated C widgets are not
    called, not even on 9.3 where they still exist.
  - **Each widget registers an `lv_obj_class_t`** whose base class is
    `lv_obj_class`. That gives it a real LVGL type identity:
    - `lv_obj_check_type` can recognize it.
    - `Handle::from_raw` can verify the type before wrapping an object, for
      example one that arrived in an event.
    - Themes and selectors can target it.
  - **The parts are typed handles:** `Win::header()` and `Win::content()`,
    `List::add_text()` and `List::add_button()`, and `Menu::page()` with
    back navigation.
  - **The public API stays close to 1.x where it was sound,** so migrating is
    mostly a recompile. `MIGRATION_2_0.md` lists every difference.
  - **They follow D2 and D3** like every other widget: non-owning handles,
    with callbacks stored with the LVGL object.

  These are the first widgets whose implementation is lvgl_cpp's own. 1.x
  wraps a C widget for every class except `Screen`. The `lv_obj_class_t`
  pattern they establish becomes the template for any later composite widget.
- **New widgets** come in Phase 5: barcode, qrcode and gif. The widgets that
  9.6 deprecates are not added: rlottie, file_explorer and fragment.
- **Removed:**
  - `scripts/__pycache__`
  - the debug output
  - the stale `missing_apis.md`, `MIGRATION_9_5.md` and `api_coverage.json`

### D7. Build systems

- **CMake:**
  - The library alone builds by default.
  - `LVGL_CPP_BUILD_TESTS`, `_EXAMPLES` and `_BENCH` default to
    `PROJECT_IS_TOP_LEVEL`.
  - Python is needed only for the code generators.
  - The build finds LVGL through an `lvgl` target that the user provides.
  - It accepts both config variables, `LV_CONF_PATH` (9.3) and
    `LV_BUILD_CONF_PATH` (9.6).
- **Zephyr module.** `zephyr/module.yml`, `zephyr/CMakeLists.txt` and
  `zephyr/Kconfig` add a `CONFIG_LVGL_CPP` option. It selects C++20 and
  `REQUIRES_FULL_LIBCPP`, and requires `LV_USE_OBSERVER` when the observer API
  is used. ZMK keyboards then add lvgl_cpp as a west project or module instead
  of listing source files by hand.
- **ESP-IDF.** `idf_component.yml` accepts `>=9.3.0,<9.4 || >=9.6.0,<10`.
  Issue #213 (ESP32-S3 SIMD fonts) stays out of scope.

## Phases

Each phase is one or more PRs, and `main` stays green throughout. A phase is
done when its exit criteria hold in CI.

### Phase 0: Safety net first

- **CI (GitHub Actions)** runs this matrix:
  - LVGL: the ZMK 9.3 snapshot × 9.6.0.
  - Compilers: GCC × Clang.
  - Configs: minimal (observer, flex, grid, float and object properties off)
    × full.
  - Separately: an ASan + UBSan job, and a `-fno-exceptions -fno-rtti` job.
  - The 9.3 jobs may fail until Phase 1 is done.
- **Tests:**
  - Adopt doctest, a single header with no dependencies.
  - Wire in the nine test files that are never built, and fix or delete the
    `test_animation` entry that has no executable.
  - Turn the ten probes in `probes/` into regression tests, expected to fail
    until their fixes land.
- **Fix the 9.6 test configuration:** set `LV_USE_THORVG` when Lottie is on,
  and `LV_USE_OBJ_NAME` when object properties are on. Replace the
  hard-coded `../lvgl` paths in `scripts/check_headers.py` and the tests.
- **Code-size baseline.** Build wbws-paw60's left half and record its `.text`
  and RAM size. That gives Phase 2 a budget to measure against.

**Exit:** CI runs on every PR, and 9.6 is green apart from the known probe
failures.

### Phase 1: Build on both versions

- Add `detail/lvgl_api.h` and `detail/compat.h` as described in D1.
- Gate every optional module with its `LV_USE_*` macro. Fix the misspelled
  guards, and the guards that check the wrong thing:
  - `LV_CPP_HAS_PROPERTIES` → the real `LVGL_CPP_HAS_PROPERTIES`
  - `LV_USE_I18N` → `LV_USE_TRANSLATION`
  - `lv_gltf_class`, which exists in neither version
  - `LVGL_CPP_HAS_PROPERTIES` must also check `LV_USE_OBJ_PROPERTY`
- Remove private-header and `src/` includes.
- Bring in the parts of the wbws-paw60 patch that are correct. Replace its
  hacks with real implementations or with compile-time absence, as D1
  describes.
- On 9.6, replace the deprecated APIs with the current ones:
  - the per-flag and per-state setters instead of `lv_obj_add_flag` and
    friends
  - `lv_subject_create` and `lv_obj_bind_*` instead of `lv_subject_init_*` and
    `bind_*_if_*`
  - the scale section style API
  - `lv_obj_find_by_name`
- Write behavioural tests for `List`, `Win` and `Menu`. Run them on 9.3
  against the current implementation, which wraps the C widgets, so the
  Phase 2 reimplementation has a baseline to match.
- Until Phase 2, those three classes exist only on 9.3. On 9.6 they are
  compiled out, so the zero-warning target still holds.

**Exit:**
- Zero errors on the 9.3 snapshot.
- Zero warnings on 9.6 with `LV_DISABLE_API_MAPPING` and `-Werror`.
- Both minimal and full configs pass.

### Phase 2: Ownership and lifetime redesign

- Implement D2, D3 and D4: handles plus `Owned<T>`, closures stored with the
  object, an `EventConnection` token, and stable `Style`, `Subject` and
  `Observer`.
- **Fix `Display`:**
  - no function-static object
  - a working `get_next`
  - flush callback state kept in a side table instead of display user data,
    so it no longer overwrites Zephyr's
- **Fix `ImageDescriptor`:**
  - zero-initialize it
  - compute the stride with `lv_draw_buf_width_to_stride`
  - allocate aligned buffers through `lv_draw_buf_create`
  - drop the false alignment comment
- Add `set_name` and `find_by_name` on versions that have them, and delete the
  undefined `set_id` and `get_id` declarations.
- **Reimplement `List`, `Win` and `Menu` as composite widgets** (D6):
  - each registers its own `lv_obj_class_t`
  - none calls a deprecated C widget, on either version
  - each passes the behavioural tests written in Phase 1, on both versions

  They depend on the new handle and callback model, which is why they land in
  this phase.

**Exit:**
- All ten probe regressions pass under ASan and UBSan on both versions.
- A wrapper temporary adds no LVGL event descriptors.
- The wbws-paw60 left half stays within the Phase 0 code-size budget plus a
  limit agreed in review.

### Phase 3: API cleanup

- Implement D5:
  - the single canonical spellings
  - generated `Style`/`StyleProxy` setters and enums
  - `const`, `[[nodiscard]]`, `noexcept` and `string_view` throughout
- Fix issue #214, where the roller's property ID is ambiguous.
- Write `MIGRATION_2_0.md` as the API changes, not afterwards.

**Exit:** the generators run in CI and leave no diff. Every public symbol is
either documented or explicitly internal.

### Phase 4: Organization, packaging and docs

- Move to the D6 layout and split the headers.
- Add the D7 build changes: CMake options, the Zephyr module and the IDF
  manifest.
- Rewrite the README with a table of supported versions and required LVGL
  config options, and correct the examples.
- Delete the stale documents.
- Add a CI job that builds a small Zephyr v4.1 sample against the 9.3 snapshot
  through the new Zephyr module.

**Exit:** a fresh ZMK module consumer builds using only the Zephyr module, with
no hand-listed sources.

### Phase 5: LVGL 9.6 features

Each feature lands with tests and with a guard in the 9.6 build:

1. Observer bindings: `lv_obj_bind_bool`, `int`, `float`, `string`, `color`
   and `pointer`.
2. The new events: checked/unchecked, and gestures up, down, left and right.
3. Blur and drop-shadow styles, `text_leading_trim`, and
   `lv_obj_set_style_enabled` (this closes issue #210).
4. `lv_label_set_max_lines` and `LV_IMAGE_ALIGN_CONTAIN_DOWNSCALE`.
5. `lv_layout_create`, for custom layouts.
6. Themes: create, copy, delete, and `remove_theme`.
7. Input device gesture thresholds and key remapping.
8. The barcode, qrcode and gif widgets.

Issues #208, #209 and #215 target 9.5. Close them or retarget them to 9.6 when
this phase starts.

### Phase 6: Release and adopt downstream

- Tag lvgl_cpp `v2.0.0`.
- **wbws-paw60:**
  - point the `lib/lvgl_cpp` submodule at the tag
  - delete `patches/lvgl_cpp-zmk-compat.patch` and `LVGL_CPP_CHANGES.md`
  - switch from hand-listed sources to the Zephyr module
  - port `paw60_view.cpp` to `Owned<Screen>` and the non-owning handles
  - rebuild both halves and compare their size with the Phase 0 baseline
- **wbws-keypad:** the same steps. Its build also needs the
  `nice_nano/nrf52840/zmk` board target that current ZMK requires.

**Exit:** both keyboards build from clean clones with no patches.

## Decisions

Decided on 2026-10-31:

- **9.3 means the ZMK snapshot only.** There is no CI job for the 9.3.0
  release, and no macro to tell the release from the snapshot.
- **Stop wrapping the deprecated C widgets.** `List`, `Win` and `Menu` are
  reimplemented as lvgl_cpp composite widgets using LVGL's documented
  replacement patterns, so they keep the same higher-level API on both
  versions (see D6).

- **Ownership:** non-owning handles plus `Owned<T>`, as in D2. The runtime
  owned flag from 1.x goes away.
- **Test framework:** doctest.
- **Callbacks:** `std::function`. If the Phase 0 code-size measurement in ZMK
  shows `std::function` is too costly, Phase 2 switches to a fixed-size
  callable type, and that decision comes back for review.

Every design decision is now made, so Phase 0 can start.

## Reference material in this directory

- **`probes/`:** the ten ASan/UBSan probe programs (p1 to p10) and their shared
  `common.h`. They were built against LVGL 9.6.0.
- **`apidiff/`:**
  - `apidiff.py` classifies every `lv_`/`LV_` identifier that lvgl_cpp uses
    against two LVGL trees.
  - `diff.tsv` compares the ZMK 9.3 snapshot with 9.6.
  - `diff_930.tsv` compares the 9.3.0 release with 9.6.
  - `diff_zmk_930.tsv` compares the ZMK snapshot with the 9.3.0 release.
- **Sources:**
  - LVGL releases: https://github.com/lvgl/lvgl/releases
  - LVGL 9.6 migration guide: https://lvgl.io/docs/open/changelog/migration-v9-6
  - ZMK: `app/west.yml` at `4dd95d7`, and draft PR #3387
  - Zephyr: LVGL pins in v4.1 through v4.5
