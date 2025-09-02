@echo off

set config=Debug

cd /d "%~dp0.."

if exist external rmdir /s /q external
if exist build rmdir /s /q build

mkdir external
mkdir build

cmake -B build -G "Unix Makefiles"
cmake --build build --config %config%