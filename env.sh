
#!/bin/sh

#
# The location you specified with --prefix or where it installed
# replace this with your own pythia8 installation
export PYTHIA8=/Users/lihengne/local/llp/pythia8313

# Add Pythia8's bin directory (so you can use pythia8-config, etc.)
export PATH="$PYTHIA8/bin:$PATH"

export DYLD_LIBRARY_PATH="$PYTHIA8/lib:$DYLD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$PYTHIA8/lib:$LD_LIBRARY_PATH"

