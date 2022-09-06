# DragonslayerEngine
A rendering engine under development

# Running the engine

- Clone the repository and its git submodules:
```console
git clone --recursive https://github.com/LongJohnny/DragonslayerEngine.git
```

- Create build folder and move to it
```console
mkdir -p build/out
cd build/out
```

- Generate makefile using CMake:
```console
cmake ../../ -DBUILD_SHARED_LIBS=false -DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=false -DASSIMP_BUILD_ASSIMP_TOOLS=false -DASSIMP_BUILD_TESTS=false -DASSIMP_INSTALL=false -DASSIMP_INSTALL_PDB=false -DASSIMP_BUILD_ASSIMP_VIEW=false -DASSIMP_INJECT_DEBUG_POSTFIX=false
```

