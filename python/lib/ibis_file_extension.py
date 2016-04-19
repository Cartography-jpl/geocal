# This contains extensions to give a more python like interface to 
# IbisFile.
from geocal_swig import *

# We might not have IbisFile defined
have_ibis_file = VicarFile.vicar_available()

def _column(self, cindex):
    '''Return the proper IbisColumn<T> type for the given column index.'''
    t = self.column_data_type(cindex)
    if(t == IbisFile.VICAR_BYTE):
        return self.column_byte(cindex)
    elif(t == IbisFile.VICAR_HALF):
        return self.column_half(cindex)
    elif(t == IbisFile.VICAR_FULL):
        return self.column_full(cindex)
    elif(t == IbisFile.VICAR_FLOAT):
        return self.column_float(cindex)
    elif(t == IbisFile.VICAR_DOUBLE):
        return self.column_double(cindex)
    elif(t == IbisFile.VICAR_ASCII):
        return self.column_string(cindex)
    else:
        raise "Unrecognized type %d" % t

if have_ibis_file:
    IbisFile.column = _column

def _getitem(self, index):
    if isinstance(index[1], slice):
        r = range(*index[1].indices(self.number_col))
        if isinstance(index[0], slice):
            r2 = range(*index[0].indices(self.number_row))
            return [ [self.column(i)[j] for i in r] for j in r2]
        return [self.column(i)[index[0]] for i in r]
    return self.column(index[1])[index[0]]

if have_ibis_file:
    IbisFile.__getitem__ = _getitem

def _setitem(self, index, value):
    if isinstance(index[1], slice):
        r = range(*index[1].indices(self.number_col))
        if isinstance(index[0], slice):
            r2 = range(*index[0].indices(self.number_row))
            for i2,j2 in enumerate(r2):
                for i1, j1 in enumerate(r):
                    self.column(j1)[j2] = value[i2][i1]
            return 
        for i1, j1 in enumerate(r):
            self.column(j1)[index[0]] = value[i1]
        return
    self.column(index[1])[index[0]] = value

if have_ibis_file:
    IbisFile.__setitem__ = _setitem

def _shape(self):
    return (self.number_row, self.number_col)

if have_ibis_file:
    IbisFile.shape = property(_shape)

def _enter(self):
    return self

if have_ibis_file:
    IbisFile.__enter__ = _enter

def _exit(self, type, value, traceback):
    self.close()

if have_ibis_file:
    IbisFile.__exit__ = _exit

def _create(fname, data, type="DOUB"):
    '''Create a ibis file that exactly contains the given array, all of 
    the given type.'''
    with IbisFile(fname, data.shape[0], [type] * data.shape[1]) as f:
        f[:,:] = data

if have_ibis_file:
    IbisFile.create = staticmethod(_create)

