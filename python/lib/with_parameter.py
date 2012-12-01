from abc import *

class WithParameter(object):
    '''It is common for a class to have some set of parameters that control
    behavior, which we may then fit with something like a 
    SimultaneousBundleAdjustment. It can also be useful for only a subset
    of those parameters to be active.  This mixin class describes this
    functionality, along with some useful functions in support of this.

    Our python code uses duck typing, so as long as you supply the
    interface given in this class is it not necessary to actually
    inherit from this class (although you can if that is useful).
    '''
    __metaclass__ = ABCMeta

    @abstractproperty
    def parameter(self):
        '''Value of parameters controlling mapping to and from image 
        coordinates'''
        pass

    @parameter.setter
    def parameter(self, value):
        '''Value of parameters controlling mapping to and from image 
        coordinates'''
        pass

    @property
    def parameter_name(self):
        '''For longer parameter lists, it can be useful to have a description
        of what each parameter is. This function supplies this.

        The default just calls this "Parameter 0", "Parameter 1", etc., but
        a derived class can supply a more useful set of names.'''
        res = []
        for i in range(len(self.parameter)):
            res.append("Parameter %d" % i)
        return res

    @abstractproperty
    def parameter_subset_mask(self):
        '''This returns a list of flags indicating which parameters should
        be included in the parameter_subset values.

        This should be a list that is the same length as parameter, and
        indicated True for values to include and False for values to
        exclude.'''
        pass

    @property
    def parameter_subset_name(self):
        '''The names of each parameter in parameter_subset.'''
        res = []
        pname = self.parameter_name
        pm = self.parameter_subset_mask
        for i, pmv in enumerate(pm):
            if(pmv):
                res.append(pname[i])
        return res

    @property
    def parameter_subset(self):
        '''A subset of the parameters, as determined by 
        parameter_subset_mask. This might be used by something like
        SimultaneousBundleAdjustment for the actual values to fit for.'''
        p = self.parameter
        pm = self.parameter_subset_mask
        res = []
        for i, pv in enumerate(p):
            if(pm[i]):
                res.append(pv)
        return res

    @parameter_subset.setter
    def parameter_subset(self, val):
        '''Set just the subset of parameters given by parameter_subset_mask,
        leaving the remaining parameters alone.'''
        p = self.parameter
        pm = self.parameter_subset_mask
        if(len(val) != list(pm).count(True)):
            raise ValueError("Length of val passed to parameter_subset must match the number of True values in parameter_subset_mask")
        vindex = 0
        for i, pmv in enumerate(pm):
            if(pmv):
                p[i] = val[vindex]
                vindex += 1
        self.parameter = p

    def parameter_index_to_subset_index(self, i):
        '''This maps a index in the full parameter to  the index in the
        parameter_subset matching it. If the full parameter index is not in
        parameter_subset, then we return None'''
        pm = self.parameter_subset_mask
        if(not pm[i]):
            return None
        return list(pm[0:i]).count(True)

        
