/*!
 * \file   include/MFrontJIT/Config.hxx
 * \brief  This file defines the MFRONTJIT_EXPORT macro, which controls
 * the visibility of the symbols of the `MFrontJIT` library.
 * \author Thomas Helfer
 * \date   03/06/2024
 *
 * The macros which describe the visibility of a symbol are inspired by
 * the page "Why is the new C++ visibility support so useful ?" of the
 * `GCC` wiki (<http://gcc.gnu.org/wiki/Visibility>).
 */

#ifndef LIB_MFRONT_JIT_CONFIG_HXX
#define LIB_MFRONT_JIT_CONFIG_HXX

/*!
 * \def MFRONTJIT_VISIBILITY_IMPORT
 * \brief this macro decorates a symbol imported from a dynamic library.
 * It has no effect on platforms and with compilers which do not support
 * symbol visibility.
 */

/*!
 * \def MFRONTJIT_VISIBILITY_EXPORT
 * \brief this macro decorates a symbol exported by a dynamic library. It
 * has no effect on platforms and with compilers which do not support
 * symbol visibility.
 */

/*!
 * \def MFRONTJIT_VISIBILITY_LOCAL
 * \brief this macro decorates a symbol which is defined in a header but
 * shall not be exported. It has no effect on platforms and with
 * compilers which do not support symbol visibility.
 */

/*!
 * \def MFRONTJIT_EXPORT
 * \brief this macro decorates a type or a function of the public
 * interface of the `MFrontJIT` library. It is written immediately before
 * `struct`, or before the return type of a function, after the
 * attributes.
 *
 * Symbols which are not part of the interface of the library are either
 * `static` functions of a `.cxx` file or members of the `internals`
 * namespace, and are decorated with `MFRONTJIT_VISIBILITY_LOCAL` when
 * they are defined in a header.
 *
 * \see mfront::jit::compile
 */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#define MFRONTJIT_VISIBILITY_IMPORT __declspec(dllimport)
#define MFRONTJIT_VISIBILITY_EXPORT __declspec(dllexport)
#define MFRONTJIT_VISIBILITY_LOCAL
#else /* defined _WIN32 || defined _WIN64 || defined __CYGWIN__ */
#if (defined __GNUC__) && (!defined __INTEL_COMPILER)
#if __GNUC__ >= 4
#define MFRONTJIT_VISIBILITY_IMPORT __attribute__((visibility("default")))
#define MFRONTJIT_VISIBILITY_EXPORT __attribute__((visibility("default")))
#define MFRONTJIT_VISIBILITY_LOCAL __attribute__((visibility("hidden")))
#else /* __GNUC__ >= 4 */
#define MFRONTJIT_VISIBILITY_IMPORT
#define MFRONTJIT_VISIBILITY_EXPORT
#define MFRONTJIT_VISIBILITY_LOCAL
#endif /* __GNUC__ >= 4 */
#elif defined __INTEL_COMPILER
#define MFRONTJIT_VISIBILITY_IMPORT __attribute__((visibility("default")))
#define MFRONTJIT_VISIBILITY_EXPORT __attribute__((visibility("default")))
#define MFRONTJIT_VISIBILITY_LOCAL __attribute__((visibility("hidden")))
#else /* defined __INTEL_COMPILER */
#define MFRONTJIT_VISIBILITY_IMPORT
#define MFRONTJIT_VISIBILITY_EXPORT
#define MFRONTJIT_VISIBILITY_LOCAL
#endif /* defined __INTEL_COMPILER */
#endif /* defined _WIN32 || defined _WIN64 || defined __CYGWIN__ */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined MFrontJIT_EXPORTS
#define MFRONTJIT_EXPORT MFRONTJIT_VISIBILITY_EXPORT
#else /* defined MFrontJIT_EXPORTS */
#ifndef MFRONTJIT_STATIC_BUILD
#define MFRONTJIT_EXPORT MFRONTJIT_VISIBILITY_IMPORT
#else /* MFRONTJIT_STATIC_BUILD */
#define MFRONTJIT_EXPORT
#endif /* MFRONTJIT_STATIC_BUILD */
#endif /* defined MFrontJIT_EXPORTS */
#else  /* defined _WIN32 || defined _WIN64 || defined __CYGWIN__ */
#define MFRONTJIT_EXPORT MFRONTJIT_VISIBILITY_EXPORT
#endif /* defined _WIN32 || defined _WIN64 || defined __CYGWIN__ */

#endif /* LIB_MFRONT_JIT_CONFIG_HXX */
