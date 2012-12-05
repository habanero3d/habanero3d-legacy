#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/VDLODSkeletalAnimatedObject.o \
	${OBJECTDIR}/src/Color.o \
	${OBJECTDIR}/src/Keyboard.o \
	${OBJECTDIR}/src/HeightMapObject.o \
	${OBJECTDIR}/src/ShadowCasterVolume.o \
	${OBJECTDIR}/src/SkeletalAnimatedObject.o \
	${OBJECTDIR}/src/Thread.o \
	${OBJECTDIR}/src/ShaderProgram.o \
	${OBJECTDIR}/src/VDLODStaticObject.o \
	${OBJECTDIR}/src/IResource.o \
	${OBJECTDIR}/src/Polyhedron.o \
	${OBJECTDIR}/src/CLODSkinnedMesh.o \
	${OBJECTDIR}/src/MFCommon.o \
	${OBJECTDIR}/src/ShaderManager.o \
	${OBJECTDIR}/src/SkeletonVisualizer.o \
	${OBJECTDIR}/src/AmbientLight.o \
	${OBJECTDIR}/src/SCF.o \
	${OBJECTDIR}/src/ResourceManager.o \
	${OBJECTDIR}/src/File.o \
	${OBJECTDIR}/src/HMF.o \
	${OBJECTDIR}/src/SRT.o \
	${OBJECTDIR}/src/PointLight.o \
	${OBJECTDIR}/src/VDLODSkinnedMesh.o \
	${OBJECTDIR}/src/SkeletonJoint.o \
	${OBJECTDIR}/src/Skeleton.o \
	${OBJECTDIR}/src/GenericGeometry.o \
	${OBJECTDIR}/src/SubMesh.o \
	${OBJECTDIR}/src/SkeletalAnimation.o \
	${OBJECTDIR}/src/OpenGL_Renderer.o \
	${OBJECTDIR}/src/StringUtil.o \
	${OBJECTDIR}/src/SkeletalShader.o \
	${OBJECTDIR}/src/Camera.o \
	${OBJECTDIR}/src/AABB.o \
	${OBJECTDIR}/src/Spatial.o \
	${OBJECTDIR}/src/RootObject.o \
	${OBJECTDIR}/src/StaticObject.o \
	${OBJECTDIR}/src/LogManager.o \
	${OBJECTDIR}/src/Mutex.o \
	${OBJECTDIR}/src/RT.o \
	${OBJECTDIR}/src/MTF.o \
	${OBJECTDIR}/src/SAF.o \
	${OBJECTDIR}/src/ShaderFlags.o \
	${OBJECTDIR}/src/DLODTMF.o \
	${OBJECTDIR}/src/DLODStaticObject.o \
	${OBJECTDIR}/src/CLODStaticMesh.o \
	${OBJECTDIR}/src/CLODSkeletalAnimatedObject.o \
	${OBJECTDIR}/src/VDLODSMF.o \
	${OBJECTDIR}/src/Frustum.o \
	${OBJECTDIR}/src/newProcessedScene.o \
	${OBJECTDIR}/src/DirectionalLight.o \
	${OBJECTDIR}/src/Cylinder.o \
	${OBJECTDIR}/src/Mouse.o \
	${OBJECTDIR}/src/VDLODStaticMesh.o \
	${OBJECTDIR}/src/DLODStaticMesh.o \
	${OBJECTDIR}/src/SkeletonJointKeyframeSequence.o \
	${OBJECTDIR}/src/LightShader.o \
	${OBJECTDIR}/src/IStream.o \
	${OBJECTDIR}/src/Shader.o \
	${OBJECTDIR}/src/MaterialManager.o \
	${OBJECTDIR}/src/line3.o \
	${OBJECTDIR}/src/PlaneGrid.o \
	${OBJECTDIR}/src/TMF.o \
	${OBJECTDIR}/src/CLODSMF.o \
	${OBJECTDIR}/src/StaticMesh.o \
	${OBJECTDIR}/src/Pick.o \
	${OBJECTDIR}/src/DLODSkeletalAnimatedObject.o \
	${OBJECTDIR}/src/CLODTMF.o \
	${OBJECTDIR}/src/plane.o \
	${OBJECTDIR}/src/CullingVolumesSet.o \
	${OBJECTDIR}/src/matrix4.o \
	${OBJECTDIR}/src/vector3.o \
	${OBJECTDIR}/src/RenderThread.o \
	${OBJECTDIR}/src/Window.o \
	${OBJECTDIR}/src/DLODSMF.o \
	${OBJECTDIR}/src/CFileStream.o \
	${OBJECTDIR}/src/VSplit.o \
	${OBJECTDIR}/src/Exception.o \
	${OBJECTDIR}/src/assert.o \
	${OBJECTDIR}/src/FPSLogger.o \
	${OBJECTDIR}/src/Node.o \
	${OBJECTDIR}/src/Texture.o \
	${OBJECTDIR}/src/vector2.o \
	${OBJECTDIR}/src/SkeletalAnimationState.o \
	${OBJECTDIR}/src/CLODStaticObject.o \
	${OBJECTDIR}/src/DLODSkinnedMesh.o \
	${OBJECTDIR}/src/Material.o \
	${OBJECTDIR}/src/psx_File.o \
	${OBJECTDIR}/src/Event.o \
	${OBJECTDIR}/src/Logger.o \
	${OBJECTDIR}/src/HeightMapMesh.o \
	${OBJECTDIR}/src/SkinnedMesh.o \
	${OBJECTDIR}/src/SMF.o \
	${OBJECTDIR}/src/SkeletalAnimationStateSet.o \
	${OBJECTDIR}/src/DLODLevel.o \
	${OBJECTDIR}/src/quaternion.o \
	${OBJECTDIR}/src/EventConverter.o \
	${OBJECTDIR}/src/ModelFactory.o \
	${OBJECTDIR}/src/VDLODTMF.o \
	${OBJECTDIR}/src/Sphere.o \
	${OBJECTDIR}/src/Light.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libhabaneroport.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libhabaneroport.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libhabaneroport.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libhabaneroport.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libhabaneroport.a

