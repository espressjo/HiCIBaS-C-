##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=rm8
ConfigurationName      :=Debug
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=/home/espressjo/Documents/git/HiCIBaS-C-/rm8-server/.codelite
ProjectPath            :=/home/espressjo/Documents/git/HiCIBaS-C-/rm8-server/.codelite/.codelite
IntermediateDirectory  :=../build-$(ConfigurationName)/_codelite
OutDir                 :=../build-$(ConfigurationName)/_codelite
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=espressjo
Date                   :=14/06/23
CodeLitePath           :=/home/espressjo/.codelite
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=../build-$(ConfigurationName)/bin/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)/opt/rm8/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)uics 
ArLibs                 :=  "uics" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/opt/rm8/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=../build-$(ConfigurationName)/_codelite/up_up_src_main.cpp$(ObjectSuffix) ../build-$(ConfigurationName)/_codelite/up_up_src_rm8_config.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ../build-$(ConfigurationName)/_codelite/.d $(Objects) 
	@mkdir -p "../build-$(ConfigurationName)/_codelite"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@mkdir -p "../build-$(ConfigurationName)/_codelite"
	@mkdir -p ""../build-$(ConfigurationName)/bin""

../build-$(ConfigurationName)/_codelite/.d:
	@mkdir -p "../build-$(ConfigurationName)/_codelite"

PreBuild:


##
## Objects
##
../build-$(ConfigurationName)/_codelite/up_up_src_main.cpp$(ObjectSuffix): ../../src/main.cpp ../build-$(ConfigurationName)/_codelite/up_up_src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/rm8-server/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_src_main.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/_codelite/up_up_src_main.cpp$(DependSuffix): ../../src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/_codelite/up_up_src_main.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/_codelite/up_up_src_main.cpp$(DependSuffix) -MM ../../src/main.cpp

../build-$(ConfigurationName)/_codelite/up_up_src_main.cpp$(PreprocessSuffix): ../../src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/_codelite/up_up_src_main.cpp$(PreprocessSuffix) ../../src/main.cpp

../build-$(ConfigurationName)/_codelite/up_up_src_rm8_config.cpp$(ObjectSuffix): ../../src/rm8_config.cpp ../build-$(ConfigurationName)/_codelite/up_up_src_rm8_config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/rm8-server/src/rm8_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_src_rm8_config.cpp$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/_codelite/up_up_src_rm8_config.cpp$(DependSuffix): ../../src/rm8_config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/_codelite/up_up_src_rm8_config.cpp$(ObjectSuffix) -MF../build-$(ConfigurationName)/_codelite/up_up_src_rm8_config.cpp$(DependSuffix) -MM ../../src/rm8_config.cpp

../build-$(ConfigurationName)/_codelite/up_up_src_rm8_config.cpp$(PreprocessSuffix): ../../src/rm8_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/_codelite/up_up_src_rm8_config.cpp$(PreprocessSuffix) ../../src/rm8_config.cpp


-include ../build-$(ConfigurationName)/_codelite//*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


