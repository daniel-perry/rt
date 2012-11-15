
#FILTERS = Filter/FilterFactory.o Filter/Filter1D.o Filter/Filter2D.o \
#	SeparableFilter2D.o RadialFilter2D.o \
#	BoxFilter1D.o LanczosSincFilter1D.o TruncatedGaussianFilter1.o \
#	TriangleFilter1D.o
FILTERS = Filter/FilterFactory.o Filter/Filter1D.o Filter/Filter2D.o
KERNELS = \
	Filter/RadialKernel2D.h Filter/SeparableKernel2D.h \
	Filter/BoxKernel1D.h Filter/GaussianKernel1D.h \
	Filter/SincKernel1D.h Filter/TriangleKernel1D.h \
	Filter/BoxWindow1D.h Filter/LanczosWindow1D.h

OBJS = Background.o BoundingBox.o Box.o \
	Camera.o Color.o ConstantBackground.o \
	DielectricMaterial.o Disk.o \
	GriddedGroup.o Group.o \
	Heightfield.o \
	Image.o \
	LambertianMaterial.o Light.o \
	Material.o MetalMaterial.o \
	Object.o OrenNayarMaterial.o \
	PhongMaterial.o PinholeCamera.o Point.o PointLight.o Primitive.o \
	Ring.o \
	Scene.o Sphere.o \
	Time.o Triangle.o \
	Vector.o \
	$(FILTERS)

SCENES := $(wildcard scenes/*.h)

CXX := /opt/ibmcmp/vacpp/6.0/bin/xlc++
#CXX := g++
#CXXFLAGS := -g -mcpu=G5 -mtune=G5 -mpowerpc64 -mpowerpc-gpopt -O3 -fsigned-char -falign-loops=32 -ffast-math -faltivec -maltivec -mabi=altivec -mpowerpc-gfxopt -funroll-loops -finline -fobey-inline -malign-natural -mno-multiple
#CXXFLAGS := -Wall -g -O3 -mcpu=G5 -mtune=G5 -mpowerpc64 -mpowerpc-gpopt -fsigned-char -falign-loops=32 -ffast-math -faltivec -maltivec -mabi=altivec -mpowerpc-gfxopt -funroll-loops -finline -fobey-inline -mno-multiple -mdynamic-no-pic
#CXXFLAGS := -qlinedebug -O5 -qarch=g5 -qtune=g5 
CXXFLAGS := -O5 -qarch=g5 -qtune=g5 
#CXXFLAGS := -Wall -g -O2
LDFLAGS :=

.cc.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

all: specere genheightfield filter_test filter_graph

specere: specere.o $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ specere.o $(OBJS)

specere.o: $(SCENES)

genheightfield: genheightfield.o
	$(CXX) $(LDFLAGS) -o $@ genheightfield.o

filter_test: filter_test.o Image.o Time.o $(FILTERS)
	$(CXX) $(LDFLAGS) -o $@ filter_test.o Image.o Time.o $(FILTERS)

filter_graph: filter_graph.o Image.o Time.o $(FILTERS)
	$(CXX) $(LDFLAGS) -o $@ filter_graph.o Image.o Time.o $(FILTERS)

info.h: Makefile
	rm -f info.h
	echo "static char info[] = " > info.h
	echo "\"Programming language:\tC++\\n\"" >> info.h
	echo "\"CPU(s):\t\t\tPowerPC 970\\n\"" >> info.h
	echo "\"Clock rate:\t\t2.5 Ghz\\n\"" >> info.h
	echo "\"Threads count:\t\t1\\n\"" >> info.h
	echo "\"Thirdparty libraries used: none\\n\"" >> info.h
	#	echo "\"Compiler:\t\t"`g++ -v 2>&1 | tail -1 `"\\n\"" >> info.h
	echo "\"Compiler:\t\tIBM Xlc++ version 6.0\\n\"" >> info.h
	echo "\"Compiler flags:\t\t$(CXXFLAGS)\\n\"" >> info.h
	echo ";" >> info.h

Background.o: Background.h
Box.o: Box.h HitRecord.h Object.h Point.h Primitive.h Ray.h Vector.h
Camera.o: Camera.h
Color.o: Color.h
ConstantBackground.o: ConstantBackground.h Background.h Color.h
DielectricMaterial.o: Material.h
Disk.o: Disk.h HitRecord.h Math.h Object.h Primitive.h Point.h Ray.h Vector.h
Heightfield.o: Heightfield.h HitRecord.h Object.h Point.h Primitive.h Ray.h Vector.h
Image.o: Image.h Color.h
LambertianMaterial.o: LambertianMaterial.h Color.h HitRecord.h Light.h Material.h Object.h Point.h Primitive.h Ray.h RenderContext.h Scene.h Vector.h
Light.o: Light.h
Group.o: Group.h Object.h
Material.o: Material.h
MetalMaterial.o: Material.h
Object.o: Object.h
PinholeCamera.o: PinholeCamera.h Point.h Ray.h Vector.h
#Plane.o: Plane.h HitRecord.h Math.h Object.h Primitive.h Ray.h
Point.o: Point.h
PointLight.o: PointLight.h Point.h Vector.h
Primitive.o: Primitive.h Material.h Object.h
Ring.o: Ring.h HitRecord.h Math.h Object.h Primitive.h Point.h Ray.h Vector.h
Scene.o: Scene.h Background.h Camera.h Color.h Image.h Light.h Material.h Object.h Ray.h RenderContext.h $(KERNELS)
Sphere.o: Sphere.h HitRecord.h Object.h Point.h Primitive.h Ray.h Vector.h
Time.o: Time.h
Triangle.o: Triangle.h HitRecord.h Math.h Object.h Primitive.h Ray.h
Vector.o: Vector.h
ray.o: Background.h Box.h Camera.h ConstantBackground.h Color.h Group.h Image.h
ray.o: LambertianMaterial.h Light.h Material.h Object.h PinholeCamera.h Plane.h
ray.o: PointLight.h QAEB.h Ray.h Scene.h Sphere.h Time.h info.h
ray.o: req1.h
filter_test.o: info.h $(KERNELS)
Filter/FilterFactory.o: Filter/Filter1D.h Filter/Filter2D.h $(KERNELS)
GriddedGroup.o: Group.h GriddedGroup.h Array3.h
specere.o: info.h $(KERNELS)
