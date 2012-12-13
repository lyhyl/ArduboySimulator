call app_info_setup.bat
REM get our ftp logon info
call ../../../SetFTPLogonInfo.bat
rmdir upload /S /Q
mkdir upload
copy *.swf upload\
copy *.html upload\
ncftpput -u %_FTP_USER_% -p %_FTP_PASS_% -R %_FTP_SITE_% /www/games/test upload\*

echo File uploaded:  http://www.rtsoft.com/games/test/%FILENAME%.swf

:Let's go ahead an open a browser to test it
start http://www.rtsoft.com/games/test/test1.html
pause