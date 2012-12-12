// viewer - shows the current results of the ray tracer..

#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#ifdef _WIN32
# include <windows.h>
#else
# include <sys/time.h>
#endif

#include "RayTracer.h"

#define VIEWING_DISTANCE_MIN  0.1
#define TEXTURE_ID_SQUARE 1
enum {
  MENU_LIGHTING = 1,
  MENU_POLYMODE,
  MENU_TEXTURING,
  MENU_EXIT
};
typedef int BOOL;
#define TRUE 1
#define FALSE 0
static BOOL g_bButton1Down = FALSE;
static BOOL g_bButton2Down = FALSE;
static int g_Width = 600;                          // Initial window width
static int g_Height = 600;                         // Initial window height
static int g_xClick = 0;
static int g_yClick = 0;
static int g_zClick = 0;
#ifdef _WIN32
static DWORD last_idle_time;
#else
static struct timeval last_idle_time;
#endif

RayTracer * g_rayTracer = 0;
int g_patienceValue = 0;
// in space of ray tracer
vector3d g_eye(-5,-5,-5);
vector3d g_lookat(0,0,0);
vector3d g_nup(0,0,1);
double g_theta = 55;
double g_aspectRatio=1;
std::string g_nrrd_fn;
std::string g_cmap_fn;
float g_curvThick; 
bool g_normFlipped;
// auto mode:
bool g_automode = false;

void DrawSquare(float x, float y) {
  glBegin(GL_QUADS);
  glVertex2f(0, 0);    glTexCoord2f (0, 0);
  glVertex2f(0, y);     glTexCoord2f (0, 1);
  glVertex2f(x, y);      glTexCoord2f (1, 1);
  glVertex2f(x, 0);     glTexCoord2f (1, 0);
  glEnd();
}
void RenderObjects(void)
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  // draw single quad with the texture overlaid
  glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_SQUARE);
  DrawSquare(g_Width,g_Height);
}
void display(void)
{
   // Clear frame buffer and depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // Set up viewing transformation - identity
   glLoadIdentity();
   // make sure lighting disabled
   glDisable(GL_LIGHTING);
   glEnable(GL_TEXTURE_2D);
   // Render the scene
   RenderObjects();
   // Make sure changes appear onscreen
   glutSwapBuffers();
}

void showResult()
{
  glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_SQUARE);
  gluBuild2DMipmaps(GL_TEXTURE_2D,            // texture to specify
                    GL_RGBA,                  // internal texture storage format
                    g_rayTracer->getWidth(),  // texture width
                    g_rayTracer->getHeight(), // texture height
                    GL_RGBA,                  // pixel format
                    GL_UNSIGNED_BYTE,         // color component format
                    g_rayTracer->getBuffer());// pointer to texture image
  glutPostRedisplay();
}

void showProgress(int value)
{
  // TODO: may be able to do this automatically by removing swap buffer?
  //std::cerr << "showProgres()" << std::endl;
  showResult();
  if(g_rayTracer->getState() == RayTracer::RENDER)
  {
    glutTimerFunc(500, showProgress, 0); // call again, until state changes..
  }
}

// to prevent multiple unnecessary renderings:
void patience(int value)
{
  if( value == g_patienceValue ) // if this is the last call to render..
  {
    g_rayTracer->restartRender();
    g_patienceValue = 0;

    glutTimerFunc(500, showProgress, 0);
  }
}

// what to do when ray tracer is done rendering...
void doneRendering()
{
  std::cerr << "done rendering." << std::endl;
  showResult();
}
void reshape(GLint width, GLint height)
{
   g_Width = width;
   g_Height = height;
   glViewport(0, 0, g_Width, g_Height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.f, g_Width, g_Height, 0.f, 0.f, 1.f);
   glMatrixMode(GL_MODELVIEW);
  
   //if( g_rayTracer->getWidth() != g_Width || g_rayTracer->getHeight() != g_Height )
   //{
   //  g_rayTracer->setSize(g_Width,g_Height);
   //  glutTimerFunc(100, patience, ++g_patienceValue);
   //}
}
unsigned char *  makeTexture(int width, int height)
{
   if( !g_rayTracer )
   {
     g_rayTracer = new RayTracer(width,height);
     g_rayTracer->setDoneRenderingCB( & doneRendering );
     g_rayTracer->makeScene( g_nrrd_fn, g_cmap_fn, g_curvThick, g_normFlipped );
   }
   if( !(g_rayTracer->getWidth() == width && g_rayTracer->getHeight() == height) )
   {
     g_rayTracer->setSize(width,height); 
   }

   g_rayTracer->restartRender();
   return g_rayTracer->getBuffer();
#if 0
   unsigned char * pCircle = new unsigned char[ width * height * 4 ];
   // make a texture image...
   for(size_t r=0; r<width; ++r)
   {
     for(size_t c=0; c<height; ++c)
     {
       size_t x_pos = 100;
       size_t y_pos = 100;
       size_t radius = 30;
       unsigned char red = static_cast<unsigned char>(0);
       unsigned char green = static_cast<unsigned char>(0);
       unsigned char blue = static_cast<unsigned char>(0);
       unsigned char alpha = static_cast<unsigned char>(255);
       if( (r-x_pos)*(r-x_pos) + (c-y_pos)*(c-y_pos) <= radius*radius )
       {
         red = (unsigned char) 255;
       }
       else
       {
         green = (unsigned char) 255;
       }
       pCircle[r*height*4+c*4+0] = red;
       pCircle[r*height*4+c*4+1] = green;
       pCircle[r*height*4+c*4+2] = blue;
       pCircle[r*height*4+c*4+3] = alpha;
     }
   }
   return pCircle;
#endif
}
void InitGraphics(void)
{
   int width, height;
   int nComponents;
   void* pTextureImage;
   width = g_Width;
   height = g_Height;
   pTextureImage = makeTexture(width, height);
   glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_SQUARE);

   int tWidth = 1;
   int tHeight = 1;
   while (tWidth < width)
     tWidth <<= 1;
   while (tHeight < width)
     tHeight <<= 1;


