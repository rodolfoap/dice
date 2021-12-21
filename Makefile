CPP  = g++
CC   = gcc
RM   = rm -f

CHIP_OBJ := chips/7400.o chips/7402.o chips/7404.o chips/7407.o chips/7408.o chips/7410.o chips/7411.o chips/7420.o chips/7421.o chips/7425.o \
   			chips/7427.o chips/7430.o chips/7432.o chips/7437.o chips/7442.o chips/7448.o chips/7450.o chips/7454.o chips/7474.o chips/7475.o \
			chips/7476.o chips/7483.o chips/7485.o chips/7486.o chips/7490.o chips/7492.o chips/7493.o chips/7495.o chips/7496.o chips/74107.o \
		   	chips/74109.o chips/74S112.o chips/74116.o chips/74121.o chips/74150.o chips/74151.o chips/74153.o chips/74155.o chips/74164.o \
		   	chips/74165.o chips/74166.o chips/74174.o chips/74175.o chips/74191.o chips/74192.o chips/74193.o chips/74194.o chips/74279.o \
		   	chips/9301.o chips/9310.o chips/9311.o chips/9312.o chips/9314.o chips/9316.o chips/9321.o chips/9322.o \
		   	chips/9602.o chips/555astable.o chips/555mono.o chips/555pwm.o chips/74S287.o chips/8225.o chips/8277.o chips/2533.o \
			chips/82S16.o chips/82S115.o chips/82S123.o chips/82S131.o chips/TMS4800.o \
			chips/clock.o chips/capacitor.o chips/diode_matrix.o chips/latch.o chips/clk_gate.o chips/wired_logic.o \
			chips/mixer.o chips/566.o \
			chips/input.o chips/audio.o chips/video.o chips/dipswitch.o chips/rom.o chips/vcd_log.o chips/wav_log.o 

GAME_OBJ := games/pong.o games/rebound.o games/gotcha.o games/spacerace.o games/stuntcycle.o games/pongdoubles.o \
   			games/tvbasketball.o games/breakout.o games/antiaircraft.o games/attack.o \
		   	games/sharkjaws.o games/quadrapong.o games/jetfighter.o games/crashnscore.o \
			games/crossfire.o games/pinpong.o games/cleansweep.o games/wipeout.o \
			games/highway.o games/steeplechase.o games/indy4.o

MANYMOUSE_OBJ := manymouse/manymouse.o manymouse/windows_wminput.o manymouse/linux_evdev.o \
				 manymouse/macosx_hidmanager.o manymouse/macosx_hidutilities.o manymouse/x11_xinput2.o

OBJ := main.o chip.o circuit.o settings.o game_config.o phoenix/phoenix.o $(CHIP_OBJ) $(GAME_OBJ) $(MANYMOUSE_OBJ)

LIBS := -s
CFLAGS := -Iphoenix -O3 #-g -march=core2 #-march=i686 #-fprofile-generate #-fprofile-use #-flto #-Wall
CPPFLAGS = $(CFLAGS) -std=c++11

BIN := dice

ifeq ($(PLATFORM),)
    uname := $(shell uname -a)
    ifeq ($(uname),)
        PLATFORM := windows
    else ifneq ($(findstring Darwin,$(uname)),)
        PLATFORM := osx
    else ifneq ($(findstring Linux,$(uname)),)
        PLATFORM := linux
    endif
endif

ifeq ($(PLATFORM),windows)
	OBJ += ui/dice.o
	LIBS += -lSDL -static-libgcc -static-libstdc++ -lmingw32 -lopengl32 -lkernel32 -luser32 -lgdi32 -ladvapi32 -lcomctl32 -lcomdlg32 -lshell32 -lole32 -mwindows #-mconsole
	CFLAGS += -DPHOENIX_WINDOWS
	BIN := dice.exe
else ifeq ($(PLATFORM),linux)
	OBJ := phoenix/qt/platform.moc $(OBJ)
	LIBS += `pkg-config --libs QtCore QtGui` -lSDL -lGL -ldl -lX11
	CFLAGS += `pkg-config --cflags QtCore QtGui` -DPHOENIX_QT
	#LIBS += `pkg-config --libs gtk+-2.0` -lX11 -lGL
	#CFLAGS += -DPHOENIX_GTK `pkg-config --cflags gtk+-2.0`
else ifeq ($(PLATFORM),osx)
	LIBS += -lc++ -lobjc -framework OpenGL -framework SDL -framework Cocoa -framework Carbon -framework IOKit
	CFLAGS += -x objective-c++ -stdlib=libc++ -DPHOENIX_COCOA -F/Library/Frameworks
endif

# For profiling
#LIBS += -pg 
#CFLAGS += -pg 

# For gcov
#CFLAGS += -fprofile-arcs -ftest-coverage

all: $(BIN)

clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(filter %.o,$(OBJ)) -o "$(BIN)" $(CPPFLAGS) $(LIBS)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.rc
	windres $< $@

%.moc: %.moc.hpp
	moc -i -o $@ $<

dumpasm: $(BIN)
	objdump -d -M intel -S $(BIN) > $(BIN).s

profile: $(BIN)
	$(BIN)
	gprof $(BIN) > prof.txt

coverage: $(BIN)
	$(BIN)
	gcov chip.cpp

osx: $(BIN)
	mkdir -p dice.app/Contents/MacOS
	mkdir -p dice.app/Contents/Resources
	cp $(BIN) dice.app/Contents/MacOS
	cp ui/Info.plist dice.app/Contents/Info.plist
	cp ui/icon.icns dice.app/Contents/Resources/dice.icns
