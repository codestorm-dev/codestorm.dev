@echo off
rem Compila o estoque.c e abre o programa.
rem Usa o gcc do PATH ou, se nao tiver, o gcc que vem com o Dev-C++.
cd /d "%~dp0"

set "GCC="
where gcc >nul 2>nul
if not errorlevel 1 set "GCC=gcc"
if not defined GCC if exist "C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\gcc.exe" set "GCC=C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\gcc.exe"
if not defined GCC if exist "C:\Program Files\Dev-Cpp\MinGW64\bin\gcc.exe" set "GCC=C:\Program Files\Dev-Cpp\MinGW64\bin\gcc.exe"

if not defined GCC (
    echo gcc nao encontrado. Instale o Dev-C++ ou o MinGW e tente de novo.
    pause
    exit /b 1
)

echo Compilando estoque.c ...
"%GCC%" -Wall -Wextra -std=c99 -o estoque.exe estoque.c
if errorlevel 1 (
    echo.
    echo Erro na compilacao.
    pause
    exit /b 1
)

echo Compilado sem erros. Abrindo o programa...
echo.
"%~dp0estoque.exe"
pause
