import re
from pathlib import Path

import geocal


def test_init_stub_in_sync() -> None:
    """geocal/__init__.pyi lists the modules that geocal/__init__.py imports,
    so type checkers can see them. Check that it is up to date (e.g., a new
    module was added to geocal but not to the stub)."""
    gdir = Path(geocal.__file__).parent
    # Same selection as the exec loop in __init__.py
    expected = {
        p.stem
        for p in gdir.glob("*.py")
        if p.stem not in ("__init__", "ipython", "cython_try")
        and not re.search("_test", p.stem)
    }
    stub = (gdir / "__init__.pyi").read_text()
    listed = set(re.findall(r"^from \.(\w+) import \*", stub, re.MULTILINE))
    assert listed == expected
