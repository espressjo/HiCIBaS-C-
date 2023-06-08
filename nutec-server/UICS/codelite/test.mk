##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=test
ConfigurationName      :=Debug
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=/home/espressjo/Documents/git/HiCIBaS-C-/UICS/codelite
ProjectPath            :=/home/espressjo/Documents/git/HiCIBaS-C-/UICS/codelite
IntermediateDirectory  :=./build-$(ConfigurationName)/
OutDir                 :=./build-$(ConfigurationName)/
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=espressjo
Date                   :=17/01/23
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
OutputFile             :=./build-$(ConfigurationName)/bin/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -std=c++11 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=./build-$(ConfigurationName)//main2.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//b64.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ./build-$(ConfigurationName)//.d $(Objects) 
	@mkdir -p "./build-$(ConfigurationName)/"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@mkdir -p "./build-$(ConfigurationName)/"
	@mkdir -p ""./build-$(ConfigurationName)/bin""

./build-$(ConfigurationName)//.d:
	@mkdir -p "./build-$(ConfigurationName)/"

PreBuild:


##
## Objects
##
./build-$(ConfigurationName)//main2.cpp$(ObjectSuffix): main2.cpp ./build-$(ConfigurationName)//main2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/codelite/main2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main2.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//main2.cpp$(DependSuffix): main2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//main2.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//main2.cpp$(DependSuffix) -MM main2.cpp

./build-$(ConfigurationName)//main2.cpp$(PreprocessSuffix): main2.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//main2.cpp$(PreprocessSuffix) main2.cpp

./build-$(ConfigurationName)//b64.cpp$(ObjectSuffix): b64.cpp ./build-$(ConfigurationName)//b64.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/codelite/b64.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/b64.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//b64.cpp$(DependSuffix): b64.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//b64.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//b64.cpp$(DependSuffix) -MM b64.cpp

./build-$(ConfigurationName)//b64.cpp$(PreprocessSuffix): b64.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//b64.cpp$(PreprocessSuffix) b64.cpp


-include ./build-$(ConfigurationName)///*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


