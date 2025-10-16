# Install conda environment
# --------------------------------------
# Things the user can change. Note you can change this by creating a Makefile.local
# to set these variables, or just set it on the make line.

# Should we install the latest conda packages, or the "pixi.lock" set known to
# work (which will often lag the latest conda packages). Most of the time
# latest should work fine, but there might be the occasional breakage (e.g.,
# conda moves to a newer version of python and our various packages haven't
# been rebuilt yet the newer python, a python package update breaks
# something).
# ---------------------------------------------------------------------------
PIXI_LOCKED=yes

mkfile_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
# Location of environment we create
# ---------------------------------------------------------------------------
ENV_DIR=$(mkfile_dir)../../geocal-build/build-pixi

# Location of afids conda packages (you can check this out from
# git@github.jpl.nasa.gov:Cartography/afids-conda-package.git
# ---------------------------------------------------------------------------
CONDA_PACKAGE_DIR=$(mkfile_dir)../../afids-conda-package/afids-conda-channel

# Do we have the MSP (Mensuration Service Program) software and want to
# wrap it?
# ---------------------------------------------------------------------------

MSP_INSTALL=no

# Location of MSP software, if we are installing
# ---------------------------------------------------------------------------

MSP_PREFIX=need_to_Set

# Do we have the ISIS install, and want to use it?
# ---------------------------------------------------------------------------

HAVE_ISIS=no

# Location of ISIS software and data
# ---------------------------------------------------------------------------

ISISROOT=/opt/isis
ISISDATA=/opt/afids/data/isisdata

# Include a Makefile.local to override things, if found
# ---------------------------------------------------------------------------

-include Makefile.local

# ---------------------------------------------------------------------------
# Past this point are the rules, shouldn't need to modify these
# ---------------------------------------------------------------------------

OS := $(shell uname -s)

ifeq ($(OS),Darwin)
  PIXI_LOCK_DIR:=$(mkfile_dir)/osx
else
  PIXI_LOCK_DIR:=$(mkfile_dir)/linux
endif

ifeq '$(PIXI_LOCKED)' 'yes'
$(ENV_DIR)/pixi.toml: $(PIXI_LOCK_DIR)/pixi.toml
	sed s"|fake-afids-conda-channel|$(CONDA_PACKAGE_DIR)|"g $< > $@

$(ENV_DIR)/pixi.lock: $(PIXI_LOCK_DIR)/pixi.lock
	sed s"|fake-afids-conda-channel|$(CONDA_PACKAGE_DIR)|"g $< > $@

PIXI_FILE_INSTALL= $(ENV_DIR)/pixi.toml $(ENV_DIR)/pixi.lock

PIXI_FILE_DEPEND=$(PIXI_LOCK_DIR)/pixi.toml $(PIXI_LOCK_DIR)/pixi.lock

PIXI_INSTALL_COMMAND= pixi install --frozen --manifest-path $(ENV_DIR) 
else
$(ENV_DIR)/pixi.toml: $(mkfile_dir)pixi.toml
	sed s"|fake-afids-conda-channel|$(CONDA_PACKAGE_DIR)|"g $< > $@

PIXI_FILE_INSTALL= $(ENV_DIR)/pixi.toml

PIXI_FILE_DEPEND=$(mkfile_dir)pixi.toml

PIXI_INSTALL_COMMAND= pixi install --manifest-path $(ENV_DIR) 

endif

ifeq '$(MSP_INSTALL)' 'yes'

PIXI_INSTALL_COMMAND+= && pixi run --manifest-path $(ENV_DIR) bash -c "\$$CONDA_PREFIX/bin/conda env config vars set MSP_DATA_DIR=$(MSP_PREFIX)/data MSPCCS_DATA=$(MSP_PREFIX)/data CSM_PLUGIN_DIR=$(MSP_PREFIX)/plugins/"
EXTRA_CONFIG=--with-msp=$(MSP_PREFIX)
else
EXTRA_CONFIG=
endif

ifeq '$(HAVE_ISIS)' 'yes'

PIXI_INSTALL_COMMAND+= && pixi run --manifest-path $(ENV_DIR) bash -c "\$$CONDA_PREFIX/bin/conda env config vars set ISISROOT=$(ISISROOT) ISISDATA=$(ISISDATA)"

endif
$(ENV_DIR): $(PIXI_FILE_DEPEND)
	-rm -rf $(ENV_DIR)
	pixi init -c conda-forge -c $(CONDA_PACKAGE_DIR) $(ENV_DIR)
	rm $(ENV_DIR)/pixi.toml
	$(MAKE) $(PIXI_FILE_INSTALL)
	pixi config set --local run-post-link-scripts insecure --manifest-path $(ENV_DIR)
	$(PIXI_INSTALL_COMMAND)
	- pixi task rm configure --manifest-path $(ENV_DIR)
	pixi task add configure "$(mkfile_dir)/../configure --prefix=\$$CONDA_PREFIX --enable-maintainer-mode $(EXTRA_CONFIG)" --manifest-path $(ENV_DIR)

# Recreate the environment, deleting if already there
# ------------------------------------------------------------
recreate-env:
	-rm -rf $(ENV_DIR)
	$(MAKE) create-env

# Build the conda envirornment, if needed or out of date.
# ------------------------------------------------------------
create-env: $(ENV_DIR)

# Update lock files, from existing environment
# --------------------------------------------
update-lock:
	sed "s|$(CONDA_PACKAGE_DIR)|fake-afids-conda-channel|"g $(ENV_DIR)/pixi.toml > $(PIXI_LOCK_DIR)/pixi.toml
	sed "s|$(CONDA_PACKAGE_DIR)|fake-afids-conda-channel|"g $(ENV_DIR)/pixi.lock > $(PIXI_LOCK_DIR)/pixi.lock

