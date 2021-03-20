$originalFolder = Get-Location
$localFolder = Split-Path $MyInvocation.MyCommand.Path
Set-Location $localFolder/build

Write-Host "`n`rGenerating makefiles..."
cmake . -G "MinGW Makefiles"

Write-Host "`n`rCompiling project..."
mingw32-make.exe

Set-Location $originalFolder
Write-Host "Operation complete."

#Read-Host "`n`rPress any key to continue..."
