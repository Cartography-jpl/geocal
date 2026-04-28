# Basic test of typing in geocal_swig. This is pretty minimal now, but checkpoint what we
# have

from geocal_swig import ImageCoordinate, VicarImageCoordinate
import pytest


def test_temp() -> None:
    ic = ImageCoordinate(10, 20)
    # Should get a typing error here, wrong argument to constructor
    with pytest.raises(TypeError):
        ic2 = ImageCoordinate("Foo", 20)
    f = "hi"
    # And here, assigning a float to a str variable
    f = ic.line
    # This should work, testing handling overloading correctly
    ic3 = ImageCoordinate()
    ic4 = ImageCoordinate(VicarImageCoordinate(10, 20))
