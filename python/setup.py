from setuptools import setup

# Namespace packages are a bit on the new side. If you haven't seen
# this before, look at https://packaging.python.org/guides/packaging-namespace-packages/#pkgutil-style-namespace-packages for a description of
# this.
setup(
    name="geocal",
    version="1.26.1",
    description="GeoCal",
    author="Mike Smyth",
    author_email="mike.m.smyth@jpl.nasa.gov",
    license="Apache 2.0",
    packages=["geocal"],
    package_data={
        "geocal": ["cib01_mapinfo.xml", "data/isis_camera/*", "py.typed", "*.pyi"]
    },
    install_requires=[
        "numpy",
        "scipy",
        "matplotlib",
        "h5py",
        "scikit-umfpack",
        "pandas",
    ],
)
