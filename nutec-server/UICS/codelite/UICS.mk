##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=UICS
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
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src 
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
CXXFLAGS :=  -g -O0 -std=c++11 -Wall -DCONFPATH='"/home/espressjo/Documents/git/HiCIBaS-C-/UICS/codelite/config"' $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -DCONFPATH='"/home/espressjo/Documents/git/HiCIBaS-C-/UICS/codelite/config"' $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
CONFPATH:=/home/espressjo/Documents/git/HiCIBaS-C-/UICS/codelite/config
Objects0=./build-$(ConfigurationName)//up_src_fhtable.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//up_src_inst_log.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//up_src_msghandler.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//up_src_uics_sockets.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//up_src_uics_sndmsg.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//up_src_uics_cmds.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//up_src_uics_state.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//up_src_b64.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//up_src_inst_time.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//up_src_udp_msghandler.cpp$(ObjectSuffix) \
	./build-$(ConfigurationName)//up_src_state_handler.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//up_client_udp_client_socket.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//main.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//up_src_uics_join.cpp$(ObjectSuffix) 



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
./build-$(ConfigurationName)//up_src_fhtable.cpp$(ObjectSuffix): ../src/fhtable.cpp ./build-$(ConfigurationName)//up_src_fhtable.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/fhtable.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_fhtable.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_fhtable.cpp$(DependSuffix): ../src/fhtable.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_fhtable.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_fhtable.cpp$(DependSuffix) -MM ../src/fhtable.cpp

./build-$(ConfigurationName)//up_src_fhtable.cpp$(PreprocessSuffix): ../src/fhtable.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_fhtable.cpp$(PreprocessSuffix) ../src/fhtable.cpp

./build-$(ConfigurationName)//up_src_inst_log.cpp$(ObjectSuffix): ../src/inst_log.cpp ./build-$(ConfigurationName)//up_src_inst_log.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/inst_log.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_inst_log.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_inst_log.cpp$(DependSuffix): ../src/inst_log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_inst_log.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_inst_log.cpp$(DependSuffix) -MM ../src/inst_log.cpp

./build-$(ConfigurationName)//up_src_inst_log.cpp$(PreprocessSuffix): ../src/inst_log.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_inst_log.cpp$(PreprocessSuffix) ../src/inst_log.cpp

./build-$(ConfigurationName)//up_src_msghandler.cpp$(ObjectSuffix): ../src/msghandler.cpp ./build-$(ConfigurationName)//up_src_msghandler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/msghandler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_msghandler.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_msghandler.cpp$(DependSuffix): ../src/msghandler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_msghandler.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_msghandler.cpp$(DependSuffix) -MM ../src/msghandler.cpp

./build-$(ConfigurationName)//up_src_msghandler.cpp$(PreprocessSuffix): ../src/msghandler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_msghandler.cpp$(PreprocessSuffix) ../src/msghandler.cpp

./build-$(ConfigurationName)//up_src_uics_sockets.cpp$(ObjectSuffix): ../src/uics_sockets.cpp ./build-$(ConfigurationName)//up_src_uics_sockets.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/uics_sockets.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_uics_sockets.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_uics_sockets.cpp$(DependSuffix): ../src/uics_sockets.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_uics_sockets.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_uics_sockets.cpp$(DependSuffix) -MM ../src/uics_sockets.cpp

./build-$(ConfigurationName)//up_src_uics_sockets.cpp$(PreprocessSuffix): ../src/uics_sockets.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_uics_sockets.cpp$(PreprocessSuffix) ../src/uics_sockets.cpp

./build-$(ConfigurationName)//up_src_uics_sndmsg.cpp$(ObjectSuffix): ../src/uics_sndmsg.cpp ./build-$(ConfigurationName)//up_src_uics_sndmsg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/uics_sndmsg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_uics_sndmsg.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_uics_sndmsg.cpp$(DependSuffix): ../src/uics_sndmsg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_uics_sndmsg.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_uics_sndmsg.cpp$(DependSuffix) -MM ../src/uics_sndmsg.cpp

./build-$(ConfigurationName)//up_src_uics_sndmsg.cpp$(PreprocessSuffix): ../src/uics_sndmsg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_uics_sndmsg.cpp$(PreprocessSuffix) ../src/uics_sndmsg.cpp

./build-$(ConfigurationName)//up_src_uics_cmds.cpp$(ObjectSuffix): ../src/uics_cmds.cpp ./build-$(ConfigurationName)//up_src_uics_cmds.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/uics_cmds.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_uics_cmds.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_uics_cmds.cpp$(DependSuffix): ../src/uics_cmds.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_uics_cmds.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_uics_cmds.cpp$(DependSuffix) -MM ../src/uics_cmds.cpp

./build-$(ConfigurationName)//up_src_uics_cmds.cpp$(PreprocessSuffix): ../src/uics_cmds.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_uics_cmds.cpp$(PreprocessSuffix) ../src/uics_cmds.cpp

