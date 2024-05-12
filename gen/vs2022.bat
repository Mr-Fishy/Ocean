
pushd ..\

echo Generating Visual Studio 2022 Solution Files
call vendor\premake\premake5.exe vs2022

PAUSE
