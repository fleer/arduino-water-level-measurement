ARDUINO_DIR = /Applications/Arduino.app/Contents/Java
#ARDMK_DIR = $(HOME)/.arduino_mk
CTAGS_PATH = $(ARDUINO_DIR)/tools-builder/ctags/5.8-arduino11/ctags

#include $(ARDMK_DIR)/Arduino.mk

.PHONY: ctags
ctags:
	$(CTAGS_PATH) -f tags.cpp $(shell find . -name "*.cpp" -o -name "*.h")
	$(CTAGS_PATH) -f tags.ino --langmap=c++:.ino $(shell find . -name "*.ino")
	cat tags.cpp tags.ino > tags
	sort tags -o tags
	rm -f tags.*
