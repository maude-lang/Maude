This project contains the Maude interpreter source tree.

## Maude Overview

Maude is a high-performance reflective language and system supporting both equational and rewriting logic specification and programming for a wide range of applications. Maude has been influenced in important ways by the OBJ3 language, which can be regarded as an equational logic sublanguage. Besides supporting equational specification and programming, Maude also supports rewriting logic computation.

Rewriting logic is a logic of concurrent change that can naturally deal with state and with concurrent computations. It has good properties as a general semantic framework for giving executable semantics to a wide range of languages and models of concurrency. In particular, it supports very well concurrent object-oriented computation. The same reasons making rewriting logic a good semantic framework make it also a good logical framework, that is, a metalogic in which many other logics can be naturally represented and executed.

Maude supports in a systematic and efficient way logical reflection. This makes Maude remarkably extensible and powerful, supports an extensible algebra of module composition operations, and allows many advanced metaprogramming and metalanguage applications. Indeed, some of the most interesting applications of Maude are metalanguage applications, in which Maude is used to create executable environments for different logics, theorem provers, languages, and models of computation.

## Maude 3.2.1

The current version of Maude is 3.2.1. You can download it directly 
from the [Releases section](https://github.com/SRI-CSL/Maude/releases/tag/Maude3.2.1). The new features in this new version are 
basically around Maude's symbolic capabilities: 
* Support for filtered variant unification in vu-narrow command,
* Several improvements in unification modulo several axioms,
* Several improvements of the external Maude I/O objects,
* A command flag for the depth of searching for A/AU unifiers, and
* Some bugs fixed.

## Documentation

The Maude manual for Maude 3.2 is available in [PDF format](http://maude.cs.illinois.edu/w/images/b/bf/Maude-3.2.1-manual.pdf) and in [HTML](http://maude.lcc.uma.es/maude321-manual-html/maude-manual.html). 
The source code for most of the examples used in the manual, together with the examples from the Maude book, are available [here](http://maude.cs.illinois.edu/w/images/b/bf/Maude-3.1-manual-book-examples.zip).

Additional information on background, applications, and previous versions is available at its [official web site](http://maude.cs.uiuc.edu). 

For help using Maude 3:	maude-help@maude.cs.uiuc.edu

Report bugs to:		maude-bugs@maude.cs.uiuc.edu

## Copyright

Copyright 1997-2022 SRI International, Menlo Park, CA 94025, USA.

The Maude 3 interpreter is free software. See the file COPYING for
copying permission.
