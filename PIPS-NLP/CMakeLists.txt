#add_definitions(-DTIMING -DSTOCH_TESTING) # timing output

include_directories(Core/Abstract Core/Vector Core/Utilities Core/QpGen
  Core/SparseLinearAlgebra Core/DenseLinearAlgebra Core/Readers 
  Core/LinearSolvers/Ma27Solver Core/LinearSolvers/Ma57Solver 
  Core/LinearSolvers/Ma86Solver Core/LinearSolvers/PardisoSolver 
  Core/LinearSolvers/BiCGStabSolver
  Core/LinearSolvers/UmfPackSolver)
include_directories(Core/StochLinearAlgebra)
include_directories(Core/NlpSolvers Core/NlpGen)
include_directories(Core/NlpInfo)
include_directories(Core/NlpStoch)
include_directories(Core/CInterface)
#include_directories(Core/MtxSchurDecomp)
include_directories(Core/StructureSolver)

if(HAVE_PETSC)
include_directories(Core/PetscLinearAlgebra)
endif(HAVE_PETSC)

add_subdirectory(Core)


set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")


#message("HAVE_PARDISO: ${HAVE_PARDISO}")
#message("HAVE_MA57: ${HAVE_MA57}")

if(HAVE_PARDISO OR HAVE_MA57)

#set(MYBLAS_LIB "/usr/lib/libblas.a")
set(MYBLAS_LIB "/usr/local/lib/libopenblas.a")

if(HAVE_PETSC)

### parallel filter line search IPM with MA57/Pardiso, use NL as the input, seperate Hessian
  add_executable(pipsnlp_parallel Drivers/parallelAmplDriver.cpp)
  target_link_libraries(pipsnlp_parallel
    genNLparallel StochInfoFromAMPL nlpstoch 
    pipsnlpFLSStoch 
#    ReducedSpaceSolver blockSCSolver
	reducedSolverSpecial 
#    schurDecompSolver
    nlpgensparse LU_solvers 
    nlpbase nlpsparse nlpdense nlpstochla 
    ${COIN_LIBS}
    ${MA27_LIBRARY} ${MA57_LIBRARY}
    ${UMF_ALLINONE_LIBRARY} 
    ${METIS_LIBRARY} ${AMPL_LIBRARY} ${MATH_LIBS}
#    ${PETSC_LIBRARY}
    ${PARDISO_LIBRARY} 
    )

### serial filter line search IPM with option to choose MA57/Pardiso. Schur-comp method is available
  add_executable(pipsnlp_serial Drivers/serialAmplDriver.cpp)
  target_link_libraries(pipsnlp_serial      
     genNLserial updateFromAMPL  nlpcNlpGenSparse  nlpwithsolver 
     nlpgensparse  LU_solvers 
#	schurDecompSolver  
#     ReducedSpaceSolver 
	reducedSolverSpecial 
    petsc_solver_sparse
    nlpsparse nlpdense 
    pipsnlpFLS  nlpbase 
    ${COIN_LIBS} 
    ${AMPL_LIBRARY} 
    ${MA57_LIBRARY}  ${MA27_LIBRARY} 
    ${UMF_ALLINONE_LIBRARY}
#    ${UMFPACK_LIBRARY_PIPS} 
#    ${AMD_LIBRARY}
    ${METIS_LIBRARY} 
    ${PARDISO_LIBRARY} 
    ${PETSC_LIBRARY}
    ${MATH_LIBS} )    
else(HAVE_PETSC)

###  parallel filter line search IPM with MA57/Pardiso, use NL as the input, seperate Hessian
  add_executable(pipsnlp_parallel Drivers/parallelAmplDriver.cpp)
  target_link_libraries(pipsnlp_parallel
    genNLparallel StochInfoFromAMPL 
    pipsnlpFLSStoch nlpstoch nlpstochla
    nlpgensparse nlpbase nlpsparse nlpdense  
#    schurDecompSolver
#    ReducedSpaceSolver blockSCSolver
	reducedSolverSpecial     
    ${MA27_LIBRARY} ${MA57_LIBRARY} ${METIS_LIBRARY}
    ${PARDISO_LIBRARY} 
    LU_solvers ${UMF_ALLINONE_LIBRARY} 
    ${AMPL_LIBRARY} ${COIN_LIBS} ${MATH_LIBS}
    )

### serial filter line search IPM with option to choose MA57/Pardiso. Schur-comp method is available
  add_executable(pipsnlp_serial Drivers/serialAmplDriver.cpp)
  target_link_libraries(pipsnlp_serial      
    genNLserial updateFromAMPL  
    pipsnlpFLS nlpcNlpGenSparse nlpwithsolver 
    nlpgensparse nlpbase nlpsparse nlpdense
#	 schurDecompSolver 
#     ReducedSpaceSolver reducedSolverSpecial     
    ${MA27_LIBRARY} ${MA57_LIBRARY} ${METIS_LIBRARY} 
    ${PARDISO_LIBRARY}  
    LU_solvers ${UMF_ALLINONE_LIBRARY}
    ${AMPL_LIBRARY} ${COIN_LIBS} ${MATH_LIBS} 
    )

### parallel filter line search IPM with DCOPF network aggregation preconditioner 
  add_executable(pipsqpFromDCOPF Drivers/parallelDcopfDriver.cpp) 
  target_link_libraries(pipsqpFromDCOPF 
    dcopfcinput nlpstochInputBase  aggregationstoch sInfoQPLP
    pipsnlpFLSStoch nlpstoch nlpstochla 
    reducedSolverSpecial
    nlpgensparse nlpbase nlpsparse nlpdense  
    ${MA27_LIBRARY} ${MA57_LIBRARY} ${METIS_LIBRARY}
    ${PARDISO_LIBRARY} 
    LU_solvers ${UMF_ALLINONE_LIBRARY} 
    ${AMPL_LIBRARY} ${COIN_LIBS} ${MATH_LIBS}
    )
    
endif(HAVE_PETSC)

endif(HAVE_PARDISO OR HAVE_MA57)


add_library(pipsnlp SHARED Drivers/pipsipmNlp_C_callbacks.cpp)
target_link_libraries(pipsnlp
  "-Wl,--whole-archive"     
     updateFromCallBack  nlpcNlpGenSparse  nlpwithsolver 
	 nlpgensparse  
	 LU_solvers  
    nlpsparse nlpdense 
    pipsnlpFLS  nlpbase 
    ${COIN_LIBS} 
    ${MA57_LIBRARY} 
    ${MA27_LIBRARY}
#    ${UMF_ALLINONE_LIBRARY}
    ${METIS_LIBRARY} 
    ${PARDISO_LIBRARY}
    "-Wl,--no-whole-archive"
    ${MATH_LIBS} )
  