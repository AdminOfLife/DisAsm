rmdir /s/q windows32-msvc6
mkdir windows32-msvc6
cd windows32-msvc6
cmake -G "Visual Studio 6" -DCMAKE_C_FLAGS="/W4 /Zm200" ..
"%MSDEVDIR%\Bin\msdev.exe" DisAsm.dsw /MAKE "ALL_BUILD - Win32 Release"
Release\DisAsmTest.exe
Release\DisAsmBenchmark.exe %IPP_HOME%\ipp.bin
cd ..
