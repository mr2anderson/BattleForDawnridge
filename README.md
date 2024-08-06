# Battle for Dawnridge
Here you can watch gameplay recorded on one of the early versions of the game
[![](https://img.youtube.com/vi/bAOoODWE8Ok/0.jpg)](https://www.youtube.com/watch?v=bAOoODWE8Ok)
## Dependents
* **SFML** **System**, **Window**, **Graphics**, **Audio**
* **Boost** **serialization**, **iostreams**
* **bzip2**
## Compilation
* If you use Visual Studio, you should place SFML library in _extlibs_ directory and boost library, compiled with serialization, iostreams and bzip2 support, in _boost_1_85_0_ directory and compile using Visual Studio UI.
* In case you use linux, copy CMakeLists.txt file from _linux_ dir to repository root and compile it using _cmake CMakeLists.txt & make_. Make sure SFML, boost and bzip2 development packages installed in your system.
