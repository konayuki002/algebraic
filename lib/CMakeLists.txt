add_library(algebraic
  STATIC 
    AlgebraicReal.cpp
    IntervalRational.cpp
    MaybeOrdering.cpp
  )

target_include_directories(algebraic 
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)
