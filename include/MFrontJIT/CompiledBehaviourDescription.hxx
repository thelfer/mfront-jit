/*!
 * \file   include/MFrontJIT/CompiledBehaviourDescription.hxx
 * \brief  
 * \author Thomas Helfer
 * \date   03/06/2024
 */

#ifndef LIB_COMPILEDBEHAVIOURDESCRIPTION_HXX
#define LIB_COMPILEDBEHAVIOURDESCRIPTION_HXX

#include <vector>
#include <string_view>
#include "MFront/VariableDescription.hxx"
#include "MFront/Gradient.hxx"
#include "MFront/ThermodynamicForce.hxx"
#include "MFrontJIT/Compile.hxx"

namespace mfront::jit {

  /*!
   * This structure contains the list of material properties,
   * internal state variables and external state variables
   * that are defined by the implementation of a behaviour
   * for a given interface and modelling hypothesis.
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
  };

  /*!
   * \brief return the runtime description of implementation of a behaviour
   * for a given interface and modelling hypothesis.
   * \param[in] mfront_path: path to an MFront implementation
   * \param[in] dsl_options: option passed to the DSL
   * \param[in] hypothesis: modelling hypothesis
   */
  MFRONTJIT_EXPORT CompiledBehaviourDescription getCompiledBehaviourDescription(
      std::string_view, const JITOptions&, std::string_view) noexcept(false);

} // end of namespace mfront::jit

#endif /* LIB_COMPILEDBEHAVIOURDESCRIPTION_HXX */
