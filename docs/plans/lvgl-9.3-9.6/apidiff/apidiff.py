#!/usr/bin/env python3
"""Classify lv_/LV_ identifiers used by lvgl_cpp against two LVGL trees.

usage: apidiff.py ids.txt lvgl93_root lvgl96_root out.tsv
"""
import os, re, sys, collections

ids_file, root93, root96, out = sys.argv[1:5]
ids = [l.strip() for l in open(ids_file) if l.strip()]


def load_headers(root, subdirs):
    text = {}
    for sd in subdirs:
        base = os.path.join(root, sd)
        if os.path.isfile(base):
            text[base] = open(base, errors="replace").read()
            continue
        for dp, dn, fn in os.walk(base):
            for f in fn:
                if f.endswith(".h"):
                    p = os.path.join(dp, f)
                    text[p] = open(p, errors="replace").read()
    return text


def index(text):
    """token -> list of (path, kind, normalized decl, deprecated)"""
    idx = collections.defaultdict(list)
    for path, t in text.items():
        # join continued lines for prototypes
        flat = re.sub(r"/\*.*?\*/", " ", t, flags=re.S)
        flat = re.sub(r"//[^\n]*", "", flat)
        lines = t.split("\n")
        # function prototypes: type name(args);
        for m in re.finditer(r"(^|\n)([^\n;{}#]*?)\b((?:lv|LV)_[A-Za-z0-9_]+)\s*\(([^;{}]*?)\)\s*;", flat):
            name = m.group(3)
            ret = " ".join(m.group(2).split())
            if ret.startswith("return") or ret == "" and False:
                continue
            args = " ".join(m.group(4).split())
            if not ret or re.search(r"\b(if|while|return|define)\b", ret):
                continue
            dep = "LV_DEPRECATED" in ret
            ret = re.sub(r"LV_DEPRECATED\([^)]*\)", "", ret).strip()
            idx[name].append((path, "func", f"{ret} {name}({args})", dep))
        for m in re.finditer(r"#\s*define\s+((?:lv|LV)_[A-Za-z0-9_]+)(\(?)([^\n]*)", flat):
            idx[m.group(1)].append((path, "macro", (m.group(2) + m.group(3)).strip()[:80], False))
        for m in re.finditer(r"\b((?:lv|LV)_[A-Za-z0-9_]+)\s*(=[^,\n}]*)?\s*[,}\n]", flat):
            # enum constant heuristic: preceding context inside enum braces
            pass
        for m in re.finditer(r"typedef\s+[^;]*?\b((?:lv|LV)_[A-Za-z0-9_]+_t)\s*;", flat):
            idx[m.group(1)].append((path, "type", "typedef", False))
        for m in re.finditer(r"}\s*((?:lv|LV)_[A-Za-z0-9_]+_t)\s*;", flat):
            idx[m.group(1)].append((path, "type", "struct/enum", False))
        for m in re.finditer(r"\bstruct\s+_?((?:lv|LV)_[A-Za-z0-9_]+_t)\s*\{", flat):
            idx[m.group(1)].append((path, "type", "struct", False))
        # enum constants
        for em in re.finditer(r"enum\s*[A-Za-z0-9_]*\s*\{([^}]*)\}", flat):
            for c in re.finditer(r"\b((?:LV|lv)_[A-Za-z0-9_]+)\s*(=|,|$)", em.group(1)):
                idx[c.group(1)].append((path, "enumc", "", False))
        for m in re.finditer(r"(?:extern\s+)?const\s+lv_[a-z_]+_t\s+(lv_[A-Za-z0-9_]+)\s*;", flat):
            idx[m.group(1)].append((path, "var", "", False))
        # deprecated by preceding-line marker
        for i, ln in enumerate(lines):
            if "LV_DEPRECATED(" in ln and i + 1 < len(lines):
                for nm in re.findall(r"\b((?:lv|LV)_[A-Za-z0-9_]+)\s*\(", lines[i + 1] + " " + (lines[i + 2] if i + 2 < len(lines) else "")):
                    idx[nm].append((path, "deprecated-mark", "", True))
    return idx


def any_token(text, name):
    pat = re.compile(r"\b" + re.escape(name) + r"\b")
    return [p for p, t in text.items() if pat.search(t)]


t93 = load_headers(root93, ["src", "lvgl.h"])
t96 = load_headers(root96, ["include", "src", "lvgl.h"])
i93, i96 = index(t93), index(t96)

rows = []
for n in ids:
    a, b = i93.get(n, []), i96.get(n, [])
    a_exists = bool(a) or bool(any_token(t93, n))
    b_exists = bool(b) or bool(any_token(t96, n))
    fa = sorted({d for _, k, d, _ in a if k == "func"})
    fb = sorted({d for _, k, d, _ in b if k == "func"})
    mb = [d for p, k, d, _ in b if k == "macro" and "api_map" in p]
    dep = any(dp for _, _, _, dp in b)
    priv96 = b and all(("private" in p) for p, *_ in b)
    if not a_exists and not b_exists:
        cls = "missing-in-both"
    elif a_exists and not b_exists:
        cls = "removed-in-9.6"
    elif not a_exists and b_exists:
        cls = "9.6-only" + ("(api-map)" if mb else "")
    else:
        norm = lambda s: re.sub(r"\s+", "", re.sub(r"\b(LV_ATTRIBUTE_[A-Z_]+|const)\b", "", s))
        if fa and fb and {norm(x) for x in fa} != {norm(x) for x in fb}:
            cls = "signature-changed"
        elif mb and not fb:
            cls = "renamed(api-map-in-9.6)"
        else:
            cls = "same"
    if dep:
        cls += "+DEPRECATED-9.6"
    if priv96:
        cls += "+private-in-9.6"
    rows.append((n, cls, " | ".join(fa)[:200], " | ".join(fb)[:200], (mb[0] if mb else "")))

with open(out, "w") as f:
    for r in rows:
        f.write("\t".join(r) + "\n")
c = collections.Counter(r[1] for r in rows)
for k, v in c.most_common():
    print(f"{v:5d} {k}")
