
//static int assignment = 8;

Scene* make_scene()
{
  Scene* scene = new Scene();

  scene->imageX = 512;
  scene->imageY = 512;

  scene->setBackground(new ConstantBackground(Color(112/255., 204/255., 255./255.)));
  Group* world = new Group();

  //Material* box_m = new PhongMaterial( Color(.9, .1, .3), 1 , 0 , Color(1,1,1), 30);
  //Primitive * box1 = new Sphere( box_m , Point(0,0,0), .3 );
  //world->addObject(box1);
 
  Material* volume = new PhongVolumeMaterial( 
   "../data/CThead.hdr", // data hdr name
   "../data/CThead.cmap", // color map name
   Point(0,0,0), // lower
   Point(.75,1,1), // upper
   //Point(1,0,1), // lower
   //Point(0,1,0), // upper
   0.5,  // grid stepsize
   0.999, // max opacity
   0.8,           // Kd
   0.2,           // Ka
   Color(1,1,1),  // Phong color
   100            // phong exponent
   ); 

  //Material * volume = new PhongMaterial( Color(.5,.6,.3) , .6 , .4 , Color(1,1,1), 30 );
  
  world->addObject(new Box(volume, Point(0,0,0), Point(.75,1,1)));
  //world->addObject(new Box(volume, Point(1,0,1), Point(0,1,0)));

  /*
  Material * ground = new PhongMaterial( Color(.5,.6,.3) , .6 , .4 , Color(1,1,1), 30 );

  world->addObject(new Triangle(ground,
                                Point(-20,-20,-.1), Point(20,-20,-.1), Point(20,20,-.1)));
  world->addObject(new Triangle(ground,
                                Point(-20,-20,-.1), Point(20,20,-.1), Point(-20,20,-.1)));
  */

  scene->setObject(world);

  scene->setAmbient(Color(.4, .4, .4));
  scene->addLight(new PointLight(Point(100, -80, 100), Color(1,1,1)));

  //scene->setCamera(new PinholeCamera(Point(3, -8, 4.0),
  //                                   Point(0.5,0, 0.8),
  //                                   Vector(0, 0, 1),
  //                                   10,1));
 
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
