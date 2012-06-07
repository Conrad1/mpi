#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=mpic++
CXX=mpic++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/cdp/GPC_RouteC.o \
	${OBJECTDIR}/cdp/PhonFeature.o \
	${OBJECTDIR}/common.o \
	${OBJECTDIR}/cdp/PhonemeOutBufC.o \
	${OBJECTDIR}/cdp/Semantic.o \
	${OBJECTDIR}/cdp/LetterInput.o \
	${OBJECTDIR}/cdp/CDPAggregator.o \
	${OBJECTDIR}/cdp/FeatSetC.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/MPIRunner.o \
	${OBJECTDIR}/cdp/DRC_Overall.o \
	${OBJECTDIR}/cdp/LetterInputC.o \
	${OBJECTDIR}/cdp/PhonemeFeatures.o \
	${OBJECTDIR}/cdp/WordDBFuncs.o \
	${OBJECTDIR}/cdp/DRC_OS.o \
	${OBJECTDIR}/cdp/Feature.o \
	${OBJECTDIR}/CommChannel.o \
	${OBJECTDIR}/cdp/PhonOutputBufC.o \
	${OBJECTDIR}/cdp/ZorziNet.o \
	${OBJECTDIR}/cdp/FeatureSetC.o \
	${OBJECTDIR}/cdp/WordDataC.o \
	${OBJECTDIR}/cdp/GlobalC.o \
	${OBJECTDIR}/cdp/The_DRC_Parts.o \
	${OBJECTDIR}/cdp/SemanticFeatures.o \
	${OBJECTDIR}/cdp/GraphemeParsing.o \
	${OBJECTDIR}/Task.o \
	${OBJECTDIR}/cdp/PGCRuleList.o \
	${OBJECTDIR}/cdp/CDPTask.o \
	${OBJECTDIR}/MapSerializer.o \
	${OBJECTDIR}/ResultSender.o \
	${OBJECTDIR}/cdp/Rule.o \
	${OBJECTDIR}/cdp/PGC_Route.o \
	${OBJECTDIR}/cdp/OrthInputLexC.o \
	${OBJECTDIR}/Aggregator.o \
	${OBJECTDIR}/cdp/RuleList.o \
	${OBJECTDIR}/cdp/PhonLexC.o \
	${OBJECTDIR}/cdp/ContextRule.o \
	${OBJECTDIR}/cdp/stdafx.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=-D ENGLISH

# CC Compiler Flags
CCFLAGS=-D ENGLISH
CXXFLAGS=-D ENGLISH

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpi_wrapper

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpi_wrapper: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpi_wrapper ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/cdp/GPC_RouteC.o: cdp/GPC_RouteC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/GPC_RouteC.o cdp/GPC_RouteC.cpp

${OBJECTDIR}/cdp/PhonFeature.o: cdp/PhonFeature.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PhonFeature.o cdp/PhonFeature.cpp

${OBJECTDIR}/common.o: common.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/common.o common.cpp

${OBJECTDIR}/cdp/PhonemeOutBufC.o: cdp/PhonemeOutBufC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PhonemeOutBufC.o cdp/PhonemeOutBufC.cpp

${OBJECTDIR}/cdp/Semantic.o: cdp/Semantic.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/Semantic.o cdp/Semantic.cpp

${OBJECTDIR}/cdp/LetterInput.o: cdp/LetterInput.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/LetterInput.o cdp/LetterInput.cpp

${OBJECTDIR}/cdp/CDPAggregator.o: cdp/CDPAggregator.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/CDPAggregator.o cdp/CDPAggregator.cpp

${OBJECTDIR}/cdp/FeatSetC.o: cdp/FeatSetC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/FeatSetC.o cdp/FeatSetC.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/MPIRunner.o: MPIRunner.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/MPIRunner.o MPIRunner.cpp

${OBJECTDIR}/cdp/DRC_Overall.o: cdp/DRC_Overall.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/DRC_Overall.o cdp/DRC_Overall.cpp

${OBJECTDIR}/cdp/LetterInputC.o: cdp/LetterInputC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/LetterInputC.o cdp/LetterInputC.cpp

