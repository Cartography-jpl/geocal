# Hand written stub for the SWIG generated image_coordinate.py. A stub
# replaces the whole module for type checkers, so everything public in
# image_coordinate.py needs to be listed here.

from typing import Any, overload

from .auto_derivative import AutoDerivativeDouble
from .generic_object import GenericObject

class ImageCoordinate(GenericObject):
    @overload
    def __init__(self, Line: float, Sample: float) -> None: ...
    @overload
    def __init__(self, Vic: VicarImageCoordinate) -> None: ...
    @overload
    def __init__(self) -> None: ...

    line: float
    sample: float

class VicarImageCoordinate(GenericObject):
    @overload
    def __init__(self, Line: float, Sample: float) -> None: ...
    @overload
    def __init__(self, Ic: ImageCoordinate) -> None: ...
    @overload
    def __init__(self) -> None: ...

    line: float
    sample: float

class ImageCoordinateWithDerivative(GenericObject):
    @overload
    def __init__(
        self, Line: AutoDerivativeDouble, Sample: AutoDerivativeDouble
    ) -> None: ...
    @overload
    def __init__(self) -> None: ...

    line: AutoDerivativeDouble
    sample: AutoDerivativeDouble
    def value(self) -> ImageCoordinate: ...

# std::vector wrappers. Not worth describing in detail.
Vector_ImageCoordinate: Any
Vector_ImageCoordinatePtr: Any
