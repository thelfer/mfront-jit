/*!
 * \file   include/MFrontJIT/CompiledBehaviourDescription.hxx
 * \brief  This file introduces the CompiledBehaviourDescription
 * structure and the `getCompiledBehaviourDescription` function.
 * \author Thomas Helfer
 * \date   03/06/2024
 */

#ifndef LIB_MFRONT_JIT_COMPILEDBEHAVIOURDESCRIPTION_HXX
#define LIB_MFRONT_JIT_COMPILEDBEHAVIOURDESCRIPTION_HXX

#include <vector>
#include <string_view>
#include <utility>
#include "MFront/VariableDescription.hxx"
#include "MFront/Gradient.hxx"
#include "MFront/ThermodynamicForce.hxx"
#include "MFrontJIT/Compile.hxx"

namespace mfront::jit {

  /*!
   * \brief runtime description of an implementation of a behaviour
   *
   * This structure contains the list of material properties,
   * internal state variables and external state variables
   * that are defined by the implementation of a behaviour
   * for a given interface and modelling hypothesis.
   *
   * The data members are `const`: an instance of this structure is a
   * read only view of the behaviour description, which is built by the
   * `getCompiledBehaviourDescription` function.
   *
   * \see getCompiledBehaviourDescription
   */
  struct [[nodiscard]] CompiledBehaviourDescription {
    //! \brief gradients
    const std::vector<Gradient> gradients;
    //! \brief thermodynamic forces
    const std::vector<ThermodynamicForce> thermodynamic_forces;
    //! \brief tangent operator blocks
    const std::vector<std::pair<VariableDescription, VariableDescription>>
        tangent_operator_blocks;
    //! \brief parameters
    const VariableDescriptionContainer parameters;
    //! \brief material properties
    const VariableDescriptionContainer material_properties;
    //! \brief internal state variables
    const VariableDescriptionContainer internal_state_variables;
    //! \brief external state variables
    const VariableDescriptionContainer external_state_variables;
  };  // end of CompiledBehaviourDescription

  /*!
   * \brief return the runtime description of an implementation of a
   * behaviour for a given interface and modelling hypothesis.
   *
   * The file is analysed by the appropriate domain specific language,
   * which is queried for the variables of the behaviour described by the
   * given modelling hypothesis. Contrary to the `compile` function, no
   * output file is generated and nothing is built.
   *
   * \param[in] mfront_path: path to an MFront implementation
   * \param[in] options: options used to analyse the implementation. Only
   * the `interface`, `dsl_options`, `ecmds` and `substitutions` data
   * members are used: the description of a behaviour is independent from
   * the way it is built
   * \param[in] hypothesis: modelling hypothesis, that is to say a string
   * which can be understood by the `fromString` method of the
   * `ModellingHypothesis` class
   *
   * \return the list of the gradients, thermodynamic forces, tangent
   * operator blocks, parameters, material properties and state variables
   * of the behaviour.
   *
   * \note Any error reported by the analysis of the file is thrown as an
   * exception. In particular, an exception is thrown if the file does not
   * describe a behaviour or if the modelling hypothesis is not supported
   * by the behaviour.
   *
   * \see compile
   */
  MFRONTJIT_EXPORT CompiledBehaviourDescription
  getCompiledBehaviourDescription(std::string_view mfront_path,
                                  const JITOptions& options,
                                  std::string_view hypothesis) noexcept(false);

}  // end of namespace mfront::jit

#endif /* LIB_MFRONT_JIT_COMPILEDBEHAVIOURDESCRIPTION_HXX */
