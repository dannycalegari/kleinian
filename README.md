kleinian
========

GLUT Kleinian group visualizer version 0.01

make from the command line; edit makefile to reflect local values for X11 and OpenGL

usage
-----

From the command line, type

* ./kleinian -t *filename.tri* to read (precomputed) triangle mesh from file
* ./kleinian -g *filename.grp* to read semigroup generators and triangle orbits from file
* ./kleinian -e to run a (built-in) example

Some sample .grp files are in the directory *example_groups*

GLUT mode
---------

When in GLUT mode,

* press [t] to save triangulation to *output_file.tri*
* press [g] to save group to *output_file.grp*
* press [e] to save figure as .eps file *output.eps*
* press [command-q] to quit

manual
------

The manual (i.e. the file manual.pdf) contains a much more detailed description
of file formats and user interface.

copyright
---------

This program is released under the terms of the GNU GPL version 3.0. See the
attached license file.


![Schottky example](https://raw.github.com/dannycalegari/kleinian/master/example_pictures/schottky.png)
