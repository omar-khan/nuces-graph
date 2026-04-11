AM_CPPFLAGS = -I$(top_srcdir)/include
AM_LDFLAGS = $(top_builddir)/src/libNucesGraph.la

# Programs that are part of the test suite
TEST_PROGS = \
    binaryops \
    clique \
    eccentricity \
    generateGraphs \
    glpk \
    shortestPath \
    similarity \
    spectral \
    visual

# No-install programs (built only for testing)
check_PROGRAMS = $(TEST_PROGS)

binaryops_SOURCES = binaryops.c
clique_SOURCES = clique.c
eccentricity_SOURCES = eccentricity.c
generateGraphs_SOURCES = generateGraphs.c
glpk_SOURCES = glpk.c
shortestPath_SOURCES = shortestPath.c
similarity_SOURCES = similarity.c
spectral_SOURCES = spectral.c
visual_SOURCES = visual.c

# Custom tests target
.PHONY: tests
tests: $(check_PROGRAMS)
	@echo "Running tests..."
	@for t in $(TEST_PROGS); do \
		echo "Executing $$t..."; \
		./$$t || exit 1; \
	done
	@echo "All tests passed!"

clean-local:
	rm -f *.log *.dimac *.eps
