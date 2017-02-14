#include <fstream>

void add_triangles_from_point_file(const std::string & point_fn, Material * mat, Group * group){
	if(point_fn.substr(point_fn.size()-3) == "raw"){
		// read in the float block:
		std::ifstream ifp(point_fn.c_str(), std::ios::in | std::ios::binary);
		int numfloats = 0;
		ifp.read(reinterpret_cast<char*>(&numfloats), sizeof(int)*1);
		vector<float> block(numfloats);
		ifp.read(reinterpret_cast<char*>(block.data()), sizeof(float)*numfloats);
		ifp.close();

		// now convert the float block into the data structure of triangles:
		int numtriangles = numfloats / 9;
		std::cout << numfloats << " floats read => " << numtriangles << " triangles ..." << std::endl;
		int bi = 0;
		for(int i=0; i<numtriangles; ++i){
			Point p0(block[bi],block[bi+1],block[bi+2]);
			bi += 3;
			Point p1(block[bi],block[bi+1],block[bi+2]);
			bi += 3;
			Point p2(block[bi],block[bi+1],block[bi+2]);
			bi += 3;
			group->addObject(new Triangle(mat, p0,p1,p2));
		}
		std::cout << "done adding triangles." << std::endl;
		std::cout << "group has " << group->objects.size() << " objects." << std::endl;
	} else {
		std::ifstream f(point_fn.c_str());
		vector<Point> points;
		while(!f.eof()){
			Point p;
			for(int i=0; i<3; ++i){
				std::string in;
				f >> in;
				p[i] = atof(in.c_str());
			}
			points.push_back(p);
		}
		std::cout << points.size() << " points read in." << std::endl;
		std::cout.flush();
		vector<bool> used(points.size(), false);
		int num_triangles = 0;
		for(int i=0; i<points.size(); ++i){
			// find the closest two points:
			std::pair<int,int> closest(-1,-1);
			std::pair<float,float> dists(1e7,1e7);
			for(int j=0; j<points.size(); ++j){
				if(i == j) continue;
				float diff = (points[i] - points[j]).length();
				if(diff < dists.first){
					dists.second = dists.first;
					closest.second = closest.first;
					dists.first = diff;
					closest.first = j;
				} else if (diff < dists.second){
					dists.second = diff;
					closest.second = j;
				}
			}
			// now form them into triangles and add to group:
			if(!(used[i] && used[closest.first] && used[closest.second])){
				group->addObject(new Triangle(mat, points[i], points[closest.first], points[closest.second]));
				used[i] = true;
				used[closest.first] = true;
				used[closest.second] = true;
				++num_triangles;
			}
			if (num_triangles%1000 == 0) std::cout << num_triangles << "/" << points.size() << std::endl;
		}
		std::cout << num_triangles << " triangles added." << std::endl;

		// dump out geometry as a float block
		std::vector<float> block(group->objects.size()*3*3); // 3*3=9 floats per triangle (3 points, 3 floats each)
		int bi = 0;
		for(int i=0; i<group->objects.size(); ++i){
			Triangle* tri = reinterpret_cast<Triangle*>( group->objects[i] );
			if(tri){
				Point & p0 = tri->p0;
				for(int j=0; j<3; ++j){
					block[bi++] = p0[j];
				}
				Point & p1 = tri->p1;
				for(int j=0; j<3; ++j){
					block[bi++] = p1[j];
				}
				Point & p2 = tri->p2;
				for(int j=0; j<3; ++j){
					block[bi++] = p2[j];
				}
			}
		}
		std::stringstream fn;
		fn << point_fn << "_triangles.raw";
		std::ofstream out(fn.str().c_str(), std::ios::out | std::ios::binary);
		int numfloats = block.size();
		out.write( reinterpret_cast<const char*>(&numfloats), 1*sizeof(int) );
		out.write( reinterpret_cast<const char*>(block.data()), block.size() * sizeof(float) );
		out.close();
	}
}

Scene* make_scene(const std::string & point_fn, const std::string & value_fn, const std::string & cmap_fn )
{
  Scene* scene = new Scene();

  scene->imageX = 512;
  scene->imageY = 512;

  scene->setBackground(new ConstantBackground(Color(255./255., 255./255., 255./255.)));
  //scene->setBackground(new ConstantBackground(Color(112/255., 204/255., 255./255.)));
  //scene->setBackground(new ConstantBackground(Color(0./255., 0./255., 0./255.)));
  Group* world = new Group();

  //Material * phong = new PhongMaterial( Color(.9,.6,.3) , .6 , .4 , Color(1,1,1), 30 );
	Material* phong = new PhongMaterial( Color(.9, .6, .6), .6 , .4, Color(1,1,1),30 );
	//Material* phong = new LambertianMaterial( Color(.6, .5, .7), .6 , .4 );
	//Material * phong = new MetalMaterial( Color(.9,.9,.9) , 100 );

	add_triangles_from_point_file(point_fn, phong, world);

	std::cout << "world size: " << world->objects.size() << std::endl;
  
  //world->addObject(new Box(phong, Point(0,0,0), Point(.75,1,1)));

	// add in the BVH accel structure:
  BVH * bvh = new BVH();
  bvh->buildTree( *world , 0 , 0);
  scene->setObject(bvh);

  //scene->setObject(world);

  scene->setAmbient(Color(.4, .4, .4));
  scene->addLight(new PointLight(Point(100, -80, 100), Color(1,1,1)));

  scene->setCamera(new PinholeCamera(Point(-10, -10, -8.0),
                                     Point(0.5,0, 0.8),
                                     Vector(0, 0, 1),
                                     10,1));
  scene->setMaxRayDepth(10);
  scene->setMinAttenuation(.01);
  scene->setSampler( new UniformSample( 1 , scene));
  //scene->setSampler( new JitteredSample( 9 , scene));
  scene->setFilter( new BoxFilter() );

  return scene;
}
