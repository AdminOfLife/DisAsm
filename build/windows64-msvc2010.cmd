rmdir /s/q windows64-msvc2010
mkdir windows64-msvc2010
copy CMakeLists.txt windows64-msvc2010\CMakeLists.txt
cd windows64-msvc2010
cmake -G "Visual Studio 10 Win64" -DCMAKE_C_FLAGS="/W4"
"%VS100COMNTOOLS%\..\IDE\devenv.com" DisAsm.sln /Build "Debug|x64"
"%VS100COMNTOOLS%\..\IDE\devenv.com" DisAsm.sln /Build "Release|x64"
Release\DisAsmTest.exe
cd ..
