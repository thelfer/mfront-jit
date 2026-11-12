#include "MFront/AbstractDSL.hxx"
#include "MFront/MFrontBase.hxx"
#include "MFront/SearchPathsHandler.hxx"
#include "MFront/GeneratorOptions.hxx"
#include "MFront/MakefileGenerator.hxx"
#include "MFront/CMakeGenerator.hxx"
#include "MFrontJIT/Compile.hxx"

namespace mfront::jit {

  namespace internals {

    static TargetsDescription compile(std::string_view mfront_path,
                                      const JITOptions& options) {
      const auto mpath = std::string{mfront_path};
      auto adsl = MFrontBase::getDSL(mpath, options.dsl_options);
      if (!options.interface.empty()) {
        adsl->setInterfaces({options.interface});
      }
      adsl->analyseFile(mpath, options.ecmds, options.substitutions);
      adsl->generateOutputFiles();
      const auto targets = adsl->getTargetsDescription();
      if (options.generator == JITOptions::CMAKE) {
        generateCMakeListsFile(targets, {});
        callCMake(options.target);
      } else if (options.generator == JITOptions::MAKE) {
        generateMakeFile(targets, {});
        callMake(options.target);
      } else {
        tfel::raise("internal error: unsupported generator");
      }
      return targets;
    }  // end of compile

  }  // end of namespace internals

  TargetsDescription compile(std::string_view mfront_path,
                             const JITOptions& options) {
    // the search paths of MFront are global to the process: they are
    // saved before the compilation and restored afterwards, even if the
    // compilation fails, so that two calls to this function are
    // independent
    const auto old_paths = SearchPathsHandler::getRegistredPaths();
    try {
      const auto targets = internals::compile(mfront_path, options);
      SearchPathsHandler::resetPaths(old_paths);
      return targets;
    } catch (...) {
      SearchPathsHandler::resetPaths(old_paths);
      throw;
    }
  }  // end of compile

}  // end of namespace mfront::jit