#ifndef rt_H
#define rt_H

// local
#include "gl/hpoint.h"
#include "gl/image.h"
#include "gl/ray.h"
#include "gl/rgb.h"
#include "gl/vector3d.h"

#include "gl/Scene.h"
#include "gl/Sphere.h"
#include "gl/Triangle.h"
#include "gl/Disk.h"
#include "gl/Ring.h"
#include "gl/AxisBox.h"
#include "gl/HeightField.h"

#include "gl/LambertianMaterial.h"
#include "gl/PhongMaterial.h"
#include "gl/MetalMaterial.h"
#include "gl/DielectricMaterial.h"
#include "gl/CoupledMaterial.h"
#include "gl/PhongVolumeMaterial.h"
#include "gl/MIPVolumeMaterial.h"
#include "gl/VolumeMaterial.h"
#include "gl/CurvatureVolumeMaterial.h"

#include "gl/Colormap.h"

#include "gl/Camera.h"
#include "gl/Light.h"
#include "gl/DirectionalLight.h"
#include "gl/PointLight.h"
#include "gl/ConstantBackground.h"
#include "gl/PinholeCamera.h"
#include "gl/Background.h"

#include "gl/Object.h"
#include "gl/Material.h"
#include "gl/Group.h"
#include "gl/Plane.h"

#include "gl/Filter.h"
#include "gl/BoxFilter.h"
#include "gl/TentFilter.h"
#include "gl/GaussianFilter.h"
//#include "gl/LanczosSincFilter.h"

#include "gl/Sample.h"
#include "gl/JitteredSample.h"
#include "gl/UniformSample.h"

//#include "gl/FunctionEval.h"

#include "gl/Time.h"
#include "gl/util.h"

#include "gl/BBox.h"
#include "gl/BVH.h"

#include "gl/matrix.h"
#include "gl/TInstance.h"

#endif
