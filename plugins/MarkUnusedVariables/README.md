# Unused vars marking Clang plugin

## Enviroment setup
```sh
export LLVM_INSTALL_DIR=/path/to/llvm-install
export PATH=$LLVM_INSTALL_DIR/bin:$PATH
export LD_LIBRARY_PATH=$LLVM_INSTALL_DIR/lib:$LD_LIBRARY_PATH
```

## Build
```sh
cmake ..
make
```

## Using
```sh
clang -Xclang -load -Xclang build/MarkUnusedVariables.so -Xclang -plugin -Xclang mark-unused tests/test.cpp
```
 
## Test
```sh
llvm-lit tests/test.cpp    
```       