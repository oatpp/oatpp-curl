
add_library(${OATPP_THIS_MODULE_NAME}
        oatpp-curl/RequestExecutor.cpp
        oatpp-curl/RequestExecutor.hpp
        oatpp-curl/io/BodyDecoder.cpp
        oatpp-curl/io/BodyDecoder.hpp
        oatpp-curl/io/BodyInputStream.cpp
        oatpp-curl/io/BodyInputStream.hpp
        oatpp-curl/io/BodyOutputStream.cpp
        oatpp-curl/io/BodyOutputStream.hpp
        oatpp-curl/io/Curl.cpp
        oatpp-curl/io/Curl.hpp
        oatpp-curl/io/CurlBodyReader.cpp
        oatpp-curl/io/CurlBodyReader.hpp
        oatpp-curl/io/CurlBodyWriter.cpp
        oatpp-curl/io/CurlBodyWriter.hpp
        oatpp-curl/io/CurlHeadersReader.cpp
        oatpp-curl/io/CurlHeadersReader.hpp
)

set_target_properties(${OATPP_THIS_MODULE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

if(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_EXTERNAL)
    add_dependencies(${OATPP_THIS_MODULE_NAME} ${LIB_OATPP_EXTERNAL})
endif()

target_link_oatpp(${OATPP_THIS_MODULE_NAME})

target_include_directories(${OATPP_THIS_MODULE_NAME}
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_include_directories(${OATPP_THIS_MODULE_NAME}
        PUBLIC ${PKG_CURL_INCLUDE_DIRS}
)

#target_link_directories(${OATPP_THIS_MODULE_NAME}
#        PRIVATE ${PKG_CURL_LIBRARY_DIRS}
#)

target_link_libraries(${OATPP_THIS_MODULE_NAME}
        PRIVATE ${PKG_CURL_LIBRARIES}
)

#######################################################################################################
## install targets

if(OATPP_INSTALL)
    include("../cmake/module-install.cmake")
endif()
