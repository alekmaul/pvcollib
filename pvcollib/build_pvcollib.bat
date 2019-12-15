@SET "DEVKITCOL=%cd:\=/%"
@SET SVGPA=%PATH%
@SET PATH=%DEVKITCOL%\bin;%PATH%

@ECHO.                               
@ECHO.                               
@ECHO.                               
@ECHO.
%DEVKITCOL%\bin\make -f Makelib clean
%DEVKITCOL%\bin\make -f Makelib release

@ECHO -------------------------------------------
@ECHO PVcollib is now ready! Press any key to exit...
@SET PATH=%SVGPA%
@PAUSE >nul
