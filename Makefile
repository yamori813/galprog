#
# Makefile for galprog
#

FT_DXX_LIB=../
FT_DXX_INC=../

OBJS = main.o gal.o jedec.o bitbang.o

galprog: $(OBJS) dylib
	cc -o galprog $(OBJS) -L. -lftd2xx 

.c.o:
	cc -c -I$(FT_DXX_INC) $<

dylib:
	cp $(FT_DXX_LIB)/libftd2xx.*.dylib libftd2xx.dylib
	install_name_tool -id @executable_path/libftd2xx.dylib libftd2xx.dylib

un:
	sudo kextunload /System/Library/Extensions/AppleUSBFTDI.kext

clean:
	rm -rf libftd2xx.dylib *.o galprog
