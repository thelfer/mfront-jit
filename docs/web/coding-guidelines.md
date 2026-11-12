---
title: Coding guidelines
author: Thomas Helfer
date: 12/11/2026
lang: en-EN
---

This page gathers the coding conventions used in the `MFrontJIT`
sources. It is written for **new developers** and for **coding
agents**, which are expected to produce code indistinguishable from the
existing sources.

`MFrontJIT` is a small, separate library of the `TFEL`/`MFront`
ecosystem: it follows the same conventions as the
[`TFEL` coding guidelines](https://github.com/thelfer/tfel/blob/master/docs/web/coding-guidelines.md),
restricted to what is meaningful here, with the few local differences
listed in [Differences with `TFEL`](#differences-with-tfel). When this
page is silent, the `TFEL` rules apply.

Rules stated with **shall** are mandatory. When a rule is only
partially satisfied by the existing sources, this is explicitly
mentioned: a coding agent shall apply the rule to the code it writes or
modifies, but shall not rename unrelated entities as a side effect.

# Quick reference

| Topic | Rule |
|---|---|
| Formatting | `clang-format` with the `.clang-format` file at the root of the repository |
| File name | same name as the main type it declares, `UpperCamelCase`, extension `.hxx`/`.cxx` |
| File header | doxygen comment with `\file`, `\brief`, `\author`, `\date` (no copyright statement) |
| Include guard | `LIB_MFRONT_JIT_<NAME>_HXX`, closing `#endif` repeats the macro name |
| Include order | standard headers, then `TFEL` headers, then `MFront` headers, then `MFrontJIT` headers, then own header |
| Types | `UpperCamelCase`, always `struct`, public members first |
| Methods and free functions | `camelCase`, verb first |
| Data members and variables | `snake_case` |
| Namespace | `mfront::jit`; implementation details in `mfront::jit::internals` |
| Documentation | every public type, function and member documented in the `.hxx`, with `doxygen` |
| Error handling | `tfel::raise` / `tfel::raise_if`, `noexcept(false)`, no error reporting macro |
| Exported symbols | `MFRONTJIT_EXPORT` before `struct` or before the return type |
| Closing comments | `}  // end of <name>`, with two spaces |
| New files | shall be added to `src/CMakeLists.txt` and, for headers, to `include/CMakeLists.txt` |

# Tooling

## Formatting

The `.clang-format` file at the root of the repository is the one used
by `TFEL`:

~~~~{.yaml}
Language: Cpp
Standard: Cpp11
BasedOnStyle: Google
ColumnLimit: 80
Cpp11BracedListStyle: true
BinPackParameters: false
NamespaceIndentation: All
SortIncludes: false
~~~~

The main consequences are:

- a hard limit of **80 columns**,
- a **2 space** indentation, and everything inside a namespace is
  indented (`NamespaceIndentation: All`),
- function arguments are either all on the same line or **one per
  line** aligned on the opening parenthesis,
- **`SortIncludes: false`**: the include order is maintained manually
  (see [Including headers](#including-headers)).

This repository has no `format.sh` script; format the sources you
touched:

~~~~{.sh}
clang-format -i include/MFrontJIT/Compile.hxx src/Compile.cxx
~~~~

The whole tree is currently formatted: a `clang-format` run shall
produce no other modification than those of the files which were really
changed.

## API documentation

The `doxygen` documentation is generated from the `.hxx` files only, as
in `TFEL`, using `docs/doxygen/Doxyfile.in`:

~~~~{.sh}
cmake -S . -B ../build -DCMAKE_BUILD_TYPE=Release \
      -DTFEL_DIR=/path/to/tfel/install -Dgenerate_documentation=ON
cmake --build ../build --target doc
~~~~

The `HTML` pages are written in `../build/docs/doxygen/html` and are
installed in `share/doc/mfront-jit` when the `doxygen` component is
installed. Two points are worth noting:

- `EXTRACT_ALL` is `NO` and `WARN_IF_UNDOCUMENTED` is `YES`: an
  undocumented public entity is a defect,
- `WARN_AS_ERROR` is `YES`: a missing comment, a `\param` which does not
  name a real argument or a `\see` with a bad spelling breaks the `doc`
  target, which is why documenting a new public entity and generating the
  documentation are part of the same change,
- `PREDEFINED` expands `MFRONTJIT_EXPORT` to nothing, so the visibility
  macro does not appear in the signatures of the generated pages. Do not
  add `MSC_VER` to `PREDEFINED`: it would change the default value of
  `JITOptions::generator` shown by the documentation.

## Verification

There is no test suite, no continuous integration, no `cppcheck` or
`clang-tidy` configuration in this repository. The available
verifications are building the library against a `TFEL` install, and
generating the documentation as described above:

~~~~{.sh}
cmake -S . -B ../build -DCMAKE_BUILD_TYPE=Release \
      -DTFEL_DIR=/path/to/tfel/install
cmake --build ../build
~~~~

`git commit` messages are plain sentences describing the change.

# Source files

## Extensions and roles

| Extension | Role |
|---|---|
| `.hxx` | interface: declarations, types, `MFRONTJIT_EXPORT` function declarations, doxygen documentation |
| `.cxx` | definitions, `static` and `internals` namespace helpers |

The `.ixx` files of `TFEL` are not used in this repository; introduce
one only if a header needs inline or template definitions. `.h` and
`.c` files are not used either.

## Naming

A file is named after the main type it declares, in `UpperCamelCase`:
`JITOptions.hxx`, `CompiledBehaviourDescription.cxx`. A file declaring a
free function is named after that function: `Compile.hxx` declares
`compile`.

## Declaring a new file

A new file which is not declared is not a part of the library:

- add the `.cxx` file to the `add_library(MFrontJIT SHARED ...)` list of
  `src/CMakeLists.txt`,
- add the `.hxx` file to the `install(FILES ...)` list of
  `include/CMakeLists.txt`.

Both lists are explicit; nothing is gathered by a glob and no
`<Target>_SOURCES` variable is used.

# File header

Every source file and every header shall start by a `doxygen` comment:

~~~~{.cxx}
/*!
 * \file   include/MFrontJIT/JITOptions.hxx
 * \brief  This file introduces the JITOptions structure.
 * \author Thomas Helfer
 * \date   03/06/2024
 */
~~~~

The tags are always in this order: `\file`, `\brief`, `\author`,
`\date`.

- `\file` is the path of the file, relative to the root of the source
  tree.
- `\brief` is a one line description of the file. It shall not be left
  empty in new files, and shall not be followed by trailing spaces.
- `\date` is the creation date, in the `DD/MM/YYYY` format.

Unlike `TFEL`, the source files of this repository carry **no**
`\copyright` statement: the licence is the GNU GPL v3, whose verbatim
text is the `LICENSE` file at the root of the repository, and no source
file has a copyright paragraph. Do not copy the `CEA`/`EDF` copyright
statement of the `TFEL` sources here.

# Include guards

Headers are protected by an include guard built from the path of the
header inside the `include` directory, in all upper case:
`MFrontJIT/Compile.hxx` gives `LIB_MFRONT_JIT_COMPILE_HXX`.

~~~~{.cxx}
#ifndef LIB_MFRONT_JIT_COMPILE_HXX
#define LIB_MFRONT_JIT_COMPILE_HXX
...
#endif /* LIB_MFRONT_JIT_COMPILE_HXX */
~~~~

The closing `#endif` shall always repeat the name of the macro in a
block comment; the same applies to every conditional compilation block.
`#pragma once` is not used and `.cxx` files have no include guard.

The guard is built from the path of the header inside the `include`
directory: `MFrontJIT/Compile.hxx` gives `LIB_MFRONT_JIT_COMPILE_HXX`.

# Including headers

## Order

The order of the `#include` directives is meaningful and shall be
preserved:

1. standard headers, in angle brackets,
2. `TFEL` headers (`TFEL/Raise.hxx`, `TFEL/Utilities/Data.hxx`, ...),
3. `MFront` headers (`MFront/AbstractDSL.hxx`, ...),
4. `MFrontJIT` headers,
5. the header implemented by the current `.cxx` file, last.

~~~~{.cxx}
#include <map>
#include <vector>
#include <string>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/GeneratorOptions.hxx"
~~~~

A blank line between the standard headers and the project headers is
optional and rarely used here. Since `SortIncludes` is disabled, adding
an include shall not trigger a reordering of the existing ones.

## Syntax

- Project headers are included with quotes and a path relative to the
  `include` directory of their library:
  `#include "MFront/BehaviourDescription.hxx"`,
  `#include "MFrontJIT/Compile.hxx"`. Relative paths (`"../Foo.hxx"`)
  and angle brackets are never used for project headers.
- `TFEL/Raise.hxx` is the conventional first project header of a `.cxx`
  file which reports errors.
- The `#include` directives of a header are placed **after** its include
  guard.

# Namespaces

Public entities live in `mfront::jit`, which is closed by a comment
giving its full name:

~~~~{.cxx}
namespace mfront::jit {

  ...

}  // end of namespace mfront::jit
~~~~

Implementation details are put in an `internals` sub-namespace and made
`static`, so that the exported entry point stays a thin wrapper:

~~~~{.cxx}
  namespace internals {

    static TargetsDescription compile(std::string_view mfront_path,
                                      const JITOptions& options) {
      ...
    }  // end of compile

  }  // end of namespace internals
~~~~

`using namespace` is forbidden in headers.

# Naming conventions

## Types

Types, enumerations and type aliases are in `UpperCamelCase`:
`JITOptions`, `CompiledBehaviourDescription`. Aliases use `using`, never
`typedef`, and are documented by the conventional comment
`//! a simple alias`.

## Methods and free functions

Methods and free functions are written in `camelCase`, starting with a
verb: `compile`, `getCompiledBehaviourDescription`. The conventional
prefixes are `get`/`set`, `is`/`has`, `check`, `compute`,
`add`/`insert`/`remove`, `to`.

## Data members and variables

Data members, variables and non trivial parameters are written in
`snake_case`: `mfront_path`, `dsl_options`, `generator_options`,
`internal_state_variables`, `nb_of_integration_points`. Multi-word names
use underscores, never internal capitals. Short lived variables and
parameters whose meaning is given by the `\param` documentation use a
single word: `n`, `s`, `h`, `mpath`.

No prefix (`_`, `m_`, `s_`) is ever used.

## Enumerations and macros

Enumerations are in `UpperCamelCase`, their enumerators in
`ALL_UPPER_CASE`. In this repository, the only enumeration is a local,
anonymous one, used as a defaulted member of `JITOptions`:

~~~~{.cxx}
    //! \brief generator to be used
    const enum { CMAKE, MAKE } generator = MAKE;
~~~~

Macros are in `ALL_UPPER_CASE` and prefixed by `MFRONTJIT_`
(`MFRONTJIT_EXPORT`, `MFRONTJIT_VISIBILITY_EXPORT`, ...).

Conditional compilation for `MSVC` uses the compiler provided macro
`_MSC_VER`, as `TFEL` does. The `#ifdef MSC_VER` which selects the
default value of `JITOptions::generator` matches no predefined macro and
is therefore always false; it is a known defect of that header, not a
spelling to reuse.

# Structures and functions

`struct` shall be used in place of `class`, with public members
declared first, so that most types have no `public:` label at all.

Data transfer objects are **aggregates of `const` members with default
member initialisers**, which callers build with designated
initialisers. This is the idiomatic shape of this library, and shall not
be turned into a type with setters:

~~~~{.cxx}
  struct JITOptions {
    //! \brief generator to be used
    const enum { CMAKE, MAKE } generator = MAKE;
    //! \brief target to be built
    const std::string target = "all";
    //! \brief interface used
    const std::string interface = "generic";
    //! \brief options passed to the Domain Specific Language
    const tfel::utilities::DataMap dsl_options;
    ...
  };  // end of JITOptions
~~~~

~~~~{.cxx}
mfront::jit::compile(
    "Inconel600_YoungModulus.mfront",
    {.dsl_options = {{"parameters_as_static_variables", true},
                     {"overriding_parameters",
                      tfel::utilities::DataMap{{"Temperature", 293.15}}}}});
~~~~

An aggregate returned by value and whose result shall not be discarded
is marked `[[nodiscard]]` on the type:

~~~~{.cxx}
  struct [[nodiscard]] CompiledBehaviourDescription {
    //! \brief gradients
    const std::vector<Gradient> gradients;
    ...
  };  // end of CompiledBehaviourDescription
~~~~

Non-copyable types explicitly disable the special members rather than
omitting them. Leaf types are declared `final`. Data members and methods
are qualified by `this->` in definitions, as in `TFEL`.

# Documentation

All public types, functions and members shall be documented with
`doxygen`, in the `.hxx` file; a `.cxx` file is never documented.
Documentation uses backslash commands (`\brief`, never `@brief`):

- `/*! ... */` for types, functions and non-trivial members,
- `//! ...` for one line comments: aliases, getters, data members,
  enumerators.

`///` and `/** ... */` are not used. Parameters are annotated with their
direction and their name is followed by a colon:

~~~~{.cxx}
  /*!
   * \brief compile an MFront implementation
   * \param[in] mfront_path: path to an MFront implementation
   * \param[in] options: options used to compile the implementation
   */
  MFRONTJIT_EXPORT TargetsDescription
  compile(std::string_view mfront_path,
          const JITOptions& options) noexcept(false);
~~~~

`\return` documents the returned value and is often the only tag of a
trivial accessor. The conventional one line comments are
`//! a simple alias`, `//! \brief default constructor`,
`//! \brief copy constructor (disabled)`, `//! destructor`,
`//! \return the name of the class`. The short comment forms used here,
`//! \brief gradients`, are also acceptable for data members.

The other conventions of this library are:

- `\see` refers to an entity of the same library (`\see JITOptions`,
  `\see getCompiledBehaviourDescription`),
- the conditions of use, the side effects on the global state and the
  fact that an exception may be thrown are stated with `\note`. `\throw`
  is not a `doxygen` command and would be reported as a documentation
  error,
- an example is written in the comment itself, between tildes:
  `~~~{.cxx}` ... `~~~`,
- a macro is documented by a single `\def` block, written **before** the
  conditional block which defines it, and not in front of each of its
  definitions: `doxygen` merges the comments of a macro defined in
  several branches and keeps the last one, which would otherwise describe
  the platform specific behaviour of a dead branch (see
  `include/MFrontJIT/Config.hxx`).

# Language conventions

- `const` is written before the type and is applied to parameters passed
  by value, in the declarations as well as in the definitions.
- Local variables are `const` by default: `const auto` is the norm, and
  so is `for (const auto& x : ...)`. Unused structured bindings are
  silenced with `static_cast<void>(x);`.
- Variables, members and parameters are not declared `long` nor
  `long long`.
- `[[nodiscard]]` is applied to non-void queries, `noexcept` to what
  cannot throw; the exported entry points are explicitly declared
  `noexcept(false)` because they report errors by throwing.
- Parameters of a documented function are **named** in its declaration,
  even if the definition is elsewhere: `doxygen` binds `\param` to the
  parameters of the declaration and an unnamed parameter makes the `doc`
  target fail.
- IIFE lambdas are used to build a local in a single expression, and
  large expressions are followed by a `//` comment when their role is
  not obvious:

~~~~{.cxx}
    auto gradients = [&bd] {
      auto r = std::vector<Gradient>{};
      for (const auto [g, thf] : bd.getMainVariables()) {
        static_cast<void>(thf);
        r.push_back(g);
      }
      return r;
    }();
~~~~

## Closing comments

Namespaces, types, functions and enumerations are closed by a comment
giving their name, with **two spaces** before the comment:

~~~~{.cxx}
  }  // end of getCompiledBehaviourDescription
};  // end of CompiledBehaviourDescription
}  // end of namespace mfront::jit
~~~~

Control flow blocks (`if`, `for`, `while`) are not annotated.

# Exported symbols

The library is a single shared library, `MFrontJIT`. Every type and
function of the public interface is annotated with `MFRONTJIT_EXPORT`,
defined in `include/MFrontJIT/Config.hxx`; the macro is written
immediately before `struct`, or before the return type of a function,
after the attributes.

`MFRONTJIT_EXPORT` is the spelling used here; the
`MFRONTJIT_VISIBILITY_IMPORT`, `MFRONTJIT_VISIBILITY_EXPORT` and
`MFRONTJIT_VISIBILITY_LOCAL` macros it expands to are defined in
`include/MFrontJIT/Config.hxx` and shall not be used directly by the
callers.

Templates and `static` helpers are not exported: they stay in the
`internals` namespace or are file local `static` functions.

# Error handling

There is no error reporting macro. Errors are reported with the
functions of `TFEL/Raise.hxx`:

~~~~{.cxx}
    if (options.interface.empty()) {
      tfel::raise(
          "getCompiledBehaviourDescription: no interface specified while "
          "treating '" +
          std::string{mpath} + "'");
    }
~~~~

- Error messages are written as `<entity>: <message>`, split into
  adjacent string literals to respect the 80 columns limit, and
  completed by string concatenation when they contain dynamic parts.
- `tfel::raise_if` is preferred to `if` followed by `tfel::raise` when
  the test is a simple condition.
- The prohibition of exceptions which applies to `TFEL/Math` and
  `TFEL/Material` does not apply here: `MFrontJIT` is host only code and
  throwing is the only error channel of the `MFront` libraries it calls.
- An internal inconsistency is reported as
  `"...: internal error ..."`, as done for the failed `dynamic_cast` of
  `getCompiledBehaviourDescription`.
- Global state touched on the way out of an error is restored
  explicitly; see how `compile` saves and resets
  `SearchPathsHandler::getRegistredPaths()` in a `try`/`catch (...)`
  block. The same care shall be taken for any new global state of
  `MFront` this library would touch, which is also why `compile` shall
  not be called concurrently.

# Differences with `TFEL`

The following parts of the `TFEL` guidelines do not apply to this
repository:

- **GPU and `SYCL` portability**: no `TFEL_HOST_DEVICE`, `TFEL_HOST`,
  `TFEL_DEVICE`, `TFEL_INLINE`, nor the related code restrictions.
- **`.ixx` files, `Forward/` and `Internals/` sub-directories**: not
  used; the whole public interface is the four headers of
  `include/MFrontJIT`.
- **Factories and proxies**: this library declares none; adding a type
  to an existing `MFront` hierarchy is not the purpose of this library.
- **`C` sources**: none, so the `snake_case` prefixed `C` names and the
  `/* end of ... */` block comments do not apply.
- **Unit tests**: there is no `tests` directory and no `TFEL/Tests`
  glue. If a test is added, it shall use the `tfel::tests::TestCase`
  layout of `TFEL`, declare its own `add_executable` in a
  `tests/CMakeLists.txt` added by `add_subdirectory(tests)`, and be
  signalled to the reviewer as new infrastructure.
- **Compatibility modes**: the mechanism driven by `getCompatibilityMode`
  belongs to `MFront` and the `TFEL` libraries. It has no direct use
  here, but a change which silently modifies what `compile` produces
  (default generator, default interface, default target) changes the
  assets users generate and shall be discussed explicitly, not merged as
  a side effect.
- **Build system helpers**: this repository calls `add_library`,
  `target_include_directories`, `target_link_libraries` and `install`
  directly; it does not use `tfel_library`, `install_header`, nor the
  `enable-static` variants. `CMake` conditionals are still always closed
  by their argument: `else(use_cpm)`, `endif(use_cpm)`. The options are
  spelled with an underscore, `use_cpm` and `generate_documentation`,
  instead of the dash separated `TFEL` style; keep these spellings, `use_cpm`
  being the one used by the existing build directories.
- `TFEL_COMPILER_FLAGS` is applied `PRIVATE` to the library, so it is
  not propagated to the users of `MFrontJIT`; add the flags an
  application needs on its own target.

# Documentation of the website

The `doxygen` reference manual of the library and the pages of the
website are two separate things: the former is generated from the
`.hxx` files as described in [API documentation](#api-documentation),
the latter are the markdown files of `docs/web`, described here.

Pages are markdown files of `docs/web`, processed by `pandoc`. The
conventions are the ones of `TFEL`:

- a `YAML` header with at least `title`, `author`, `date` and `lang`;
  `mfront-jit.md` adds the keys used to produce the PDF version
  (`documentclass`, `geometry`, `papersize`, `figPrefixTemplate`, ...),
- a blank line after every section title, and no bold markers inside
  titles,
- code blocks use tildes, with the `cxx` language token rather than
  `cpp`: `~~~~{.cxx}` ... `~~~~`, for shell blocks `~~~~{.sh}`,
- `TFEL`, `MFront` and `MFrontJIT` are written in code font when they
  refer to the software.

Unlike `TFEL`, this repository has no `docs/web/CMakeLists.txt` and no
HTML template: adding a page here is not enough to publish it, the page
must also be declared in the documentation of the project which
integrates this library.

# Checklist before submitting

Before submitting a modification, or a patch generated by a coding
agent, check that:

1. the file header of every new file has the four `\file`, `\brief`,
   `\author`, `\date` tags, with a non empty `\brief` and no copyright
   statement,
2. the includes are ordered as specified and the header implemented by a
   `.cxx` file is included last,
3. headers use a `LIB_MFRONT_JIT_<NAME>_HXX` guard whose name is repeated
   on the closing `#endif`,
4. types are `struct`, public members are declared first, options
   structures stay aggregates of `const` members,
5. types are `UpperCamelCase`, functions are `camelCase` and verb first,
   data members and variables are `snake_case`,
6. every public type, function and member is documented in the `.hxx`,
   with `\param[in]`/`\param[out]` and `\return`, and the parameters of
   the documented declarations are named,
7. namespaces, types, functions and enumerations are closed by a
   `}  // end of ...` comment with two spaces,
8. new public symbols are annotated `MFRONTJIT_EXPORT`, internal helpers
   are `static` or in the `internals` namespace,
9. errors are reported by `tfel::raise`/`tfel::raise_if` with messages
   prefixed by the name of the entity, and global state is restored on
   the error path,
10. the new sources and headers are declared in `src/CMakeLists.txt` and
   `include/CMakeLists.txt`,
11. `clang-format` has been run on the files which were touched,
12. the library builds with the command of [Verification](#verification),
13. the documentation is generated without any warning, with the `doc`
   target of [API documentation](#api-documentation).
