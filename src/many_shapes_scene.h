
static int assignment = 3;

Scene* make_scene()
{
  Scene* scene = new Scene();
  scene->setBackground(new ConstantBackground(Color(.1, .2, .3)));
  Group* world = new Group();
  Material* boxmatl = new LambertianMaterial(Color(.8, .4, .4), .6, .4);
  world->addObject(new Box(boxmatl,
                           Point(-4, -4, -0), Point(4,4,2)));
  Material* ringmatl = new LambertianMaterial(Color(.6, .6, .9), .6, .4);
  world->addObject(new Ring(ringmatl,
                            Point(-2, 0, 4), Vector(3, -1, 2), 1.2, 1.8));
  Material* trimatl = new LambertianMaterial(Color(.6, .9, .6), .6, .4);
  world->addObject(new Triangle(trimatl,
                                Point(1,1,2), Point(3,1,3), Point(2, 2, 6)));
  Material* spherematl = new LambertianMaterial(Color(.3, .3, .3), .8, .2);
  world->addObject(new Sphere(spherematl,
                              Point(-3.3, -3.3, 2.5), .5));
  Material* hfmatl = new LambertianMaterial(Color(1, 1, 0), .8, .2);
  world->addObject(new Heightfield(hfmatl,
                                   "../data/sinc_200_200.hf",
                                   Point(0, -3, 2), Point(3, 0, 4)));
  scene->setObject(world);

  scene->setAmbient(Color(.4, .4, .4));
  scene->addLight(new PointLight(Point(20, 30, 100), Color(.9,.9,.9)));
  scene->addLight(new PointLight(Point(-40, -30, 50), Color(.3,.1,.1)));

  scene->setCamera(new PinholeCamera(Point(6, -18, 8.5),
                                     Point(-.5,0, 2.5),
                                     Vector(0, 0, 1),
                                     30, 1));
  scene->setMaxRayDepth(10);
  scene->setMinAttenuation(.01);
  scene->setSampler( new UniformSample( 1 , scene));
  //scene->setSampler( new JitteredSample( 9 , scene));
  scene->setFilter( new BoxFilter() );


  return scene;
}
