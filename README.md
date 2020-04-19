# Phase 2: Parser Generator

## Introduction
this phase aims to design and implement an LL (1) parser generator tool.
The parser generator expects an LL (1) grammar as input. It should compute First and Follow
sets and uses them to construct a predictive parsing table for the grammar. 

The table is to be used to drive a predictive top-down parser. If the input grammar is not
LL (1), an appropriate error message should be produced.
The generated parser is required to produce some representation of the leftmost derivation for
a correct input. If an error is encountered, a panic-mode error recovery routine is to be called
to print an error message and to resume parsing.

## Installing Dependencies
- C++ Compiler, you can install Mingw:
    - [`mingw`](http://www.mingw.org/) for windows.
    - [`mingu`](http://www.mingw.org/wiki/LinuxCrossMinGW) for Linux
- [`CMake`](https://cmake.org/download/) build tool to automate building and running the project.
- if you get a trouble while installing CMake on windwos, you can use this link [`Make`](https://stat545.com/make-windows.html).
    

## Running The project locally
- make sure that Dependencies have been installed correctly.
#### Linux Environment
- inside the project directory, open terminal and run
```bash
mkdir build
cd build
cmake ..
make
```
- the first step must be done for the first time only, after a change you made or to run the project:
    - make sure you are in build directory and run the following
```bash
make
parser
```

#### Windows Environment
- The simplest way to build the project is to use `CMake GUI` for windows.
- After building the project, navigate to build directory and with command line, run:
```bash
make
parser
```


#### Helpful links if you encounter a trouble
- https://github.com/tudelft3d/masbcpp/wiki/Building-on-Windows-with-CMake-and-MinGW
- https://cmake.org/runningcmake/



## Project organization
#### Parsing input file
- There are two file that are resposible for dealing with input file that contains production rules:
    - [`productionsParser.cpp`](./src/productionsParser.cpp)
    - [`productionsParser.h`](./include/productionsParser.h)

- To use the parsing:
    - import `productionsParser.h` in the source file.
    - use `ProductionParser::parseProductionRuls({filePath})`:
        - It takes a valid input file path.
        - return a map that containing production name as a key and production parts as 2D array where each row represents a collection of concatenating parts of the production and `or` operation must be performed on all rows. 
#### Construction parsing table
 - two main files:
    - [`parser_table.cpp`](./src/parser_table.cpp)
    - [`parse_table.h`](./include/parse_table.h)
        
 - to construct a paring table for a file:
    - import `parse_table.h` in the source file.
    - make an object from parsing_table class which take the file path of the input file.
    - use `Parsing_table::get_parsing_table()`:
      - return a map its key is a pair of non terminal and terminal its value is a vector of production part which          represent the prodcution rule for this entry of the paring table.
        
### Assumptions 
 - input rules are left recursion eliminated and left factored
 - "#" represent epsilon in the parsing table

### Hints
- All source files must be in `src` folder.
- All header files must be in `include` folder.
- **header files not source files** must be included in source files.
