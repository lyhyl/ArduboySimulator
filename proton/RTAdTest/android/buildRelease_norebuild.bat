#call Clean.bat

call build_prepare.bat
:Next package it with the java part

call ant release
call InstallOnDefaultPhoneRelease.bat
pause