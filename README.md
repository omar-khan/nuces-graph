# NUCES Graph
## C based Graph Theory Library

**NUCES Graph** is a simple C based library that can be used to analyze small to
medium sized graph networks. The code in the library is continuously improved
and developed by students (as a learning effort) taking the course *CS629
Networks & Graph Theory* at the *National University of Computer and Emerging
Sciences (NUCES), Peshawar, Pakistan*.

### Installation

For compilation of the library, run `./configure.sh`, followed by `make`. This
will add relevant linking files to the *lib* folder.

For compilation of tests included in the library, run `make tests`, which are
then accessible in the *tests* folder. 

For a system-wide installation, run `make install` with super user privileges.
The installation defaults to */usr/local/lib* for linking files, and
*/usr/local/include* for the header files. To change the default location, pass 
the new location to configure script as `./configure.sh --prefix=/new/location`.

Lastly, configure the `LD_LIBRARY_PATH` environment variable to point to the
installation location. Otherwise, you have to specify this location every time
you run your code. 

### Example Usage

```c
#include <nucesGraph.h>

int main(int argc, char **argv)
{
    int i, def_weight = 1;

    nGraph G = newGraph("G");

    for (i = 0; i < 5; i++) {
        addVertex(&G, i);
    }

    for (i = 0; i < 5; i++) {
        addRandomEdge(&G, def_weight);
    }

    addEdge(&G, 0, 1, def_weight);
}
```

For compiling this code, run `gcc code.c -lNucesGraph`. Specify the include and
linking file locations using `-I` and `-L` if necessary (E.g. `gcc code.c -I/usr/local/include -L/usr/local/lib -lNucesGraph`).

To run the code, run directly as `./a.out`, or if the `LD_LIBRARY_PATH` is not
configured, as `LD_LIBRARY_PATH=/location/of/lib/folder ./a.out`.

### Documentation

The documentation is provided as doxygen generated pdf and html pages. The HTML
pages can be generated using `make doc` while the pdf can be generated using
`make docpdf`.

### Uninstall

The build directory can be cleaned using `make clean`. System wide installation
can be reversed by calling `make uninstall` with super user privileges. 
