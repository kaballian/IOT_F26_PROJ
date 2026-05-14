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
FINAL_IMAGE=${DISTDIR}/IOT_DEV_SENSOR.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/IOT_DEV_SENSOR.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/main.c src/ARG419BR.c src/ENS_160.c src/FAN.c src/I2C.c src/PWM.c src/TMR0.c src/TMR1.c src/clock.c src/eusart1.c src/event_queue.c src/interrupt.c src/parse.c src/pins.c src/system.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/main.p1 ${OBJECTDIR}/src/ARG419BR.p1 ${OBJECTDIR}/src/ENS_160.p1 ${OBJECTDIR}/src/FAN.p1 ${OBJECTDIR}/src/I2C.p1 ${OBJECTDIR}/src/PWM.p1 ${OBJECTDIR}/src/TMR0.p1 ${OBJECTDIR}/src/TMR1.p1 ${OBJECTDIR}/src/clock.p1 ${OBJECTDIR}/src/eusart1.p1 ${OBJECTDIR}/src/event_queue.p1 ${OBJECTDIR}/src/interrupt.p1 ${OBJECTDIR}/src/parse.p1 ${OBJECTDIR}/src/pins.p1 ${OBJECTDIR}/src/system.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/src/main.p1.d ${OBJECTDIR}/src/ARG419BR.p1.d ${OBJECTDIR}/src/ENS_160.p1.d ${OBJECTDIR}/src/FAN.p1.d ${OBJECTDIR}/src/I2C.p1.d ${OBJECTDIR}/src/PWM.p1.d ${OBJECTDIR}/src/TMR0.p1.d ${OBJECTDIR}/src/TMR1.p1.d ${OBJECTDIR}/src/clock.p1.d ${OBJECTDIR}/src/eusart1.p1.d ${OBJECTDIR}/src/event_queue.p1.d ${OBJECTDIR}/src/interrupt.p1.d ${OBJECTDIR}/src/parse.p1.d ${OBJECTDIR}/src/pins.p1.d ${OBJECTDIR}/src/system.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/main.p1 ${OBJECTDIR}/src/ARG419BR.p1 ${OBJECTDIR}/src/ENS_160.p1 ${OBJECTDIR}/src/FAN.p1 ${OBJECTDIR}/src/I2C.p1 ${OBJECTDIR}/src/PWM.p1 ${OBJECTDIR}/src/TMR0.p1 ${OBJECTDIR}/src/TMR1.p1 ${OBJECTDIR}/src/clock.p1 ${OBJECTDIR}/src/eusart1.p1 ${OBJECTDIR}/src/event_queue.p1 ${OBJECTDIR}/src/interrupt.p1 ${OBJECTDIR}/src/parse.p1 ${OBJECTDIR}/src/pins.p1 ${OBJECTDIR}/src/system.p1

