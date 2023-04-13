This project contains the Maude interpreter source tree.

## Maude Overview

Maude is a high-performance reflective language and system supporting both equational and rewriting logic specification and programming for a wide range of applications. Maude has been influenced in important ways by the OBJ3 language, which can be regarded as an equational logic sublanguage. Besides supporting equational specification and programming, Maude also supports rewriting logic computation.

Rewriting logic is a logic of concurrent change that can naturally deal with state and with concurrent computations. It has good properties as a general semantic framework for giving executable semantics to a wide range of languages and models of concurrency. In particular, it supports very well concurrent object-oriented computation. The same reasons making rewriting logic a good semantic framework make it also a good logical framework, that is, a metalogic in which many other logics can be naturally represented and executed.

Maude supports in a systematic and efficient way logical reflection. This makes Maude remarkably extensible and powerful, supports an extensible algebra of module composition operations, and allows many advanced metaprogramming and metalanguage applications. Indeed, some of the most interesting applications of Maude are metalanguage applications, in which Maude is used to create executable environments for different logics, theorem provers, languages, and models of computation.


## Maude 3.3.1

* Restrictions on pseudo-parameters
* More overparsing
* Many bug fixes

## Maude 3.3

* Support for object-oriented syntactic sugar (omods/oths)
* Parameterized constants and pconst constants
* generated-by importation mode
* show path states command
* show processed view command
* Better handling of runaway statements and commands
* More overparsing
* Many bug fixes

## Maude 3.2.2

* fixed bug where multiple instantiations (say via a theory-view) was generating bad internal names for sorts
* fixed bug where the first sort in a kind printed by show mod was including backquotes
* show desugared is now a documented feature
* desugar omod to mod, oth to th
* fixed bug where filtered variant unify crashed in the degenerate case where there were no variables
* fixed issue where SIGSTKSZ is no longer guaranteed to be a compile-time constant in glibc
* fixed issue with fileTest failing in environments when first user file handle is something other than 3

## Maude 3.2.1

* use filtered variant unification for unifying with goal when filter flag is passed to vu-narrow

## Maude 3.2

* Support for filtered variant unification in vu-narrow command
* Several improvements in unification modulo several axioms
* Several improvements of the external Maude I/O objects
* A command flag for the depth of searching for A/AU unifiers
* Some bugs fixed.

## Documentation

The Maude manual for Maude 3.2.1 is available in [PDF format](http://maude.cs.illinois.edu/w/images/b/bf/Maude-3.2.1-manual.pdf) and in [HTML](http://maude.lcc.uma.es/maude321-manual-html/maude-manual.html). 
The source code for most of the examples used in the manual, together with the examples from the Maude book, are available [here](http://maude.cs.illinois.edu/w/images/b/bf/Maude-3.1-manual-book-examples.zip).

Additional information on background, applications, and previous versions is available at its [official web site](http://maude.cs.uiuc.edu). 

For help using Maude 3:	maude-help@maude.cs.uiuc.edu

Report bugs to:		maude-bugs@maude.cs.uiuc.edu

## Copyright

Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

The Maude 3 interpreter is free software. See the file COPYING for copying permission.
