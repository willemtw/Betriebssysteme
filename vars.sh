# Set up environment vars to run on NixOS
export LDFLAGS="-L/run/current-system/sw/lib"
export CPPFLAGS="-I/run/current-system/sw/include"
export PKG_CONFIG_PATH="/run/current-system/sw/lib/pkgconfig"
export LD_LIBRARY_PATH="$NIX_LD_LIBRARY_PATH:$LD_LIBRARY_PATH"
