# CH32V003 SDK template

## Usage
1. Compile your own GCC, usually GCC distributions lack rv32emc. _I wonder why..._
2. Call CMake with your GCC path.
```bash
cmake -B build -DCOMPILER_PATH=<your path> -DCOMPILER_PREFIX=<your prefix> && make -C build/
```
3. Upload using `wlink`
```bash
wlink -v flash build/template.bin
```

## Why a custom template?

The official wch SDK (_if you can even call it that_) is... lacking... at best. Good luck finding a CMakeLists in there. Or even a makefile. Or even an instruction on how to compile it. You get the point. Also for some reason this SDK tries **really** hard to be STM32 HAL. To the point where calling it "a very obvious inspiration" becomes an understatement.