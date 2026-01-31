# Engineering Design: Strategic Path to Platinum Coverage


## 1. Objective

Upgrade all "Gold" status subsystems in `lvgl_cpp` to "Platinum" status (100% verified idiomatic coverage). This requires closing specific API gaps in high-priority widgets and core mechanisms.

## 2. Target Subsystems (Gold)

Based on the [API Coverage Report](api_coverage_report.md), the following subsystems are currently in the Gold category:

| Subsystem | Verified Coverage | Status | Primary Blockers |
| :--- | :--- | :--- | :--- |
| **lv_vector** | 93.8% (15/16) | Gold | specialized path transforms |
| **lv_table** | 94.1% (16/17) | Gold | low-level cell resize events |
| **lv_canvas** | 92.9% (13/14) | Gold | raw buffer pointer access |
| **lv_label** | 95.2% (20/21) | Gold | legacy wrap mode compatibility |
| **lv_spinbox** | 95.0% (19/20) | Gold | internal cursor position control |
| **lv_subject** | 93.8% (30/32) | Gold | niche notification flags |

## 3. Implementation Approach

We will move from Gold to Platinum using a three-pronged strategy:

### 3.1. Gap Bridging (Direct Implementation)
For deterministic gaps (Spinbox, Label, Canvas), we will implement the missing C++ wrappers directly.

*   **Spinbox**: Add `get_cursor_pos()` and `set_cursor_pos()`.
*   **Label**: Add `get_long_mode()` (aliasing modern long mode to legacy wrap expectations).
*   **Canvas**: Implement `get_buf()` with proper `void*` return type or wrapped in a `BufferView`.

### 3.2. Event-Driven Idioms (Interaction Proxies)
For notification-based gaps (Table), we will extend the `InteractionProxy` or `EventProxy` to handle specialized widget events.

*   **Table**: Ensure `on_cell_value_changed` and `on_cell_resize` are fully exposed via the idiomatic `on()` syntax.

### 3.3. AST Verification Refinement
For items that are logically covered but AST-flagged (Vector, Subject), we will refine the `audit_api_coverage.py` logical equivalence mapping to correctly identify C++ equivalents.

*   **Vector**: Map `lv_vector_path_transform` to the existing C++ `transform()` method.
*   **Subject**: Map `lv_subject_set_int_notify_only` to specific internal update modes.

## 4. Execution Plan

### Phase 1: Direct API Bridging
- [ ] Implement `Canvas::get_buffer()` and `Canvas::set_buffer()`.
- [ ] Implement `Spinbox::get_cursor_pos()` and `Spinbox::set_cursor_pos()`.
- [ ] Implement `Label::get_long_mode()`.

### Phase 2: Enhanced Logic Mapping
- [ ] Update `audit_api_coverage.py` with specific mappings for Vector and Subject.
- [ ] Verify that specialized event handlers in `lv_table` are counted towards verified coverage.

### Phase 3: Final Verification
- [ ] Run `python3 scripts/audit_api_coverage.py`.
- [ ] Verify Platinum status in `docs/api_coverage.json`.
- [ ] Update `design/api_coverage_report.md`.

## 5. Definition of Done
A subsystem attains **Platinum** status when:
1.  Verified Coverage is 100% in `api_coverage.json`.
2.  No "Logical Equivalence Gaps" exist for the subsystem.
3.  Direct and Idiomatic coverage combined account for all public C functions identified by the AST.
