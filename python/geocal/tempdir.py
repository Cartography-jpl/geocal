# This is tempdir from https://pypi.python.org/pypi/tempdir. Just include
# source because it is small, and not worth adding as a requirement.
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

from __future__ import annotations

import os
import shutil
import tempfile
import typing
from collections.abc import Callable, Iterator
from contextlib import contextmanager
from functools import wraps
from typing import Any

if typing.TYPE_CHECKING:
    from typing import Self


class TempDir:
    """class for temporary directories
    creates a (named) directory which is deleted after use.
    All files created within the directory are destroyed
    Might not work on windows when the files are still opened.

    Use
    ---

    TempDir implements the contextmanager protocol.
    So it can be used in a with-statements as follows:

    >>> with tempdir.TempDir() as t:
    ...     <create files and directories>
    ...
    >>> #all gone

    It can also be used in a regular way:

    >>> t = tempdir.TempDir()
    >>> # do things
    >>> t.dissolve()

    The parameters are like tempfile.mkdtemp.

    If you have a code block where you need to change the current working directory, there is a convenience-wrapper in_tempdir:

    >>> with in_tempdir():
    >>> #  os.chdir'ed into the temporary directory
    >>>    pass
    >>> # back in wherever you were before

    and a function decorator:

    >>> @run_in_tempdir():
    >>> def function_in_tempdir(test):
    >>> #   new working directory is created for every call
    >>>     pass
    """

    def __init__(
        self, suffix: str = "", prefix: str = "tmp", basedir: str | None = None
    ) -> None:
        self.name = tempfile.mkdtemp(suffix=suffix, prefix=prefix, dir=basedir)

    def __del__(self) -> None:
        if "name" in self.__dict__:
            self.__exit__(None, None, None)

    def __enter__(self) -> Self:
        return self

    def __exit__(self, *errstuff: object) -> None:
        return self.dissolve()

    def dissolve(self) -> None:
        """remove all files and directories created within the tempdir"""
        if self.name:
            shutil.rmtree(self.name)
        self.name = ""

    def __str__(self) -> str:
        if self.name:
            return f"temporary directory at: {self.name}"
        else:
            return "dissolved temporary directory"


@contextmanager
def in_tempdir(*args: Any, **kwargs: Any) -> Iterator[TempDir]:
    """Create a temporary directory and change to it."""
    old_path = os.getcwd()
    try:
        with TempDir(*args, **kwargs) as t:
            os.chdir(t.name)
            yield t
    finally:
        os.chdir(old_path)


def run_in_tempdir(
    *args: Any, **kwargs: Any
) -> Callable[[Callable[..., Any]], Callable[..., Any]]:
    """Make a function execute in a new tempdir.
    Any time the function is called, a new tempdir is created and destroyed.
    """

    def change_dird(fnc: Callable[..., Any]) -> Callable[..., Any]:
        @wraps(fnc)
        def wrapper(*funcargs: Any, **funckwargs: Any) -> Any:
            with in_tempdir(*args, **kwargs):
                return fnc(*funcargs, **funckwargs)

        return wrapper

    return change_dird


__all__ = ["TempDir", "in_tempdir", "run_in_tempdir"]
