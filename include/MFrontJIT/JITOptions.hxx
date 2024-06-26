/*!
 * \file   include/MFrontJIT/JITOptions.hxx
 * \brief  This file introduces the JITOptions structure.
 * \author Thomas Helfer
 * \date   03/06/2024
 */

#ifndef LIB_MFRONT_JIT_JITOPTIONS_HXX
#define LIB_MFRONT_JIT_JITOPTIONS_HXX

#include <map>
#include <vector>
#include <string>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/GeneratorOptions.hxx"

namespace mfront::jit {

  /*!
   * \brief structure passed to the compile function
   */
  struct JITOptions {
    //! \brief generator to be used
#ifdef MSC_VER
    const enum { CMAKE, MAKE } generator = CMAKE;
#else
    const enum { CMAKE, MAKE } generator = MAKE;
#endif
    //! \brief target to be built
    const std::string target = "all";
    //! \brief interface used
    const std::string interface = "generic";
    //! \brief options passed to the Domain Specific Language
    const tfel::utilities::DataMap dsl_options;
    /*!
     * \brief external commands.
     */
    const std::vector<std::string> ecmds;
    /*!
     * \brief substitutions: every occurrence of `XXX` in the input
     * files will be replaced by `YYY`
     */
    const std::map<std::string, std::string> substitutions;
    //! \brief options passed to the generator
    const GeneratorOptions generator_options;
  };

}  // end of namespace mfront::jit

#endif /* LIB_MFRONT_JIT_JITOPTIONS_HXX */
