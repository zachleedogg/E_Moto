#
#  There exist several targets which are by default empty and which can be 
#  used for execution of your targets. These targets are usually executed 
#  before and after some main targets. They are: 
#
#     .build-pre:              called before 'build' target
#     .build-post:             called after 'build' target
#     .clean-pre:              called before 'clean' target
#     .clean-post:             called after 'clean' target
#     .clobber-pre:            called before 'clobber' target
#     .clobber-post:           called after 'clobber' target
#     .all-pre:                called before 'all' target
#     .all-post:               called after 'all' target
#     .help-pre:               called before 'help' target
#     .help-post:              called after 'help' target
#
#  Targets beginning with '.' are not intended to be called on their own.
#
#  Main targets can be executed directly, and they are:
#  
#     build                    build a specific configuration
#     clean                    remove built files from a configuration
#     clobber                  remove all built files
#     all                      build all configurations
#     help                     print help message
#  
#  Targets .build-impl, .clean-impl, .clobber-impl, .all-impl, and
#  .help-impl are implemented in nbproject/makefile-impl.mk.
#
#  Available make variables:
#
#     CND_BASEDIR                base directory for relative paths
#     CND_DISTDIR                default top distribution directory (build artifacts)
#     CND_BUILDDIR               default top build directory (object files, ...)
#     CONF                       name of current configuration
#     CND_ARTIFACT_DIR_${CONF}   directory of build artifact (current configuration)
#     CND_ARTIFACT_NAME_${CONF}  name of build artifact (current configuration)
#     CND_ARTIFACT_PATH_${CONF}  path to build artifact (current configuration)
#     CND_PACKAGE_DIR_${CONF}    directory of package (current configuration)
#     CND_PACKAGE_NAME_${CONF}   name of package (current configuration)
#     CND_PACKAGE_PATH_${CONF}   path to package (current configuration)
#
# NOCDDL


# Environment 
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
RM=rm
MV=mv ${1} ${2}
UNAME := $(shell uname)


# Suppress make[x] Entering directory messages
MAKEFLAGS += --no-print-directory


# Continue making if there are any errors. Useful for debugging when a failure 
# occurs in one of the other makes
#MAKEFLAGS += --ignore-errors

# Create a list of important directories for cleaning and specify which build configurations should be omitted from building
exampleFolders := "ex_app_led_blink" "ex_boot_uart" "ex_boot_i2c" "ex_boot_can" "ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware" "ex_boot_uart_icsp_prog" "ex_boot_uart_icsp_prog_to_go" "ex_boot_spi_flash_blob" "ex_app_uart_spi_flash" "ex_app_live_update_smps_v1" "ex_app_live_update_smps_v2"
archiveOutputFilename = ${PROJECTNAME}.a
cleanList := ../make-all-stdout.txt ../make-all-stderr.txt ../ezbl_tools/test ../ezbl_tools/build ../ezbl_tools/dist ../ezbl_comm/ezbl_comm/Release build debug dist disassembly nbproject/Package-*.bash
dontBuild := build_all	# List of Build Configurations which we shouldn't actually try to make
buildAllConfs = ${filter-out ${dontBuild},${ALLCONFS}}
archiveDestFolders := nbproject/ezbl_cache/${archiveOutputFilename} # Duplicate output archive that can be hard-linked to on the filesystem without being unlinked when updated
localObjects = ${patsubst ${OBJECTDIR}/%,%,${OBJECTFILES}}  # Object output file list with the preceeding "build/${CONF}/production" paths removed to suppress 8191 character command line limit in Windows

# Macro to copy one or more files to a destination folder, ignore failures and
# follow symbolic links (-L)
# ${1} - space separated source file(s)
# ${2} - dest directory
define COPY_FILES_TO_DIR =
	-cp -L ${1} ${2}

endef

# Macro to copy a file to a destination, ignore failures and follow symbolic
# links (-L)
# ${1} - source file
# ${2} - dest directory
# ${3} - more dest directory + filename
define COPY_FILE =
	-cp -L ${1} ${2}${3}

endef

# Macro to touch a file (set the last written timestamp to now)
define TOUCH_FILE =
	touch ${1}

endef

# Macro to remove a folder or file (recursive)
define REMOVE_FILE =
	-test -d ${1} && rm -r ${1} || test -e ${1} && rm ${1} || true

