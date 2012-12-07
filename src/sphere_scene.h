#ifndef SCENE_DEF_H
#define SCENE_DEF_H

Scene* make_scene()
{
  Scene* scene = new Scene();
  
  scene->imageX = 512;
  scene->imageY = 512;
  
  scene->setBackground(new ConstantBackground(Color(0.5, 0.8, 0.9)));
  Group* world = new Group();
  
  
  //Material* ball_m = new PhongMaterial( Color(.9, .6, .3), 1 , 0 , Color(1,1,1), 30);
  Material* ball_m = new LambertianMaterial( Color(.9, .6, .3), 1 , 0 );

  Primitive * ball1 = new Box( ball_m , Point(-1,-1,-1), Point(1,1,1) );

  world->addObject( ball1 );


  //Material* red_m = new PhongMaterial( Color(.9, .3, .3), 1 , 0 , Color(1,1,1), 30);
  Material* red_m = new LambertianMaterial( Color(.9, .3, .3), .6 , .4 );
  world->addObject( new Sphere( red_m , Point(2,2,2), .5) );
  //Material* green_m = new PhongMaterial( Color(.3, .9, .3), 1 , 0 , Color(1,1,1), 30);
  Material* green_m = new LambertianMaterial( Color(.3, .9, .3), .6 , .4 );
  world->addObject( new Sphere( green_m , Point(-2,2,2), .5) );
  //Material* blue_m = new PhongMaterial( Color(.3, .3, .9), 1 , 0 , Color(1,1,1), 30);
  Material* blue_m = new LambertianMaterial( Color(.3, .3, .9), .6 , .4 );
  world->addObject( new Sphere( blue_m , Point(-2,-2,2), .5) );
  //Material* mix1_m = new PhongMaterial( Color(.9, .3, .9), 1 , 0 , Color(1,1,1), 30);
  Material* mix1_m = new LambertianMaterial( Color(.9, .3, .9), .6 , .4 );
  world->addObject( new Sphere( mix1_m , Point(2,-2,2), .5) );


  scene->setObject(world);
  scene->setAmbient(Color(.4, .4, .4));

  scene->addLight(new PointLight(Point(0,10,10), Color(.9,.9,.9)));
  scene->addLight(new PointLight(Point(0,10,-10), Color(.9,.9,.9)));
  scene->addLight(new PointLight(Point(20,0,10), Color(.5,.5,.5)));

  scene->setCamera(new PinholeCamera(Point(5,5,0), Point(0,0,0), Vector(0,0,1), 55 , 1));

  scene->setMaxRayDepth( 10 );
  scene->setMinAttenuation( .01 );


  //scene->setSampler( new UniformSample( 1 , scene));
  scene->setSampler( new JitteredSample( 9 , scene));
  scene->setFilter( new BoxFilter() );
  //scene->setFilter( new TentFilter() );
  //scene->setFilter( new GaussianFilter(.2) );
  //scene->setFilter( new LanczosSincFilter() );

  return scene;
}

#endif
