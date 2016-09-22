# $Id: setup.py 990 2016-04-08 22:39:25Z gcontin $
from distutils.core import setup, Extension
 
module1 = Extension('ftdIO',
                    include_dirs = ['/usr/include/libftdi1'],
                    libraries = ['ftdi1'],
                    library_dirs = ['/usr/lib64'],
                    sources = ['ftdIOmodule.c'])
 
setup (name = 'PackageName',
        version = '1.1',
        description = 'ftdIO functions for Python',
        ext_modules = [module1])
