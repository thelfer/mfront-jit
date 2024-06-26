/*!
 * \file   include/MFrontJIT/Compile.hxx
 * \brief  This file introduces the `compile` function
 * \author Thomas Helfer
 * \date   03/06/2024
 */

#include <string_view>
#include "MFront/TargetsDescription.hxx"
#include "MFrontJIT/Config.hxx"
#include "MFrontJIT/JITOptions.hxx"

#ifndef LIB_MFRONT_JIT_COMPILE_HXX
#define LIB_MFRONT_JIT_COMPILE_HXX

namespace mfront::jit {

  /*!
   * \brief compile an MFront implementation
   * \param[in] mfront_path: path to an MFront implementation
   * \param[in] dsl_options: option passed to the DSL
   */
  MFRONTJIT_EXPORT TargetsDescription
  compile(std::string_view, const JITOptions&) noexcept(false);

}  // namespace mfront::jit

#endif /* LIB_MFRONT_JIT_COMPILE_HXX */