endef

define RENAME_FILE =
	-mv ${1} ${2}

endef


# Macro to build one of the configurations passed in, but first undo MPLAB
# regeneration of identical nbproject/Makefile-${1}.mk files with a new
# timestamp every time you switch build profiles.
ifeq ($(UNAME), windows32)
define BUILD_A_CONF =
	cd nbproject && (test -d "ezbl_cache" || mkdir "ezbl_cache") && \
	cd "ezbl_cache" && \
	(test -e "Makefile-${1}.mk" && (fc /b "Makefile-${1}.mk" "../Makefile-${1}.mk" > NUL) && touch --reference="Makefile-${1}.mk" "../Makefile-${1}.mk" || true) && \
	(cp "../Makefile-${1}.mk" "Makefile-${1}.mk" && touch --reference="../Makefile-${1}.mk" "Makefile-${1}.mk" && \
	cd ../..)
	${MAKE} --ignore-errors CONF=${1} ${if ${findstring 16ep,${1}},MP_PROCESSOR_OPTION=generic-16bit-ep,${if ${findstring 16ch,${1}},MP_PROCESSOR_OPTION=generic-16dsp-ch,${if ${findstring 16da,${1}},MP_PROCESSOR_OPTION=generic-16bit-da,}}} .build-post

endef
else
define BUILD_A_CONF =
	cd nbproject && (test -d "ezbl_cache" || mkdir "ezbl_cache") && \
	cd "ezbl_cache" && \
	(test -e "Makefile-${1}.mk" && (cmp "Makefile-${1}.mk" "../Makefile-${1}.mk" > nul) && touch --reference="Makefile-${1}.mk" "../Makefile-${1}.mk" || true) && \
	(cp "../Makefile-${1}.mk" "Makefile-${1}.mk" && touch --reference="../Makefile-${1}.mk" "Makefile-${1}.mk" && \
	cd ../..)
	${MAKE} --ignore-errors CONF=${1} ${if ${findstring 16ep,${1}},MP_PROCESSOR_OPTION=generic-16bit-ep,${if ${findstring 16ch,${1}},MP_PROCESSOR_OPTION=generic-16dsp-ch,${if ${findstring 16da,${1}},MP_PROCESSOR_OPTION=generic-16bit-da,}}} .build-post

endef
endif


# Only invoke one command shell to speed up building on Windows
ifeq (,$(findstring nbproject/Makefile-${CONF}.mk,$(MAKEFILE_LIST)))
.ONESHELL:
endif

# Tell make the "build" target doesn't output any files (not actually true) so
# that it will always execute these recipe lines, regardless of if the "build"
# folder is found (which it almost always would be)
.PHONY: build


# build
build:
# Make all of the configurations when MPLAB X IDE's make "build" target is invoked

#	Actually build (make) each MPLAB X defined configuration. We need to
#   rebuild everything for each configuration to target non-compatible ISAs 
#	between dsPIC30F/PIC24F/PIC24H/dsPIC33F devices without EDS,
#	PIC24F/PIC24E devices with EDS, dsPIC33E devices, and dsPIC33C devices.
#	Each build profile contributes separately named .o object files to the
#	archive for automatic selection by the linker when building an
#	Application or Bootloader project.
ifeq (${CONF}, build_all)
	$(foreach config,$(buildAllConfs), ${call BUILD_A_CONF,${config}})
else
ifneq (,${dontBuild})
	${call BUILD_A_CONF,${CONF}}
endif
endif


.build-pre:
# Add your pre 'build' code here...





.build-post: .build-impl
# Add your post 'build' code here...

