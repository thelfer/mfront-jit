/*!
 * \file   include/MFrontJIT/Compile.hxx
 * \brief  This file introduces the `compile` function
 * \author Thomas Helfer
 * \date   03/06/2024
 */

#ifndef LIB_MFRONT_JIT_COMPILE_HXX
#define LIB_MFRONT_JIT_COMPILE_HXX

#include <string_view>
#include "MFront/TargetsDescription.hxx"
#include "MFrontJIT/Config.hxx"
#include "MFrontJIT/JITOptions.hxx"

namespace mfront::jit {

  /*!
   * \brief compile an MFront implementation
   *
   * The implementation is analysed by the appropriate domain specific
   * language, the source files of the material knowledge are generated,
   * and a build file, either a `Makefile` or a `CMakeLists.txt`, is
   * generated and run to build the library implementing the behaviour.
   *
   * \param[in] mfront_path: path to an MFront implementation. It can be
   * the path to an `.mfront` file or the path to an implementation of a
   * `madnex` file, that is to say a path of the form
   * `file.madnex::implementation`.
   * \param[in] options: options used to compile the implementation
   * (interface, generator, options passed to the domain specific
   * language, external commands, substitutions, ...)
   *
   * \return the description of the targets which have been built, which
   * gives in particular the name of the behaviours and the path to the
   * libraries to be loaded by a solver.
   *
   * \note The generated files and the build products are written in the
   * current working directory, which must then be kept alive as long as
   * the compiled material is used.
   * \note The search paths of `MFront` are global to the process. They
   * are saved and restored by this function, which does not make the
   * function thread safe as all the other global states of `MFront` are
   * left untouched.
   * \note Any error reported by the analysis of the file or by the build
   * is thrown as an exception.
   *
   * \see JITOptions
   * \see getCompiledBehaviourDescription
   */
  MFRONTJIT_EXPORT TargetsDescription compile(
      std::string_view mfront_path, const JITOptions& options) noexcept(false);

}  // end of namespace mfront::jit

#endif /* LIB_MFRONT_JIT_COMPILE_HXX */