# Source Files
SOURCEFILES=src/main.c src/ARG419BR.c src/ENS_160.c src/FAN.c src/I2C.c src/PWM.c src/TMR0.c src/TMR1.c src/clock.c src/eusart1.c src/event_queue.c src/interrupt.c src/parse.c src/pins.c src/system.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/IOT_DEV_SENSOR.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F18124
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/main.p1: src/main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.p1.d 
	@${RM} ${OBJECTDIR}/src/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/main.p1 src/main.c 
	@-${MV} ${OBJECTDIR}/src/main.d ${OBJECTDIR}/src/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ARG419BR.p1: src/ARG419BR.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ARG419BR.p1.d 
	@${RM} ${OBJECTDIR}/src/ARG419BR.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/ARG419BR.p1 src/ARG419BR.c 
	@-${MV} ${OBJECTDIR}/src/ARG419BR.d ${OBJECTDIR}/src/ARG419BR.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ARG419BR.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ENS_160.p1: src/ENS_160.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ENS_160.p1.d 
	@${RM} ${OBJECTDIR}/src/ENS_160.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/ENS_160.p1 src/ENS_160.c 
	@-${MV} ${OBJECTDIR}/src/ENS_160.d ${OBJECTDIR}/src/ENS_160.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ENS_160.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/FAN.p1: src/FAN.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/FAN.p1.d 
	@${RM} ${OBJECTDIR}/src/FAN.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/FAN.p1 src/FAN.c 
	@-${MV} ${OBJECTDIR}/src/FAN.d ${OBJECTDIR}/src/FAN.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/FAN.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/I2C.p1: src/I2C.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/I2C.p1.d 
	@${RM} ${OBJECTDIR}/src/I2C.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/I2C.p1 src/I2C.c 
	@-${MV} ${OBJECTDIR}/src/I2C.d ${OBJECTDIR}/src/I2C.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/I2C.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/PWM.p1: src/PWM.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/PWM.p1.d 
	@${RM} ${OBJECTDIR}/src/PWM.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/PWM.p1 src/PWM.c 
	@-${MV} ${OBJECTDIR}/src/PWM.d ${OBJECTDIR}/src/PWM.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/PWM.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/TMR0.p1: src/TMR0.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/TMR0.p1.d 
	@${RM} ${OBJECTDIR}/src/TMR0.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/TMR0.p1 src/TMR0.c 
	@-${MV} ${OBJECTDIR}/src/TMR0.d ${OBJECTDIR}/src/TMR0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/TMR0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/TMR1.p1: src/TMR1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/TMR1.p1.d 
	@${RM} ${OBJECTDIR}/src/TMR1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/TMR1.p1 src/TMR1.c 
	@-${MV} ${OBJECTDIR}/src/TMR1.d ${OBJECTDIR}/src/TMR1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/TMR1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/clock.p1: src/clock.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/clock.p1.d 
	@${RM} ${OBJECTDIR}/src/clock.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/clock.p1 src/clock.c 
	@-${MV} ${OBJECTDIR}/src/clock.d ${OBJECTDIR}/src/clock.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/clock.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/eusart1.p1: src/eusart1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eusart1.p1.d 
	@${RM} ${OBJECTDIR}/src/eusart1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/eusart1.p1 src/eusart1.c 
	@-${MV} ${OBJECTDIR}/src/eusart1.d ${OBJECTDIR}/src/eusart1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/eusart1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/event_queue.p1: src/event_queue.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/event_queue.p1.d 
	@${RM} ${OBJECTDIR}/src/event_queue.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/event_queue.p1 src/event_queue.c 
	@-${MV} ${OBJECTDIR}/src/event_queue.d ${OBJECTDIR}/src/event_queue.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/event_queue.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/interrupt.p1: src/interrupt.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/interrupt.p1.d 
	@${RM} ${OBJECTDIR}/src/interrupt.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/interrupt.p1 src/interrupt.c 
	@-${MV} ${OBJECTDIR}/src/interrupt.d ${OBJECTDIR}/src/interrupt.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/interrupt.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/parse.p1: src/parse.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/parse.p1.d 
	@${RM} ${OBJECTDIR}/src/parse.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/parse.p1 src/parse.c 
	@-${MV} ${OBJECTDIR}/src/parse.d ${OBJECTDIR}/src/parse.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/parse.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/pins.p1: src/pins.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/pins.p1.d 
	@${RM} ${OBJECTDIR}/src/pins.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/pins.p1 src/pins.c 
	@-${MV} ${OBJECTDIR}/src/pins.d ${OBJECTDIR}/src/pins.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/pins.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/system.p1: src/system.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.p1.d 
	@${RM} ${OBJECTDIR}/src/system.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit4   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/system.p1 src/system.c 
	@-${MV} ${OBJECTDIR}/src/system.d ${OBJECTDIR}/src/system.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/system.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/src/main.p1: src/main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.p1.d 
	@${RM} ${OBJECTDIR}/src/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/main.p1 src/main.c 
	@-${MV} ${OBJECTDIR}/src/main.d ${OBJECTDIR}/src/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ARG419BR.p1: src/ARG419BR.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ARG419BR.p1.d 
	@${RM} ${OBJECTDIR}/src/ARG419BR.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/ARG419BR.p1 src/ARG419BR.c 
	@-${MV} ${OBJECTDIR}/src/ARG419BR.d ${OBJECTDIR}/src/ARG419BR.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ARG419BR.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ENS_160.p1: src/ENS_160.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ENS_160.p1.d 
	@${RM} ${OBJECTDIR}/src/ENS_160.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/ENS_160.p1 src/ENS_160.c 
	@-${MV} ${OBJECTDIR}/src/ENS_160.d ${OBJECTDIR}/src/ENS_160.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ENS_160.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/FAN.p1: src/FAN.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/FAN.p1.d 
	@${RM} ${OBJECTDIR}/src/FAN.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/FAN.p1 src/FAN.c 
	@-${MV} ${OBJECTDIR}/src/FAN.d ${OBJECTDIR}/src/FAN.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/FAN.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/I2C.p1: src/I2C.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/I2C.p1.d 
	@${RM} ${OBJECTDIR}/src/I2C.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/I2C.p1 src/I2C.c 
	@-${MV} ${OBJECTDIR}/src/I2C.d ${OBJECTDIR}/src/I2C.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/I2C.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/PWM.p1: src/PWM.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/PWM.p1.d 
	@${RM} ${OBJECTDIR}/src/PWM.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/PWM.p1 src/PWM.c 
	@-${MV} ${OBJECTDIR}/src/PWM.d ${OBJECTDIR}/src/PWM.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/PWM.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/TMR0.p1: src/TMR0.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/TMR0.p1.d 
	@${RM} ${OBJECTDIR}/src/TMR0.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/TMR0.p1 src/TMR0.c 
	@-${MV} ${OBJECTDIR}/src/TMR0.d ${OBJECTDIR}/src/TMR0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/TMR0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/TMR1.p1: src/TMR1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/TMR1.p1.d 
	@${RM} ${OBJECTDIR}/src/TMR1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/TMR1.p1 src/TMR1.c 
	@-${MV} ${OBJECTDIR}/src/TMR1.d ${OBJECTDIR}/src/TMR1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/TMR1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/clock.p1: src/clock.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/clock.p1.d 
	@${RM} ${OBJECTDIR}/src/clock.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/clock.p1 src/clock.c 
	@-${MV} ${OBJECTDIR}/src/clock.d ${OBJECTDIR}/src/clock.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/clock.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/eusart1.p1: src/eusart1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eusart1.p1.d 
	@${RM} ${OBJECTDIR}/src/eusart1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/eusart1.p1 src/eusart1.c 
	@-${MV} ${OBJECTDIR}/src/eusart1.d ${OBJECTDIR}/src/eusart1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/eusart1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/event_queue.p1: src/event_queue.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/event_queue.p1.d 
	@${RM} ${OBJECTDIR}/src/event_queue.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/event_queue.p1 src/event_queue.c 
	@-${MV} ${OBJECTDIR}/src/event_queue.d ${OBJECTDIR}/src/event_queue.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/event_queue.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/interrupt.p1: src/interrupt.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/interrupt.p1.d 
	@${RM} ${OBJECTDIR}/src/interrupt.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/interrupt.p1 src/interrupt.c 
	@-${MV} ${OBJECTDIR}/src/interrupt.d ${OBJECTDIR}/src/interrupt.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/interrupt.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/parse.p1: src/parse.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/parse.p1.d 
	@${RM} ${OBJECTDIR}/src/parse.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/parse.p1 src/parse.c 
	@-${MV} ${OBJECTDIR}/src/parse.d ${OBJECTDIR}/src/parse.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/parse.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/pins.p1: src/pins.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/pins.p1.d 
	@${RM} ${OBJECTDIR}/src/pins.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/pins.p1 src/pins.c 
	@-${MV} ${OBJECTDIR}/src/pins.d ${OBJECTDIR}/src/pins.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/pins.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/system.p1: src/system.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.p1.d 
	@${RM} ${OBJECTDIR}/src/system.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/system.p1 src/system.c 
	@-${MV} ${OBJECTDIR}/src/system.d ${OBJECTDIR}/src/system.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/system.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/IOT_DEV_SENSOR.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/IOT_DEV_SENSOR.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=pickit4  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/IOT_DEV_SENSOR.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/IOT_DEV_SENSOR.X.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/IOT_DEV_SENSOR.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/IOT_DEV_SENSOR.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I"." -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/IOT_DEV_SENSOR.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
