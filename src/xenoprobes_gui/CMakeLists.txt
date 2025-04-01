if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Necessary to hide the console on startup in Windows
    set(WIN32_OPTION "WIN32")
endif ()

qt_add_executable(${PROJECT_NAME}_gui ${WIN32_OPTION}
        ${PROJECT_SOURCE_DIR}/resources/resources.qrc
        AboutDialog.cpp
        AboutDialog.h
        DataProbe.cpp
        DataProbe.h
        DoubleSliderWithValWidget.cpp
        DoubleSliderWithValWidget.h
        FnSiteWidget.cpp
        FnSiteWidget.h
        InventoryLoader.cpp
        InventoryLoader.h
        InventoryModel.cpp
        InventoryModel.h
        MainWindow.cpp
        MainWindow.h
        MiraMap.cpp
        MiraMap.h
        RunOptionsWidget.cpp
        RunOptionsWidget.h
        SiteListLoader.cpp
        SiteListLoader.h
        SliderWithValWidget.cpp
        SliderWithValWidget.h
        SolutionWidget.cpp
        SolutionWidget.h
        SolverRunner.cpp
        SolverRunner.h
        main.cpp
        themeIcon.cpp
        themeIcon.h
)

# Application icon
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(${PROJECT_NAME}_gui PRIVATE xenoprobes.rc)
endif ()

find_package(Boost COMPONENTS container REQUIRED)
target_link_libraries(${PROJECT_NAME}_gui PRIVATE
        probeoptimizer
        Boost::container
        Qt::Svg
        Qt::Widgets
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}_gui
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES "$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}_gui>"
        TYPE BIN
)
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if ("${Qt6_VERSION}" VERSION_GREATER_EQUAL "6.3")
        qt_generate_deploy_app_script(
                TARGET "${PROJECT_NAME}_gui"
                OUTPUT_SCRIPT DEPLOY_SCRIPT
                NO_UNSUPPORTED_PLATFORM_ERROR
        )
    else ()
        include(deployqt)
        deployqt(
                TARGET "${PROJECT_NAME}_gui"
                OUTPUT_SCRIPT DEPLOY_SCRIPT
        )
    endif ()
    install(SCRIPT ${DEPLOY_SCRIPT})
    # This feels like a horrible hack, but we need several libraries that end up in the build dir
    # but qt_generate_deploy_app_script does not include in its output, so we just manually copy
    # them over here.  Is there a better way to do this?
    install(
        FILES
        ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/zlib1.dll
        ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/libpng16.dll
        ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/harfbuzz.dll
        ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/freetype.dll
        ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/pcre2-16.dll
        ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/zstd.dll
        ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/double-conversion.dll
        ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bz2.dll
        ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/brotlidec.dll
        ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/brotlicommon.dll
        DESTINATION
        ${CMAKE_INSTALL_BINDIR}
    )
endif ()
