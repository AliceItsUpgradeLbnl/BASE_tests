set(SOURCES_LIB
	MainFrame.cxx
	TimedFrame.cxx
	TabFrame.cxx
	HistoList.cxx
	GuiTestFrame.cxx
	FileMonitor.cxx
	BaseMonitorFrame.cxx
	GraphicMonitorFrame.cxx
	TextMonitorFrame.cxx	
	DecoderMonitorFrame.cxx
	LogFile.cxx
	DList.cxx
	Options.cxx
	PayloadADC.cxx
	Masks.cxx
	Decoder.cxx
)

set(SOURCES_EXE
	main.cxx
	)

set(NAME_LIB TestGui)

# Headers from sources
string(REPLACE ".cxx" ".h" HEADERS_LIB "${SOURCES_LIB}")

# generate dictionaries
set(DICT ${NAME_LIB}_Dict )

set(LINKDEFS "${NAME_LIB}_LinkDef.h" )

# for cint to find the LinkDef 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${NAME_LIB} SHARED ${SOURCES_LIB} ${DICT}.cxx)
target_include_directories(${NAME_LIB} PUBLIC ${ROOT_INCLUDE_DIR})
ROOT_GENERATE_DICTIONARY(${DICT} ${HEADERS_LIB} LINKDEF ${LINKDEFS})
target_link_libraries(${NAME_LIB} ${ROOT_LIBRARIES})

install(TARGETS ${NAME_LIB} LIBRARY DESTINATION lib COMPONENT library)
install(FILES ${HEADERS_LIB} DESTINATION include)

set(NAME_EXE testTBGui)
add_executable(${NAME_EXE} ${SOURCES_EXE})
target_include_directories(${NAME_EXE} PUBLIC ${ROOT_INCLUDE_DIR})
target_link_libraries( ${NAME_EXE} ${NAME_LIB})

install(TARGETS ${NAME_EXE} RUNTIME DESTINATION bin)

add_executable(hex hex.cxx)
install(TARGETS hex RUNTIME DESTINATION bin)

add_executable(hex32bit hex32bit.cxx)
target_include_directories(hex32bit PUBLIC ${ROOT_INCLUDE_DIR})
target_link_libraries(hex32bit ${NAME_LIB})
install(TARGETS hex32bit RUNTIME DESTINATION bin)
