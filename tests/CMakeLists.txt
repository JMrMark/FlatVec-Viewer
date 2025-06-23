find_package(Qt6 REQUIRED COMPONENTS Test Gui)

# CurvedRectangle
add_executable(test_curvedrectangle test_curvedrectangle.cpp)
target_link_libraries(test_curvedrectangle PRIVATE Qt6::Test Qt6::Gui rectangle_logic)
add_test(NAME test_curvedrectangle COMMAND test_curvedrectangle)

# OrdinaryRectangle
add_executable(test_ordinaryrectangle test_ordinaryrectangle.cpp)
target_link_libraries(test_ordinaryrectangle PRIVATE Qt6::Test Qt6::Gui rectangle_logic)
add_test(NAME test_ordinaryrectangle COMMAND test_ordinaryrectangle)

# SlantedRectangle
add_executable(test_slantedrectangle test_slantedrectangle.cpp)
target_link_libraries(test_slantedrectangle PRIVATE Qt6::Test Qt6::Gui rectangle_logic)
add_test(NAME test_slantedrectangle COMMAND test_slantedrectangle)
