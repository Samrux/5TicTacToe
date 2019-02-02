echo off

set RUNTIME=linux-x64
dotnet publish TTT5.csproj --runtime %RUNTIME% --configuration Release --self-contained

::del /Q "bin\Release\netcoreapp2.0\%RUNTIME%\*"

pause