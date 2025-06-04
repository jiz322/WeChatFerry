if not exist "%~dp0Out" md "%~dp0Out"
xcopy /Y "%~dp0x64\Release\spy.dll" "%~dp0Out"
xcopy /Y "%~dp0x64\Release\spy.exp" "%~dp0Out"
xcopy /Y "%~dp0x64\Release\spy.lib" "%~dp0Out"
xcopy /Y "%~dp0DISCLAIMER.md"   "%~dp0Out"
xcopy /Y "%~dp0x64\Release\spy.dll" "%~dp0..\clients\python\wcferry"
xcopy /Y "%~dp0DISCLAIMER.md"       "%~dp0..\clients\python\wcferry"
exit /b 0
