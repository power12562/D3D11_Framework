@echo off
chcp 65001

echo [확인 단계] 삭제 대상 폴더를 검색 중입니다...

:: 삭제 대상 폴더 검색 및 출력
for %%f in (.vs x64 x86 bin) do (
    for /d /r %~dp0 %%d in (%%f) do (
        if exist "%%d" echo 삭제할 폴더: "%%d"
    )
)

echo.
echo 삭제를 진행하시겠습니까? (Y/N)
choice /c YN /n /m "선택: "

if errorlevel 2 (
    echo 삭제를 취소했습니다.
    pause
    exit /b
)

echo 삭제를 진행합니다...

:: 실제 삭제 작업 수행
for %%f in (.vs x64 x86 bin) do (
    for /d /r %~dp0 %%d in (%%f) do (
        if exist "%%d" rd /s /q "%%d"
    )
)

echo 모든 지정된 폴더가 삭제되었습니다.

pause