${OBJECTDIR}/cdp/PhonemeFeatures.o: cdp/PhonemeFeatures.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PhonemeFeatures.o cdp/PhonemeFeatures.cpp

${OBJECTDIR}/cdp/WordDBFuncs.o: cdp/WordDBFuncs.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/WordDBFuncs.o cdp/WordDBFuncs.cpp

${OBJECTDIR}/cdp/DRC_OS.o: cdp/DRC_OS.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/DRC_OS.o cdp/DRC_OS.cpp

${OBJECTDIR}/cdp/Feature.o: cdp/Feature.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/Feature.o cdp/Feature.cpp

${OBJECTDIR}/CommChannel.o: CommChannel.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/CommChannel.o CommChannel.cpp

${OBJECTDIR}/cdp/PhonOutputBufC.o: cdp/PhonOutputBufC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PhonOutputBufC.o cdp/PhonOutputBufC.cpp

${OBJECTDIR}/cdp/ZorziNet.o: cdp/ZorziNet.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/ZorziNet.o cdp/ZorziNet.cpp

${OBJECTDIR}/cdp/FeatureSetC.o: cdp/FeatureSetC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/FeatureSetC.o cdp/FeatureSetC.cpp

${OBJECTDIR}/cdp/WordDataC.o: cdp/WordDataC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/WordDataC.o cdp/WordDataC.cpp

${OBJECTDIR}/cdp/GlobalC.o: cdp/GlobalC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/GlobalC.o cdp/GlobalC.cpp

${OBJECTDIR}/cdp/The_DRC_Parts.o: cdp/The_DRC_Parts.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/The_DRC_Parts.o cdp/The_DRC_Parts.cpp

${OBJECTDIR}/cdp/SemanticFeatures.o: cdp/SemanticFeatures.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/SemanticFeatures.o cdp/SemanticFeatures.cpp

${OBJECTDIR}/cdp/GraphemeParsing.o: cdp/GraphemeParsing.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/GraphemeParsing.o cdp/GraphemeParsing.cpp

${OBJECTDIR}/Task.o: Task.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/Task.o Task.cpp

${OBJECTDIR}/cdp/PGCRuleList.o: cdp/PGCRuleList.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PGCRuleList.o cdp/PGCRuleList.cpp

${OBJECTDIR}/cdp/CDPTask.o: cdp/CDPTask.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/CDPTask.o cdp/CDPTask.cpp

${OBJECTDIR}/MapSerializer.o: MapSerializer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/MapSerializer.o MapSerializer.cpp

${OBJECTDIR}/ResultSender.o: ResultSender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ResultSender.o ResultSender.cpp

${OBJECTDIR}/cdp/Rule.o: cdp/Rule.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/Rule.o cdp/Rule.cpp

${OBJECTDIR}/cdp/PGC_Route.o: cdp/PGC_Route.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PGC_Route.o cdp/PGC_Route.cpp

${OBJECTDIR}/cdp/OrthInputLexC.o: cdp/OrthInputLexC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/OrthInputLexC.o cdp/OrthInputLexC.cpp

${OBJECTDIR}/Aggregator.o: Aggregator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/Aggregator.o Aggregator.cpp

${OBJECTDIR}/cdp/RuleList.o: cdp/RuleList.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/RuleList.o cdp/RuleList.cpp

${OBJECTDIR}/cdp/PhonLexC.o: cdp/PhonLexC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PhonLexC.o cdp/PhonLexC.cpp

${OBJECTDIR}/cdp/ContextRule.o: cdp/ContextRule.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/ContextRule.o cdp/ContextRule.cpp

${OBJECTDIR}/cdp/stdafx.o: cdp/stdafx.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/stdafx.o cdp/stdafx.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/newsimpletest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/newsimpletest.o: tests/newsimpletest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -g -I. -MMD -MP -MF $@.d -o ${TESTDIR}/tests/newsimpletest.o tests/newsimpletest.cpp


${OBJECTDIR}/cdp/GPC_RouteC_nomain.o: ${OBJECTDIR}/cdp/GPC_RouteC.o cdp/GPC_RouteC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/GPC_RouteC.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/GPC_RouteC_nomain.o cdp/GPC_RouteC.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/GPC_RouteC.o ${OBJECTDIR}/cdp/GPC_RouteC_nomain.o;\
	fi

