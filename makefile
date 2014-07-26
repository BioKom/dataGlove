#
# @author Betti Oesterholz
# @date 00.07.2014
# @mail webmaster@BioKom.info
#
# System: make
#
# This makefile for the data glove programms (C++).
#
#
# Copyright (C) @c GPL3 2014 Betti Oesterholz
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# The data glove programms are for working with the data glove.
# It should convert hand gestures into actions.
#
# call:
# 	make
# 		to create the data glove programms
# 	make clean
# 		to clean all files this make has created
# 	make test
# 		to create the data glove programms testfiles (testcase/)
# 	make win
# 		to create the data glove programms windows
# 	make win_test
# 		to create the data glove programms windows testfiles (testcase/)
#
##########################################################################
#
# History:
# 00.07.2014   Oesterholz   created


##########################################################################
#
# folder variables
#
##########################################################################

include namespaces.mk


# relativ path to the data glove base directory
BASE_DIR:=.


# folders of the data glove source code
DIR_SRC:=src/
DIR_INCL:=incl/

# folder of the test cases
DIR_TEST:=test/

# folder of the objects
DIR_OBJ:=obj/

# folder of the test objects
DIR_OBJ_TEST:=obj_test/

# folder of the library
DIR_LIB:=lib/

# folder of the executebels
DIR_BIN:=bin/

# folder of the test executebels
DIR_TESTCASE:=testcase/


##########################################################################
#
# compile variables
#
##########################################################################

SHELL:=/bin/sh
CC:=gcc
#debugg flags
#CFLAG:=-g -Wall
#optimize compile flags

# TODO revert
CFLAG:=-O -O2

CFLAG_TEST:=-g -pg -Wall -DTEST
INCL:=-I$(BASE_DIR) -I$(DIR_INCL)
LIBS:=-lstdc++
# -lm -lpthread
LIBS_TEST:=$(LIBS)
LIBS_WIN:=-static -lstdc++ -lm
LIBS_WIN_TEST:=$(LIBS_WIN_TEST)


##########################################################################
#
# source variables
#
##########################################################################

# SRC_FILES:=$(SRC_FILES) $(shell ls $(DIR_SRC)/*.cpp )

