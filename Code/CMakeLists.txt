cmake_minimum_required(VERSION 3.8...3.12 FATAL_ERROR)
project(CGTMB)

set(PKG "CGTMB")

find_package(VTK
  COMPONENTS
    CommonCore
    GUISupportQt
    InfovisCore
    RenderingFreeType
    ViewsQt
    CommonDataModel
    CommonTransforms
    FiltersGeneral
    FiltersSources
    FiltersCore
    FiltersModeling
    ImagingHybrid
    InteractionStyle
    InteractionWidgets
    RenderingAnnotation
    RenderingContextOpenGL2
    RenderingCore
    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
    )

if (NOT VTK_FOUND)
    message("Skipping example: ${VTK_NOT_FOUND_MESSAGE}")
    return ()
endif ()




# The following lets header files be included with <>, angle brackets.
# target_include_directories does not let some .h files to include other
# .h files with the angle brackets.  Hence, use include_directories.
include_directories(${CMAKE_CURRENT_LIST_DIR})

find_package(Qt5 COMPONENTS Widgets Gui)

if (NOT TARGET Qt5::Widgets OR NOT TARGET Qt5::Gui)
    message("Skipping example: ${Qt5_NOT_FOUND_MESSAGE}")
    return ()
endif ()



include_directories(/home/rmartz/Special/TVD1/Ptrac/SSS_ptrac)

# Set your files and resources here
set(Sources
    baseVtkWindow.cpp
    buildbodycellcontrols.cpp
    buildbodycontrols.cpp
    buildcellcontrols.cpp
    buildcellwindow.cpp
    cardimageview.cpp
    cgutility.cpp
    cutawaywidget.cpp
    license.cpp
    mainwindow.cpp
    masterdata.cpp
    main.cpp
    rgbColors.cpp
    simplecrypt.cpp
    sss_checkbox.cpp
    sss_combobox.cpp
    sss_guiutility.cpp
    ssslightsource.cpp
    sss_linedit.cpp
    sss_pushbutton.cpp
    sssIntersectionPolyDataCheck.cxx
    sssBooleanOperationCheck.cxx
    sssEllipticalCylinderSource.cxx
    sssGeometryHelpers.cxx
    sssEllipsoidSource.cxx
    sssCubeSource.cxx
    sssTorusSource.cxx
    projectsettings.cpp
    view3dmodel.cpp
    view3dcontrol.cpp
    celllistwidget.cpp
    meshlistwidget.cpp
    abaqusReadFile.cpp
    gridcell.cpp
    skdTree.cpp
    umObject.cpp
    unstructuredmesh.cpp
    sss_math_utilities.cpp
    sss_string_utils.cpp
    rgbColors.cpp
    textviewwindow.cpp
    embeddedmeshbuilder.cpp
    meshuniversebuild.cpp
    customzaidlist.cpp
    materialmainwindow.cpp
    materialmessagewidget.cpp
    )

set(Headers
    baseVtkWindow.h
    bodybuttoninfo.h
    bodydata.h
    bodytypemap.h
    bounds.h
    buildbodycellcontrols.h
    buildbodycontrols.h
    buildcellcontrols.h
    buildcellwindow.h
    cardimageview.h
    celldata.h
    cgbodybasics.h
    cgtmb_constants.h
    cg_constants.h
    cgutility.h
    cgzoneops.h
    cutawaywidget.h
    data_structs.h
    mainwindow.h
    masterdata.h
    opengl_lighting.h
    rgbColors.h
    simplecrypt.h
    sss_checkbox.h
    sss_combobox.h
    sss_guiutility.h
    ssslightsource.h
    sss_linedit.h
    sss_pushbutton.h
    sssIntersectionPolyDataCheck.h
    sssBooleanOperationCheck.h
    sssEllipticalCylinderSource.h
    sssGeometryHelpers.h
    sssEllipsoidSource.h
    sssCubeSource.h
    sssTorusSource.h
    projectsettings.h
    view3dmodel.h
    view3dcontrol.h
    celllistwidget.h
    meshlistwidget.h
    abaqusReadFile.h
    gridcell.h
    skdTree.h
    umDataStructures.h
    umObject.h
    unstructuredmesh.h
    sss_math_utilities.h
    sss_string_utils.h
    rgbColors.h
    textviewwindow.h
    embeddedmeshbuilder.h
    meshuniversebuild.h
    customzaidlist.h
    materialdata.h
    materialmainwindow.h
    materialmessagewidget.h
    )
set(QRCs
  Icons/icons.qrc
  Resources/resources.qrc)


message(status "CMAKE_SYSTEM_NAME IS ${CMAKE_SYSTEM_NAME}")
message(status "CMAKE_CXX_PLATFORM_ID IS ${CMAKE_CXX_PLATFORM_ID}")
message(status "CMAKE_HOST_SYSTEM_NAME IS ${CMAKE_HOST_SYSTEM_NAME}")
message(status "CMAKE_HOST_UNIX IS ${CMAKE_HOST_UNIX}")
message(status "CMAKE_HOST_WIN32 IS ${CMAKE_HOST_WIN32}")


if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_definitions(-DUSING_WINDOWS)
endif()

if (MINGW)
    message(status ">>>>> MINGW")
endif()
if (WIN32)
    message(status ">>>>> WIN32")
endif()
if (APPLE)
    message(status ">>>>> APPLE")
endif()
if (UNIX)
    message(status ">>>>> UNIX")
endif()



# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

source_group("Resources" FILES
  ${QRCs})

add_executable(CGTMB
  ${Sources} ${Headers}  ${QRCs})


target_link_libraries(CGTMB
  PRIVATE
    ${VTK_LIBRARIES}
    Qt5::Gui
    Qt5::Widgets
    /home/rmartz/Special/TVD1/Ptrac/SSS_ptrac/Build/libptrac.so
    )