/*
   glTexImage2D(GL_TEXTURE_2D,     // texture to specify
                0,                 // texture level
                GL_RGBA,           // internal texture storage format
                tWidth,             // texture width
                tHeight,            // texture height
                0,                 // texture border
                GL_RGBA,           // pixel format
                GL_UNSIGNED_BYTE,  // color component format
                0);                // pointer to texture image

   glTexSubImage2D(GL_TEXTURE_2D, 
                   0, 
                   0, 
                   0, 
                   width, 
                   height, 
                   GL_RGBA, 
                   GL_UNSIGNED_BYTE, 
                   pTextureImage);
*/

   gluBuild2DMipmaps(GL_TEXTURE_2D,     // texture to specify
                     GL_RGBA,           // internal texture storage format
                     width,             // texture width
                     height,            // texture height
                     GL_RGBA,           // pixel format
                     GL_UNSIGNED_BYTE,  // color component format
                     pTextureImage);    // pointer to texture image
}

void updateCameraDistance(float change_pct)
{
  vector3d direction = g_eye-g_lookat;
  ray to_eye( g_lookat, direction );
  g_eye = to_eye.eval(change_pct);
}

void rotateCamera(float pct_x, float pct_y)
{
  vector3d direction = g_eye-g_lookat;

  // "left/right" rotation:
  hpoint dir(direction,1);
  dir = makeRotateMatrix( pct_x * M_PI/4, g_nup.x(), g_nup.y(), g_nup.z() ) * dir;

  // "up/down" rotation:
  //hpoint nup(g_nup,1);
  //vector3d left = cross( direction, g_nup );
  //left.MakeUnitVector();
  //std::cerr << "left: " << left << std::endl;
  //matrix rotate = makeRotateMatrix( pct_y * M_PI/4 , left.x(), left.y(), left.z() );


  //dir = rotate * dir;
  //nup = rotate * nup;

  direction = dir.tovector3d();

  // hack - use translation to fake movement in z..
  float dist = direction.length();
  direction += g_nup * dist * pct_y;

  //g_nup = nup.tovector3d();
  //g_nup.MakeUnitVector(); // make sure it's still unit length

  ray to_eye( g_lookat, direction );
  g_eye = to_eye.eval(1.f);
}

void syncCamera()
{
  PinholeCamera * ph = dynamic_cast<PinholeCamera*>(g_rayTracer->getScene()->camera);
  if(ph)
  {
    std::cerr << "new eye:" << g_eye << std::endl;
    ph->initialize(g_eye,g_lookat,g_nup,g_theta,g_aspectRatio);
    glutTimerFunc(500, patience, ++g_patienceValue);
  }
  // else - do nothing
}

void updateAutomatic(int value)
{
  if( g_automode )
  {
    if(g_rayTracer->getState() == RayTracer::DONE)
    {
      rotateCamera( 0.5, 0 );
      syncCamera();
    }

    // try again in 4 seconds..
    glutTimerFunc(4000, updateAutomatic, 0);
  }
}

void runAutomatic()
{
  if(g_automode)
  {
    // position initial camera position:
    g_eye = Point(-2, -2, -0.5);
    g_lookat = Point(0.5,0, 0.8);
    g_nup = Vector(0, 0, 1);
    syncCamera();
    
    // start regular intervals of camera rotation:
    glutTimerFunc(4000, updateAutomatic, 0);
  }
}

