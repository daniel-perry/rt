#ifndef RayTracer_H
#define RayTracer_H

#include "rt.h"

/** Encapsulates the ray tracer..
 */
class RayTracer
{
public:
  /** ctor,dtor
   */
  RayTracer(int width, int height);
  ~RayTracer();

  /** update size
   */
  void setSize(int width, int height);
  size_t getWidth(){ return m_scene->imageX; }
  size_t getHeight(){ return m_scene->imageY; }

  /** update camera
   */
  void setCamera(Camera * camera){ m_scene->setCamera(camera); }

  /** restart the rendering process
   */
  void restartRender();
  
  /** access to the render buffer
   */
  unsigned char * getBuffer(){ return m_buffer; }

private:
  /** render a sub region of the image
   */
  void render(size_t r0, size_t c0, size_t r1, size_t c1);
  /** convert color to unsigned char and set in buffer.
   */
  void set_color(size_t r, size_t c, const Color &clr);

  // holds the scene definition
  Scene * m_scene;
  RenderContext m_rc;
  // holds the rendered image
  unsigned char * m_buffer;
  // num of components for each pixel
  size_t m_numComponents;
};

#endif
