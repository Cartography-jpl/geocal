from setuptools import setup

setup(name='pynitf',
      version='0.5',
      description='This is a package for reading and writing NITF',
      url='https://github.jpl.nasa.gov/Cartography/pynitf',
      author='Mike Smyth <Mike.M.Smyth@jpl.nasa.gov>, Philip Yoon <Philip.J.Yoon@jpl.nasa.gov>',
      author_email='Mike.M.Smyth@jpl.nasa.gov',
      license='Copyright 2018, California Institute of Technology. ALL RIGHTS RESERVED. U.S. Government Sponsorship acknowledged.',
      packages=['pynitf'],
      install_requires=[
          'six',
          'numpy'
      ],
      extras_require={
          'test': ['pytest'],
      },
      zip_safe=False)