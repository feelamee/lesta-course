#### lesta-course

###### build
    git clone --recurse-submodules https://github.com/hiebyshek/lesta-course
    cd lesta-course/
    mkdir build/
    cd build/
    cmake .. -DBUILD_ALL=ON [-DSDL_STATIC=ON -DSDL_STATIC_PIC=ON]*
    cmake --build .

*optional arguments
