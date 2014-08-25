/* CurvatureVolumeMaterial: volume rendering using curvature..
 */

// local
#include "CurvatureVolumeMaterial.h"
#include "HitRecord.h"
#include "Primitive.h"
#include "Scene.h"

// libs
//#include <NrrdIO.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkExceptionObject.h>
#include <itkNumericTraits.h>
#include <itkCurvatureAnisotropicDiffusionImageFilter.h>
#include <itkGradientAnisotropicDiffusionImageFilter.h>

// std
#include <cmath>
#include <fstream>
#include <cstdlib>
using namespace std;

void CurvatureVolumeMaterial::preprocess(){}

template<class MatrixType, class VectorType >
MatrixType outer_product( const VectorType & v1, const VectorType & v2 )
{
  MatrixType m;
  for( size_t r=0; r<v1.Size(); ++r )
  {
    for( size_t c=0; c<v2.Size(); ++c )
    {
      m[r][c] = v1[r] * v2[c];
    }
  }
  return m;
}

// dot product between two different vector types
template<class V1, class V2>
float inner_product( const V1 & v1, const V2 & v2 )
{
  float dot = 0.f;
  for(size_t i=0; i<v1.length(); ++i)
  {
    dot += v1[i]*v2[i];
  }
  return dot;
}

Color GoochShade( const Color & objectColor, float normalLightCosine )
{
  // see http://www.cs.northwestern.edu/~ago820/SIG98/paper/node6.html

  Color blue(0.f,0.f,1.f);
  Color yellow(1.f,1.f,0.f);
  Color black(0.f,0.f,0.f);

  float coolPart = 0.15;
  Color objectCool = (1-coolPart)*black + coolPart*objectColor;
  float warmPart = 0.5;
  Color objectWarm = (1-warmPart)*black + warmPart*objectColor;
  
  float darken = 0.4;
  Color cool = darken*blue + objectCool;
  Color warm = darken*yellow + objectWarm;

  float lightPart = (1+normalLightCosine)/2;
  Color result =  lightPart * cool + (1-lightPart) * warm;

  return result;
}

void CurvatureVolumeMaterial::shade( rgb & result, const RenderContext & context, const ray & r, HitRecord & hit, int depth, double attenuation) const{

  // find texit:
  double t_enter = hit.t, t_exit=t_enter;
  vector3d safety_offset = r.direction() * 1e-2;
  ray r_inside( r.eval( hit.t ) + safety_offset , r.direction() );
  HitRecord hit_inside;
  hit_inside.t = MYMAX;
 
  hit.prim->intersect( hit_inside , context , r_inside );
  if( hit_inside.t < MYMAX ){ // hit other side
    t_exit = t_enter+hit_inside.t;
  }

  double it = ceil( t_enter / grid_stepsize);
  double t = t_enter;

  Color accum_color(0,0,0);
  double accum_opacity = 0;  
  float max_intensity = 0;

  while( t < t_exit ){
    Point pt = r.eval( t );
    Point P = pt - lower;
    P = P / diag;

    for(size_t i=0; i<3; ++i) if(P[i] < 0) P[i] = 0;
    P = P * vector3d( (size1-1) , (size2-1) , (size3-1) );
    Point P_lower = vector3d( floor( P.x() ) , floor( P.y() ) , floor( P.z() ) );
    Point P_diff = P - P_lower;
   
    int lower_int[3] = { ((int)P_lower.x()) , ((int)P_lower.y()) , ((int)P_lower.z()) };

    // now P in lattice coordinates:
    if( lower_int[0] == (size1-1))
      lower_int[0]--;
    if(lower_int[1] == (size2-1))
      lower_int[1]--;
    if(lower_int[2] == (size3-1))
      lower_int[2]--;

    double value = 0.f;
    if(nearest) // nearest neighbor
    {
      ImageType::IndexType ind = {{ lower_int[0], lower_int[1], lower_int[2] }};
      double v000 = data->GetPixel(ind);
      value = v000;
    }
    else // linear interpolation
    {
      ImageType::IndexType ind = {{ lower_int[0], lower_int[1], lower_int[2] }};
      double v000 = data->GetPixel(ind);
      ind[0]+=1;
      double v100 = data->GetPixel(ind);
      ind[1]+=1;
      double v110 = data->GetPixel(ind);
      ind[2]+=1;
      double v111 = data->GetPixel(ind);
      ind[0]-=1;
      double v011 = data->GetPixel(ind); 
      ind[2]-=1;
      double v010 = data->GetPixel(ind);
      ind[1]-=1;
      ind[2]+=1;
      double v001 = data->GetPixel(ind);
      ind[0]+=1;
      double v101 = data->GetPixel(ind);

      value = v000 * (1-P_diff.x())*(1-P_diff.y())*(1-P_diff.z()) +
              v100 * P_diff.x() * (1-P_diff.y())*(1-P_diff.z()) +
              v010 * (1-P_diff.x()) * P_diff.y() * (1-P_diff.z()) +
              v001 * (1-P_diff.x()) * (1-P_diff.y()) * P_diff.z() +
              v101 * P_diff.x() * (1-P_diff.y()) * P_diff.z() +
              v011 * (1-P_diff.x()) * P_diff.y() * P_diff.z() +
              v110 * P_diff.x() * P_diff.y() * (1-P_diff.z()) +
              v111 * P_diff.x() * P_diff.y() * P_diff.z();
    }

    float opacity;
    Color color;
    cmap.lookup( (float)value , opacity , color );

    if( opacity > 0 )
    {

      // get the gradient and hessian for this point:
      ImageType::IndexType ind = {{ lower_int[0], lower_int[1], lower_int[2] }};
      GradientType g = gradient->GetPixel(ind);
      HessianType H = hessian->GetPixel(ind);
      
      float g_mag = g.GetNorm();
      GradientType n = -g / g_mag;
      MatrixType P;
      P.SetIdentity();
      P = P - outer_product<MatrixType,GradientType>( n, n );
      MatrixType G;
      if(normalFlipped)
        G = P * H.PostMultiply(P);
      else
        G = (P*-1) * H.PostMultiply(P);
      G /= g_mag;

      // convert vector3d to itk::CovariantVector
      GradientType v;
      Vector v_data = r.direction();
      for(size_t vi=0; vi<v.Size(); ++vi)
        v[vi] = v_data[vi];
      
      float num = v * ( G * v );
      float den = v * ( P * v );

      float k_v = num / (den + itk::NumericTraits<float>::min());

      float T = 0.75 * curvThick; //  ~= num pixels to shade..

      float boundary = std::sqrt( T * k_v * (2-T*k_v) );

      float intersection_cosine = fabs(v * n);

      float fuzzy_boundary = curvThick - T;
      float diff = intersection_cosine - boundary;

      // Convert to gooch shaded color:
      Color lightColor;
      Vector lightDir;
      float lightDistance = context.scene->lights[0]->getLight( lightColor , lightDir , context , pt );
      color = GoochShade( color, inner_product(lightDir, n ) );

      //if( intersection_cosine <= boundary + fuzzy_boundary )
      if( diff <= fuzzy_boundary )
      {
        float interp = 1.f;
        if( diff > 0 )
          interp = 1-diff/fuzzy_boundary;
        color = (1-interp) * color + interp * Color(0.f,0.f,0.f);
      }

    }

    accum_color += color * opacity * (1-accum_opacity);
    accum_opacity += opacity*(1-accum_opacity);

    t += world_stepsize;
  }

  // volume should be "see-through", so now going to cast ray behind it...
  //(may need to add safety small vector to new_r origin.....)
  ray new_r( r.eval( t_exit ) , r.direction() );
  Color exit_color(0,0,0);
  context.scene->render( exit_color , new_r );
  accum_color += exit_color * (1-accum_opacity);
 
  result = accum_color;
}



