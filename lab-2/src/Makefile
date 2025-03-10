all: ex.exe

ex.exe: main.obj countSort.obj parser.obj generator.obj
	cl /std:c++17 /Fe:ex.exe main.obj countSort.obj parser.obj generator.obj

%.obj: %.cpp
	cl /std:c++17 /c $< /Fo$@

clean:
	del /Q *.obj *.ilk *.pdb ex.exe
