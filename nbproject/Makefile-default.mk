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
FINAL_IMAGE=${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=main.c CLK.c ILI9341.c SERCOM.c DMA.c GPIO.c XPT2046.c keyboard.c TC.c A7672E.c RTC.c windows.c Cosmos.c refraction.c TCC.c Towers.c RS485.c crc8.c eeprom.c Sensors.c LED.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/CLK.o ${OBJECTDIR}/ILI9341.o ${OBJECTDIR}/SERCOM.o ${OBJECTDIR}/DMA.o ${OBJECTDIR}/GPIO.o ${OBJECTDIR}/XPT2046.o ${OBJECTDIR}/keyboard.o ${OBJECTDIR}/TC.o ${OBJECTDIR}/A7672E.o ${OBJECTDIR}/RTC.o ${OBJECTDIR}/windows.o ${OBJECTDIR}/Cosmos.o ${OBJECTDIR}/refraction.o ${OBJECTDIR}/TCC.o ${OBJECTDIR}/Towers.o ${OBJECTDIR}/RS485.o ${OBJECTDIR}/crc8.o ${OBJECTDIR}/eeprom.o ${OBJECTDIR}/Sensors.o ${OBJECTDIR}/LED.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/CLK.o.d ${OBJECTDIR}/ILI9341.o.d ${OBJECTDIR}/SERCOM.o.d ${OBJECTDIR}/DMA.o.d ${OBJECTDIR}/GPIO.o.d ${OBJECTDIR}/XPT2046.o.d ${OBJECTDIR}/keyboard.o.d ${OBJECTDIR}/TC.o.d ${OBJECTDIR}/A7672E.o.d ${OBJECTDIR}/RTC.o.d ${OBJECTDIR}/windows.o.d ${OBJECTDIR}/Cosmos.o.d ${OBJECTDIR}/refraction.o.d ${OBJECTDIR}/TCC.o.d ${OBJECTDIR}/Towers.o.d ${OBJECTDIR}/RS485.o.d ${OBJECTDIR}/crc8.o.d ${OBJECTDIR}/eeprom.o.d ${OBJECTDIR}/Sensors.o.d ${OBJECTDIR}/LED.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/CLK.o ${OBJECTDIR}/ILI9341.o ${OBJECTDIR}/SERCOM.o ${OBJECTDIR}/DMA.o ${OBJECTDIR}/GPIO.o ${OBJECTDIR}/XPT2046.o ${OBJECTDIR}/keyboard.o ${OBJECTDIR}/TC.o ${OBJECTDIR}/A7672E.o ${OBJECTDIR}/RTC.o ${OBJECTDIR}/windows.o ${OBJECTDIR}/Cosmos.o ${OBJECTDIR}/refraction.o ${OBJECTDIR}/TCC.o ${OBJECTDIR}/Towers.o ${OBJECTDIR}/RS485.o ${OBJECTDIR}/crc8.o ${OBJECTDIR}/eeprom.o ${OBJECTDIR}/Sensors.o ${OBJECTDIR}/LED.o

# Source Files
SOURCEFILES=main.c CLK.c ILI9341.c SERCOM.c DMA.c GPIO.c XPT2046.c keyboard.c TC.c A7672E.c RTC.c windows.c Cosmos.c refraction.c TCC.c Towers.c RS485.c crc8.c eeprom.c Sensors.c LED.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${CMSIS_DIR}/CMSIS/Core/Include"



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATSAMD51G19A
MP_LINKER_FILE_OPTION=
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
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/a840ddf6dde8906d943891bdad54919746f4165 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/CLK.o: CLK.c  .generated_files/flags/default/149532aa6e61af4f8eb8cfdb74071a7c0a5a3661 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CLK.o.d 
	@${RM} ${OBJECTDIR}/CLK.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/CLK.o.d" -o ${OBJECTDIR}/CLK.o CLK.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/ILI9341.o: ILI9341.c  .generated_files/flags/default/a19c7c683a246d855d807cb792f1be6e50a2700e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ILI9341.o.d 
	@${RM} ${OBJECTDIR}/ILI9341.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ILI9341.o.d" -o ${OBJECTDIR}/ILI9341.o ILI9341.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/SERCOM.o: SERCOM.c  .generated_files/flags/default/4895bb1e4d1d7a3c44e5475c8a1bd312386d1d94 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SERCOM.o.d 
	@${RM} ${OBJECTDIR}/SERCOM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/SERCOM.o.d" -o ${OBJECTDIR}/SERCOM.o SERCOM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/DMA.o: DMA.c  .generated_files/flags/default/b978a13546cb2c498f50ff5453b2967ba78b5d9c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DMA.o.d 
	@${RM} ${OBJECTDIR}/DMA.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/DMA.o.d" -o ${OBJECTDIR}/DMA.o DMA.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/GPIO.o: GPIO.c  .generated_files/flags/default/1ed0d60871bede1f5c975e5122524ba203be1636 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/GPIO.o.d 
	@${RM} ${OBJECTDIR}/GPIO.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/GPIO.o.d" -o ${OBJECTDIR}/GPIO.o GPIO.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/XPT2046.o: XPT2046.c  .generated_files/flags/default/f57c31d63f8d7b0bfebeaf8f4b7911ff586599c8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/XPT2046.o.d 
	@${RM} ${OBJECTDIR}/XPT2046.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/XPT2046.o.d" -o ${OBJECTDIR}/XPT2046.o XPT2046.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/keyboard.o: keyboard.c  .generated_files/flags/default/c20ae651481e1e523179b2bfbe39b249b2e52873 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keyboard.o.d 
	@${RM} ${OBJECTDIR}/keyboard.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/keyboard.o.d" -o ${OBJECTDIR}/keyboard.o keyboard.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/TC.o: TC.c  .generated_files/flags/default/3d44348c5afd90828eed11f98d517f0de6949e53 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TC.o.d 
	@${RM} ${OBJECTDIR}/TC.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/TC.o.d" -o ${OBJECTDIR}/TC.o TC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/A7672E.o: A7672E.c  .generated_files/flags/default/40275f95c4c5a7759e1474c1adfa3282637efbde .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/A7672E.o.d 
	@${RM} ${OBJECTDIR}/A7672E.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/A7672E.o.d" -o ${OBJECTDIR}/A7672E.o A7672E.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/RTC.o: RTC.c  .generated_files/flags/default/5878f3b3abbf00b8304c0e98d5c2090abeb7b35b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RTC.o.d 
	@${RM} ${OBJECTDIR}/RTC.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/RTC.o.d" -o ${OBJECTDIR}/RTC.o RTC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/windows.o: windows.c  .generated_files/flags/default/890aa535001eb182ee89c4fd4c60cdab2ece6b2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/windows.o.d 
	@${RM} ${OBJECTDIR}/windows.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/windows.o.d" -o ${OBJECTDIR}/windows.o windows.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/Cosmos.o: Cosmos.c  .generated_files/flags/default/bfc469430706c13f9eccea4a9aee504137d3df49 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Cosmos.o.d 
	@${RM} ${OBJECTDIR}/Cosmos.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/Cosmos.o.d" -o ${OBJECTDIR}/Cosmos.o Cosmos.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/refraction.o: refraction.c  .generated_files/flags/default/8d3ec9de88ac76f117b92853c3dd2f3a55561d93 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/refraction.o.d 
	@${RM} ${OBJECTDIR}/refraction.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/refraction.o.d" -o ${OBJECTDIR}/refraction.o refraction.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/TCC.o: TCC.c  .generated_files/flags/default/5e7243d1df0482a781b384b1fb35a55002b11da .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TCC.o.d 
	@${RM} ${OBJECTDIR}/TCC.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/TCC.o.d" -o ${OBJECTDIR}/TCC.o TCC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/Towers.o: Towers.c  .generated_files/flags/default/db9a8774c12b6dfad77c92bacf6936c1d7aa14a5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Towers.o.d 
	@${RM} ${OBJECTDIR}/Towers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/Towers.o.d" -o ${OBJECTDIR}/Towers.o Towers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/RS485.o: RS485.c  .generated_files/flags/default/101df03c7f952e4a12487961a58957e91608175e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RS485.o.d 
	@${RM} ${OBJECTDIR}/RS485.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/RS485.o.d" -o ${OBJECTDIR}/RS485.o RS485.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/crc8.o: crc8.c  .generated_files/flags/default/3e75b4622bf985badab8352bdf7238c7a3c2969a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/crc8.o.d 
	@${RM} ${OBJECTDIR}/crc8.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/crc8.o.d" -o ${OBJECTDIR}/crc8.o crc8.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/eeprom.o: eeprom.c  .generated_files/flags/default/e57c96294bd185e3a2ac65efb74dae458626a88b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom.o.d 
	@${RM} ${OBJECTDIR}/eeprom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/eeprom.o.d" -o ${OBJECTDIR}/eeprom.o eeprom.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/Sensors.o: Sensors.c  .generated_files/flags/default/25d50882ae3316442834badc83d9e2f89c403c70 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sensors.o.d 
	@${RM} ${OBJECTDIR}/Sensors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/Sensors.o.d" -o ${OBJECTDIR}/Sensors.o Sensors.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/LED.o: LED.c  .generated_files/flags/default/661d7aa71c3276229c3a76c16bb68d8f772ff77 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LED.o.d 
	@${RM} ${OBJECTDIR}/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/LED.o.d" -o ${OBJECTDIR}/LED.o LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
else
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/86dbf1e0383808d4551ff44c4545a320864defb0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/CLK.o: CLK.c  .generated_files/flags/default/31dd0e9254fd619fa4e0bc4f58bf057b30b466b8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CLK.o.d 
	@${RM} ${OBJECTDIR}/CLK.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/CLK.o.d" -o ${OBJECTDIR}/CLK.o CLK.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/ILI9341.o: ILI9341.c  .generated_files/flags/default/b7c4917c651525c9f4e4c1e9d9bacebe9cd025f6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ILI9341.o.d 
	@${RM} ${OBJECTDIR}/ILI9341.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ILI9341.o.d" -o ${OBJECTDIR}/ILI9341.o ILI9341.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/SERCOM.o: SERCOM.c  .generated_files/flags/default/e23a83f979e7b32d5b7b88bff5a83d9132c8c1c8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SERCOM.o.d 
	@${RM} ${OBJECTDIR}/SERCOM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/SERCOM.o.d" -o ${OBJECTDIR}/SERCOM.o SERCOM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/DMA.o: DMA.c  .generated_files/flags/default/6487a231a23eae4db4b0849c2509a6f059eb54f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DMA.o.d 
	@${RM} ${OBJECTDIR}/DMA.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/DMA.o.d" -o ${OBJECTDIR}/DMA.o DMA.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/GPIO.o: GPIO.c  .generated_files/flags/default/be913e2d2c12fb9a48a0163796846c4c27450fb3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/GPIO.o.d 
	@${RM} ${OBJECTDIR}/GPIO.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/GPIO.o.d" -o ${OBJECTDIR}/GPIO.o GPIO.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/XPT2046.o: XPT2046.c  .generated_files/flags/default/e2fe82b7d324495442bd0af7673d620fd9541114 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/XPT2046.o.d 
	@${RM} ${OBJECTDIR}/XPT2046.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/XPT2046.o.d" -o ${OBJECTDIR}/XPT2046.o XPT2046.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/keyboard.o: keyboard.c  .generated_files/flags/default/6137eae4c89113ef715cfadfc5032ca7044e4649 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keyboard.o.d 
	@${RM} ${OBJECTDIR}/keyboard.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/keyboard.o.d" -o ${OBJECTDIR}/keyboard.o keyboard.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/TC.o: TC.c  .generated_files/flags/default/db873021786555b10affcb086555a4bb1f902940 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TC.o.d 
	@${RM} ${OBJECTDIR}/TC.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/TC.o.d" -o ${OBJECTDIR}/TC.o TC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/A7672E.o: A7672E.c  .generated_files/flags/default/ae426de777d840388f4284f0ed1a80ffffc15670 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/A7672E.o.d 
	@${RM} ${OBJECTDIR}/A7672E.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/A7672E.o.d" -o ${OBJECTDIR}/A7672E.o A7672E.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/RTC.o: RTC.c  .generated_files/flags/default/49d8d03cabf6ded9cc67ba1ab5e850370bc103f6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RTC.o.d 
	@${RM} ${OBJECTDIR}/RTC.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/RTC.o.d" -o ${OBJECTDIR}/RTC.o RTC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/windows.o: windows.c  .generated_files/flags/default/5ddded4c439bb5920ff2431d113a621c9e53ae6e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/windows.o.d 
	@${RM} ${OBJECTDIR}/windows.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/windows.o.d" -o ${OBJECTDIR}/windows.o windows.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/Cosmos.o: Cosmos.c  .generated_files/flags/default/bd7648149aca7dae267f86237cbe8149ce1312 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Cosmos.o.d 
	@${RM} ${OBJECTDIR}/Cosmos.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/Cosmos.o.d" -o ${OBJECTDIR}/Cosmos.o Cosmos.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/refraction.o: refraction.c  .generated_files/flags/default/81a4e38c378ecf633a0d8922f806f5265e51c926 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/refraction.o.d 
	@${RM} ${OBJECTDIR}/refraction.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/refraction.o.d" -o ${OBJECTDIR}/refraction.o refraction.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/TCC.o: TCC.c  .generated_files/flags/default/f8a30ff108cfe44aade3ae2c66844d89ea92977b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TCC.o.d 
	@${RM} ${OBJECTDIR}/TCC.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/TCC.o.d" -o ${OBJECTDIR}/TCC.o TCC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/Towers.o: Towers.c  .generated_files/flags/default/3390c02e1eaa0188433500aa82e80933da72b06c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Towers.o.d 
	@${RM} ${OBJECTDIR}/Towers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/Towers.o.d" -o ${OBJECTDIR}/Towers.o Towers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/RS485.o: RS485.c  .generated_files/flags/default/b344f27ae12313a732f5d5702e782295a1a8225f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RS485.o.d 
	@${RM} ${OBJECTDIR}/RS485.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/RS485.o.d" -o ${OBJECTDIR}/RS485.o RS485.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/crc8.o: crc8.c  .generated_files/flags/default/556fd2ecfdf138f024d93d2d22ce2211be04c9c7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/crc8.o.d 
	@${RM} ${OBJECTDIR}/crc8.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/crc8.o.d" -o ${OBJECTDIR}/crc8.o crc8.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/eeprom.o: eeprom.c  .generated_files/flags/default/398d993d085243280e873bd224b1508f06b970f3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom.o.d 
	@${RM} ${OBJECTDIR}/eeprom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/eeprom.o.d" -o ${OBJECTDIR}/eeprom.o eeprom.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/Sensors.o: Sensors.c  .generated_files/flags/default/ed2d25af75007f006c90c8c436113f0ebdeb8e83 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sensors.o.d 
	@${RM} ${OBJECTDIR}/Sensors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/Sensors.o.d" -o ${OBJECTDIR}/Sensors.o Sensors.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/LED.o: LED.c  .generated_files/flags/default/ad05bf494ce3c8b0340b03f6d4fc6e43749061ad .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LED.o.d 
	@${RM} ${OBJECTDIR}/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/LED.o.d" -o ${OBJECTDIR}/LED.o LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g  -D__MPLAB_DEBUGGER_PK5=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK5=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}/samd51a"
	
else
${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}/samd51a"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
