Notes for Windows users:
========================

Graphviz
--------
You have to install the Graphviz Package (for msvc only the package graphviz-2.22.0.msi
is known to work) and to add the bin directory to the PATH environmend variable.
This is done by the installer itself, when selecting installation for everyone on this computer.

MSVC runtime
------------
If one of the following error messages occurs:
- The application failed to initialize properly (0xc0000135).
- This application has failed to start because the application configuration is incorrect.
  Reinstalling application may fix this problem.
- The system cannot execute the specified program.
- R6034 An application has made an attempt to load the C runtime library incorrectly.
- This application has failed to start because MSVCR??.dll was not found.
  Re-installing the application may fix this problem.

then some of the packages below have to be installed:

for Graphviz:
Microsoft Visual C++ 2005 SP1 Redistributable Package (x86)
http://www.microsoft.com/downloads/details.aspx?familyid=200B2FD9-AE1A-4A14-984D-389C36F85647&displaylang=de

eventually needed:
Microsoft Visual C++ 2005 Redistributable Package (x86)
http://www.microsoft.com/downloads/details.aspx?familyid=32BC1BEE-A3F9-4C13-9C99-220B62A191EE&displaylang=de

Microsoft Visual C++ 2008 Redistributable Package (x86)
http://www.microsoft.com/downloads/details.aspx?familyid=9B2DA534-3E03-4391-8A4D-074B9F2BC1BF&displaylang=de

Microsoft Visual C++ 2008 SP1 Redistributable Package (x86)
http://www.microsoft.com/downloads/details.aspx?familyid=A5C84275-3B97-4AB7-A40D-3802B2AF5FC2&displaylang=de