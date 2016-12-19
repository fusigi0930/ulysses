md %1\..\auto\inc
md %1\..\auto\lib
copy %1\luacore.h %1\..\auto\inc
copy %1\luacore_global.h %1\..\auto\inc
copy %2\*.a %1\..\auto\lib
copy %1\lua-5.3.2\src\*.a %1\..\auto\lib
copy %1\lua-5.3.2\src\*.exe %2
copy %1\lua-5.3.2\src\*.dll %2
copy %1\opencv\bin\*.dll %2

exit 0