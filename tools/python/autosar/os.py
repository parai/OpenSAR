# Os of autosar implemented in python

import os,sys

# start to learn how to use reflection
def At():
    print 'JJJ'

my_script = __import__('task')

main = getattr(my_script,'main')

main()

def call(levels):
    try:
        raise RuntimeError
    except RuntimeError:
        e,b,t = sys.exc_info()
        f = t.tb_frame
        while levels > 0:
            f = f.f_back                   
            levels -= 1
        ldict = f.f_globals.copy()
        if f.f_globals != f.f_locals:
            ldict.update(f.f_locals)
        for (item,key) in ldict.items():
            print (item,key)

call(1)