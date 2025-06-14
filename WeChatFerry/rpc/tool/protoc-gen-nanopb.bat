@echo off
:: This file is used to invoke nanopb_generator.py as a plugin
:: to protoc on Windows.
:: Use it like this:
:: protoc --plugin=protoc-gen-nanopb=..../protoc-gen-nanopb.bat --nanopb_out=dir foo.proto
::
:: Note that if you use the binary package of nanopb, the protoc
:: path is already set up properly and there is no need to give
:: --plugin= on the command line.

set mydir=%~dp0
"D:\anaconda3\python.exe" "%mydir%\nanopb_generator.py" --protoc-plugin %*

