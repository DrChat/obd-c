set(libobd_SOURCES
    "elm_device.cc"
    "elm_device.h"
    "scanner.h"
)

if (WIN32)
    set(libobd_SOURCES
        "${libobd_SOURCES}"
        "transport_bt_win32.cc"
        "transport_bt_win32.h"
    )
endif()

add_library(libobd STATIC ${libobd_SOURCES})
