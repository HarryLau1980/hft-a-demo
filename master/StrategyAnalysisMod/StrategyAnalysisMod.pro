#-------------------------------------------------
#
# Project created by QtCreator 2015-05-24T23:17:28
#
#-------------------------------------------------

QT       -= gui

TARGET = StrategyAnalysisMod
TEMPLATE = lib

DEFINES += STRATEGYANALYSISMOD_LIBRARY

SOURCES += strategyanalysismod.cpp \
    hftmath.cpp \
    indexfilter.cpp \
    indexsynchronizer.cpp \
    strategyanalyzer.cpp \
    indexstream.cpp \
    strategyworker.cpp \
    ../utils/indexsequenceset.cpp \
    ../utils/diagnosis.cpp \
    strategy/deltastrategy.cpp \
    localindexstream.cpp \
    strategy/deltastgworker.cpp

HEADERS += strategyanalysismod.h\
        strategyanalysismod_global.h \
    ../Eigen/src/Cholesky/LDLT.h \
    ../Eigen/src/Cholesky/LLT.h \
    ../Eigen/src/Cholesky/LLT_MKL.h \
    ../Eigen/src/CholmodSupport/CholmodSupport.h \
    ../Eigen/src/Core/arch/AltiVec/Complex.h \
    ../Eigen/src/Core/arch/AltiVec/PacketMath.h \
    ../Eigen/src/Core/arch/Default/Settings.h \
    ../Eigen/src/Core/arch/NEON/Complex.h \
    ../Eigen/src/Core/arch/NEON/PacketMath.h \
    ../Eigen/src/Core/arch/SSE/Complex.h \
    ../Eigen/src/Core/arch/SSE/MathFunctions.h \
    ../Eigen/src/Core/arch/SSE/PacketMath.h \
    ../Eigen/src/Core/products/CoeffBasedProduct.h \
    ../Eigen/src/Core/products/GeneralBlockPanelKernel.h \
    ../Eigen/src/Core/products/GeneralMatrixMatrix.h \
    ../Eigen/src/Core/products/GeneralMatrixMatrix_MKL.h \
    ../Eigen/src/Core/products/GeneralMatrixMatrixTriangular.h \
    ../Eigen/src/Core/products/GeneralMatrixMatrixTriangular_MKL.h \
    ../Eigen/src/Core/products/GeneralMatrixVector.h \
    ../Eigen/src/Core/products/GeneralMatrixVector_MKL.h \
    ../Eigen/src/Core/products/Parallelizer.h \
    ../Eigen/src/Core/products/SelfadjointMatrixMatrix.h \
    ../Eigen/src/Core/products/SelfadjointMatrixMatrix_MKL.h \
    ../Eigen/src/Core/products/SelfadjointMatrixVector.h \
    ../Eigen/src/Core/products/SelfadjointMatrixVector_MKL.h \
    ../Eigen/src/Core/products/SelfadjointProduct.h \
    ../Eigen/src/Core/products/SelfadjointRank2Update.h \
    ../Eigen/src/Core/products/TriangularMatrixMatrix.h \
    ../Eigen/src/Core/products/TriangularMatrixMatrix_MKL.h \
    ../Eigen/src/Core/products/TriangularMatrixVector.h \
    ../Eigen/src/Core/products/TriangularMatrixVector_MKL.h \
    ../Eigen/src/Core/products/TriangularSolverMatrix.h \
    ../Eigen/src/Core/products/TriangularSolverMatrix_MKL.h \
    ../Eigen/src/Core/products/TriangularSolverVector.h \
    ../Eigen/src/Core/util/BlasUtil.h \
    ../Eigen/src/Core/util/Constants.h \
    ../Eigen/src/Core/util/DisableStupidWarnings.h \
    ../Eigen/src/Core/util/ForwardDeclarations.h \
    ../Eigen/src/Core/util/Macros.h \
    ../Eigen/src/Core/util/Memory.h \
    ../Eigen/src/Core/util/Meta.h \
    ../Eigen/src/Core/util/MKL_support.h \
    ../Eigen/src/Core/util/NonMPL2.h \
    ../Eigen/src/Core/util/ReenableStupidWarnings.h \
    ../Eigen/src/Core/util/StaticAssert.h \
    ../Eigen/src/Core/util/XprHelper.h \
    ../Eigen/src/Core/Array.h \
    ../Eigen/src/Core/ArrayBase.h \
    ../Eigen/src/Core/ArrayWrapper.h \
    ../Eigen/src/Core/Assign.h \
    ../Eigen/src/Core/Assign_MKL.h \
    ../Eigen/src/Core/BandMatrix.h \
    ../Eigen/src/Core/Block.h \
    ../Eigen/src/Core/BooleanRedux.h \
    ../Eigen/src/Core/CommaInitializer.h \
    ../Eigen/src/Core/CoreIterators.h \
    ../Eigen/src/Core/CwiseBinaryOp.h \
    ../Eigen/src/Core/CwiseNullaryOp.h \
    ../Eigen/src/Core/CwiseUnaryOp.h \
    ../Eigen/src/Core/CwiseUnaryView.h \
    ../Eigen/src/Core/DenseBase.h \
    ../Eigen/src/Core/DenseCoeffsBase.h \
    ../Eigen/src/Core/DenseStorage.h \
    ../Eigen/src/Core/Diagonal.h \
    ../Eigen/src/Core/DiagonalMatrix.h \
    ../Eigen/src/Core/DiagonalProduct.h \
    ../Eigen/src/Core/Dot.h \
    ../Eigen/src/Core/EigenBase.h \
    ../Eigen/src/Core/Flagged.h \
    ../Eigen/src/Core/ForceAlignedAccess.h \
    ../Eigen/src/Core/Functors.h \
    ../Eigen/src/Core/Fuzzy.h \
    ../Eigen/src/Core/GeneralProduct.h \
    ../Eigen/src/Core/GenericPacketMath.h \
    ../Eigen/src/Core/GlobalFunctions.h \
    ../Eigen/src/Core/IO.h \
    ../Eigen/src/Core/Map.h \
    ../Eigen/src/Core/MapBase.h \
    ../Eigen/src/Core/MathFunctions.h \
    ../Eigen/src/Core/Matrix.h \
    ../Eigen/src/Core/MatrixBase.h \
    ../Eigen/src/Core/NestByValue.h \
    ../Eigen/src/Core/NoAlias.h \
    ../Eigen/src/Core/NumTraits.h \
    ../Eigen/src/Core/PermutationMatrix.h \
    ../Eigen/src/Core/PlainObjectBase.h \
    ../Eigen/src/Core/ProductBase.h \
    ../Eigen/src/Core/Random.h \
    ../Eigen/src/Core/Redux.h \
    ../Eigen/src/Core/Ref.h \
    ../Eigen/src/Core/Replicate.h \
    ../Eigen/src/Core/ReturnByValue.h \
    ../Eigen/src/Core/Reverse.h \
    ../Eigen/src/Core/Select.h \
    ../Eigen/src/Core/SelfAdjointView.h \
    ../Eigen/src/Core/SelfCwiseBinaryOp.h \
    ../Eigen/src/Core/SolveTriangular.h \
    ../Eigen/src/Core/StableNorm.h \
    ../Eigen/src/Core/Stride.h \
    ../Eigen/src/Core/Swap.h \
    ../Eigen/src/Core/Transpose.h \
    ../Eigen/src/Core/Transpositions.h \
    ../Eigen/src/Core/TriangularMatrix.h \
    ../Eigen/src/Core/VectorBlock.h \
    ../Eigen/src/Core/VectorwiseOp.h \
    ../Eigen/src/Core/Visitor.h \
    ../Eigen/src/Eigen2Support/Geometry/AlignedBox.h \
    ../Eigen/src/Eigen2Support/Geometry/All.h \
    ../Eigen/src/Eigen2Support/Geometry/AngleAxis.h \
    ../Eigen/src/Eigen2Support/Geometry/Hyperplane.h \
    ../Eigen/src/Eigen2Support/Geometry/ParametrizedLine.h \
    ../Eigen/src/Eigen2Support/Geometry/Quaternion.h \
    ../Eigen/src/Eigen2Support/Geometry/Rotation2D.h \
    ../Eigen/src/Eigen2Support/Geometry/RotationBase.h \
    ../Eigen/src/Eigen2Support/Geometry/Scaling.h \
    ../Eigen/src/Eigen2Support/Geometry/Transform.h \
    ../Eigen/src/Eigen2Support/Geometry/Translation.h \
    ../Eigen/src/Eigen2Support/Block.h \
    ../Eigen/src/Eigen2Support/Cwise.h \
    ../Eigen/src/Eigen2Support/CwiseOperators.h \
    ../Eigen/src/Eigen2Support/Lazy.h \
    ../Eigen/src/Eigen2Support/LeastSquares.h \
    ../Eigen/src/Eigen2Support/LU.h \
    ../Eigen/src/Eigen2Support/Macros.h \
    ../Eigen/src/Eigen2Support/MathFunctions.h \
    ../Eigen/src/Eigen2Support/Memory.h \
    ../Eigen/src/Eigen2Support/Meta.h \
    ../Eigen/src/Eigen2Support/Minor.h \
    ../Eigen/src/Eigen2Support/QR.h \
    ../Eigen/src/Eigen2Support/SVD.h \
    ../Eigen/src/Eigen2Support/TriangularSolver.h \
    ../Eigen/src/Eigen2Support/VectorBlock.h \
    ../Eigen/src/Eigenvalues/ComplexEigenSolver.h \
    ../Eigen/src/Eigenvalues/ComplexSchur.h \
    ../Eigen/src/Eigenvalues/ComplexSchur_MKL.h \
    ../Eigen/src/Eigenvalues/EigenSolver.h \
    ../Eigen/src/Eigenvalues/GeneralizedEigenSolver.h \
    ../Eigen/src/Eigenvalues/GeneralizedSelfAdjointEigenSolver.h \
    ../Eigen/src/Eigenvalues/HessenbergDecomposition.h \
    ../Eigen/src/Eigenvalues/MatrixBaseEigenvalues.h \
    ../Eigen/src/Eigenvalues/RealQZ.h \
    ../Eigen/src/Eigenvalues/RealSchur.h \
    ../Eigen/src/Eigenvalues/RealSchur_MKL.h \
    ../Eigen/src/Eigenvalues/SelfAdjointEigenSolver.h \
    ../Eigen/src/Eigenvalues/SelfAdjointEigenSolver_MKL.h \
    ../Eigen/src/Eigenvalues/Tridiagonalization.h \
    ../Eigen/src/Geometry/arch/Geometry_SSE.h \
    ../Eigen/src/Geometry/AlignedBox.h \
    ../Eigen/src/Geometry/AngleAxis.h \
    ../Eigen/src/Geometry/EulerAngles.h \
    ../Eigen/src/Geometry/Homogeneous.h \
    ../Eigen/src/Geometry/Hyperplane.h \
    ../Eigen/src/Geometry/OrthoMethods.h \
    ../Eigen/src/Geometry/ParametrizedLine.h \
    ../Eigen/src/Geometry/Quaternion.h \
    ../Eigen/src/Geometry/Rotation2D.h \
    ../Eigen/src/Geometry/RotationBase.h \
    ../Eigen/src/Geometry/Scaling.h \
    ../Eigen/src/Geometry/Transform.h \
    ../Eigen/src/Geometry/Translation.h \
    ../Eigen/src/Geometry/Umeyama.h \
    ../Eigen/src/Householder/BlockHouseholder.h \
    ../Eigen/src/Householder/Householder.h \
    ../Eigen/src/Householder/HouseholderSequence.h \
    ../Eigen/src/IterativeLinearSolvers/BasicPreconditioners.h \
    ../Eigen/src/IterativeLinearSolvers/BiCGSTAB.h \
    ../Eigen/src/IterativeLinearSolvers/ConjugateGradient.h \
    ../Eigen/src/IterativeLinearSolvers/IncompleteLUT.h \
    ../Eigen/src/IterativeLinearSolvers/IterativeSolverBase.h \
    ../Eigen/src/Jacobi/Jacobi.h \
    ../Eigen/src/LU/arch/Inverse_SSE.h \
    ../Eigen/src/LU/Determinant.h \
    ../Eigen/src/LU/FullPivLU.h \
    ../Eigen/src/LU/Inverse.h \
    ../Eigen/src/LU/PartialPivLU.h \
    ../Eigen/src/LU/PartialPivLU_MKL.h \
    ../Eigen/src/MetisSupport/MetisSupport.h \
    ../Eigen/src/misc/blas.h \
    ../Eigen/src/misc/Image.h \
    ../Eigen/src/misc/Kernel.h \
    ../Eigen/src/misc/Solve.h \
    ../Eigen/src/misc/SparseSolve.h \
    ../Eigen/src/OrderingMethods/Amd.h \
    ../Eigen/src/OrderingMethods/Eigen_Colamd.h \
    ../Eigen/src/OrderingMethods/Ordering.h \
    ../Eigen/src/PardisoSupport/PardisoSupport.h \
    ../Eigen/src/PaStiXSupport/PaStiXSupport.h \
    ../Eigen/src/plugins/ArrayCwiseBinaryOps.h \
    ../Eigen/src/plugins/ArrayCwiseUnaryOps.h \
    ../Eigen/src/plugins/BlockMethods.h \
    ../Eigen/src/plugins/CommonCwiseBinaryOps.h \
    ../Eigen/src/plugins/CommonCwiseUnaryOps.h \
    ../Eigen/src/plugins/MatrixCwiseBinaryOps.h \
    ../Eigen/src/plugins/MatrixCwiseUnaryOps.h \
    ../Eigen/src/QR/ColPivHouseholderQR.h \
    ../Eigen/src/QR/ColPivHouseholderQR_MKL.h \
    ../Eigen/src/QR/FullPivHouseholderQR.h \
    ../Eigen/src/QR/HouseholderQR.h \
    ../Eigen/src/QR/HouseholderQR_MKL.h \
    ../Eigen/src/SparseCholesky/SimplicialCholesky.h \
    ../Eigen/src/SparseCholesky/SimplicialCholesky_impl.h \
    ../Eigen/src/SparseCore/AmbiVector.h \
    ../Eigen/src/SparseCore/CompressedStorage.h \
    ../Eigen/src/SparseCore/ConservativeSparseSparseProduct.h \
    ../Eigen/src/SparseCore/MappedSparseMatrix.h \
    ../Eigen/src/SparseCore/SparseBlock.h \
    ../Eigen/src/SparseCore/SparseColEtree.h \
    ../Eigen/src/SparseCore/SparseCwiseBinaryOp.h \
    ../Eigen/src/SparseCore/SparseCwiseUnaryOp.h \
    ../Eigen/src/SparseCore/SparseDenseProduct.h \
    ../Eigen/src/SparseCore/SparseDiagonalProduct.h \
    ../Eigen/src/SparseCore/SparseDot.h \
    ../Eigen/src/SparseCore/SparseFuzzy.h \
    ../Eigen/src/SparseCore/SparseMatrix.h \
    ../Eigen/src/SparseCore/SparseMatrixBase.h \
    ../Eigen/src/SparseCore/SparsePermutation.h \
    ../Eigen/src/SparseCore/SparseProduct.h \
    ../Eigen/src/SparseCore/SparseRedux.h \
    ../Eigen/src/SparseCore/SparseSelfAdjointView.h \
    ../Eigen/src/SparseCore/SparseSparseProductWithPruning.h \
    ../Eigen/src/SparseCore/SparseTranspose.h \
    ../Eigen/src/SparseCore/SparseTriangularView.h \
    ../Eigen/src/SparseCore/SparseUtil.h \
    ../Eigen/src/SparseCore/SparseVector.h \
    ../Eigen/src/SparseCore/SparseView.h \
    ../Eigen/src/SparseCore/TriangularSolver.h \
    ../Eigen/src/SparseLU/SparseLU.h \
    ../Eigen/src/SparseLU/SparseLU_column_bmod.h \
    ../Eigen/src/SparseLU/SparseLU_column_dfs.h \
    ../Eigen/src/SparseLU/SparseLU_copy_to_ucol.h \
    ../Eigen/src/SparseLU/SparseLU_gemm_kernel.h \
    ../Eigen/src/SparseLU/SparseLU_heap_relax_snode.h \
    ../Eigen/src/SparseLU/SparseLU_kernel_bmod.h \
    ../Eigen/src/SparseLU/SparseLU_Memory.h \
    ../Eigen/src/SparseLU/SparseLU_panel_bmod.h \
    ../Eigen/src/SparseLU/SparseLU_panel_dfs.h \
    ../Eigen/src/SparseLU/SparseLU_pivotL.h \
    ../Eigen/src/SparseLU/SparseLU_pruneL.h \
    ../Eigen/src/SparseLU/SparseLU_relax_snode.h \
    ../Eigen/src/SparseLU/SparseLU_Structs.h \
    ../Eigen/src/SparseLU/SparseLU_SupernodalMatrix.h \
    ../Eigen/src/SparseLU/SparseLU_Utils.h \
    ../Eigen/src/SparseLU/SparseLUImpl.h \
    ../Eigen/src/SparseQR/SparseQR.h \
    ../Eigen/src/SPQRSupport/SuiteSparseQRSupport.h \
    ../Eigen/src/StlSupport/details.h \
    ../Eigen/src/StlSupport/StdDeque.h \
    ../Eigen/src/StlSupport/StdList.h \
    ../Eigen/src/StlSupport/StdVector.h \
    ../Eigen/src/SuperLUSupport/SuperLUSupport.h \
    ../Eigen/src/SVD/JacobiSVD.h \
    ../Eigen/src/SVD/JacobiSVD_MKL.h \
    ../Eigen/src/SVD/UpperBidiagonalization.h \
    ../Eigen/src/UmfPackSupport/UmfPackSupport.h \
    hftmath.h \
    indexfilter.h \
    indexsynchronizer.h \
    strategyanalyzer.h \
    indexstream.h \
    strategyworker.h \
    indextick.h \
    ../utils/utils.h \
    ../utils/indexsequenceset.h \
    ../utils/diagnosis.h \
    ../utils/common.h \
    strategy/deltastrategy.h \
    localindexstream.h \
    strategy/deltastgworker.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DataAccInterf/release/ -lDataAccInterf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DataAccInterf/debug/ -lDataAccInterf
else:unix: LIBS += -L$$OUT_PWD/../DataAccInterf/ -lDataAccInterf

INCLUDEPATH += $$PWD/../DataAccInterf
DEPENDPATH += $$PWD/../DataAccInterf

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PlotMod/release/ -lPlotMod
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PlotMod/debug/ -lPlotMod
else:unix: LIBS += -L$$OUT_PWD/../PlotMod/ -lPlotMod

INCLUDEPATH += $$PWD/../PlotMod
DEPENDPATH += $$PWD/../PlotMod

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../TradeExecMod/release/ -lTradeExecMod
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../TradeExecMod/debug/ -lTradeExecMod
else:unix: LIBS += -L$$OUT_PWD/../TradeExecMod/ -lTradeExecMod

INCLUDEPATH += $$PWD/../TradeExecMod
DEPENDPATH += $$PWD/../TradeExecMod