${OBJECTDIR}/src/VDLODSkeletalAnimatedObject.o: src/VDLODSkeletalAnimatedObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/VDLODSkeletalAnimatedObject.o src/VDLODSkeletalAnimatedObject.cpp

${OBJECTDIR}/src/Color.o: src/Color.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Color.o src/Color.cpp

${OBJECTDIR}/src/Keyboard.o: src/Keyboard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Keyboard.o src/Keyboard.cpp

${OBJECTDIR}/src/HeightMapObject.o: src/HeightMapObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HeightMapObject.o src/HeightMapObject.cpp

${OBJECTDIR}/src/ShadowCasterVolume.o: src/ShadowCasterVolume.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ShadowCasterVolume.o src/ShadowCasterVolume.cpp

${OBJECTDIR}/src/SkeletalAnimatedObject.o: src/SkeletalAnimatedObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletalAnimatedObject.o src/SkeletalAnimatedObject.cpp

${OBJECTDIR}/src/Thread.o: src/Thread.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Thread.o src/Thread.cpp

${OBJECTDIR}/src/ShaderProgram.o: src/ShaderProgram.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ShaderProgram.o src/ShaderProgram.cpp

${OBJECTDIR}/src/VDLODStaticObject.o: src/VDLODStaticObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/VDLODStaticObject.o src/VDLODStaticObject.cpp

${OBJECTDIR}/src/IResource.o: src/IResource.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/IResource.o src/IResource.cpp

${OBJECTDIR}/src/Polyhedron.o: src/Polyhedron.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Polyhedron.o src/Polyhedron.cpp

${OBJECTDIR}/src/CLODSkinnedMesh.o: src/CLODSkinnedMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CLODSkinnedMesh.o src/CLODSkinnedMesh.cpp

${OBJECTDIR}/src/MFCommon.o: src/MFCommon.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/MFCommon.o src/MFCommon.cpp

${OBJECTDIR}/src/ShaderManager.o: src/ShaderManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ShaderManager.o src/ShaderManager.cpp

${OBJECTDIR}/src/SkeletonVisualizer.o: src/SkeletonVisualizer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletonVisualizer.o src/SkeletonVisualizer.cpp

${OBJECTDIR}/src/AmbientLight.o: src/AmbientLight.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AmbientLight.o src/AmbientLight.cpp

${OBJECTDIR}/src/SCF.o: src/SCF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SCF.o src/SCF.cpp

${OBJECTDIR}/src/ResourceManager.o: src/ResourceManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ResourceManager.o src/ResourceManager.cpp

${OBJECTDIR}/src/File.o: src/File.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/File.o src/File.cpp

${OBJECTDIR}/src/HMF.o: src/HMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HMF.o src/HMF.cpp

${OBJECTDIR}/src/SRT.o: src/SRT.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SRT.o src/SRT.cpp

${OBJECTDIR}/src/PointLight.o: src/PointLight.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/PointLight.o src/PointLight.cpp

${OBJECTDIR}/src/VDLODSkinnedMesh.o: src/VDLODSkinnedMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/VDLODSkinnedMesh.o src/VDLODSkinnedMesh.cpp

${OBJECTDIR}/src/SkeletonJoint.o: src/SkeletonJoint.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletonJoint.o src/SkeletonJoint.cpp

${OBJECTDIR}/src/Skeleton.o: src/Skeleton.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Skeleton.o src/Skeleton.cpp

