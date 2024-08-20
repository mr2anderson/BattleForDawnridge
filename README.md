# Battle for Dawnridge
Here you can watch gameplay recorded on one of the early versions of the game
[![](https://img.youtube.com/vi/bAOoODWE8Ok/0.jpg)](https://www.youtube.com/watch?v=bAOoODWE8Ok)
## Compilation
* If you use _Visual Studio_, place *SFML* library in _extlibs_ directory and *boost* library, compiled with *serialization*, *iostreams* and *bzip2* support, in _boost_1_85_0_ directory.
* In case you use _Linux_, install all necessary libraries and copy CMakeLists.txt file from _linux_ dir to the root.
### Warning
The server and client must have the same version of the Boost library. Currently the main server is using 1.85.0.