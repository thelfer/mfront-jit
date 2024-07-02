---
title: The MFrontJIT library
author: Thomas Helfer
date: 2024
lang: en-EN
numbersections: true
documentclass: article
from: markdown+tex_math_single_backslash
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

The `MFrontJIT` library aims at simplifying the Just In Time compilation
of `MFront` files.

# The `compile` function

The `compile` function has the following prototype:

~~~~{.cxx}
TargetsDescription compile(std::string_view, const JITOptions&);
~~~~

The first argument is the path to the mfront implementation. It can be a
path to the a file or a path to an implementation in a `madnex` file
(see [this page](https://thelfer.github.io/tfel/web/madnex.html) for
details).

The second argument describes a list of options controling how this
implementation must be compiled.

## Example of usage

The following code compiles the `Inconel600_YoungModulus.mfront` file:

~~~~{.cxx}
mfront::jit::compile("Inconel600_YoungModulus.mfront", {});
~~~~

with the default options. In particular, the `generic` interface is
used.

This file describes the Young's modulus of an Inconel alloy as a
function of the temperature. The following line now compiles the same
file with various options:

~~~~{.cxx}
mfront::jit::compile(
    "Inconel600_YoungModulus.mfront",
    {.dsl_options = {{"parameters_as_static_variables", true},
                     {"overriding_parameters",
                      tfel::utilities::DataMap{{"Temperature", 293.15}}}}});
~~~~

Those options specifies that:

- the temperature shall be treated as a parameter whose default value is
  \(293.15\,K\),
- parameters shall be treated as static variables.


## The `JITOptions` data structure

