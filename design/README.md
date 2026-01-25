# LVGL C++ Design Wiki

This directory serves as the architectural knowledge base for `lvgl_cpp`.

## 🏛️ Core Architecture

Fundamental design patterns and system-level components.

*   **[Core/Object System](README.md)**: (This file) Overview of Monolithic Object avoidance, Mixins, and CRTP.
*   **[Issue #61: Standardization](issue_61_standardization.md)**: The "One Constructor to Rule Them All" pattern for widgets.
*   **[Callback Utilities](callback_utilities.md)**: Design of the `CallbackProxy` and `Event` functional wrappers.
*   **[Style System](core/STYLES.md)**: Detailed design of the Fluent Style Builder API.

## 🔧 Subsystem Designs

Detailed specifications for specific LVGL subsystems.

*   **[Animations](animations.md)**: `lv_anim` RAII wrappers and Timelines.
*   **[Filesystems](filesystems.md)**: `lv_fs` wrappers for `File` and `Directory` usage.
*   **[Fonts](fonts.md)**: Font management and `OwnedFont` resource classes.
*   **[Input Devices](input_devices.md)**: `Indev` wrappers (Pointer, Keypad, Encoder) and `Group` integration.

## 📦 Widget Designs

*   **[Complex Widgets](widgets/COMPLEX.md)**: `Menu`, `Chart` (with Proxies), `Scale`, `Calendar`.
*   **[Input Widgets](widgets/INPUTS.md)**: `Button`, `Slider`, `Textarea`.
*   **[Output Widgets](widgets/OUTPUTS.md)**: `Label`, `Bar`, `Spinner`.
*   **[Containers](widgets/CONTAINERS.md)**: `TabView`, `TileView`.

## 🧪 Testing & QA

*   **[Memory Test Suite Plan](memory_test_suite_plan.md)**: Plan for Valgrind/ASAN integration and leak detection.
*   **[Memory Analysis](memory_analysis.md)**: Study of RAII overhead and callback memory safety.

---

## 📜 Historical Plans & Roadmaps

These documents provided the strategic direction for previous versions (v0.2.0) but are preserved for context.

*   **[Roadmap V2](ROADMAP_V2.md)**: Phases for the v0.2.0 refactoring.
*   **[Strategic Improvement Plan](strategic_improvement_plan.md)**: Gap Analysis and Implementation Plan for API coverage (v0.2.0).
*   **[Gap Analysis](gap_analysis.md)**: Specific breakdown of missing widget APIs vs. the C library.
*   **[Remediation Plan](remediation_plan.md)**: Strategy for addressing technical debt.
*   **[API Coverage Report](api_coverage_report.md)**: Automated report snapshot.

---

## 📂 Directory Structure

*   `core/`: Design docs specific to the `lvgl_cpp/core` module.
*   `system/`: System-level abstraction designs (Displays, Drivers).
*   `widgets/`: Detailed design notes for complex widgets.