${OBJECTDIR}/src/GenericGeometry.o: src/GenericGeometry.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/GenericGeometry.o src/GenericGeometry.cpp

${OBJECTDIR}/src/SubMesh.o: src/SubMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SubMesh.o src/SubMesh.cpp

${OBJECTDIR}/src/SkeletalAnimation.o: src/SkeletalAnimation.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletalAnimation.o src/SkeletalAnimation.cpp

${OBJECTDIR}/src/OpenGL_Renderer.o: src/OpenGL_Renderer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/OpenGL_Renderer.o src/OpenGL_Renderer.cpp

${OBJECTDIR}/src/StringUtil.o: src/StringUtil.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/StringUtil.o src/StringUtil.cpp

${OBJECTDIR}/src/SkeletalShader.o: src/SkeletalShader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletalShader.o src/SkeletalShader.cpp

${OBJECTDIR}/src/Camera.o: src/Camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Camera.o src/Camera.cpp

${OBJECTDIR}/src/AABB.o: src/AABB.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AABB.o src/AABB.cpp

${OBJECTDIR}/src/Spatial.o: src/Spatial.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Spatial.o src/Spatial.cpp

${OBJECTDIR}/src/RootObject.o: src/RootObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RootObject.o src/RootObject.cpp

${OBJECTDIR}/src/StaticObject.o: src/StaticObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/StaticObject.o src/StaticObject.cpp

${OBJECTDIR}/src/LogManager.o: src/LogManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/LogManager.o src/LogManager.cpp

${OBJECTDIR}/src/Mutex.o: src/Mutex.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Mutex.o src/Mutex.cpp

${OBJECTDIR}/src/RT.o: src/RT.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RT.o src/RT.cpp

${OBJECTDIR}/src/MTF.o: src/MTF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/MTF.o src/MTF.cpp

${OBJECTDIR}/src/SAF.o: src/SAF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SAF.o src/SAF.cpp

${OBJECTDIR}/src/ShaderFlags.o: src/ShaderFlags.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ShaderFlags.o src/ShaderFlags.cpp

${OBJECTDIR}/src/DLODTMF.o: src/DLODTMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DLODTMF.o src/DLODTMF.cpp

${OBJECTDIR}/src/DLODStaticObject.o: src/DLODStaticObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DLODStaticObject.o src/DLODStaticObject.cpp

${OBJECTDIR}/src/CLODStaticMesh.o: src/CLODStaticMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CLODStaticMesh.o src/CLODStaticMesh.cpp

${OBJECTDIR}/src/CLODSkeletalAnimatedObject.o: src/CLODSkeletalAnimatedObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CLODSkeletalAnimatedObject.o src/CLODSkeletalAnimatedObject.cpp

${OBJECTDIR}/src/VDLODSMF.o: src/VDLODSMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/VDLODSMF.o src/VDLODSMF.cpp

${OBJECTDIR}/src/Frustum.o: src/Frustum.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Frustum.o src/Frustum.cpp

${OBJECTDIR}/src/newProcessedScene.o: src/newProcessedScene.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/newProcessedScene.o src/newProcessedScene.cpp

${OBJECTDIR}/src/DirectionalLight.o: src/DirectionalLight.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DirectionalLight.o src/DirectionalLight.cpp

${OBJECTDIR}/src/Cylinder.o: src/Cylinder.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Cylinder.o src/Cylinder.cpp

${OBJECTDIR}/src/Mouse.o: src/Mouse.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Mouse.o src/Mouse.cpp

${OBJECTDIR}/src/VDLODStaticMesh.o: src/VDLODStaticMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/VDLODStaticMesh.o src/VDLODStaticMesh.cpp

${OBJECTDIR}/src/DLODStaticMesh.o: src/DLODStaticMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DLODStaticMesh.o src/DLODStaticMesh.cpp

${OBJECTDIR}/src/SkeletonJointKeyframeSequence.o: src/SkeletonJointKeyframeSequence.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletonJointKeyframeSequence.o src/SkeletonJointKeyframeSequence.cpp

${OBJECTDIR}/src/LightShader.o: src/LightShader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/LightShader.o src/LightShader.cpp

${OBJECTDIR}/src/IStream.o: src/IStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/IStream.o src/IStream.cpp

${OBJECTDIR}/src/Shader.o: src/Shader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Shader.o src/Shader.cpp

${OBJECTDIR}/src/MaterialManager.o: src/MaterialManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/MaterialManager.o src/MaterialManager.cpp

${OBJECTDIR}/src/line3.o: src/line3.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/line3.o src/line3.cpp

${OBJECTDIR}/src/PlaneGrid.o: src/PlaneGrid.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/PlaneGrid.o src/PlaneGrid.cpp

${OBJECTDIR}/src/TMF.o: src/TMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/TMF.o src/TMF.cpp

