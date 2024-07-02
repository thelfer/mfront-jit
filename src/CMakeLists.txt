add_library(MFrontJIT SHARED
            Compile.cxx
            CompiledBehaviourDescription.cxx)
target_include_directories(MFrontJIT PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(MFrontJIT PUBLIC tfel::TFELMFront)
target_compile_options(MFrontJIT PRIVATE "${TFEL_COMPILER_FLAGS}")
install(TARGETS MFrontJIT DESTINATION "lib")
