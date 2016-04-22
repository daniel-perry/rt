

Scene* make_scene(const std::string & nrrd_fn, const std::string & cmap_fn, float curvThick, bool flipNormal )
{
  Scene* scene = new Scene();

  scene->imageX = 512;
  scene->imageY = 512;

  scene->setBackground(new ConstantBackground(Color(112/255., 204/255., 255./255.)));
  //scene->setBackground(new ConstantBackground(Color(0./255., 0./255., 0./255.)));
  Group* world = new Group();

  // maximum intensity based volume material
  Material* volume = new CurvatureVolumeMaterial( 
   nrrd_fn, // data hdr name
   cmap_fn, // color map name
   Point(0,0,0), // lower
   Point(.75,1,1), // upper
   0.5,  // grid stepsize
   0.999, // max opacity
   curvThick, // curv thickness
   flipNormal, // flip normal
   true // nearest neighbor
   ); 

  
  world->addObject(new Box(volume, Point(0,0,0), Point(.75,1,1)));

  scene->setObject(world);

  scene->setAmbient(Color(.4, .4, .4));
  scene->addLight(new PointLight(Point(100, -80, 100), Color(1,1,1)));

  scene->setCamera(new PinholeCamera(Point(-10, -10, -8.0),
                                     Point(0.5,0, 0.8),
                                     Vector(0, 0, 1),
                                     10,1));
  scene->setMaxRayDepth(10);
  scene->setMinAttenuation(.01);
  //scene->setSampler( new UniformSample( 1 , scene));
  scene->setSampler( new JitteredSample( 9 , scene));
  scene->setFilter( new BoxFilter() );

  return scene;
}

Scene* make_scene(const std::string & nrrd_fn, float isoval)
{
  Scene* scene = new Scene();

  scene->imageX = 512;
  scene->imageY = 512;

  scene->setBackground(new ConstantBackground(Color(255/255., 255/255., 255./255.)));
  //scene->setBackground(new ConstantBackground(Color(112/255., 204/255., 255./255.)));
  //scene->setBackground(new ConstantBackground(Color(0./255., 0./255., 0./255.)));
  Group* world = new Group();

  // maximum intensity based volume material
  Material* volume = new CurvatureVolumeMaterial( 
   nrrd_fn, // data hdr name
   cmap_fn, // color map name
   Point(0,0,0), // lower
   Point(.75,1,1), // upper
   0.5,  // grid stepsize
   0.999, // max opacity
   curvThick, // curv thickness
   flipNormal, // flip normal
   true // nearest neighbor
   ); 

  
  world->addObject(new Box(volume, Point(0,0,0), Point(.75,1,1)));

  scene->setObject(world);

  scene->setAmbient(Color(.4, .4, .4));
  scene->addLight(new PointLight(Point(100, -80, 100), Color(1,1,1)));

  scene->setCamera(new PinholeCamera(Point(-10, -10, -8.0),
                                     Point(0.5,0, 0.8),
                                     Vector(0, 0, 1),
                                     10,1));
  scene->setMaxRayDepth(10);
  scene->setMinAttenuation(.01);
  //scene->setSampler( new UniformSample( 1 , scene));
  scene->setSampler( new JitteredSample( 9 , scene));
  scene->setFilter( new BoxFilter() );

  return scene;
}
