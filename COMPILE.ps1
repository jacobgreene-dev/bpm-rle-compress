# COMPILE.ps1 - Script to compile the project

$sourceFiles = @("src/main.cpp", "src/rle_compress.cpp", "src/rle_decompress.cpp")
$outputExecutable = "rlec.exe"

# Compile
g++ $sourceFiles -o $outputExecutable -std=c++17 -Wall -Wextra

# Check
if ($LASTEXITCODE -eq 0) {
    Write-Host "Compilation successful. Executable created: $outputExecutable" -ForegroundColor Green
} else {
    Write-Host "Compilation failed. Please check the error messages above." -ForegroundColor Red
}