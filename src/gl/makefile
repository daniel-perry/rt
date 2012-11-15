# Daniel Perry
# makefile for my graphics library (gl) started in cs6620

## CHECK TO MAKE SURE THERE AREN'T ANY CIRCULAR DEPENDENCIES.
##

objects = image.o ray.o rgb.o Time.o vector3d.o LambertianMaterial.o PinholeCamera.o Scene.o Sphere.o Triangle.o Disk.o Ring.o AxisBox.o HeightField.o PhongMaterial.o MetalMaterial.o DielectricMaterial.o CoupledMaterial.o PhongVolumeMaterial.o JitteredSample.o UniformSample.o BoxFilter.o TentFilter.o FunctionEval.o GaussianFilter.o BBox.o BVH.o Colormap.o matrix.o TInstance.o #LanczosSincFilter.o

hfiles = Background.h HitRecord.h RenderContext.h Time.h Camera.h hpoint.h Object.h rgb.h image.h PinholeCamera.h Scene.h util.h ConstantBackground.h LambertianMaterial.h Plane.h vector3d.h DirectionalLight.h PointLight.h Sphere.h dynarray.h Light.h Primitive.h Group.h Material.h ray.h triangle.h Disk.h Ring.h AxisBox.h HeightField.h PhongMaterial.h MetalMaterial.h DielectricMaterial.h CoupledMaterial.h PhongVolumeMaterial.h Sample.h JitteredSample.h UniformSample.h Filter.h BoxFilter.h TentFilter.h FunctionEval.h GaussianFilter.h BBox.h BVH.h Colormap.h matrix.h TInstance.h #LanczosSincFilter.h 


CC = g++
#CFLAGS = -Wall -g -Wno-deprecated
CFLAGS = -Wall -O4 -Wno-deprecated



.cc.o:
	$(CC) -c $(CFLAGS) $<

#this pattern causes dependencies on .h's to take effect by changing the time stamp.
%.h:
	touch $@

############
# make all
############
.PHONY : all
all : $(objects) $(hfiles)

##########################
# new classes for cs6620
###########################.
#note also, that for classes with an object (.o) the .o file is listed in other file's dependency lists since the object file is dependent on the .h (both are not listed in the dependency list). 
#note also the classes immediately following the dependency list that are commented out, are classes that created a circular dependency problem when compiling, and so are not "#include"'ed in the files, but are pseudo-declared instead.
######
# changed direction of dependencies so that those relying on superclasses change when subclasses change isntead of normally where only subclasses change as a result of superclass changes -> because the former will happen more often I believe.
#######
# so now change trickles kind of looks like this:
# object <- primitive <- sphere  
#######
#also changed so that anything that RenderContext and HitRecord depend on change they are updated, instead of the other way around (the former case seems more often to take place)
# RenderContext <- Scene
# HitRecord <- Material or Primitive
# however this means that if RenderContext of HitRecord are changed there needs to be a clean build made.


# classes with just .h
util.h : 
Background.h : ray.o rgb.o ConstantBackground.h #RenderContext.h
HitRecord.h : util.h Material.h Primitive.h #
RenderContext.h : Scene.o
Camera.h : ray.o PinholeCamera.o #RenderContext.h
Object.h :  ray.o Group.h Primitive.h BBox.o BVH.o #HitRecord.h RenderContext.h
ConstantBackground.h : rgb.h ray.h #Background.h#RenderContext.h
Plane.h : vector3d.o util.h #Primitive.h
DirectionalLight.h : vector3d.o hpoint.h rgb.o #Light.h RenderContext.h
PointLight.h : #Light.h
Light.h : rgb.o vector3d.o hpoint.h DirectionalLight.h PointLight.h #RenderContext.h
Primitive.h : vector3d.o ray.o hpoint.h Plane.h Sphere.o Triangle.o Disk.o Ring.o AxisBox.o HeightField.o TInstance.o #Object.h #HitRecord.h RenderContext.h
Group.h : #Object.h
Material.h : rgb.o LambertianMaterial.o PhongMaterial.o MetalMaterial.o DielectricMaterial.o CoupledMaterial.o PhongVolumeMaterial.o #RenderContext.h #HitRecord.h
Sample.h: JitteredSample.o UniformSample.o
Filter.h: BoxFilter.o TentFilter.o GaussianFilter.o #LanczosSincFilter.o
#classes with .cc and .o file.
Time.h :
Time.o :
Sphere.h : LambertianMaterial.o util.h ray.o #Primitive.h
Sphere.o : Sphere.h
Triangle.h: LambertianMaterial.o util.h ray.o #Primitive.h
Triangle.o: Triangle.h
Disk.h: LambertianMaterial.o util.h ray.o
Disk.o: Disk.h
Ring.h:	LambertianMaterial.o util.h ray.o
Ring.o: Ring.h
AxisBox.h: LambertianMaterial.o util.h ray.o
AxisBox.o: AxisBox.h
HeightField.h: util.h ray.o Material.h
HeightField.o: HeightField.h
PinholeCamera.h : hpoint.h vector3d.o #RenderContext.h #Camera.h
PinholeCamera.o : PinholeCamera.h
LambertianMaterial.h : rgb.o #HitRecord.h RenderContext.h #Material.h
LambertianMaterial.o : LambertianMaterial.h
PhongMaterial.h: rgb.o
PhongMaterial.o: PhongMaterial.h
MetalMaterial.h: rgb.o
MetalMaterial.o: MetalMaterial.h
DielectricMaterial.h: rgb.o
DielectricMaterial.o: DielectricMaterial.h
CoupledMaterial.h: rgb.o
CoupledMaterial.o: CoupledMaterial.h
PhongVolumeMaterial.h: rgb.o Colormap.h
PhongVolumeMaterial.o: PhongVolumeMaterial.h

Colormap.h: 
Colormap.o: Colormap.h

JitteredSample.h: vector3d.o FunctionEval.h util.h
JitteredSample.o: JitteredSample.h
UniformSample.h: FunctionEval.h util.h
UniformSample.o: UniformSample.h
BoxFilter.h: 
BoxFilter.o: BoxFilter.h
TentFilter.h:
TentFilter.o: TentFilter.h
GaussianFilter.h: util.h
GaussianFilter.o: GaussianFilter.h
#LanczosSincFilter.h: util.h
#LanczosSincFilter.o: LanczosSincFilter.h

Scene.h : Camera.h Light.h Background.h rgb.o Object.h #
Scene.o : Scene.h

BBox.h: 
BBox.o: BBox.h
BVH.h:
BVH.o: BVH.h

matrix.h:
matrix.o: matrix.h
TInstance.h: matrix.o
TInstance.o: TInstance.h


#######################
# classes from cs3505 
#######################
# classes with just .h
dynarray.h : 
hpoint.h : vector3d.o util.h
#classes with .cc and .o file.
image.h : 
image.o : image.h util.h
ray.h : vector3d.o 
ray.o : ray.h
rgb.h : 
rgb.o : rgb.h
vector3d.h : 
vector3d.o : vector3d.h

#triangle.h : 
#triangle.o : 


#################
# clean
#################
.PHONY : clean
clean : 
	rm *.o
 

