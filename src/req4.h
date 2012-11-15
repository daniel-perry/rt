
//static int assignment = 4;

Scene* make_scene()
{
  Scene* scene = new Scene();

  scene->imageX = 512;
  scene->imageY = 512;

  scene->setBackground(new ConstantBackground(Color(0.5, 0.8, 0.9)));
  Group* world = new Group();
  Material* groundmatl = new LambertianMaterial(Color(0.95, 0.65, 0.35), .6, .4);
  world->addObject(new Plane(groundmatl,
                             Vector(0,0,1), Point(0,0,2.5)));

  
  for(int i=0;i<4;i++){
    double eta = 1 + i*0.5 + .05;
    Material* transp_matl = new DielectricMaterial(eta, 100);
    Point corner(i*1.3 - 4, -3, 2.5+1.e-4);
    Vector size(0.20, 2.5, 1.4);
    world->addObject(new Box(transp_matl,
                             corner, corner+size));
  }
  
  Material* ringmatl = new LambertianMaterial(Color(.6, .6, .9), .6, .4);
  for(int i=0;i<9;i++){
    double r = .30;
    Point center(-6, 0, 2.5+r);
    Vector offset(2.25*r, 0, 0);
    world->addObject(new Ring(ringmatl,
                              center+offset*i, Vector(0.2, -1, -0.2), r*0.5, r));
  }
  Material* ball_matl = new MetalMaterial(Color(0.8, 0.8, 0.8), 100);
  world->addObject(new Sphere(ball_matl, Point(-6, 3.5, 3.5), 1.0));

  Material* hfmatl = new PhongMaterial(Color(0.5, 0.6, .3), 0.6, 0.4, Color(1,1,1), 30);
  world->addObject(new Heightfield(hfmatl,
                                   "/home/dperry/school/cs6620/rt/src/sinc_200_200.hf",
                                   Point(-4.5, 2.0, 2), Point(-1.5, 5.0 , 4)));
  scene->setObject(world);

  scene->setAmbient(Color(.4, .4, .4));
  scene->addLight(new PointLight(Point(20, 30, 100), Color(.9,.9,.9)));
  scene->addLight(new PointLight(Point(-40, -30, 50), Color(.3,.1,.1)));
  
  scene->setCamera(new PinholeCamera(Point(8, -18, 8.5),
                                     Point(-4.7, 2.5, 2.5),
                                     Vector(0, 0, 1),
                                     15,1));
  /*
  scene->setCamera(new PinholeCamera(Point(8, -18, 8.5),
                                     Point(-4.7, 1.6, 2.5),
                                     Vector(0, 0, 1),
                                     1,1));
  */
  scene->setMaxRayDepth(25 );
  scene->setMinAttenuation(.01);


  
  //scene->setSampler( new UniformSample( 1 , scene));
  scene->setSampler( new JitteredSample( 9 , scene));
  //scene->setFilter( new BoxFilter() );
  scene->setFilter( new TentFilter() );
  //scene->setFilter( new GaussianFilter(.2) );

  return scene;
}
