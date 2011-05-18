::@echo off
call app_info_setup.bat

::Kill old one if running
::plink -P 10022 root@localhost -pw "" "killall -9 gdbserver %APPNAME%"

::Copy file over? (commented out for now, don't need it)
::pscp -scp -P 10022 -pw "" %APPNAME% root@localhost:/media/cryptofs/apps/usr/palm/applications/%BUNDLEIDPREFIX%%APPNAME%

::play a sfx
start ..\..\shared\win\utils\playwav.exe ..\..\shared\media\success.wav

::Run it
plink -P 10022 root@localhost -pw "" "/media/cryptofs/apps/usr/palm/applications/%BUNDLEIDPREFIX%%APPNAME%/%APPNAME%"