${OBJECTDIR}/src/CLODSMF.o: src/CLODSMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CLODSMF.o src/CLODSMF.cpp

${OBJECTDIR}/src/StaticMesh.o: src/StaticMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/StaticMesh.o src/StaticMesh.cpp

${OBJECTDIR}/src/Pick.o: src/Pick.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Pick.o src/Pick.cpp

${OBJECTDIR}/src/DLODSkeletalAnimatedObject.o: src/DLODSkeletalAnimatedObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DLODSkeletalAnimatedObject.o src/DLODSkeletalAnimatedObject.cpp

${OBJECTDIR}/src/CLODTMF.o: src/CLODTMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CLODTMF.o src/CLODTMF.cpp

${OBJECTDIR}/src/plane.o: src/plane.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/plane.o src/plane.cpp

${OBJECTDIR}/src/CullingVolumesSet.o: src/CullingVolumesSet.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CullingVolumesSet.o src/CullingVolumesSet.cpp

${OBJECTDIR}/src/matrix4.o: src/matrix4.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/matrix4.o src/matrix4.cpp

${OBJECTDIR}/src/vector3.o: src/vector3.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/vector3.o src/vector3.cpp

${OBJECTDIR}/src/RenderThread.o: src/RenderThread.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RenderThread.o src/RenderThread.cpp

${OBJECTDIR}/src/Window.o: src/Window.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Window.o src/Window.cpp

${OBJECTDIR}/src/DLODSMF.o: src/DLODSMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DLODSMF.o src/DLODSMF.cpp

${OBJECTDIR}/src/CFileStream.o: src/CFileStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CFileStream.o src/CFileStream.cpp

${OBJECTDIR}/src/VSplit.o: src/VSplit.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/VSplit.o src/VSplit.cpp

${OBJECTDIR}/src/Exception.o: src/Exception.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Exception.o src/Exception.cpp

${OBJECTDIR}/src/assert.o: src/assert.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/assert.o src/assert.cpp

${OBJECTDIR}/src/FPSLogger.o: src/FPSLogger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/FPSLogger.o src/FPSLogger.cpp

${OBJECTDIR}/src/Node.o: src/Node.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Node.o src/Node.cpp

${OBJECTDIR}/src/Texture.o: src/Texture.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Texture.o src/Texture.cpp

${OBJECTDIR}/src/vector2.o: src/vector2.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/vector2.o src/vector2.cpp

${OBJECTDIR}/src/SkeletalAnimationState.o: src/SkeletalAnimationState.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletalAnimationState.o src/SkeletalAnimationState.cpp

${OBJECTDIR}/src/CLODStaticObject.o: src/CLODStaticObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CLODStaticObject.o src/CLODStaticObject.cpp

${OBJECTDIR}/src/DLODSkinnedMesh.o: src/DLODSkinnedMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DLODSkinnedMesh.o src/DLODSkinnedMesh.cpp

${OBJECTDIR}/src/Material.o: src/Material.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Material.o src/Material.cpp

${OBJECTDIR}/src/psx_File.o: src/psx_File.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/psx_File.o src/psx_File.cpp

${OBJECTDIR}/src/Event.o: src/Event.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Event.o src/Event.cpp

${OBJECTDIR}/src/Logger.o: src/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Logger.o src/Logger.cpp

${OBJECTDIR}/src/HeightMapMesh.o: src/HeightMapMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HeightMapMesh.o src/HeightMapMesh.cpp

${OBJECTDIR}/src/SkinnedMesh.o: src/SkinnedMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkinnedMesh.o src/SkinnedMesh.cpp

${OBJECTDIR}/src/SMF.o: src/SMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SMF.o src/SMF.cpp

${OBJECTDIR}/src/SkeletalAnimationStateSet.o: src/SkeletalAnimationStateSet.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SkeletalAnimationStateSet.o src/SkeletalAnimationStateSet.cpp

${OBJECTDIR}/src/DLODLevel.o: src/DLODLevel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DLODLevel.o src/DLODLevel.cpp

${OBJECTDIR}/src/quaternion.o: src/quaternion.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/quaternion.o src/quaternion.cpp

${OBJECTDIR}/src/EventConverter.o: src/EventConverter.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/EventConverter.o src/EventConverter.cpp

${OBJECTDIR}/src/ModelFactory.o: src/ModelFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ModelFactory.o src/ModelFactory.cpp

${OBJECTDIR}/src/VDLODTMF.o: src/VDLODTMF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/VDLODTMF.o src/VDLODTMF.cpp

${OBJECTDIR}/src/Sphere.o: src/Sphere.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Sphere.o src/Sphere.cpp

${OBJECTDIR}/src/Light.o: src/Light.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Light.o src/Light.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libhabaneroport.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
