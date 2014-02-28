# This contains extensions to give a more python like interface to 
# IbisFile.
from geocal import *

# We might not have IbisFile defined
try:
    IbisFile
    have_ibis_file = True
except NameError:
    have_ibis_file = False

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
    return self.column(index[1])[index[0]]

if have_ibis_file:
    IbisFile.__getitem__ = _getitem

def _setitem(self, index, value):
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

