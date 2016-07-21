#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/TemplateProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/TemplateProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c ../../Libraries/PIC33_plib/src/bolt_ADC.c ../../Libraries/PIC33_plib/src/bolt_CAN.c ../../Libraries/PIC33_plib/src/bolt_i2c.c ../../Libraries/PIC33_plib/src/bolt_init.c ../../Libraries/PIC33_plib/src/bolt_OC.c ../../Libraries/PIC33_plib/src/bolt_pins.c ../../Libraries/PIC33_plib/src/bolt_sleep.c ../../Libraries/PIC33_plib/src/bolt_spi.c ../../Libraries/PIC33_plib/src/bolt_uart.c ../framework/framework.c EventCheckers.c FSM_TEMPLATE.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/_ext/356824117/bolt_ADC.o ${OBJECTDIR}/_ext/356824117/bolt_CAN.o ${OBJECTDIR}/_ext/356824117/bolt_i2c.o ${OBJECTDIR}/_ext/356824117/bolt_init.o ${OBJECTDIR}/_ext/356824117/bolt_OC.o ${OBJECTDIR}/_ext/356824117/bolt_pins.o ${OBJECTDIR}/_ext/356824117/bolt_sleep.o ${OBJECTDIR}/_ext/356824117/bolt_spi.o ${OBJECTDIR}/_ext/356824117/bolt_uart.o ${OBJECTDIR}/_ext/487427183/framework.o ${OBJECTDIR}/EventCheckers.o ${OBJECTDIR}/FSM_TEMPLATE.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/_ext/356824117/bolt_ADC.o.d ${OBJECTDIR}/_ext/356824117/bolt_CAN.o.d ${OBJECTDIR}/_ext/356824117/bolt_i2c.o.d ${OBJECTDIR}/_ext/356824117/bolt_init.o.d ${OBJECTDIR}/_ext/356824117/bolt_OC.o.d ${OBJECTDIR}/_ext/356824117/bolt_pins.o.d ${OBJECTDIR}/_ext/356824117/bolt_sleep.o.d ${OBJECTDIR}/_ext/356824117/bolt_spi.o.d ${OBJECTDIR}/_ext/356824117/bolt_uart.o.d ${OBJECTDIR}/_ext/487427183/framework.o.d ${OBJECTDIR}/EventCheckers.o.d ${OBJECTDIR}/FSM_TEMPLATE.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/_ext/356824117/bolt_ADC.o ${OBJECTDIR}/_ext/356824117/bolt_CAN.o ${OBJECTDIR}/_ext/356824117/bolt_i2c.o ${OBJECTDIR}/_ext/356824117/bolt_init.o ${OBJECTDIR}/_ext/356824117/bolt_OC.o ${OBJECTDIR}/_ext/356824117/bolt_pins.o ${OBJECTDIR}/_ext/356824117/bolt_sleep.o ${OBJECTDIR}/_ext/356824117/bolt_spi.o ${OBJECTDIR}/_ext/356824117/bolt_uart.o ${OBJECTDIR}/_ext/487427183/framework.o ${OBJECTDIR}/EventCheckers.o ${OBJECTDIR}/FSM_TEMPLATE.o