ifeq (,${filter ${CONF},${dontBuild}})
ifeq ($(UNAME), windows32)
	cd ${CND_ARTIFACT_DIR_${CONF}}
	rm -f ${notdir ${wildcard ${CND_ARTIFACT_DIR_${CONF}}/*.o}}
	rm -f ${notdir ${wildcard ${CND_ARTIFACT_DIR_${CONF}}/*.${CONF}o}} 
	cd ../../../${OBJECTDIR}
	${call COPY_FILES_TO_DIR, -p ${localObjects}, ../../../${CND_ARTIFACT_DIR_${CONF}}}
	cd ../../../${CND_ARTIFACT_DIR_${CONF}}
	ren *.o *.${CONF}o
	cd ../../..
else
	rm -f ${wildcard ${CND_ARTIFACT_DIR_${CONF}}/*.o}
	rm -f ${wildcard ${CND_ARTIFACT_DIR_${CONF}}/*.${CONF}o}
	${foreach objfile,${OBJECTFILES}, ${call COPY_FILE,-p ${objfile},${CND_ARTIFACT_DIR_${CONF}}/${notdir ${objfile:.o=.${CONF}o}}}}
endif
	cd "${CND_ARTIFACT_DIR_${CONF}}" && \
	${MP_AR} ruc ../../../${archiveOutputFilename} ${notdir ${OBJECTFILES:.o=.${CONF}o}} && \
	cd ../../..
	${foreach destFolder,${archiveDestFolders}, ${call COPY_FILE,${archiveOutputFilename},${destFolder}}}

endif


.PHONY: clean-all
clean-all: clean
	$(foreach item,${cleanList}, $(call REMOVE_FILE,${item}))
	$(foreach folder,${exampleFolders}, $(call REMOVE_FILE,../${folder}/build))
	$(foreach folder,${exampleFolders}, $(call REMOVE_FILE,../${folder}/debug))
	$(foreach folder,${exampleFolders}, $(call REMOVE_FILE,../${folder}/dist))
	$(foreach folder,${exampleFolders}, $(call REMOVE_FILE,../${folder}/disassembly))
	$(foreach folder,${exampleFolders}, $(call REMOVE_FILE,../${folder}/nbproject/Package-*.bash))



# Tag all these clean targets as not creating any file output (so make doesn't
# have to waste time checking dependencies (files named clean, .clean-post,
# .clean-pre, .clean-impl, etc. and can just run always)
.PHONY: clean


# clean
clean: .clean-post

.clean-pre:
# Add your pre 'clean' code here...
#
# Help MPLAB actually finish Clean/Clean and Build commands on Windows where 
# open mapped file handles prevent the first attempt at removing the folder tree 
# from succeeding.
	-test -d "dist" && rm -r "dist" || true
	-test -d "build" && rm -r "build" || true
	-test -d "debug" && rm -r "debug" || true
	-test -d "disassembly" && rm -r "disassembly" || true


.clean-post: .clean-impl
# Add your post 'clean' code here...


# clobber
clobber: .clobber-post

.clobber-pre:
# Add your pre 'clobber' code here...

.clobber-post: .clobber-impl
# Add your post 'clobber' code here...


# all
all: .all-post

.all-pre:
# Add your pre 'all' code here...

.all-post: .all-impl
# Add your post 'all' code here...


# help
help: .help-post

.help-pre:
# Add your pre 'help' code here...

.help-post: .help-impl
# Add your post 'help' code here...



# include project implementation makefile
include nbproject/Makefile-impl.mk

# include project make variables
include nbproject/Makefile-variables.mk


# Test if Makefile-${CONF}.mk has already been read in, and if so, don't read it 
# in again (doing so would make duplicate rules and associated recipes whenever 
# you invoke a 'Compile' or 'Assemble' on a single source file).
ifeq (,$(findstring nbproject/Makefile-${CONF}.mk,$(MAKEFILE_LIST)))

# Obtain paths for MPLAB executables (ex: ${MP_JAVA_PATH} variable)
include nbproject/Makefile-local-${CONF}.mk

# include the meat of this configuration's makefile that has all the useful
# MPLAB project settings assigned to variables (ex: ${MP_PROCESSOR_OPTION},
# ${DISTDIR}, etc.). Directly including this file will not work since it in turn 
# includes Makefile (this file), which would set up a circular reference and 
# redefine all recipes. Setting IGNORE_LOCAL = TRUE fixes this, but since it 
# also has the repercussion of losing access to Makefile-local-${CONF}.mk 
# definitions, we can correct this by just exporting all variables so they 
# automatically propagate for use when executing the recipes.
IGNORE_LOCAL = TRUE
export
include nbproject/Makefile-${CONF}.mk
IGNORE_LOCAL = FALSE
unexport IGNORE_LOCAL
endif	# ifeq duplicate rule and recipe suppression