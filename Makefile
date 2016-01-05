PREFIX  = x86_64-w64-mingw32
CC      = $(PREFIX)-c++
all: studio-link.cpp studio-link.h
	$(CC) -g -static-libgcc -static-libstdc++ -Wall -shared -o studio-link.dll studio-link.cpp vstsdk2.4/public.sdk/source/vst2.x/audioeffect.cpp vstsdk2.4/public.sdk/source/vst2.x/audioeffectx.cpp vstsdk2.4/public.sdk/source/vst2.x/vstplugmain.cpp -I vstsdk2.4/public.sdk/source/vst2.x/ -I vstsdk2.4/ -I ../my_include  ../baresip/libbaresip.a \
		    ../re/libre.a ../rem/librem.a -lwsock32 -liphlpapi -lssl -lcrypt32 -lcrypto -lws2_32 -lgdi32 -lz -lwinmm -lm -lopus -pthread -L/usr/$(PREFIX)/lib -static -Wl,--export-all-symbols
