from test_support import *
from geocal_swig import IostreamSwigTest
from io import BytesIO

def test_iostream_write():
    '''Test writing to a python file handle like an iostream. This checks the
    handling by swig of the mapping between the two.'''
    iostream_swig_test = IostreamSwigTest("hello world")
    fh = BytesIO()
    fh.write(b"start string ")
    iostream_swig_test.write(fh)
    fh.write(b" end string")
    assert fh.getvalue() == b"start string hello world end string"

def test_iostream_read():
    '''Test reading from a python file handle like an iostream. This checks the
    handling by swig of the mapping between the two.'''
    iostream_swig_test = IostreamSwigTest("hello world", 2)
    start_string = b"start string "
    content = b"hello world"
    end_string = b"end string"
    fh = BytesIO(start_string + content + b" " + end_string)
    fh.read(len(start_string))
    assert iostream_swig_test.read(fh).encode('utf-8') == content
    assert fh.tell() == len(start_string) + len(content + b" ")
    assert fh.read() == end_string
    
        
    
    
