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
FINAL_IMAGE=${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=main.c CLK.c ILI9341.c SERCOM.c DMA.c GPIO.c XPT2046.c keyboard.c TC.c A7672E.c RTC.c BME680.c windows.c Cosmos.c refraction.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/CLK.o ${OBJECTDIR}/ILI9341.o ${OBJECTDIR}/SERCOM.o ${OBJECTDIR}/DMA.o ${OBJECTDIR}/GPIO.o ${OBJECTDIR}/XPT2046.o ${OBJECTDIR}/keyboard.o ${OBJECTDIR}/TC.o ${OBJECTDIR}/A7672E.o ${OBJECTDIR}/RTC.o ${OBJECTDIR}/BME680.o ${OBJECTDIR}/windows.o ${OBJECTDIR}/Cosmos.o ${OBJECTDIR}/refraction.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/CLK.o.d ${OBJECTDIR}/ILI9341.o.d ${OBJECTDIR}/SERCOM.o.d ${OBJECTDIR}/DMA.o.d ${OBJECTDIR}/GPIO.o.d ${OBJECTDIR}/XPT2046.o.d ${OBJECTDIR}/keyboard.o.d ${OBJECTDIR}/TC.o.d ${OBJECTDIR}/A7672E.o.d ${OBJECTDIR}/RTC.o.d ${OBJECTDIR}/BME680.o.d ${OBJECTDIR}/windows.o.d ${OBJECTDIR}/Cosmos.o.d ${OBJECTDIR}/refraction.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/CLK.o ${OBJECTDIR}/ILI9341.o ${OBJECTDIR}/SERCOM.o ${OBJECTDIR}/DMA.o ${OBJECTDIR}/GPIO.o ${OBJECTDIR}/XPT2046.o ${OBJECTDIR}/keyboard.o ${OBJECTDIR}/TC.o ${OBJECTDIR}/A7672E.o ${OBJECTDIR}/RTC.o ${OBJECTDIR}/BME680.o ${OBJECTDIR}/windows.o ${OBJECTDIR}/Cosmos.o ${OBJECTDIR}/refraction.o

# Source Files
SOURCEFILES=main.c CLK.c ILI9341.c SERCOM.c DMA.c GPIO.c XPT2046.c keyboard.c TC.c A7672E.c RTC.c BME680.c windows.c Cosmos.c refraction.c

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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/c84e4c964e7a099d5e15ac2e128292c12453f2a7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/CLK.o: CLK.c  .generated_files/flags/default/46c2a4fc4fbd1b0cb31b237c0ea8451b418e1d38 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CLK.o.d 
	@${RM} ${OBJECTDIR}/CLK.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/CLK.o.d" -o ${OBJECTDIR}/CLK.o CLK.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/ILI9341.o: ILI9341.c  .generated_files/flags/default/818ad98dc2a3cdcc06f6d4f2f8d21a820f4d077 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ILI9341.o.d 
	@${RM} ${OBJECTDIR}/ILI9341.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ILI9341.o.d" -o ${OBJECTDIR}/ILI9341.o ILI9341.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/SERCOM.o: SERCOM.c  .generated_files/flags/default/7897ab1dee11f84e2cc684553fa1a15c800519f8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SERCOM.o.d 
	@${RM} ${OBJECTDIR}/SERCOM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/SERCOM.o.d" -o ${OBJECTDIR}/SERCOM.o SERCOM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/DMA.o: DMA.c  .generated_files/flags/default/a588ee4e54630cbc87891bd3b5eda76c3bb4bb3b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DMA.o.d 
	@${RM} ${OBJECTDIR}/DMA.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/DMA.o.d" -o ${OBJECTDIR}/DMA.o DMA.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/GPIO.o: GPIO.c  .generated_files/flags/default/73c10ad3709732f4cde28f0661caab48d7fa1402 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/GPIO.o.d 
	@${RM} ${OBJECTDIR}/GPIO.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/GPIO.o.d" -o ${OBJECTDIR}/GPIO.o GPIO.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/XPT2046.o: XPT2046.c  .generated_files/flags/default/9350485b14070114382de4d2e5f2aa542bb203ff .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/XPT2046.o.d 
	@${RM} ${OBJECTDIR}/XPT2046.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/XPT2046.o.d" -o ${OBJECTDIR}/XPT2046.o XPT2046.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/keyboard.o: keyboard.c  .generated_files/flags/default/d2bc59abbc5a20b5ac9aa0842f06d0ad9f7ebf6c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keyboard.o.d 
	@${RM} ${OBJECTDIR}/keyboard.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/keyboard.o.d" -o ${OBJECTDIR}/keyboard.o keyboard.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/TC.o: TC.c  .generated_files/flags/default/5d172103b69327889b7c36aa0d9c3d7125950360 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TC.o.d 
	@${RM} ${OBJECTDIR}/TC.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/TC.o.d" -o ${OBJECTDIR}/TC.o TC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/A7672E.o: A7672E.c  .generated_files/flags/default/b8e20535f282c32bf526b3424b8a01f1d4f61cfe .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/A7672E.o.d 
	@${RM} ${OBJECTDIR}/A7672E.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/A7672E.o.d" -o ${OBJECTDIR}/A7672E.o A7672E.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/RTC.o: RTC.c  .generated_files/flags/default/717ecbf577056a1a5ea7d325532d5e900b7abf43 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RTC.o.d 
	@${RM} ${OBJECTDIR}/RTC.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC.o.d" -o ${OBJECTDIR}/RTC.o RTC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/BME680.o: BME680.c  .generated_files/flags/default/2e034a8e870aed33059479d99923a67cf1d7e577 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BME680.o.d 
	@${RM} ${OBJECTDIR}/BME680.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/BME680.o.d" -o ${OBJECTDIR}/BME680.o BME680.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/windows.o: windows.c  .generated_files/flags/default/efef8b2c040d8d865abaa1cc9e3870dcadd57b9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/windows.o.d 
	@${RM} ${OBJECTDIR}/windows.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/windows.o.d" -o ${OBJECTDIR}/windows.o windows.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/Cosmos.o: Cosmos.c  .generated_files/flags/default/ca5a00d6feac9f27a9268ba15ba805524c114d5d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Cosmos.o.d 
	@${RM} ${OBJECTDIR}/Cosmos.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Cosmos.o.d" -o ${OBJECTDIR}/Cosmos.o Cosmos.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/refraction.o: refraction.c  .generated_files/flags/default/c333f04b8943e7b6f7c81e87a567d028e1250b1b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/refraction.o.d 
	@${RM} ${OBJECTDIR}/refraction.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK5=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/refraction.o.d" -o ${OBJECTDIR}/refraction.o refraction.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
else
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/433d8efcd188980b918f9cf79c248d8fc9f9df2b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/CLK.o: CLK.c  .generated_files/flags/default/c27b8d7bf184d86d9dc34bdf45fec879f5e77d8b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CLK.o.d 
	@${RM} ${OBJECTDIR}/CLK.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/CLK.o.d" -o ${OBJECTDIR}/CLK.o CLK.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/ILI9341.o: ILI9341.c  .generated_files/flags/default/1cb1731774913645031eb354c2be96e7ed93f8ae .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ILI9341.o.d 
	@${RM} ${OBJECTDIR}/ILI9341.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ILI9341.o.d" -o ${OBJECTDIR}/ILI9341.o ILI9341.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/SERCOM.o: SERCOM.c  .generated_files/flags/default/ab37bd522910ece200b866482735fb3f670b5f1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SERCOM.o.d 
	@${RM} ${OBJECTDIR}/SERCOM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/SERCOM.o.d" -o ${OBJECTDIR}/SERCOM.o SERCOM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/DMA.o: DMA.c  .generated_files/flags/default/7eb0cc2f2bde97ddc25aedb259f37776a243c03d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DMA.o.d 
	@${RM} ${OBJECTDIR}/DMA.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/DMA.o.d" -o ${OBJECTDIR}/DMA.o DMA.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/GPIO.o: GPIO.c  .generated_files/flags/default/50a0ef43f09fe3a63358c1dab091b46676567445 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/GPIO.o.d 
	@${RM} ${OBJECTDIR}/GPIO.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/GPIO.o.d" -o ${OBJECTDIR}/GPIO.o GPIO.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/XPT2046.o: XPT2046.c  .generated_files/flags/default/6cb152724469886d2769aecdceb8a4d4ccb69be2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/XPT2046.o.d 
	@${RM} ${OBJECTDIR}/XPT2046.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/XPT2046.o.d" -o ${OBJECTDIR}/XPT2046.o XPT2046.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/keyboard.o: keyboard.c  .generated_files/flags/default/3d3eaae89e9668849f6fadda43948353dbbbeaf5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keyboard.o.d 
	@${RM} ${OBJECTDIR}/keyboard.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/keyboard.o.d" -o ${OBJECTDIR}/keyboard.o keyboard.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/TC.o: TC.c  .generated_files/flags/default/e6d8fdab8e657155991a6c8daa09f3130f39f204 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TC.o.d 
	@${RM} ${OBJECTDIR}/TC.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/TC.o.d" -o ${OBJECTDIR}/TC.o TC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/A7672E.o: A7672E.c  .generated_files/flags/default/b8ca580f53be41bdc15ddd8655172237fa26761 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/A7672E.o.d 
	@${RM} ${OBJECTDIR}/A7672E.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/A7672E.o.d" -o ${OBJECTDIR}/A7672E.o A7672E.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/RTC.o: RTC.c  .generated_files/flags/default/856fa3695f2a218f361f6dbcde499657e7ec3c3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RTC.o.d 
	@${RM} ${OBJECTDIR}/RTC.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC.o.d" -o ${OBJECTDIR}/RTC.o RTC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/BME680.o: BME680.c  .generated_files/flags/default/2cb41ef5d0c1ce1db828548c6b4312df27c564df .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BME680.o.d 
	@${RM} ${OBJECTDIR}/BME680.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/BME680.o.d" -o ${OBJECTDIR}/BME680.o BME680.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/windows.o: windows.c  .generated_files/flags/default/4b01149edad14094b5119aa5074df566b4e79b4e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/windows.o.d 
	@${RM} ${OBJECTDIR}/windows.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/windows.o.d" -o ${OBJECTDIR}/windows.o windows.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/Cosmos.o: Cosmos.c  .generated_files/flags/default/252d8caac23b885655d9288c2073f4adb5fc616a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Cosmos.o.d 
	@${RM} ${OBJECTDIR}/Cosmos.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Cosmos.o.d" -o ${OBJECTDIR}/Cosmos.o Cosmos.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/refraction.o: refraction.c  .generated_files/flags/default/e19ea770a6fbb508c03656dc7abedaac0b62f4ba .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/refraction.o.d 
	@${RM} ${OBJECTDIR}/refraction.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/refraction.o.d" -o ${OBJECTDIR}/refraction.o refraction.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g  -D__MPLAB_DEBUGGER_PK5=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK5=1,--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}/samd51a"
	
else
${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}/samd51a"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/ATSAMD51G19A-WEATHER-STATION-V2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
