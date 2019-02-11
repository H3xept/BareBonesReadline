CC = gcc
CFLAGS = -Wall --pedantic --std=c99
SOURCE_FOLDER = src
OUT_FOLDER = bin
SOURCE_FILES = $(SOURCE_FOLDER)/*.c
LIB_NAME = libreadline
DEPENDENCIES_FOLDER = dependencies

ANSI_IS_A_CURSE = ANSIsACurse
ANSI_IS_A_CURSE_LOCATION = ../$(ANSI_IS_A_CURSE)
ANSI_IS_A_CURSE_LOCATION_GIT = "https://github.com/H3xept/ANSIsACurse"

all: taget_dir prepare_dependencies compile_dylib copy_headers
	
prepare_dependencies: 
	@ echo "Preparing dependencies";
	@ if [ -d $(ANSI_IS_A_CURSE_LOCATION) ]; then\
		echo "Folder found locally, running make..."; \
		echo "Making $$ANSI_IS_A_CURSE"; \
		make -C $(ANSI_IS_A_CURSE_LOCATION); \
		cp $(ANSI_IS_A_CURSE_LOCATION)/bin/headers/*.h $(DEPENDENCIES_FOLDER)/$(ANSI_IS_A_CURSE)/ ; \
		cp $(ANSI_IS_A_CURSE_LOCATION)/bin/libs/*.* $(DEPENDENCIES_FOLDER)/libs ; \
	else \
		echo "Fonder not found, cloning ..."; \
		git clone $(ANSI_IS_A_CURSE_LOCATION_GIT) $(ANSI_IS_A_CURSE); \
		echo "Making $$ANSI_IS_A_CURSE ..." ; \
		make -C $(ANSI_IS_A_CURSE); \
		cp $(ANSI_IS_A_CURSE)/bin/headers/*.h $(DEPENDENCIES_FOLDER)/$(ANSI_IS_A_CURSE)/ ; \
		cp $(ANSI_IS_A_CURSE)/bin/libs/*.* $(DEPENDENCIES_FOLDER)/libs ; \
		rm -rf $(ANSI_IS_A_CURSE); \
	fi;

compile_dylib: taget_dir 
	@ echo "Compiling dylib..."
	@ $(CC) -I$(DEPENDENCIES_FOLDER) $(CFLAGS) $(SOURCE_FILES) -c 
	@ ar rs $(LIB_NAME) *.o
	@ cp $(LIB_NAME) bin/libs
	@ rm *.o $(LIB_NAME)

copy_headers: taget_dir
	@ echo "Copying headers..."
	@ cp $(SOURCE_FOLDER)/*.h $(OUT_FOLDER)/headers/

taget_dir:
	@ echo "Creating folder structure..."
	@ mkdir -p bin/headers
	@ mkdir -p bin/libs
	@ mkdir -p $(DEPENDENCIES_FOLDER)
	@ mkdir -p $(DEPENDENCIES_FOLDER)/$(ANSI_IS_A_CURSE)
	@ mkdir -p $(DEPENDENCIES_FOLDER)/libs

clean:
	@ echo "Cleaning..."
	@ rm -rf bin/
	@ rm -rf dependencies/

