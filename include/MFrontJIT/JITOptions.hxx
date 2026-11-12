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
   *
   * All the data members are `const` and have a default value: an
   * instance of this structure is built with a designated initialiser
   * listing only the options which differ from the defaults:
   *
   * ~~~{.cxx}
   * mfront::jit::compile("IsotropicMisesCreep.mfront",
   *                      {.interface = "smallstrainsmallrotation"});
   * ~~~
   *
   * \see compile
   */
  struct JITOptions {
    //! \brief generator to be used
#ifdef MSC_VER
    const enum { CMAKE, MAKE } generator = CMAKE;
#else
    const enum { CMAKE, MAKE } generator = MAKE;
#endif
    //! \brief target to be built (`all` by default)
    const std::string target = "all";
    //! \brief interface used (`generic` by default)
    const std::string interface = "generic";
    /*!
     * \brief options passed to the Domain Specific Language
     *
     * The supported options depend on the domain specific language used
     * to analyse the file, as the `--dsl-option` command line argument of
     * `mfront`. Typical options are `parameters_as_static_variables`,
     * which asks for the parameters to be handled as static variables,
     * and `overriding_parameters`, whose value is a map giving the new
     * default values of the parameters.
     */
    const tfel::utilities::DataMap dsl_options;
    /*!
     * \brief external commands.
     *
     * The external commands are the commands one may specify on the
     * command line of `mfront` through an `--@XXX` option: their content
     * is parsed and inserted at the beginning of every input file.
     */
    const std::vector<std::string> ecmds;
    /*!
     * \brief substitutions: every occurrence of `XXX` in the input
     * files will be replaced by `YYY`
     *
     * Those are the substitutions one may specify on the command line of
     * `mfront` through an `--@XXX@=YYY` option.
     */
    const std::map<std::string, std::string> substitutions;
    //! \brief options passed to the generator
    const GeneratorOptions generator_options;
  };  // end of JITOptions

}  // end of namespace mfront::jit

#endif /* LIB_MFRONT_JIT_JITOPTIONS_HXX */
