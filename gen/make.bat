
pushd ..\

echo Generating Make Files
call vendor\premake\premake5.exe gmake2

PAUSE
