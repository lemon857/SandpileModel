# How to compile and run it?

For compile it you should install any cpp compiler, [cmake](https://cmake.org/) and [git](https://git-scm.com/) on your pc

If you already have it:

Clone this repo:
```bash
git clone https://github.com/lemon857/SandpileModel.git
```

Sync git submodules:
```bash
git submodule update --init --recursive
```

Configure and compile it in main directory:
```bash
cmake . -B build
cmake --build build
```

Run the executable file located in `build/bin` directory and named like `SandpileModel` or `SandpileModel.exe`  
For example for unix-like systems:
```bash
./build/bin/SandpileModel
```
Use `Left Mouse Button` click on field to add some amout of sand  
Use `Space` to start or stop simulation  

# A little bit about it

It is a simple version simulation of [Abelian sandpile model](https://en.wikipedia.org/wiki/Abelian_sandpile_model)  

This simulation render on your screen with my [cpp engine](https://github.com/lemon857/CppEngine)  

All other logic is very simple and just calculate new state based on previous state