${OBJECTDIR}/cdp/PhonFeature_nomain.o: ${OBJECTDIR}/cdp/PhonFeature.o cdp/PhonFeature.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/PhonFeature.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PhonFeature_nomain.o cdp/PhonFeature.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/PhonFeature.o ${OBJECTDIR}/cdp/PhonFeature_nomain.o;\
	fi

${OBJECTDIR}/common_nomain.o: ${OBJECTDIR}/common.o common.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/common.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/common_nomain.o common.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/common.o ${OBJECTDIR}/common_nomain.o;\
	fi

${OBJECTDIR}/cdp/PhonemeOutBufC_nomain.o: ${OBJECTDIR}/cdp/PhonemeOutBufC.o cdp/PhonemeOutBufC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/PhonemeOutBufC.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PhonemeOutBufC_nomain.o cdp/PhonemeOutBufC.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/PhonemeOutBufC.o ${OBJECTDIR}/cdp/PhonemeOutBufC_nomain.o;\
	fi

${OBJECTDIR}/cdp/Semantic_nomain.o: ${OBJECTDIR}/cdp/Semantic.o cdp/Semantic.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/Semantic.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/Semantic_nomain.o cdp/Semantic.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/Semantic.o ${OBJECTDIR}/cdp/Semantic_nomain.o;\
	fi

${OBJECTDIR}/cdp/LetterInput_nomain.o: ${OBJECTDIR}/cdp/LetterInput.o cdp/LetterInput.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/LetterInput.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/LetterInput_nomain.o cdp/LetterInput.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/LetterInput.o ${OBJECTDIR}/cdp/LetterInput_nomain.o;\
	fi

${OBJECTDIR}/cdp/CDPAggregator_nomain.o: ${OBJECTDIR}/cdp/CDPAggregator.o cdp/CDPAggregator.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/CDPAggregator.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/CDPAggregator_nomain.o cdp/CDPAggregator.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/CDPAggregator.o ${OBJECTDIR}/cdp/CDPAggregator_nomain.o;\
	fi

${OBJECTDIR}/cdp/FeatSetC_nomain.o: ${OBJECTDIR}/cdp/FeatSetC.o cdp/FeatSetC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/FeatSetC.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/FeatSetC_nomain.o cdp/FeatSetC.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/FeatSetC.o ${OBJECTDIR}/cdp/FeatSetC_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

${OBJECTDIR}/MPIRunner_nomain.o: ${OBJECTDIR}/MPIRunner.o MPIRunner.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MPIRunner.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/MPIRunner_nomain.o MPIRunner.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MPIRunner.o ${OBJECTDIR}/MPIRunner_nomain.o;\
	fi

${OBJECTDIR}/cdp/DRC_Overall_nomain.o: ${OBJECTDIR}/cdp/DRC_Overall.o cdp/DRC_Overall.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/DRC_Overall.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/DRC_Overall_nomain.o cdp/DRC_Overall.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/DRC_Overall.o ${OBJECTDIR}/cdp/DRC_Overall_nomain.o;\
	fi

${OBJECTDIR}/cdp/LetterInputC_nomain.o: ${OBJECTDIR}/cdp/LetterInputC.o cdp/LetterInputC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/LetterInputC.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/LetterInputC_nomain.o cdp/LetterInputC.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/LetterInputC.o ${OBJECTDIR}/cdp/LetterInputC_nomain.o;\
	fi

${OBJECTDIR}/cdp/PhonemeFeatures_nomain.o: ${OBJECTDIR}/cdp/PhonemeFeatures.o cdp/PhonemeFeatures.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/PhonemeFeatures.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PhonemeFeatures_nomain.o cdp/PhonemeFeatures.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/PhonemeFeatures.o ${OBJECTDIR}/cdp/PhonemeFeatures_nomain.o;\
	fi

${OBJECTDIR}/cdp/WordDBFuncs_nomain.o: ${OBJECTDIR}/cdp/WordDBFuncs.o cdp/WordDBFuncs.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/WordDBFuncs.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/WordDBFuncs_nomain.o cdp/WordDBFuncs.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/WordDBFuncs.o ${OBJECTDIR}/cdp/WordDBFuncs_nomain.o;\
	fi

