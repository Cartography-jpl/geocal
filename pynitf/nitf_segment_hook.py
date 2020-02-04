import collections
class NitfSegmentHookSet(collections.abc.Set):
    '''To allow special handling of TREs etc. we allow a hook_list of
    these objects to be passed to NitfSegment. These then are called in
    each function of NitfSegment.

    See for example geocal_nitf_rsm.py in geocal for an example of using
    these hooks to add in support for the geocal Rsm object.

    This class handles the full set of NitfSegmentHook that are applied.
    NitfFile contains a NitfSegmentHookSet as the variable segment_hook_set. 
    The initial default value of this is given by default_hook_set, an 
    individual NitfFile object might modify this for some reason (e.g., 
    add a special hook, remove a hook).

   :ivar hook_set: The set of NitfSegmentHook to apply to NitfSegments 
                   in a NitfFile

    '''
    def __init__(self, iterable=()):
        self.hook_set = set(iterable)

    def __contains__(self, h):
        return h in self.hook_set

    def __len__(self):
        return len(self.hook_set)

    def __iter__(self):
        return iter(self.hook_set)

    def __copy__(self):
        return self.__class__(iter(self))

    def add_hook(self, h):
        self.hook_set.add(h)

    def discard_hook(self, h):
        self.hook_set.discard(h)

    def after_init_hook(self, seg, nitf_file):
        '''Called at the end of NitfSegment.__init__'''
        for h in self:
            h.after_init_hook(seg, nitf_file)

    def after_append_hook(self, seg, nitf_file):
        '''Called after a segment is added to a NitfFile'''
        for h in self:
            h.after_append_hook(seg, nitf_file)
            
    def before_write_hook(self, seg, nitf_file):
        '''Called before NitfFile writes a file'''
        for h in self:
            h.before_write_hook(seg, nitf_file)

    def after_read_hook(self, seg, nitf_file):
        '''Called after NitfFile has read a file'''
        for h in self:
            h.after_read_hook(seg, nitf_file)

    def before_str_hook(self, seg, nitf_file, fh):
        '''Called at the start of NitfSegment.__str__'''
        for h in self:
            if(not (nitf_file.report_raw and h.remove_for_report_raw())):
                h.before_str_hook(seg, nitf_file, fh)

    def before_str_tre_hook(self, seg, tre, nitf_file, fh):
        '''Called before printing a TRE. If this returns true we assume
        that this class has handled the TRE printing. Otherwise, we
        call print on the tre'''
        res = False
        for h in self:
            if(not (nitf_file.report_raw and h.remove_for_report_raw())):
                res = res or h.before_str_tre_hook(seg, tre, nitf_file, fh)
        return res
    
    @classmethod
    def default_hook_set(cls):
        '''Return the default set of hookrs to use.'''
        if(not hasattr(cls, "_default_hook_set")):
            cls._default_hook_set = cls()
        return cls._default_hook_set
    
    @classmethod            
    def add_default_hook(cls, h):
        '''Add the given hook to the default set of hookrs.  The 
        higher priority_order (larger number) items are tried first.'''
        cls.default_hook_set().add_hook(h)

    @classmethod            
    def discard_default_hook(cls, h):
        '''Discard the hook h from the default list. It is ok if h isn't 
        actually in the set of hooks.'''
        cls.default_hook_set().discard_hook(h)
        
class NitfSegmentHook(object):
    '''To allow special handling of TREs etc. we allow a hook_list of
    these objects to be passed to NitfSegment. These then are called in
    each function of NitfSegment.

    Note that you don't need to strictly derive from this class, we use
    the standard "duck" typing of python. This is just the list of functions
    that need to be supplied.

    See for example geocal_nitf_rsm.py in geocal for an example of using
    these hooks to add in support for the geocal Rsm object.
    '''
    def after_init_hook(self, seg, nitf_file):
        '''Called at the end of NitfSegment.__init__'''
        pass
    def after_append_hook(self, seg, nitf_file):
        '''Called when a segment is added to a NitfFile'''
        pass
    def before_write_hook(self, seg, nitf_file):
        '''Called before NitfFile writes a file'''
        pass
    def after_read_hook(self, seg, nitf_file):
        '''Called after NitfFile has read a file'''
        pass
    def before_str_hook(self, seg, nitf_file, fh):
        '''Called at the start of NitfSegment.__str__'''
        pass
    def remove_for_report_raw(self):
        '''Hooks usually map to some higher level object (e.g., Geocal
        handling RSM. Normally you want this, but for certain contexts it
        can be useful to suppress this behavior, e.g., nitfinfofull reporting
        the raw TRE data rather than the objects generated by the TRE
        data.

        This skips before_str_hook and before_str_tre_hook if we are
        reporting raw information if this function returns True. The
        hooks are by default marked as "True" for removing, but if you
        have some special case where you want to avoid removing the
        hook you can have the derived class change this to False.
        '''
        return True
    def before_str_tre_hook(self, seg, tre, nitf_file, fh):
        '''Called before printing a TRE. If this returns true we assume
        that this class has handled the TRE printing. Otherwise, we
        call print on the tre'''
        return False

__all__ = ["NitfSegmentHook", "NitfSegmentHookSet"]
    
