from with_parameter import *
import bisect

class CubicSpline(WithParameter):
    '''This class is a simple cubic spline, with a fixed number of knots.'''
    def __init__(self):
        self.knot = []
        self.y = []
        self.dy = []

    def add_knot(self, x):
        if(len(self.knot) ==0):
            self.knot = [x]
            self.y = [0]
            self.dy = [0]
        elif(x > self.knot[-1]):
            self.knot.append(x)
            self.y.append(0)
            self.dy.append(0)
        else:
            inew = bisect.bisect_left(self.knot, x)
            self.knot.insert(inew, x)
            self.y.insert(inew, 0)
            self.dy.insert(inew, 0)

    @property
    def parameter(self):
        res = []
        for i in range(len(self.y)):
            res.append(self.y[i])
            if(i != 0 and i != len(self.y) - 1):
                res.append(self.dy[i])
    
    @parameter.setter
    def parameter(self, value):
        if(len(value) != len(self.y) * 2 - 2):
            raise ValueError("Length of value needs to be 2 * number knots - 2")
        j = 0
        for i in range(len(self.y)):
            self.y[i] = value[j]
            j += 1
            if(i != 0 and i != len(self.y) - 1):
                self.dy[i] = value[j]
                j += 1
            else:
                self.dy[i] = 0
    
    @property
    def parameter_name(self):
        res = []
        for i in range(len(self.y)):
            res.append("Value at knot %f" % self.knot[i])
            if(i != 0 and i != len(self.y) - 1):
                res.append("dy/dx at knot %f" % self.knot[i])
        return res

    @property
    def parameter_subset_mask(self):
        return np.array([True] * len(self.parameter))

    def value(self, x):
        '''Value at x. Note that if x is behind the first or last knot we
        extrapolate.'''
        # bisect will find smallest i such that self.knot[i] >= x
        i = bisect.bisect_right(self.knot, x)
        if(i < 1):
            return self.y[0]
        if(i >= len(self.knot)):
            return self.y[-1]
        c0 = self.y[i - 1]
        c1 = self.dy[i - 1] * (self.knot[i] - self.knot[i - 1])
        c2 = 3 * (self.y[i] - self.y[i - 1]) - \
            (self.dy[i] + 2 * self.dy[i-1]) * (self.knot[i] - self.knot[i - 1])
        c3 = -2 * (self.y[i] - self.y[i - 1]) + \
            (self.dy[i] + self.dy[i-1]) * (self.knot[i] - self.knot[i - 1])
        t = (x - self.knot[i - 1]) / ((self.knot[i] - self.knot[i - 1]))
        return c0 + (c1 + (c2 + c3 * t) * t) * t
