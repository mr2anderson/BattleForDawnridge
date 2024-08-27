# Battle for Dawnridge
![](docs/screenshot.jpg)
## Compilation
* If you use _Visual Studio_, place *SFML* library in _extlibs_ directory and *boost* library, compiled with *serialization*, *iostreams* and **bzip2** support in _boost_1_85_0_ directory.
* In case you use _Linux_, install all necessary libraries and copy CMakeLists.txt file from _linux_ dir to the root.
### Warning
The server and client must have the same version of some libraries. For details, see ![](src/main.cpp)