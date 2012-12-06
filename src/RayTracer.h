#ifndef RayTracer_H
#define RayTracer_H

// system
#include <pthread.h>

// local
#include "rt.h"

class RayTracer;
struct Region
{
  size_t r0;
  size_t c0;
  size_t r1;
  size_t c1;
  RayTracer * rt;
  bool done;
};

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
  
  /** attempt join threads, confirm completion.
   */ 
  bool checkProgress();

  /** render a sub region of the image
   */
  void render(size_t r0, size_t c0, size_t r1, size_t c1);
 
  /** access to the render buffer
   */
  unsigned char * getBuffer(){ return m_buffer; }

  /** access to scene
   */
  Scene * getScene(){ return m_scene; }
  const Scene * getScene() const { return m_scene; }

  void setDoneRenderingCB( void (*doneRendering)() ){ m_doneRendering = doneRendering; }

  enum State{ 
    DONE,
    RENDER 
    };
  State getState(){ return m_state; }

private:
  /** convert color to unsigned char and set in buffer.
   */
  void set_color(size_t r, size_t c, const Color &clr);

  // holds the scene definition
  Scene * m_scene;
  RenderContext m_rc;
  // holds the rendered image
  typedef unsigned char buffer_type;
  buffer_type * m_buffer;
  // num of components for each pixel
  size_t m_numComponents;
  // state 
  State m_state;
  // callback
  void (*m_doneRendering)(); 

  // state used for threading:
  size_t m_numThreads;
  size_t m_last_numThreads;
  pthread_t * m_threads;
  Region * m_regions;
};

#endif
