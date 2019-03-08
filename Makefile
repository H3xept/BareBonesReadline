CC = gcc
CFLAGS = -Wall --pedantic --std=c99
SOURCE_FOLDER = src
OUT_FOLDER = bin
SOURCE_FILES = $(SOURCE_FOLDER)/*.c
LIB_NAME = libreadline.a
DEPENDENCIES_FOLDER = dependencies

ANSI_IS_A_CURSE = ANSIsACurse
ANSI_IS_A_CURSE_LOCATION = ../$(ANSI_IS_A_CURSE)
ANSI_IS_A_CURSE_LOCATION_GIT = "https://github.com/H3xept/ANSIsACurse"

HISTORY = BareBonesHistory
HISTORY_LOCATION = ../$(HISTORY)
HISTORY_LOCATION_GIT = "https://github.com/H3xept/BareBonesHistory"

all: taget_dir prepare_dependencies test compile_dylib copy_headers
	
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
		cp $(ANSI_IS_A_CURSE)/bin/libs/*.a $(DEPENDENCIES_FOLDER)/libs ; \
		rm -rf $(ANSI_IS_A_CURSE); \
	fi;
	@ if [ ! -d $(DEPENDENCIES_FOLDER)/$(HISTORY) ]; then \
		make dep_HISTORY; \
	fi;


dep_HISTORY:
	@ mkdir -p $(DEPENDENCIES_FOLDER)/$(HISTORY)
	@ if [ -d $(HISTORY_LOCATION) ]; then\
		echo "Folder found locally, running make..."; \
		echo "Making $$HISTORY ... "; \
		make -C $(HISTORY_LOCATION); \
		cp $(HISTORY_LOCATION)/bin/headers/*.h $(DEPENDENCIES_FOLDER)/$(HISTORY)/ ; \
		cp $(HISTORY_LOCATION)/bin/libs/*.a $(DEPENDENCIES_FOLDER)/libs ; \
	else \
		echo "Fonder not found, cloning ..."; \
		git clone $(HISTORY_LOCATION_GIT) $(HISTORY); \
		echo "Making $$HISTORY ..." ; \
		make -C $(HISTORY); \
		cp $(HISTORY)/bin/headers/*.h $(DEPENDENCIES_FOLDER)/$(HISTORY)/ ; \
		cp $(HISTORY)/bin/libs/*.a $(DEPENDENCIES_FOLDER)/libs ; \
		rm -rf $(HISTORY); \
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

test:
	@ echo "\033[1mInit testing for $(OUT_NAME)...\033[0m"
	@ python3 runTests.py tests/*Test.h
	@ echo "\033[32m\033[1mTesting complete!\033[0m"
	

clean:
	@ echo "Cleaning..."
	@ rm -rf bin/
	@ rm -rf dependencies/

