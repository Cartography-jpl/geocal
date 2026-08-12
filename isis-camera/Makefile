# We use using the isis enviroment, with the addition of:
# mamba install gxx_linux-64 gdb
all: isis_camera

%.o: %.cc
	$(CXX) $(DEBUG_CXXFLAGS) $(DEBUG_CPPFLAGS) -I $(CONDA_PREFIX)/include/cspice -I $(CONDA_PREFIX)/include/qt/QtCore -I $(CONDA_PREFIX)/include/qt -c -o $@ $<

isis_camera: isis_camera.o
	$(CXX) $(DEBUG_CXXFLAGS) $(LDFLAGS) -L /lib64 -o $@ $< -lisis -llro -lLroNarrowAngleCamera -lQt5Core -lGL -lGLX -lGLdispatch