# Source Files
SOURCEFILES=main.c ../../Libraries/PIC33_plib/src/bolt_ADC.c ../../Libraries/PIC33_plib/src/bolt_CAN.c ../../Libraries/PIC33_plib/src/bolt_i2c.c ../../Libraries/PIC33_plib/src/bolt_init.c ../../Libraries/PIC33_plib/src/bolt_OC.c ../../Libraries/PIC33_plib/src/bolt_pins.c ../../Libraries/PIC33_plib/src/bolt_sleep.c ../../Libraries/PIC33_plib/src/bolt_spi.c ../../Libraries/PIC33_plib/src/bolt_uart.c ../framework/framework.c EventCheckers.c FSM_TEMPLATE.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/TemplateProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP256MC504
MP_LINKER_FILE_OPTION=,--script=p33EP256MC504.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_ADC.o: ../../Libraries/PIC33_plib/src/bolt_ADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_ADC.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_ADC.c  -o ${OBJECTDIR}/_ext/356824117/bolt_ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_ADC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_ADC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_CAN.o: ../../Libraries/PIC33_plib/src/bolt_CAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_CAN.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_CAN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_CAN.c  -o ${OBJECTDIR}/_ext/356824117/bolt_CAN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_CAN.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_CAN.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_i2c.o: ../../Libraries/PIC33_plib/src/bolt_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_i2c.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_i2c.c  -o ${OBJECTDIR}/_ext/356824117/bolt_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_i2c.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_init.o: ../../Libraries/PIC33_plib/src/bolt_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_init.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_init.c  -o ${OBJECTDIR}/_ext/356824117/bolt_init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_init.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_init.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_OC.o: ../../Libraries/PIC33_plib/src/bolt_OC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_OC.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_OC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_OC.c  -o ${OBJECTDIR}/_ext/356824117/bolt_OC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_OC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_OC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_pins.o: ../../Libraries/PIC33_plib/src/bolt_pins.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_pins.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_pins.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_pins.c  -o ${OBJECTDIR}/_ext/356824117/bolt_pins.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_pins.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_pins.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_sleep.o: ../../Libraries/PIC33_plib/src/bolt_sleep.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_sleep.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_sleep.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_sleep.c  -o ${OBJECTDIR}/_ext/356824117/bolt_sleep.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_sleep.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_sleep.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_spi.o: ../../Libraries/PIC33_plib/src/bolt_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_spi.c  -o ${OBJECTDIR}/_ext/356824117/bolt_spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_spi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_uart.o: ../../Libraries/PIC33_plib/src/bolt_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_uart.c  -o ${OBJECTDIR}/_ext/356824117/bolt_uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_uart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/487427183/framework.o: ../framework/framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/487427183" 
	@${RM} ${OBJECTDIR}/_ext/487427183/framework.o.d 
	@${RM} ${OBJECTDIR}/_ext/487427183/framework.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../framework/framework.c  -o ${OBJECTDIR}/_ext/487427183/framework.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/487427183/framework.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/487427183/framework.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/EventCheckers.o: EventCheckers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/EventCheckers.o.d 
	@${RM} ${OBJECTDIR}/EventCheckers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  EventCheckers.c  -o ${OBJECTDIR}/EventCheckers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/EventCheckers.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/EventCheckers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/FSM_TEMPLATE.o: FSM_TEMPLATE.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FSM_TEMPLATE.o.d 
	@${RM} ${OBJECTDIR}/FSM_TEMPLATE.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FSM_TEMPLATE.c  -o ${OBJECTDIR}/FSM_TEMPLATE.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/FSM_TEMPLATE.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/FSM_TEMPLATE.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_ADC.o: ../../Libraries/PIC33_plib/src/bolt_ADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_ADC.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_ADC.c  -o ${OBJECTDIR}/_ext/356824117/bolt_ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_ADC.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_ADC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_CAN.o: ../../Libraries/PIC33_plib/src/bolt_CAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_CAN.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_CAN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_CAN.c  -o ${OBJECTDIR}/_ext/356824117/bolt_CAN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_CAN.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_CAN.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_i2c.o: ../../Libraries/PIC33_plib/src/bolt_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_i2c.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_i2c.c  -o ${OBJECTDIR}/_ext/356824117/bolt_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_i2c.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_init.o: ../../Libraries/PIC33_plib/src/bolt_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_init.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_init.c  -o ${OBJECTDIR}/_ext/356824117/bolt_init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_init.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_init.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_OC.o: ../../Libraries/PIC33_plib/src/bolt_OC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_OC.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_OC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_OC.c  -o ${OBJECTDIR}/_ext/356824117/bolt_OC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_OC.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_OC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_pins.o: ../../Libraries/PIC33_plib/src/bolt_pins.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_pins.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_pins.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_pins.c  -o ${OBJECTDIR}/_ext/356824117/bolt_pins.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_pins.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_pins.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_sleep.o: ../../Libraries/PIC33_plib/src/bolt_sleep.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_sleep.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_sleep.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_sleep.c  -o ${OBJECTDIR}/_ext/356824117/bolt_sleep.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_sleep.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_sleep.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_spi.o: ../../Libraries/PIC33_plib/src/bolt_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_spi.c  -o ${OBJECTDIR}/_ext/356824117/bolt_spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_spi.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/356824117/bolt_uart.o: ../../Libraries/PIC33_plib/src/bolt_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/356824117" 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/356824117/bolt_uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Libraries/PIC33_plib/src/bolt_uart.c  -o ${OBJECTDIR}/_ext/356824117/bolt_uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/356824117/bolt_uart.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/356824117/bolt_uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/487427183/framework.o: ../framework/framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/487427183" 
	@${RM} ${OBJECTDIR}/_ext/487427183/framework.o.d 
	@${RM} ${OBJECTDIR}/_ext/487427183/framework.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../framework/framework.c  -o ${OBJECTDIR}/_ext/487427183/framework.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/487427183/framework.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/487427183/framework.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/EventCheckers.o: EventCheckers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/EventCheckers.o.d 
	@${RM} ${OBJECTDIR}/EventCheckers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  EventCheckers.c  -o ${OBJECTDIR}/EventCheckers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/EventCheckers.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/EventCheckers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/FSM_TEMPLATE.o: FSM_TEMPLATE.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FSM_TEMPLATE.o.d 
	@${RM} ${OBJECTDIR}/FSM_TEMPLATE.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FSM_TEMPLATE.c  -o ${OBJECTDIR}/FSM_TEMPLATE.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/FSM_TEMPLATE.o.d"      -mno-eds-warn  -g -omf=elf -legacy-libc  -O0 -I"." -I"../../Libraries/PIC33_plib/inc" -I"../../Libraries/PIC33_plib/src" -I"../framework" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/FSM_TEMPLATE.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/TemplateProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/TemplateProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -legacy-libc   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/TemplateProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/TemplateProject.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -legacy-libc  -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/TemplateProject.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
