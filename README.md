# Algebraic
  Create algebraic number, operator and root finding method.

## Reference
  https://miz-ar.info/math/algebraic-real/

## What you need
  * C++17
  * boost
    * Ubuntu
    ```
      sudo apt install libboost-dev
    ```
    https://kaworu.jpn.org/cpp/boost%E3%81%AE%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB

## Test
  Ready to test
  ```
    mkdir -p build
    cd build
    cmake ..
    make
  ```
  
  Run all test
  ```
    ctest
  ```

  Run randomized test
  ```
    ctest --schedule-random
  ```

  Run specific tests that match the search string
  ```
    ctest -R AlgebraicRealTest.RealRoots
  ```

  Run in multi-core
  ```
    ctest -j8
  ```