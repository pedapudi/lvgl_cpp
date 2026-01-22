# CI integration for API audits

This document describes how to integrate the API audit scripts into a CI pipeline.

## Scripts

### 1. Declaration audit (`scripts/audit_declarations.py`)

Scans lvgl_cpp headers for declared methods without implementations.

```bash
python3 scripts/audit_declarations.py .
```

**Exit codes:**
- 0: All declarations have implementations.
- 1: Unimplemented declarations found.

**CI integration:**
```yaml
# GitHub Actions example
- name: Audit declarations
  run: python3 scripts/audit_declarations.py .
```

> [!NOTE]
> This script may report false positives for inline methods defined in headers.
> Review the output manually before acting on it.

---

### 2. API coverage (`scripts/audit_api_coverage.py`)

Generates a coverage report comparing LVGL C API to lvgl_cpp wrappers.

```bash
python3 scripts/audit_api_coverage.py ../lvgl .
```

**Outputs:**
- Console report with coverage percentages.
- `docs/api_coverage.json` with detailed mapping.

**CI integration:**
```yaml
# GitHub Actions example
- name: Generate API coverage report
  run: python3 scripts/audit_api_coverage.py ../lvgl .
- name: Upload coverage artifact
  uses: actions/upload-artifact@v3
  with:
    name: api-coverage
    path: docs/api_coverage.json
```

---

## Recommended CI workflow

1. **On every PR:**
   - Run declaration audit (fail on errors).

2. **On main branch (nightly or release):**
   - Generate API coverage report.
   - Upload as artifact or publish to docs site.
