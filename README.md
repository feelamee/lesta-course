#### lesta-course

###### build homework, nano, tests
    git clone --recurse-submodules https://github.com/hiebyshek/lesta-course
    cd lesta-course/
    mkdir build/
    cd build/
    cmake .. -DBUILD_ALL=ON [-DBUILD_NANO=ON -DBUILD_HW=ON -DBUILD_TESTS=ON -DADDRESS_SANITIZER=ON]*
    cmake --build .

###### build Tetris
    git clone --recurse-submodules https://github.com/hiebyshek/lesta-course
    cd lesta-course/tetris
    mkdir build/
    cd build/
    cmake .. [-DADDRESS_SANITIZER=ON]*
    cmake --build .

*optional arguments
