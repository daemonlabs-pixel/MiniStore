param(
    [Parameter(Mandatory = $true)]
    [ValidateSet("C", "C++")]
    [string]$Language,

    [Parameter(Mandatory = $true)]
    [ValidateSet("Auto", "Executable", "Object")]
    [string]$Mode,

    [Parameter(Mandatory = $true)]
    [string]$SourceFile
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path -LiteralPath $SourceFile -PathType Leaf)) {
    Write-Error "Source file not found: $SourceFile"
    exit 1
}

$SourceFile = (Resolve-Path -LiteralPath $SourceFile).Path
$SourceDirectory = Split-Path $SourceFile -Parent
$Extension = [System.IO.Path]::GetExtension($SourceFile).ToLower()

if ($Language -eq "C") {
    $Compiler = "C:\msys64\ucrt64\bin\gcc.exe"
    $ExpectedExtension = ".c"
    $Standard = "c17"
}
else {
    $Compiler = "C:\msys64\ucrt64\bin\g++.exe"
    $ExpectedExtension = ".cpp"
    $Standard = "c++17"
}

if ($Extension -ne $ExpectedExtension) {
    Write-Error "Expected a $ExpectedExtension source file."
    exit 1
}

if (-not (Test-Path $Compiler)) {
    Write-Error "Compiler not found: $Compiler"
    exit 1
}

$ProjectDirectory = $SourceDirectory

if ((Split-Path $SourceDirectory -Leaf) -eq "src") {
    $ProjectDirectory = Split-Path $SourceDirectory -Parent
}

$SrcDirectory = Join-Path $ProjectDirectory "src"

if (Test-Path $SrcDirectory -PathType Container) {
    $SearchDirectory = $SrcDirectory
}
else {
    $SearchDirectory = $ProjectDirectory
}

$SourceFiles = @(
    Get-ChildItem -LiteralPath $SearchDirectory `
        -Filter "*$ExpectedExtension" -File -Recurse |
        Where-Object {
            $_.FullName -notmatch '[\\/](build|\.git|\.vscode)[\\/]'
        } |
        Sort-Object FullName |
        ForEach-Object { $_.FullName }
)

if ($SourceFiles.Count -eq 0) {
    Write-Error "No $ExpectedExtension source files found."
    exit 1
}

$BuildDirectory = Join-Path $ProjectDirectory "build"

if (-not (Test-Path $BuildDirectory)) {
    New-Item -ItemType Directory -Path $BuildDirectory | Out-Null
}

$MainFiles = @(
    foreach ($File in $SourceFiles) {
        $Content = Get-Content -LiteralPath $File -Raw

        if ($Content -match '(?m)^\s*(?:(?:int|signed)\s+)?main\s*\(') {
            $File
        }
    }
)

if ($Mode -eq "Auto") {
    if ($MainFiles.Count -eq 1) {
        $Mode = "Executable"
    }
    elseif ($MainFiles.Count -eq 0) {
        $Mode = "Object"
    }
    else {
        Write-Error "Multiple main() functions detected in this project."
        exit 1
    }
}

if ($Mode -eq "Executable" -and $MainFiles.Count -eq 0) {
    Write-Error "No main() function found. Cannot build an executable."
    exit 1
}

$Arguments = @(
    "-std=$Standard",
    "-Wall",
    "-Wextra",
    "-g"
)

if ($Language -eq "C++") {
    $Arguments += "-Wpedantic"
}

$IncludeDirectory = Join-Path $ProjectDirectory "include"

if (Test-Path $IncludeDirectory -PathType Container) {
    $Arguments += "-I"
    $Arguments += $IncludeDirectory
}

$Arguments += "-I"
$Arguments += $ProjectDirectory

Write-Host ""
Write-Host "Language: $Language"
Write-Host "Project: $ProjectDirectory"
Write-Host "Mode: $Mode"
Write-Host ""

if ($Mode -eq "Executable") {
    $ProjectName = Split-Path $ProjectDirectory -Leaf
    $Output = Join-Path $BuildDirectory "$ProjectName.exe"

    if (Test-Path $Output) {
        Remove-Item $Output -Force
    }

    $Arguments += $SourceFiles
    $Arguments += "-o"
    $Arguments += $Output

    Write-Host "Building executable..."

    & $Compiler @Arguments

    if ($LASTEXITCODE -ne 0) {
        Write-Error "Executable compilation failed."
        exit $LASTEXITCODE
    }

    Write-Host "`nBuild successful: $Output"
}
elseif ($Mode -eq "Object") {
    $ObjectDirectory = Join-Path $BuildDirectory "objects"

    if (Test-Path $ObjectDirectory) {
        Remove-Item $ObjectDirectory -Recurse -Force
    }

    New-Item -ItemType Directory -Path $ObjectDirectory | Out-Null

    Write-Host "Compiling object files..."

    for ($i = 0; $i -lt $SourceFiles.Count; $i++) {
        $Source = $SourceFiles[$i]
        $FileName = [System.IO.Path]::GetFileNameWithoutExtension($Source)

        $ObjectName = "{0:D3}_{1}.o" -f $i, $FileName
        $ObjectPath = Join-Path $ObjectDirectory $ObjectName

        $ObjectArguments = @($Arguments) + @(
            "-c",
            $Source,
            "-o",
            $ObjectPath
        )

        Write-Host "Compiling: $Source"

        & $Compiler @ObjectArguments

        if ($LASTEXITCODE -ne 0) {
            Write-Error "Compilation failed: $Source"
            exit $LASTEXITCODE
        }
    }

    Write-Host "`nObject compilation successful."
    Write-Host "Output directory: $ObjectDirectory"
}