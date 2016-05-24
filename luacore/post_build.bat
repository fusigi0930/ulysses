md %1\..\auto\inc
md %1\..\auto\lib
copy %1\libcore.h %1\..\auto\inc
copy %1\libcore_global.h %1\..\auto\inc
copy %2\*.a %1\..\auto\lib

exit 0