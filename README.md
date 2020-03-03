# NUCES Graph
## C based Graph Theory Library

**NUCES Graph** is a simple C based library that can be used to analyze small to
medium sized graph networks. The code in the library is continuously improved
and developed by students (as a learning effort) taking the course *CS629
Networks & Graph Theory* at the *National University of Computer and Emerging
Sciences (NUCES), Peshawar, Pakistan*.

Example usage for the library is given below:

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

Addition of duplicate vertices and edges is not supported in the default
methods. Once the graph is constructed, it can be viewed using a number of display
options, such as printing on screen as a list sing `show(&G)`, exporting to pdf using
`showDot(&G)`, exporting to dimac using `exportDimac(&G)`, and so on.

For compilation of the library, run `make` and `make install`. For compiling
your code, use `gcc code.c -lNucesGraph`. Specify the include and linking file
using `-I` and `-L` if necessary.

The default internal data structure for holding graph information is the double
linked list. Adjacency matrices can be used by calling `adjacencyMatrix(&G)`,
and then being accessible as `G->adjacency_matrix`. Incidence matrices can be
used by calling `incidenceMatrix(&G)` and then being accessible as
`G->incidence_matrix`

For algorithms, refer to the **tests** folder which contains some example
implementations.