./build-$(ConfigurationName)//up_src_uics_state.cpp$(ObjectSuffix): ../src/uics_state.cpp ./build-$(ConfigurationName)//up_src_uics_state.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/uics_state.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_uics_state.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_uics_state.cpp$(DependSuffix): ../src/uics_state.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_uics_state.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_uics_state.cpp$(DependSuffix) -MM ../src/uics_state.cpp

./build-$(ConfigurationName)//up_src_uics_state.cpp$(PreprocessSuffix): ../src/uics_state.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_uics_state.cpp$(PreprocessSuffix) ../src/uics_state.cpp

./build-$(ConfigurationName)//up_src_b64.cpp$(ObjectSuffix): ../src/b64.cpp ./build-$(ConfigurationName)//up_src_b64.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/b64.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_b64.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_b64.cpp$(DependSuffix): ../src/b64.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_b64.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_b64.cpp$(DependSuffix) -MM ../src/b64.cpp

./build-$(ConfigurationName)//up_src_b64.cpp$(PreprocessSuffix): ../src/b64.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_b64.cpp$(PreprocessSuffix) ../src/b64.cpp

./build-$(ConfigurationName)//up_src_inst_time.cpp$(ObjectSuffix): ../src/inst_time.cpp ./build-$(ConfigurationName)//up_src_inst_time.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/inst_time.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_inst_time.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_inst_time.cpp$(DependSuffix): ../src/inst_time.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_inst_time.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_inst_time.cpp$(DependSuffix) -MM ../src/inst_time.cpp

./build-$(ConfigurationName)//up_src_inst_time.cpp$(PreprocessSuffix): ../src/inst_time.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_inst_time.cpp$(PreprocessSuffix) ../src/inst_time.cpp

./build-$(ConfigurationName)//up_src_udp_msghandler.cpp$(ObjectSuffix): ../src/udp_msghandler.cpp ./build-$(ConfigurationName)//up_src_udp_msghandler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/udp_msghandler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_udp_msghandler.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_udp_msghandler.cpp$(DependSuffix): ../src/udp_msghandler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_udp_msghandler.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_udp_msghandler.cpp$(DependSuffix) -MM ../src/udp_msghandler.cpp

./build-$(ConfigurationName)//up_src_udp_msghandler.cpp$(PreprocessSuffix): ../src/udp_msghandler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_udp_msghandler.cpp$(PreprocessSuffix) ../src/udp_msghandler.cpp

./build-$(ConfigurationName)//up_src_state_handler.cpp$(ObjectSuffix): ../src/state_handler.cpp ./build-$(ConfigurationName)//up_src_state_handler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/state_handler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_state_handler.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_state_handler.cpp$(DependSuffix): ../src/state_handler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_state_handler.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_state_handler.cpp$(DependSuffix) -MM ../src/state_handler.cpp

./build-$(ConfigurationName)//up_src_state_handler.cpp$(PreprocessSuffix): ../src/state_handler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_state_handler.cpp$(PreprocessSuffix) ../src/state_handler.cpp

./build-$(ConfigurationName)//up_client_udp_client_socket.cpp$(ObjectSuffix): ../client/udp_client_socket.cpp ./build-$(ConfigurationName)//up_client_udp_client_socket.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/client/udp_client_socket.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_client_udp_client_socket.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_client_udp_client_socket.cpp$(DependSuffix): ../client/udp_client_socket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_client_udp_client_socket.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_client_udp_client_socket.cpp$(DependSuffix) -MM ../client/udp_client_socket.cpp

./build-$(ConfigurationName)//up_client_udp_client_socket.cpp$(PreprocessSuffix): ../client/udp_client_socket.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_client_udp_client_socket.cpp$(PreprocessSuffix) ../client/udp_client_socket.cpp

./build-$(ConfigurationName)//main.cpp$(ObjectSuffix): main.cpp ./build-$(ConfigurationName)//main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/codelite/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//main.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//main.cpp$(DependSuffix) -MM main.cpp

./build-$(ConfigurationName)//main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//main.cpp$(PreprocessSuffix) main.cpp

./build-$(ConfigurationName)//up_src_uics_join.cpp$(ObjectSuffix): ../src/uics_join.cpp ./build-$(ConfigurationName)//up_src_uics_join.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/espressjo/Documents/git/HiCIBaS-C-/UICS/src/uics_join.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_uics_join.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_src_uics_join.cpp$(DependSuffix): ../src/uics_join.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_src_uics_join.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_src_uics_join.cpp$(DependSuffix) -MM ../src/uics_join.cpp

./build-$(ConfigurationName)//up_src_uics_join.cpp$(PreprocessSuffix): ../src/uics_join.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_src_uics_join.cpp$(PreprocessSuffix) ../src/uics_join.cpp


-include ./build-$(ConfigurationName)///*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


