#### lesta-course

##### build dependencies
- git
- cmake
- mingw-cmake (for Windows)

##### installing build dependencies
###### Archlinux
    sudo pacman -S git cmake mingw-w64-gcc
    paru -S mingw-w64-cmake (or build it by yourself)
    paru -S android-studio
###### Ubuntu
[install android-studio](https://dev.to/janetmutua/installing-android-studio-on-ubuntu-2204-complete-guide-1kh8)

    apt install git cmake mingw-w64


##### build homework, nano, tests
    git clone --recurse-submodules https://github.com/hiebyshek/lesta-course
    cd lesta-course/
    mkdir build/
    cd build/
    cmake .. -DBUILD_ALL=ON [-DBUILD_NANO=ON -DBUILD_HW=ON -DBUILD_TESTS=ON -DADDRESS_SANITIZER=ON]*
    cmake --build .

##### build Tetris for Linux
    git clone --recurse-submodules https://github.com/hiebyshek/lesta-course
    cd lesta-course/tetris
    mkdir build/
    cd build/
    cmake .. [-DADDRESS_SANITIZER=ON]*
    cmake --build .
    
##### build Tetris for Windows
    git clone --recurse-submodules https://github.com/hiebyshek/lesta-course
    cd lesta-course/tetris
    mkdir build/
    cd build/
    x86_64-w64-mingw-cmake .. [-DADDRESS_SANITIZER=ON]*
    cmake --build .

##### build Tetris for Android
    git clone --recurse-submodules https://github.com/hiebyshek/lesta-course
    cd lesta-course/tetris/android-project
    echo "sdk.dir=$HOME/Android/Sdk" > local.properties
    android-studio ./
Build project (Ctrl + F9)\
Replace $HOME/Android/Sdk witch your android sdk dir if need
    
*optional arguments
