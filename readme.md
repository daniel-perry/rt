Ray Tracer written for CS6620 at University of Utah in 2005

Updates:

December 2012 added (for CS6630)
- fixed volume rendering
- added non-photorealistic volume rendering
- OpenGL/GLUT based viewer with threading

Features
- volume rendering
- non-photorealistic volume rendering (gooch shading and curvature based contouring)
- reflection/refraction model
- instancing
- phong/lambertian/metallic/dielectic materials
- multi-threaded (pthreads)
- OpenGL/GLUT based viewer

Dependencies:
- ITK 4.2 (for reading and preprocessing of volumes)
- pthreads
- OpenGL
- GLUT

to build:
- mkdir bin
- cd bin
- cmake ../src
- make

