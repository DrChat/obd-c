set(libobd_SOURCES
    "elm_device.cc"
    "elm_device.h"
    "scanner.h"
)

add_library(libobd STATIC ${libobd_SOURCES})
