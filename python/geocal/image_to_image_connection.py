from builtins import object


class ImageToImageConnection(object):
    """This provides a connection between two images. Right now, we have one
    implementation that uses a ImageGroundConnection to do this. If we end
    up having other methods, we may want to set up a class
    hierarchy to do this.

    Note that this class currently isn't used anywhere. At one point,
    I thought this would be a useful abstraction. I'm not so sure
    about that now. We'll leave this in place for a bit, in case we
    come up with a place that it is useful. If not, then we can
    eventually just remove this class."""

    def __init__(self, image_ground_connnection1, image_ground_connnection2):
        """This takes a ImageGroundConnection for image 1 and image 2 along
        with a Dem to use. We then connect the images by going to the surface
        from image 1 and back into image 2"""
        self.igc1 = image_ground_connnection1
        self.igc2 = image_ground_connnection2

    def image_coordinate2(self, ic1):
        """Give a image coordinate in image 1, return the coordinates in
        image 2"""
        gc = self.igc1.ground_coordinate(ic1)
        return self.igc2.image_coordinate(gc)


__all__ = ["ImageToImageConnection"]
