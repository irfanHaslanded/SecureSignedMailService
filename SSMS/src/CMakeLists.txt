project (SSMS)

file(GLOB SSMS_Sources *.cpp) # all cpp files in the src directory
add_executable(ssms ${SSMS_Sources})

target_link_libraries(ssms
	crypto crypt
)

add_subdirectory(unittest
	EXCLUDE_FROM_ALL # Make subdir test only if 'make test' not for 'make' or 'make all'
)
