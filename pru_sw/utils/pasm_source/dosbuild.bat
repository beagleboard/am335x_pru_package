cl -W3 -D_CRT_SECURE_NO_WARNINGS pasm.c pasmpp.c pasmexp.c pasmop.c pasmdot.c pasmstruct.c pasmmacro.c path_utils.c /Fe..\pasm.exe
del *.obj