${OBJECTDIR}/cdp/DRC_OS_nomain.o: ${OBJECTDIR}/cdp/DRC_OS.o cdp/DRC_OS.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/DRC_OS.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/DRC_OS_nomain.o cdp/DRC_OS.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/DRC_OS.o ${OBJECTDIR}/cdp/DRC_OS_nomain.o;\
	fi

${OBJECTDIR}/cdp/Feature_nomain.o: ${OBJECTDIR}/cdp/Feature.o cdp/Feature.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/Feature.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/Feature_nomain.o cdp/Feature.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/Feature.o ${OBJECTDIR}/cdp/Feature_nomain.o;\
	fi

${OBJECTDIR}/CommChannel_nomain.o: ${OBJECTDIR}/CommChannel.o CommChannel.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/CommChannel.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/CommChannel_nomain.o CommChannel.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/CommChannel.o ${OBJECTDIR}/CommChannel_nomain.o;\
	fi

${OBJECTDIR}/cdp/PhonOutputBufC_nomain.o: ${OBJECTDIR}/cdp/PhonOutputBufC.o cdp/PhonOutputBufC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/PhonOutputBufC.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PhonOutputBufC_nomain.o cdp/PhonOutputBufC.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/PhonOutputBufC.o ${OBJECTDIR}/cdp/PhonOutputBufC_nomain.o;\
	fi

${OBJECTDIR}/cdp/ZorziNet_nomain.o: ${OBJECTDIR}/cdp/ZorziNet.o cdp/ZorziNet.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/ZorziNet.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/ZorziNet_nomain.o cdp/ZorziNet.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/ZorziNet.o ${OBJECTDIR}/cdp/ZorziNet_nomain.o;\
	fi

${OBJECTDIR}/cdp/FeatureSetC_nomain.o: ${OBJECTDIR}/cdp/FeatureSetC.o cdp/FeatureSetC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/FeatureSetC.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/FeatureSetC_nomain.o cdp/FeatureSetC.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/FeatureSetC.o ${OBJECTDIR}/cdp/FeatureSetC_nomain.o;\
	fi

${OBJECTDIR}/cdp/WordDataC_nomain.o: ${OBJECTDIR}/cdp/WordDataC.o cdp/WordDataC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/WordDataC.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/WordDataC_nomain.o cdp/WordDataC.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/WordDataC.o ${OBJECTDIR}/cdp/WordDataC_nomain.o;\
	fi

${OBJECTDIR}/cdp/GlobalC_nomain.o: ${OBJECTDIR}/cdp/GlobalC.o cdp/GlobalC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/GlobalC.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/GlobalC_nomain.o cdp/GlobalC.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/GlobalC.o ${OBJECTDIR}/cdp/GlobalC_nomain.o;\
	fi

${OBJECTDIR}/cdp/The_DRC_Parts_nomain.o: ${OBJECTDIR}/cdp/The_DRC_Parts.o cdp/The_DRC_Parts.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/The_DRC_Parts.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/The_DRC_Parts_nomain.o cdp/The_DRC_Parts.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/The_DRC_Parts.o ${OBJECTDIR}/cdp/The_DRC_Parts_nomain.o;\
	fi

${OBJECTDIR}/cdp/SemanticFeatures_nomain.o: ${OBJECTDIR}/cdp/SemanticFeatures.o cdp/SemanticFeatures.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/SemanticFeatures.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/SemanticFeatures_nomain.o cdp/SemanticFeatures.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/SemanticFeatures.o ${OBJECTDIR}/cdp/SemanticFeatures_nomain.o;\
	fi

${OBJECTDIR}/cdp/GraphemeParsing_nomain.o: ${OBJECTDIR}/cdp/GraphemeParsing.o cdp/GraphemeParsing.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/GraphemeParsing.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/GraphemeParsing_nomain.o cdp/GraphemeParsing.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/GraphemeParsing.o ${OBJECTDIR}/cdp/GraphemeParsing_nomain.o;\
	fi

