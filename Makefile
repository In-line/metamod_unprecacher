HLSDK = ../hlsdk-master
METAMOD = ../metamod-hl1-master/metamod
M_INCLUDE = include

NAME = metamod_unprecacher
COMPILER = g++

OBJECTS = meta_api.cpp dllapi.cpp engine_api.cpp sdk_util.cpp unprecache_list.cpp h_export.cpp cstring_utils.cpp unprecache_list_string.cpp config_file.cpp sdk_util_custom.cpp unprecache_list.h global_variables.h unprecache_list_string.h sdk_util_custom.h cstring_utils.h config_file.h

LINK = -lm -ldl  -static-libgcc

OPT_FLAGS = -O2 -funroll-loops -s -pipe -fomit-frame-pointer -fno-strict-aliasing

INCLUDE = -I. -I$(M_INCLUDE)/ -I$(HLSDK)/common -I$(HLSDK)/dlls -I$(HLSDK)/engine -I$(HLSDK)/pm_shared -I$(HLSDK)/public -I$(METAMOD)

BIN_DIR = Release
CFLAGS = $(OPT_FLAGS)

CFLAGS += -g0 -fvisibility=hidden -fvisibility-inlines-hidden -DNDEBUG -Dlinux -std=c++0x -shared

OBJ_LINUX := $(OBJECTS:%.c=$(BIN_DIR)/%.o)

$(BIN_DIR)/%.o: %.c
	$(COMPILER) $(INCLUDE) $(CFLAGS) -o $@ -c $<

all:
	mkdir -p $(BIN_DIR)

	$(MAKE) $(NAME) && strip -x $(BIN_DIR)/$(NAME)_mm_i386.so

$(NAME): $(OBJ_LINUX)
	$(COMPILER) $(INCLUDE) $(CFLAGS) $(OBJ_LINUX) $(LINK) -o$(BIN_DIR)/$(NAME)_mm_i386.so

check:
	cppcheck $(INCLUDE) --quiet --max-configs=100 -D__linux__ -DNDEBUG -DHAVE_STDINT_H .

debug:	
	$(MAKE) all DEBUG=false

default: all

clean:
	rm -rf Release/*.o
	rm -rf Release/$(NAME)_mm_i386.so
	rm -rf Debug/*.o
	rm -rf Debug/$(NAME)_mm_i386.so