void MouseButton(int button, int state, int x, int y)
{
  // Respond to mouse button presses.
  // If button1 pressed, mark this state so we know in motion function.
  if (button == GLUT_LEFT_BUTTON)
    {
      g_bButton1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
      g_xClick = x; 
      g_yClick = y; 
    }
  else if (button == GLUT_RIGHT_BUTTON)
    {
      g_bButton2Down = (state == GLUT_DOWN) ? TRUE : FALSE;
      g_zClick = y; 
    }

}
void MouseMotion(int x, int y)
{
  // If button1 pressed, zoom in/out if mouse is moved up/down.
  if (g_bButton1Down)
  {
    float change_x = -(x - g_xClick) / 3.0;
    float pct_x = change_x/g_rayTracer->getWidth();
    float change_y = (y - g_yClick) / 3.0;
    float pct_y = change_y/g_rayTracer->getHeight();
    rotateCamera(pct_x,pct_y);
    syncCamera();
    //glutPostRedisplay();
  }
  else if (g_bButton2Down)
  {
    float distance_change = (y - g_zClick);
    float pct = 1-distance_change/g_rayTracer->getHeight();
    updateCameraDistance(pct);
    syncCamera();
    //glutPostRedisplay();
  }
}

void SelectFromMenu(int idCommand)
{
  switch (idCommand)
    {
    //case MENU_LIGHTING:
    //  g_bLightingEnabled = !g_bLightingEnabled;
    //  if (g_bLightingEnabled)
    //     glEnable(GL_LIGHTING);
    //  else
    //     glDisable(GL_LIGHTING);
    //  break;
    //case MENU_POLYMODE:
    //  g_bFillPolygons = !g_bFillPolygons;
    //  glPolygonMode (GL_FRONT_AND_BACK, g_bFillPolygons ? GL_FILL : GL_LINE);
    //  break;      
    //case MENU_TEXTURING:
    //  g_bTexture = !g_bTexture;
    //  if (g_bTexture)
    //     glEnable(GL_TEXTURE_2D);
    //  else
    //     glDisable(GL_TEXTURE_2D);
    //  break;    
    case MENU_EXIT:
      exit (0);
      break;
    }
  // Almost any menu selection requires a redraw
  glutPostRedisplay();
}
void Keyboard(unsigned char key, int x, int y)
{
  static const float STEP_SIZE = 1.0;
  vector3d right = cross(g_nup,(g_lookat-g_eye).MakeUnitVector());
  switch (key)
  {
  case 27:             // ESCAPE key
    exit (0);
    break;
  //case 'l':
  //  SelectFromMenu(MENU_LIGHTING);
  //  break;
  case 'p':
    SelectFromMenu(MENU_POLYMODE);
    break;
  case 't':
    SelectFromMenu(MENU_TEXTURING);
  case 'j': // down
    g_eye -= (g_nup * STEP_SIZE);
    g_lookat -= (g_nup * STEP_SIZE);
    syncCamera();
    break;
  case 'k': // up
    g_eye += (g_nup * STEP_SIZE);
    g_lookat += (g_nup * STEP_SIZE);
    syncCamera();
    break;
  case 'l': // right
    g_eye += (right * STEP_SIZE);
    g_lookat += (right * STEP_SIZE);
    syncCamera();
    break;
  case 'h': // left
    g_eye -= (right * STEP_SIZE);
    g_lookat -= (right * STEP_SIZE);
    syncCamera();
    break;
  case 'A': // automatic mode
    g_automode = !g_automode;
    runAutomatic();
    break;
  }
}
int BuildPopupMenu (void)
{
  int menu;
  menu = glutCreateMenu (SelectFromMenu);
  glutAddMenuEntry ("Toggle lighting\tl", MENU_LIGHTING);
  glutAddMenuEntry ("Toggle polygon fill\tp", MENU_POLYMODE);
  glutAddMenuEntry ("Toggle texturing\tt", MENU_TEXTURING);
  glutAddMenuEntry ("Exit demo\tEsc", MENU_EXIT);
  return menu;
}
void idleFunction()
{
  g_rayTracer->checkProgress();
}
int main(int argc, char** argv)
{

  if( argc < 2 )
  {
    std::cerr << "usage: " << argv[0] << " <nrrd_fn> <cmap_fn> <curv-thickness> <norm-flipped>" << std::endl;
    exit(1);
  }

  g_nrrd_fn = argv[1];
  g_cmap_fn = argv[2];
  g_curvThick = atof(argv[3]);
  g_normFlipped = atoi(argv[4]) != 0;

  // GLUT Window Initialization:
  glutInit (&argc, argv);
  glutInitWindowPosition( 50, 50 );
  glutInitWindowSize (g_Width, g_Height);
  glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow ("rt");
  // Initialize OpenGL graphics state
  InitGraphics();
  // Register callbacks:
  glutDisplayFunc (display);
  glutReshapeFunc (reshape);
  glutKeyboardFunc (Keyboard);
  glutMouseFunc (MouseButton);
  glutMotionFunc (MouseMotion);
  glutIdleFunc (idleFunction);
  // Create our popup menu
  //BuildPopupMenu ();
  //glutAttachMenu (GLUT_RIGHT_BUTTON);
  // Get the initial time, for use by animation
#ifdef _WIN32
  last_idle_time = GetTickCount();
#else
  gettimeofday (&last_idle_time, NULL);
#endif
  // Turn the flow of control over to GLUT
  glutMainLoop ();
  return 0;
}
 
