SET(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

# esp32
#SET(CMAKE_CXX_FLAGS "-std=c++17 -std=gnu++17 -mlongcalls -Wno-missing-field-initializers -Wno-frame-address -fexceptions -fno-rtti -ffunction-sections -fdata-sections -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wno-unused-parameter -Wno-sign-compare -ggdb -Os -freorder-blocks -Wwrite-strings -fstack-protector -fstrict-volatile-bitfields -Wno-error=unused-but-set-variable -fno-jump-tables -fno-tree-switch-conversion -MMD")
#SET(CMAKE_C_FLAGS "-mlongcalls -Wno-frame-address -std=gnu99 -Wno-old-style-declaration -ffunction-sections -fdata-sections -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wno-unused-parameter -Wno-sign-compare -ggdb -Os -freorder-blocks -Wwrite-strings -fstack-protector -fstrict-volatile-bitfields -Wno-error=unused-but-set-variable -fno-jump-tables -fno-tree-switch-conversion -MMD")

SET(CMAKE_CXX_FLAGS "-std=c++17 -std=gnu++17 -Wno-missing-field-initializers -Wno-frame-address -fexceptions -fno-rtti -ffunction-sections -fdata-sections -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wno-unused-parameter -Wno-sign-compare -ggdb -Os -freorder-blocks -Wwrite-strings -fstack-protector -fstrict-volatile-bitfields -Wno-error=unused-but-set-variable -fno-jump-tables -fno-tree-switch-conversion -MMD")
SET(CMAKE_C_FLAGS "-Wno-frame-address -std=gnu99 -Wno-old-style-declaration -ffunction-sections -fdata-sections -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wno-unused-parameter -Wno-sign-compare -ggdb -Os -freorder-blocks -Wwrite-strings -fstack-protector -fstrict-volatile-bitfields -Wno-error=unused-but-set-variable -fno-jump-tables -fno-tree-switch-conversion -MMD")

idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "./")
