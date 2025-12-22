This project contains the Maude interpreter source tree.

## Maude Overview

Maude is a high-performance reflective language and system supporting both equational and rewriting logic specification and programming for a wide range of applications. Maude has been influenced in important ways by the OBJ3 language, which can be regarded as an equational logic sublanguage. Besides supporting equational specification and programming, Maude also supports rewriting logic computation.

Rewriting logic is a logic of concurrent change that can naturally deal with state and with concurrent computations. It has good properties as a general semantic framework for giving executable semantics to a wide range of languages and models of concurrency. In particular, it supports very well concurrent object-oriented computation. The same reasons making rewriting logic a good semantic framework make it also a good logical framework, that is, a metalogic in which many other logics can be naturally represented and executed.

Maude supports in a systematic and efficient way logical reflection. This makes Maude remarkably extensible and powerful, supports an extensible algebra of module composition operations, and allows many advanced metaprogramming and metalanguage applications. Indeed, some of the most interesting applications of Maude are metalanguage applications, in which Maude is used to create executable environments for different logics, theorem provers, languages, and models of computation.

## Get Maude

The latest version of the Maude distribution can be downloaded from the [releases](https://github.com/maude-lang/Maude/releases) section of this repository. Some previous stable and development versions are also available there, together with a summary of their main changes. More detailed notes for each development release are kept in the [doc](doc) directory.

Instructions on how to build Maude from source can be found in the [INSTALL](INSTALL) file.

## What's new in Maude 3.5

* Term disjunctions for vu-narrow
* Variant folding for vu-narrow
* Show frontier/most general states/path for vu-narrow
* States may now subsume their ancestors in vu-narrow
* Extra built-in operations for strings
* Initial equality operator handles more cases
* Reading from stdin steam supports continuation lines
* New `LTL+` and `MODEL-CHECKER+` modules for easily obtaining witnesses of LTL properties

## Documentation

The Maude manual for Maude 3.5.1 is available in [PDF format](https://maude.cs.illinois.edu/manual.pdf)
and in [HTML](https://maude.lcc.uma.es/maude-manual/).
The source code for most of the examples used in the manual, together with the examples from the Maude book,
are available [here](https://maude.cs.illinois.edu/manual-book-examples.zip).

Additional information on background, applications, and previous versions is available at its [official web site](https://maude.cs.illinois.edu).

For help using Maude 3:	maude-help@maude.cs.uiuc.edu

Report bugs to: maude-bugs@maude.cs.uiuc.edu

## Copyright

Copyright 1997-2025 SRI International, Menlo Park, CA 94025, USA.

The Maude 3 interpreter is free software. See the file [COPYING](COPYING) for copying permission.
