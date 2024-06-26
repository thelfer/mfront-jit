/*!
 * Macro extracted from :
 * "Why is the new C++ visibility support so useful?"
 * from http://gcc.gnu.org/wiki/Visibility
 */
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#define MFRONTJIT_VISIBILITY_IMPORT __declspec(dllimport)
#define MFRONTJIT_VISIBILITY_EXPORT __declspec(dllexport)
#define MFRONTJIT_VISIBILITY_LOCAL
#else /* defined _WIN32 || defined __CYGWIN__ */
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
#endif /* defined _WIN32 || defined _WIN64 ||defined __CYGWIN__ */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined MFrontJIT_EXPORTS
#define MFRONTJIT_EXPORT MFRONTJIT_VISIBILITY_EXPORT
#else
#ifndef MFRONTJIT_STATIC_BUILD
#define MFRONTJIT_EXPORT MFRONTJIT_VISIBILITY_IMPORT
#else
#define MFRONTJIT_EXPORT
#endif
#endif
#else
#define MFRONTJIT_EXPORT MFRONTJIT_VISIBILITY_EXPORT
#endif /* */