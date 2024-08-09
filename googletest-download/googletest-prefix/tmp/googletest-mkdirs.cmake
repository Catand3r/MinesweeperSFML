# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/paulziemann/Documents/C++ Projects/CalculatorAsClass/googletest-src"
  "/Users/paulziemann/Documents/C++ Projects/CalculatorAsClass/googletest-build"
  "/Users/paulziemann/Documents/C++ Projects/CalculatorAsClass/googletest-download/googletest-prefix"
  "/Users/paulziemann/Documents/C++ Projects/CalculatorAsClass/googletest-download/googletest-prefix/tmp"
  "/Users/paulziemann/Documents/C++ Projects/CalculatorAsClass/googletest-download/googletest-prefix/src/googletest-stamp"
  "/Users/paulziemann/Documents/C++ Projects/CalculatorAsClass/googletest-download/googletest-prefix/src"
  "/Users/paulziemann/Documents/C++ Projects/CalculatorAsClass/googletest-download/googletest-prefix/src/googletest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/paulziemann/Documents/C++ Projects/CalculatorAsClass/googletest-download/googletest-prefix/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/paulziemann/Documents/C++ Projects/CalculatorAsClass/googletest-download/googletest-prefix/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
