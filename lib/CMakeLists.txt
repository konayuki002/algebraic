add_library(algebraic
  STATIC 
    AlgebraicReal.cpp
    IntervalRational.cpp
    MaybeBool.cpp
  )

target_include_directories(algebraic 
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)
