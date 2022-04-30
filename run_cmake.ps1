if (!(Test-Path -Path build)){
    mkdir build
}
Set-Location build
cmake -D CMAKE_C_COMPILER=gcc ..
cmake --build .
Set-Location ..