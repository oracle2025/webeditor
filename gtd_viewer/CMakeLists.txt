add_executable(gtd_viewer
	gtdviewermain.cpp
	gtdwindow.h
	gtdwindow.cpp
	checkbox.rc
	)
if (UNIX)
	target_link_libraries(gtd_viewer ${FLTK_LIBRARIES} dl)
else()
	target_link_libraries(gtd_viewer ${FLTK_LIBRARIES})
endif()

