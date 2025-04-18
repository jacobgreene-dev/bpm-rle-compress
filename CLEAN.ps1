# CLEAN.ps1 - Script to clean up build artifacts

$outputExecutable = "rlec.exe"

$staticFolder = ".\static"

# Remove the outputExecutable if it exists
if (Test-Path $outputExecutable) {
    Remove-Item $outputExecutable -Force
    Write-Host "Cleaned up: $outputExecutable" -ForegroundColor Green
} else {
    Write-Host "No executable found to clean." -ForegroundColor Yellow
}

# Clean static folder
if (Test-Path $staticFolder) {
    Get-ChildItem $staticFolder -File | Where-Object {
        $_.Name -notin @("blackbuck.bmp", "snail.bmp", "lena.bmp")
    } | Remove-Item -Force
    Write-Host "Cleaned up: static folder, preserving blackbuck.bmp, snail.bmp, and lena.bmp." -ForegroundColor Green
} else {
    Write-Host "Static folder not found. Skipping cleanup for static files." -ForegroundColor Yellow
}