

pushd ..\
call git submodule update --init --recursive
call vendor\bin\premake\premake5.exe vs2022

PAUSE