#$(call generat_src_files, Subfolder)
#
# @param Subfolder the (namespace) subfolder which contains the src/ folder
#    with the source files
#
define generat_src_files
SRC_FILES:=$(SRC_FILES) $(shell ls $(1)/$(DIR_SRC)/*.cpp )
endef

$(foreach SUBFOLDER, $(NAMESPACES), $(eval $(call generat_src_files,$(SUBFOLDER)) ) )


##########################################################################
#
# include variables
#
##########################################################################

# INCL_FILES:=$(INCL_FILES) $(addprefix $(DIR_INCL), $(shell ls $(DIR_INCL)) )

#$(call generat_incl_files, Subfolder)
#
# @param Subfolder the (namespace) subfolder which contains the incl/ folder
#    with the header files
#
define generat_incl_files
INCL_FILES:=$(INCL_FILES) $(addprefix $(1)/$(DIR_INCL), $(shell ls $(1)/$(DIR_INCL)) )
INCL:=$(INCL) -I$(1)/$(DIR_INCL)
endef

$(foreach SUBFOLDER, $(NAMESPACES), $(eval $(call generat_incl_files,$(SUBFOLDER)) ) )



##########################################################################
#
# test variables
#
##########################################################################


TEST_TOOLS:=$(DIR_FIB)obj_test/tGeneratorForFibObjects.o

#$(call generat_test_files, Subfolder)
#
# @param Subfolder the (namespace) subfolder which contains the incl/ folder
#    with the header files
#
define generat_test_files
TEST_FILES:=$(TEST_FILES) $(addprefix $(1)/$(DIR_TEST), $(shell ls $(1)/$(DIR_TEST)) )
TEST_BIN:=$(TEST_BIN) $(addprefix $(1)/$(DIR_TESTCASE), $(subst .cpp,, $(shell ls $(1)/$(DIR_TEST)) ) )
endef

$(foreach SUBFOLDER, $(NAMESPACES), $(eval $(call generat_test_files,$(SUBFOLDER)) ) )


OBJ_TEST_FILES:=$(subst .cpp,.o,$(subst $(DIR_TEST),$(DIR_OBJ_TEST),$(TEST_FILES) ))


# TEST_PROGRAMS:=startDataGlove


##########################################################################
#
# object variables
#
##########################################################################

OBJ_DATA_GLOVE:=$(subst .cpp,.o,$(subst $(DIR_SRC),$(DIR_OBJ),$(SRC_FILES) ))

OBJ_TEST_FILES:=$(subst .cpp,.o,$(subst $(DIR_SRC),$(DIR_OBJ_TEST),$(SRC_FILES) ))



##########################################################################
#
# executebel variables
#
##########################################################################

# the to create executebels
EXE_FILES:=startDataGlove printDataGloveInfo

SRC_EXE_FILES:=$(addprefix $(DIR_SRC), $(addsuffix .cpp, $(EXE_FILES) ) )


OBJ_DATA_GLOVE := $(filter-out $(addprefix %, $(addsuffix .o, $(EXE_FILES) ) ),$(OBJ_DATA_GLOVE))



##########################################################################
#
# compile rules
#
##########################################################################

.PHONY: all test clean create_dirs win win_test

MAKEFILE_FLAG:=


all:create_dirs dependencies.dep\
	$(addprefix $(DIR_BIN), $(EXE_FILES) )

test: create_dirs dependencies.dep\
	$(addprefix $(DIR_BIN)test_, $(EXE_FILES) )

test: CFLAG:=$(CFLAG_TEST)
test: MAKEFILE_FLAG:=test


##########################################################################
#
# rules for windows compatibility
#
##########################################################################

win: CFLAG:=$(CFLAG) -DWINDOWS
win: all
win: MAKEFILE_FLAG:=win
win: LIBS:=$(LIBS_WIN)
win_test: create_dirs dependencies.dep\
	$(addprefix $(DIR_BIN)test_, $(EXE_FILES) )
win_test: CFLAG:=$(CFLAG_TEST) -DWINDOWS
win_test: MAKEFILE_FLAG:=win_test
win_test: LIBS_TEST:=$(LIBS_WIN_TEST)


##########################################################################
#
# rules to create the testprograms
#
##########################################################################


#$(call generat_test_bin, Testprogramname)
#
# @param Testprogramname (file-)name of the testprogram to generate
#
define generat_test_bin
$(DIR_TESTCASE)$(1): $(DIR_OBJ_TEST)$(1).o $(TEST_TOOLS)
	$(CC) $$(CFLAG_TEST) $$(LDFLAGS) -o $$@ $(DIR_OBJ_TEST)$(1).o $(OBJ_DATA_GLOVE).o $(TEST_TOOLS) $$(LIBS_TEST)
endef

$(foreach TESTPROGRAM, $(TEST_PROGRAMS), $(eval $(call generat_test_bin,$(TESTPROGRAM)) ) )


$(TEST_TOOLS):
	make --directory=$(DIR_FIB) test



#$(call generat_bins, Programname)
#
# @param Programname (file-)name of the program to generate
#
define generat_bins
$(DIR_BIN)$(1): $(DIR_OBJ)/$(1).o $(OBJ_DATA_GLOVE)
	$(CC) $$(CFLAG) $(LDFLAGS) -o $$@ $(DIR_OBJ)/$(1).o $$(OBJ_DATA_GLOVE) $$(LIBS)
endef

$(foreach PROGRAM, $(EXE_FILES), $(eval $(call generat_bins,$(PROGRAM)) ) )

#$(call generat_bins, Programname)
#
# @param Programname (file-)name of the program to generate
#
define generat_test_bins
$(DIR_BIN)test_$(1): $(DIR_OBJ_TEST)/$(1).o $(OBJ_DATA_GLOVE)
	$(CC) $$(CFLAG_TEST) $(LDFLAGS) -o $$@ $(DIR_OBJ_TEST)/$(1).o $$(OBJ_DATA_GLOVE) $$(LIBS_TEST)
endef

$(foreach TESTPROGRAM, $(EXE_FILES), $(eval $(call generat_test_bins,$(TESTPROGRAM)) ) )


##########################################################################
#
# creat folders
#
##########################################################################

DIRS_TO_DELETE:=$(DIRS_TO_DELETE) $(DIR_OBJ) $(DIR_OBJ_TEST) $(DIR_TESTCASE)


#$(call generat_folders, Subfolder)
#
# @param Subfolder the (namespace) subfolder in which the folder should be
#    created
#
define generat_folders
$(1)/$$(DIR_OBJ):
	mkdir -p $(1)/$$(DIR_OBJ)

$(1)/$$(DIR_OBJ_TEST):
	mkdir -p $(1)/$$(DIR_OBJ_TEST)

$(1)/$$(DIR_TESTCASE):
	mkdir -p $(1)/$$(DIR_TESTCASE)

DIRS_TO_DELETE:=$(DIRS_TO_DELETE) $(1)/$$(DIR_OBJ) $(1)/$$(DIR_OBJ_TEST) $(1)/$$(DIR_TESTCASE)
endef

$(foreach SUBFOLDER, $(NAMESPACES), $(eval $(call generat_folders,$(SUBFOLDER)) ) )



$(DIR_BIN):
	mkdir -p $(DIR_BIN)

$(DIR_LIB):
	mkdir -p $(DIR_LIB)


DIRS_TO_DELETE:=$(DIRS_TO_DELETE) $(DIR_BIN) $(DIR_LIB)

create_dirs:$(DIRS_TO_DELETE)

##########################################################################
#
# creat dependencies
#
##########################################################################

dependencies.dep: $(SRC_FILES) $(INCL_FILES) $(TEST_FILES)
	rm -f dependencies.dep
	$(foreach QUELLE,  $(SRC_FILES) $(TEST_FILES), \
	$(CC) -MM $(CFLAG) $(INCL) -c $(QUELLE) -MT $(subst $(DIR_TEST),$(DIR_OBJ), $(subst $(DIR_SRC),$(DIR_OBJ), $(subst .cpp,.o, $(QUELLE) ) ) ) >> dependencies.dep; \
	echo "	\$$(CC) \$$(CFLAG) \$$(INCL) -c \$$< -o \$$@" >> dependencies.dep; )
	$(foreach QUELLE,  $(SRC_FILES) $(TEST_FILES), \
	$(CC) -MM $(CFLAG_TEST) $(INCL) -c $(QUELLE) -MT $(subst $(DIR_SRC),$(DIR_OBJ_TEST), $(subst $(DIR_TEST),$(DIR_OBJ_TEST), $(subst .cpp,.o, $(QUELLE) ) ) ) >> dependencies.dep; \
	echo "	\$$(CC) \$$(CFLAG_TEST) \$$(INCL) -c \$$< -o \$$@" >> dependencies.dep; )


ifneq "$(MAKECMDGOALS)" "clean"
-include dependencies.dep
endif

##########################################################################
#
# clean rules
#
##########################################################################

clean:
	rm -rf $(DIRS_TO_DELETE)
	rm -f dependencies.dep dependencies.dep_tmp




