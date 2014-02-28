kleinian
========

GLUT Kleinian group visualizer version 0.01

make from the command line; edit makefile to reflect local values for X11 and OpenGL

Usage: from the command line, type

* ./kleinian -t *filename.tri* to read (precomputed) triangle mesh from file
* ./kleinian -g *filename.grp* to read semigroup generators and triangle orbits from file
* ./kleinian -e to run a (built-in) example

Some sample .grp files are in the directory *example_groups*

When in GLUT mode,

* press [t] to save triangulation to *output_file.tri*
* press [g] to save group to *output_file.grp*
* press [e] to save figure as .eps file *output.eps*
* press [command-q] to quit

![Schottky example](https://raw.github.com/dannycalegari/kleinian/master/example_pictures/schottky.png)