${OBJECTDIR}/Task_nomain.o: ${OBJECTDIR}/Task.o Task.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Task.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/Task_nomain.o Task.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Task.o ${OBJECTDIR}/Task_nomain.o;\
	fi

${OBJECTDIR}/cdp/PGCRuleList_nomain.o: ${OBJECTDIR}/cdp/PGCRuleList.o cdp/PGCRuleList.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/PGCRuleList.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PGCRuleList_nomain.o cdp/PGCRuleList.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/PGCRuleList.o ${OBJECTDIR}/cdp/PGCRuleList_nomain.o;\
	fi

${OBJECTDIR}/cdp/CDPTask_nomain.o: ${OBJECTDIR}/cdp/CDPTask.o cdp/CDPTask.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/CDPTask.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/CDPTask_nomain.o cdp/CDPTask.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/CDPTask.o ${OBJECTDIR}/cdp/CDPTask_nomain.o;\
	fi

${OBJECTDIR}/MapSerializer_nomain.o: ${OBJECTDIR}/MapSerializer.o MapSerializer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MapSerializer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/MapSerializer_nomain.o MapSerializer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MapSerializer.o ${OBJECTDIR}/MapSerializer_nomain.o;\
	fi

${OBJECTDIR}/ResultSender_nomain.o: ${OBJECTDIR}/ResultSender.o ResultSender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ResultSender.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/ResultSender_nomain.o ResultSender.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/ResultSender.o ${OBJECTDIR}/ResultSender_nomain.o;\
	fi

${OBJECTDIR}/cdp/Rule_nomain.o: ${OBJECTDIR}/cdp/Rule.o cdp/Rule.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/Rule.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/Rule_nomain.o cdp/Rule.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/Rule.o ${OBJECTDIR}/cdp/Rule_nomain.o;\
	fi

${OBJECTDIR}/cdp/PGC_Route_nomain.o: ${OBJECTDIR}/cdp/PGC_Route.o cdp/PGC_Route.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/PGC_Route.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PGC_Route_nomain.o cdp/PGC_Route.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/PGC_Route.o ${OBJECTDIR}/cdp/PGC_Route_nomain.o;\
	fi

${OBJECTDIR}/cdp/OrthInputLexC_nomain.o: ${OBJECTDIR}/cdp/OrthInputLexC.o cdp/OrthInputLexC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/OrthInputLexC.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/OrthInputLexC_nomain.o cdp/OrthInputLexC.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/OrthInputLexC.o ${OBJECTDIR}/cdp/OrthInputLexC_nomain.o;\
	fi

${OBJECTDIR}/Aggregator_nomain.o: ${OBJECTDIR}/Aggregator.o Aggregator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Aggregator.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/Aggregator_nomain.o Aggregator.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Aggregator.o ${OBJECTDIR}/Aggregator_nomain.o;\
	fi

${OBJECTDIR}/cdp/RuleList_nomain.o: ${OBJECTDIR}/cdp/RuleList.o cdp/RuleList.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/RuleList.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/RuleList_nomain.o cdp/RuleList.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/RuleList.o ${OBJECTDIR}/cdp/RuleList_nomain.o;\
	fi

${OBJECTDIR}/cdp/PhonLexC_nomain.o: ${OBJECTDIR}/cdp/PhonLexC.o cdp/PhonLexC.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/PhonLexC.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/PhonLexC_nomain.o cdp/PhonLexC.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/PhonLexC.o ${OBJECTDIR}/cdp/PhonLexC_nomain.o;\
	fi

${OBJECTDIR}/cdp/ContextRule_nomain.o: ${OBJECTDIR}/cdp/ContextRule.o cdp/ContextRule.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/ContextRule.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/ContextRule_nomain.o cdp/ContextRule.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/ContextRule.o ${OBJECTDIR}/cdp/ContextRule_nomain.o;\
	fi

${OBJECTDIR}/cdp/stdafx_nomain.o: ${OBJECTDIR}/cdp/stdafx.o cdp/stdafx.cpp 
	${MKDIR} -p ${OBJECTDIR}/cdp
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cdp/stdafx.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cdp/stdafx_nomain.o cdp/stdafx.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cdp/stdafx.o ${OBJECTDIR}/cdp/stdafx_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpi_wrapper

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
