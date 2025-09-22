## DOCUMENTATION
I'm not going to write special documentation until/unless I feel like it. If you need documentation,
read the header files and hope I wrote a comment above the function declaration.

## installation??
Running `make export` compiles the static library and exports it to `~/.local/include` and `~/.local/lib` for the header files and library file respectively.

The following lines are recommended to be in your shell-startup script (`.bashrc`, `.zshrc`, etc.):
``` sh
# add library to system library path so `-laplcore` works
export LIBRARY_PATH="$LIBRARY_PATH:$HOME/.local/lib"
# add headers to system include path so `#include <aplcore/structs/array.h>` and such works
export C_INCLUDE_PATH="$C_INCLUDE_PATH:$HOME/.local/include"
```

(the comments are obviously unnecessary)
The only extra compiler flag necessary is `-laplcore` for the linking phase.
