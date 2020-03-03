# NUCES Graph
## C based Graph Theory Library

**NUCES Graph** is a simple C based library that can be used to analyze small to
medium sized graph networks. The code in the library is continuously improved
and developed by students (as a learning effort) taking the course *CS629
Networks & Graph Theory* at the *National University of Computer and Emerging
Sciences (NUCES), Peshawar, Pakistan*.

Example usage for the library is given below:

```c
nGraph G = newGraph("G");

int i;
for (i = 0; i < 5; i++)
	addVertex(&G, i);            // creates five vertices

for (i = 0; i < 5; i++)
	addRandomEdge(&G, 1);        // adds five random edges, each of weight 1

addEdge(&G, 0, 1, 1);          // manual edge b/w vertices 0 and 1, of weight 1
```

Once the graph is constructed, it can be viewed using a number of display
options, such as printing on screen using `show(&G)`, exporting to pdf using
`showDot(&G)`, exporting to dimac using `exportDimac(&G)`, and so on.

The default internal data storage for holding graph information is the double
linked list. Adjacency matrices can be used by calling `adjacencyMatrix(&G)`,
and then being accessible as `G->adjacency_matrix`. Incidence matrices can be
used by calling `incidenceMatrix(&G)` and then being accessible as
`G->incidence_matrix`

For algorithms, refer to the **tests** folder which contains some example
implementations.


