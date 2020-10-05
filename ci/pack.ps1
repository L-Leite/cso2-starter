function CreateDirectory {
    param( [string]$newDirectory)
    New-Item -ItemType Directory -Path $newDirectory
}

$curBuildCombo = $env:BUILD_COMBO
$curConfig = $env:CONFIGURATION

# only package on Release builds, but don't error out
if ($curConfig -ne 'Release') {
    Write-Host 'Non release build detected, exiting packaging script...'
    exit 0
}

$isGccBuild = $curBuildCombo -eq 'linux-gcc'
$isLinuxClangBuild = $curBuildCombo -eq 'linux-clang'
$isMingwBuild = $curBuildCombo -eq 'windows-mingw'
$isMsvcBuild = $curBuildCombo -eq 'windows-msvc'

Write-Host "Running packaging script..."
Write-Host "Current setup build combo is: $curBuildCombo"

# create dir to store package files
CreateDirectory ./build/package

if ($isLinux) {
    # copy AppImage prebuilt files
    Copy-Item ./appimage/* -Destination ./build/package/
    
    # copy starter itself to the package dir
    Copy-Item ./build/cso2-starter -Destination ./build/package/

}
elseif ($isWindows) {
    # copy starter itself to the package dir
    Copy-Item "./build/$curConfig/cso2-starter.exe" -Destination ./build/package/
}
else {
    Write-Error 'An unknown OS is running this script, implement me.'
    exit 1
}

# copy breeze icons
Copy-Item ./build/icons-breeze.rcc -Destination ./build/package/

# copy license
Copy-Item ./COPYING -Destination ./build/package/

# copy README
Copy-Item ./README.md -Destination ./build/package/

# get app version
$versionStr = Get-Content -Path ./version.txt -TotalCount 1
Write-Host "cso2-starter version: $versionStr"

Push-Location ./build
Push-Location ./package

if ($isLinux) {
    # retrieve deployment tool
    wget -c "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
    chmod a+x linuxdeployqt-continuous-x86_64.AppImage

    $env:VERSION = $versionStr;
    ./linuxdeployqt-continuous-x86_64.AppImage ./cso2-starter -extra-plugins=iconengines,imageformats -appimage

    if ($isGccBuild) {
        Move-Item *.AppImage -Destination "cso2-starter-$versionStr-linux64_gcc.AppImage"
    }
    elseif ($isLinuxClangBuild) {
        Move-Item *.AppImage -Destination "cso2-starter-$versionStr-linux64_clang.AppImage"
    }

    Pop-Location
}
elseif ($isWindows) {
    $windeployBin = ''

    if ($isMingwBuild) {
        $windeployBin = 'C:\Qt\5.14\mingw73_64\bin\windeployqt.exe'
    }
    elseif ($isMsvcBuild) {        
        $windeployBin = 'C:\Qt\5.14\msvc2017_64\bin\windeployqt.exe'
    }
    else {        
        Write-Error 'Unknown build combo detected.'
        exit 1
    }

    if ($curConfig -eq 'Release') {
        if ($isMingwBuild) {
            & $windeployBin ./cso2-starter.exe
        }
        else {
            & $windeployBin --release ./cso2-starter.exe
        }
    }
    else {       
        & $windeployBin ./cso2-starter.exe
    }

    Pop-Location

    if ($isMingwBuild) {       
        7z a -t7z -m0=lzma2 -mx=9 -mfb=64 -md=64m -ms=on "cso2-starter-$versionStr-win64_mingw.7z" ./package/*
    }
    elseif ($isMsvcBuild) {       
        7z a -t7z -m0=lzma2 -mx=9 -mfb=64 -md=64m -ms=on "cso2-starter-$versionStr-win64_msvc.7z" ./package/*
    }
}

Pop-Location
