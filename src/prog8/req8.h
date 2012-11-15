
//static int assignment = 8;

Scene* make_scene()
{
  Scene* scene = new Scene();

  scene->imageX = 512;
  scene->imageY = 512;

  scene->setBackground(new ConstantBackground(Color(112/255., 204/255., 255./255.)));
  Group* world = new Group();

  Material* volume = new PhongVolumeMaterial( "/home/dperry/cs6620/rt/src/prog8/CThead.hdr",
					      "/home/dperry/cs6620/rt/src/prog8/CThead.cmap",
                                             Point(1,0,1), Point(0,1,0), 0.5, 0.999,
                                             0.8, 0.2, Color(1,1,1), 100);
  world->addObject(new Box(volume, Point(0,0,0), Point(.75,1,1)));

  /* Material* ground = new PhongMarbleMaterial(Color(0.0, 0.1, 0.3), // Color 1
                                             Color(0.7, 0.8, 1.0), // Color 2
                                             0.5, // scale
                                             8, // octaves
                                             20, // turbulence scale
                                             1, // frequency scale
                                             2, // lacunarity
                                             0.6, // gain
                                             0.6, 0.4, Color(1,1,1), 120);
  */

  Material * ground = new PhongMaterial( Color(.5,.6,.3) , .6 , .4 , Color(1,1,1), 30 );

  world->addObject(new Triangle(ground,
                                Point(-20,-20,-.1), Point(20,-20,-.1), Point(20,20,-.1)));
  world->addObject(new Triangle(ground,
                                Point(-20,-20,-.1), Point(20,20,-.1), Point(-20,20,-.1)));

  world->addObject(new Sphere(new MetalMaterial(Color(0.8, 0.8, 0.8), 100),
                              Point(1.3, 1.3, 0.5), 0.35));
  scene->setObject(world);

  scene->setAmbient(Color(.4, .4, .4));
  scene->addLight(new PointLight(Point(100, -80, 100), Color(1,1,1)));

  scene->setCamera(new PinholeCamera(Point(3, -8, 4.0),
                                     Point(1,0, 0.8),
                                     Vector(0, 0, 1),
                                     10,1));
  scene->setMaxRayDepth(10);
  scene->setMinAttenuation(.01);
  // scene->setSampling(Scene::Jittered, 5);
  scene->setSampler( new UniformSample( 1 , scene));
  scene->setFilter( new BoxFilter() );

  return scene;
}
