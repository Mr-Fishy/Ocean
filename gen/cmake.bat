
pushd ..\

echo Generating cmake Solution Files
call git submodule update --init --recursive
call vendor\premake\premake5.exe cmake

PAUSE
