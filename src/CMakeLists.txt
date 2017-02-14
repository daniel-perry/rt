PROJECT( rt )
cmake_minimum_required(VERSION 2.8)  

#Match everything when analyzing dependencies
INCLUDE_REGULAR_EXPRESSION("^.*$")
OPTION (BUILD_SHARED_LIBS "Build shared libraries" OFF)

IF (WIN32 AND MSVC)
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF (WIN32 AND MSVC)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# required for opengl based viewer
# (the command line version rt, can be compiled w/o this stuff)
find_package(GLUT REQUIRED)
include_directories( ${GLUT_INCLUDE_DIRS} )
find_package(OpenGL REQUIRED)
include_directories( ${OpenGL_INCLUDE_DIRS} )
find_package(Threads REQUIRED)
include_directories( ${Threads_INCLUDE_DIRS} )
#find_package(Teem REQUIRED) # version 1.10.0
#include_directories( ${Teem_INCLUDE_DIRS} )

#FIND_PACKAGE(ITK REQUIRED)
#IF (ITK_FOUND)
#  INCLUDE (${USE_ITK_FILE})
#ENDIF(ITK_FOUND)
FIND_PACKAGE(ITK REQUIRED)
IF(ITK_FOUND)
    INCLUDE_DIRECTORIES(${ITK_INCLUDES})
    INCLUDE( ${ITK_USE_FILE} ) 
ELSE(ITK_FOUND)
    MESSAGE(FATAL_ERROR "ERROR: ITK not found")
ENDIF(ITK_FOUND)




SET( gl_HDRS
  gl/AxisBox.h
  gl/Background.h
  gl/BBox.h
  gl/BoxFilter.h
  gl/BVH.h
  gl/Camera.h
  gl/Colormap.h
  gl/ConstantBackground.h
  gl/CoupledMaterial.h
  gl/DielectricMaterial.h
  gl/DirectionalLight.h
  gl/Disk.h
  gl/dynarray.h
  gl/Filter.h
  gl/FunctionEval.h
  gl/GaussianFilter.h
  gl/Group.h
  gl/HeightField.h
  gl/HitRecord.h
  gl/hpoint.h
  gl/image.h
  gl/JitteredSample.h
  gl/LambertianMaterial.h
  gl/LanczosSincFilter.h
  gl/Light.h
  gl/Material.h
  gl/matrix.h
  gl/MetalMaterial.h
  gl/Object.h
  gl/PhongMaterial.h
  gl/PhongVolumeMaterial.h
  gl/MIPVolumeMaterial.h
  gl/VolumeMaterial.h
  gl/CurvatureVolumeMaterial.h
  gl/PinholeCamera.h
  gl/Plane.h
  gl/PointLight.h
  gl/Primitive.h
  gl/ray.h
  gl/RenderContext.h
  gl/rgb.h
  gl/Ring.h
  gl/Sample.h
  gl/Scene.h
  gl/Sphere.h
  gl/TentFilter.h
  gl/Time.h
  gl/TInstance.h
  gl/Triangle.h
  gl/UniformSample.h
  gl/util.h
  gl/vector3d.h
)

SET( gl_SRCS
  gl/AxisBox.cc
  gl/BBox.cc
  gl/BoxFilter.cc
  gl/BVH.cc
  gl/Colormap.cc
  gl/CoupledMaterial.cc
  gl/DielectricMaterial.cc
  gl/Disk.cc
  gl/FunctionEval.cc
  gl/GaussianFilter.cc
  gl/HeightField.cc
  gl/image.cc
  gl/JitteredSample.cc
  gl/LambertianMaterial.cc
  gl/LanczosSincFilter.cc
  gl/matrix.cc
  gl/MetalMaterial.cc
  gl/Object.cc
  gl/PhongMaterial.cc
  gl/PhongVolumeMaterial.cc
  gl/MIPVolumeMaterial.cc
  gl/VolumeMaterial.cc
  gl/CurvatureVolumeMaterial.cc
  gl/PinholeCamera.cc
  gl/ray.cc
  gl/rgb.cc
  gl/Ring.cc
  gl/Scene.cc
  gl/Sphere.cc
  gl/TentFilter.cc
  gl/Time.cc
  gl/TInstance.cc
  gl/Triangle.cc
  gl/UniformSample.cc
  gl/vector3d.cc
)

################
# gl library
################

ADD_LIBRARY( gl
  ${gl_HDRS}
  ${gl_SRCS}
)

##################
# rt - commandline ray tracer
#################
SET( rt_HDRS
  rt.h
)

SET( rt_SRCS
  rt.cc
)

ADD_EXECUTABLE( rt
                ${rt_HDRS}
                ${rt_SRCS}
                #${Teem_LIBRARY}
				      )

TARGET_LINK_LIBRARIES(  rt
                        gl
                        ${ITK_LIBRARIES}
                     ) 

###################
# raytracer lib - encapsulates the raytracer for use in other programs
###################

ADD_LIBRARY( raytracer
  req.h # scene def
  volume_scene.h # scene def
  rt.h
  RayTracer.h
  RayTracer.cc
)

#####################
# viewer - interactive interface to the ray tracer
#####################

SET( viewer_SRCS
  viewer.cc
)

ADD_EXECUTABLE( viewer
                ${viewer_SRCS}
				      )

TARGET_LINK_LIBRARIES( viewer
                       raytracer
                       gl
                       ${GLUT_LIBRARY} 
                       ${OPENGL_LIBRARY}
                       #${Threads_LIBRARY}
                       ${CMAKE_THREAD_LIBS_INIT}
                       #${Teem_LIBRARY}
                       ${ITK_LIBRARIES}
                     )

#####################
# rtvol - commandline volume renderer
#####################

SET( rtvol_HDRS
  RayTracer.h
	gl/image.h
	gl/rgb.h
)

SET( rtvol_SRCS
  rtvol.cc
)

ADD_EXECUTABLE( rtvol
                ${rtvol_SRCS}
				      )

TARGET_LINK_LIBRARIES( rtvol
                       raytracer
											 gl
                       ${CMAKE_THREAD_LIBS_INIT}
                       ${ITK_LIBRARIES}
                     )
