/*!
 * \file   src/CompiledBehaviourDescription.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   03/06/2024
 */

#include "TFEL/Raise.hxx"
#include "MFront/MFrontBase.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFrontJIT/CompiledBehaviourDescription.hxx"

namespace mfront::jit{

  CompiledBehaviourDescription getCompiledBehaviourDescription(
      std::string_view mfront_path,
      const JITOptions& options,
      std::string_view hypothesis) noexcept(false) {
    const auto mpath = std::string{mfront_path};
    const auto h = tfel::material::ModellingHypothesis::fromString(
        std::string{hypothesis});
    if (options.interface.empty()) {
      tfel::raise(
          "getCompiledBehaviourDescription: no interface specified while "
          "treating '" +
          std::string{mpath} + "'");
    }
    auto bdsl = [&mpath, &options] {  // abstract dsl
      auto adsl = MFrontBase::getDSL(mpath, options.dsl_options);
      if (adsl->getTargetType() != AbstractDSL::BEHAVIOURDSL) {
        tfel::raise("getCompiledBehaviourDescription: path '" + mpath +
                    "' does not describe a behaviour");
      }
      auto ptr = std::dynamic_pointer_cast<AbstractBehaviourDSL>(adsl);
      if (ptr.get() == nullptr) {
        // this can't happen
        tfel::raise("getCompiledBehaviourDescription: path '" + mpath +
                    "' does not describe a behaviour (internal error)");
      }
      return ptr;
    }();
    //
    bdsl->setInterfaces({options.interface});
    bdsl->analyseFile(mpath, options.ecmds, options.substitutions);
    // building the list of material properties
    const auto& bi = [&mpath, &bdsl,
                      &options]() -> AbstractBehaviourInterface& {
      const auto& bis = bdsl->getBehaviourInterfaces();
      if (!bis.contains(options.interface)) {
        tfel::raise(
            "getCompiledBehaviourDescription: internal error "
            "while treating path '" +
            mpath + "'");
      }
      return *(bis.at(options.interface));
    }();
    const auto& bd = bdsl->getBehaviourDescription();
    const auto& bdata = bd.getBehaviourData(h);
    //
    auto gradients = [&bd] {
      auto r = std::vector<Gradient>{};
      for (const auto [g, thf] : bd.getMainVariables()) {
        static_cast<void>(thf);
        r.push_back(g);
      }
      return r;
    }();
    auto thfs = [&bd] {
      auto r = std::vector<ThermodynamicForce>{};
      for (const auto [g, thf] : bd.getMainVariables()) {
        static_cast<void>(g);
        r.push_back(thf);
      }
      return r;
    }();
    // material properties
    auto mps = [&bi, &bd, &h] {
      auto lmps = VariableDescriptionContainer{};
      for (const auto& mp : bi.buildMaterialPropertiesList(bd, h).first) {
        lmps.push_back(mp);
      }
      return lmps;
    }();
    return CompiledBehaviourDescription{
        .gradients = std::move(gradients),
        .thermodynamic_forces = std::move(thfs),
        .tangent_operator_blocks = bd.getTangentOperatorBlocks(),
        .parameters = bdata.getParameters(),
        .material_properties = std::move(mps),
        .internal_state_variables = bdata.getPersistentVariables(),
        .external_state_variables = bdata.getExternalStateVariables()};
  }  // end of getCompiledBehaviourDescription

} // end of namespace mfront::jit