/* The following two functions were given to the class from
 * the instructor and TA.
*/

void swap(short& data)
{
  char* p=(char*)&data;
  char tmp=p[0];
  p[0]=p[1];
  p[1]=tmp;
}

CurvatureVolumeMaterial::CurvatureVolumeMaterial(const std::string& data_fn,
                                         const std::string& cmap_fn,
                                         const Point& lower, 
                                         const Point& upper,
                                         double grid_stepsize, 
                                         float maxopacity,
                                         float curvThickness,
                                         bool flipNormal,  
                                         bool nearest_neighbor
                                         )
:cmap(cmap_fn), 
 lower(lower), 
 upper(upper), 
 grid_stepsize(grid_stepsize),
 maxopacity(maxopacity),
 curvThick(curvThickness),
 normalFlipped(flipNormal),
 nearest(nearest_neighbor)
{
  diag = upper-lower;
  bounds[0] = lower;
  bounds[1] = upper;

  typedef itk::ImageFileReader<ImageType> Reader;
  Reader::Pointer reader = Reader::New();
  reader->SetFileName(data_fn);
  try
  {
    reader->Update();
  }
  catch(itk::ExceptionObject e)
  {
    std::cerr << "Error reading file " << data_fn << ": " << e << std::endl;
    exit(1);
  }

  std::cerr << "smoothing data... " << std::endl;
  //typedef itk::CurvatureAnisotropicDiffusionImageFilter<ImageType,ImageType> SmoothFilter;
  typedef itk::GradientAnisotropicDiffusionImageFilter<ImageType,ImageType> SmoothFilter;
  SmoothFilter::Pointer smoother = SmoothFilter::New();
  smoother->SetInput(reader->GetOutput());
  smoother->SetNumberOfIterations(2);
  smoother->SetConductanceParameter(1.2);
	smoother->SetNumberOfThreads(8);
  try
  {
    smoother->Update();
  }
  catch(itk::ExceptionObject e)
  {
    std::cerr << "Error smoothing image: " << e << std::endl;
    exit(1);
  }

  //data = reader->GetOutput();
  data = smoother->GetOutput();


  std::cerr << "computing gradient and hessian... " << std::endl;

  GradientFilter::Pointer gradientFilter = GradientFilter::New();
  gradientFilter->SetSigma(1.0);
  gradientFilter->SetInput( data );
	gradientFilter->SetNumberOfThreads(8);
  gradient = gradientFilter->GetOutput();
  try
  {
    gradientFilter->Update();
  }
  catch(itk::ExceptionObject e)
  {
    std::cerr << "Error computing gradient: " << e << std::endl;
    exit(1);
  }
 
  HessianFilter::Pointer hessianFilter = HessianFilter::New();
  hessianFilter->SetSigma(1.0);
  hessianFilter->SetInput( data );
	hessianFilter->SetNumberOfThreads(8);
  hessian = hessianFilter->GetOutput();
  try
  {
    hessianFilter->Update();
  }
  catch(itk::ExceptionObject e)
  {
    std::cerr << "Error computing hessian: " << e << std::endl;
    exit(1);
  }

  std::cerr << "done computing gradient and hessian." << std::endl;

  size1 = data->GetLargestPossibleRegion().GetSize()[0];
  size2 = data->GetLargestPossibleRegion().GetSize()[1];
  size3 = data->GetLargestPossibleRegion().GetSize()[2];



  cellsize = diag * Vector(1./(size1-1), 1./(size2-1), 1./(size3-1));

  world_stepsize = cellsize.length()/pow(3, 1./3.) * grid_stepsize;
  std::cerr << "world step size: " << world_stepsize << std::endl;
  cmap.rescale(world_stepsize);

}
