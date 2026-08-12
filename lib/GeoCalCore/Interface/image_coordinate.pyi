from typing import overload

class ImageCoordinate:
    @overload
    def __init__(self, Line: float, Sample: float) -> None: ...

    @overload
    def __init__(self, Vic: VicarImageCoordinate) -> None: ...

    @overload
    def __init__(self) -> None: ...
    
    @property
    def line(self) -> float: ...

    @property
    def sample(self) -> float: ...
        
