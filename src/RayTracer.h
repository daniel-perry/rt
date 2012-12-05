#ifndef RayTracer_H
#define RayTracer_H

class RayTracer
{
public:
  RayTracer(int width, int height);
  void setSize(int widht, int height);
  void setCamera();
  void restartTrace();
private:
  void trace();
};

#endif
