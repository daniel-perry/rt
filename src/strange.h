Scene* make_scene()
{
  Scene* scene = new Scene();
  scene->setBackground(new ConstantBackground(Color(0.5, 0.8, 0.9)));
  Group* world = new Group();
  
  
  Material* ball_m = new LambertianMaterial( Color(.5, .6, .3), .6 , .4 );
  world->addObject( new Sphere( ball_m , Point(5.5,1,0), .5) );
  
  Material* ball3_m = new LambertianMaterial( Color(.6, .5, .7), .6 , .4 );
  world->addObject( new Plane( ball3_m , Vector(0,0,1), Point(0,0,-2)) );
  
  
  Material* dielec_m = new DielectricMaterial(1.05, 100);
  world->addObject( new Sphere( ball3_m , Point(5.5,-1,0), .5) );
  
  Material * phong_m = new PhongMaterial( Color(.5,.6,.3) , .6 , .4 , Color(1,1,1), 30 );

  world -> addObject( new Box( dielec_m , Point(3, 0 , 0) , Point(4,1,1) ) ); 
  

  /*
  Material* hfmatl = new PhongMaterial(Color(0.5, 0.6, .3), 0.6, 0.4, Color(1,1,1), 30);
  world->addObject(new Heightfield(hfmatl,
                                   "sinc_200_200.hf",
                                   Point(-4.5, 2.0, 2), Point(-1.5, 5.0 , 4)));
  */    
  scene->setObject(world);
  scene->setAmbient(Color(.4, .4, .4));

  scene->addLight(new PointLight(Point(0,2,10), Color(.9,.9,.9)));
  scene->addLight(new PointLight(Point(0,-2,10), Color(.9,.9,.9)));

  scene->setCamera(new PinholeCamera(Point(0,.5,.5), Point(3.5,.5,.5), Vector(0,0,1), 19.5 , 1));

  scene->setMaxRayDepth( 10 );
  scene->setMinAttenuation( .01 );

  return scene;
}
