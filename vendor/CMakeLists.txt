# Library obtained at "https://www.regatron.com/service/download/programming-interfaces/tcio-api.zip"

if(MSVC)
    set(REGATRON_TCIO_PATH "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.88.00 28042020")

    SET(REGATRON_LIBRARIES        "${REGATRON_TCIO_PATH}/Windows/x86_x64/Release/tcio.lib"
                                  "${REGATRON_TCIO_PATH}/Windows/x86_x64/Release_Stdcall/tcio_stdcall.lib")
    SET(REGATRON_TCIO_DLL         "${REGATRON_TCIO_PATH}/Windows/x86_x64/Release/tcio.dll")
    SET(REGATRON_TCIO_STDCALL_DLL "${REGATRON_TCIO_PATH}/Windows/x86_x64/Release_Stdcall/tcio_stdcall.dll")
    SET(REGATRON_INCLUDE          "${REGATRON_TCIO_PATH}/include")

else()
    set(REGATRON_TCIO_PATH "${REGATRON_INTERFACE_SOURCE_DIR}/vendor/Regatron/V3.80.00 30072014 (Linux)")
    SET(REGATRON_LIBRARIES "${REGATRON_TCIO_PATH}/Linux/x64/libtcio.so")
    SET(REGATRON_INCLUDE   "${REGATRON_TCIO_PATH}/include")
endif()

message(STATUS "Using Regatron TCIO from ${REGATRON_TCIO_PATH}")
message(STATUS "TCIO Include: ${REGATRON_INCLUDE}")
