/* IsosurfaceVolumeMaterial: volume rendering using curvature..
 */

// local
#include "IsosurfaceVolumeMaterial.h"
#include "HitRecord.h"
#include "Primitive.h"
#include "Scene.h"

// libs
//#include <NrrdIO.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkExceptionObject.h>
#include <itkNumericTraits.h>
#include <itkIsosurfaceAnisotropicDiffusionImageFilter.h>
#include <itkGradientAnisotropicDiffusionImageFilter.h>

// std
#include <cmath>
#include <fstream>
#include <cstdlib>
using namespace std;

void IsosurfaceVolumeMaterial::preprocess(){}

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

/**
 * phong shading routine
 */
void PhongMaterialShade( rgb & result, const Point & pt, const Vector & norm, const ray & r)
{
  HitRecord tempHit;
  tempHit.id = 2;
  double cosPhi = -dot( norm , r.direction() );
  if( cosPhi < 0 ){
    cosPhi = -cosPhi;
    norm = -norm;
  }
  rgb light = context.scene->AmbientLight * Ka;
  rgb speclight(0,0,0);
  for(int i =0 ; i< (int)context.scene->lights.size() ; i++){
    rgb lightColor;
    vector3d lightDir;
    vector3d lightPos;
    double costheta,distance;
    distance = context.scene->lights[i]->getLight( lightColor , lightDir , context , pt );
#ifdef _TWO_SIDED_LIGHTING_OP
    costheta = fabs(dot( lightDir , norm ));
#else
    costheta = dot( lightDir , norm );
#endif
    
    if( costheta > 0 ){
      tempHit.t = distance;
      if( !context.scene->object->intersect(tempHit,context,ray(pt,lightDir)) ){
				light += costheta * Kd  * lightColor;
	
				// Phong Highlight contribution.
				vector3d Half = (lightDir - r.direction());
				Half.MakeUnitVector();
				double cosAlpha = dot( Half , norm );
				if( cosAlpha > 0 ){
					speclight+=(lightColor*pow(cosAlpha,Exponent));
				}
	
			}
    }
  }
  result = light * matte + speclight * reflective;
}

void IsosurfaceVolumeMaterial::shade( rgb & result, const RenderContext & context, const ray & r, HitRecord & hit, int depth, double attenuation) const{

  // find t_exit:
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

		// the point at which the ray intersects the isosurface
    Point P_surface;

    double value = 0.f;
		bool intersection = false;
    {
      ImageType::IndexType ind = {{ lower_int[0], lower_int[1], lower_int[2] }};
			double minval = 1e30;
			double maxval = -1e30;

      double v000 = data->GetPixel(ind);
			minval = min(minval,v000);
			maxval = max(maxval,v000);
      ind[0]+=1;
      double v100 = data->GetPixel(ind);
			minval = min(minval,v100);
			maxval = max(maxval,v100);
      ind[1]+=1;
      double v110 = data->GetPixel(ind);
			minval = min(minval,v110);
			maxval = max(maxval,v110);
      ind[2]+=1;
      double v111 = data->GetPixel(ind);
			minval = min(minval,v111);
			maxval = max(maxval,v111);
      ind[0]-=1;
      double v011 = data->GetPixel(ind); 
			minval = min(minval,v011);
			maxval = max(maxval,v011);
      ind[2]-=1;
      double v010 = data->GetPixel(ind);
			minval = min(minval,v010);
			maxval = max(maxval,v010);
      ind[1]-=1;
      ind[2]+=1;
      double v001 = data->GetPixel(ind);
			minval = min(minval,v001);
			maxval = max(maxval,v001);
      ind[0]+=1;
      double v101 = data->GetPixel(ind);
			minval = min(minval,v101);
			maxval = max(maxval,v101);

			if (minval <= isovalue && isovalue <= maxval)
			{
				if(nearest) // nearest neighbor ==> use center point
				{
					ImageType::IndexType ind = {{ lower_int[0], lower_int[1], lower_int[2] }};
					double v000 = data->GetPixel(ind);
					value = v000;
					P_surface[0] = lower_int[0] + .5;
					P_surface[1] = lower_int[1] + .5;
					P_surface[2] = lower_int[2] + .5;
					intersection = true;
				}
				else // linear interpolation ==> compute exact intersection
				{
					
				}

				/*
				value = v000 * (1-P_diff.x())*(1-P_diff.y())*(1-P_diff.z()) +
								v100 * P_diff.x() * (1-P_diff.y())*(1-P_diff.z()) +
								v010 * (1-P_diff.x()) * P_diff.y() * (1-P_diff.z()) +
								v001 * (1-P_diff.x()) * (1-P_diff.y()) * P_diff.z() +
								v101 * P_diff.x() * (1-P_diff.y()) * P_diff.z() +
								v011 * (1-P_diff.x()) * P_diff.y() * P_diff.z() +
								v110 * P_diff.x() * P_diff.y() * (1-P_diff.z()) +
								v111 * P_diff.x() * P_diff.y() * P_diff.z();
				*/
			}
    }

    float opacity;
    Color color;
		if (intersection)
		{
			// do phong computation
			PhongMaterialShade( color, P_surface, intersection_norm, r);
		}

		// TODO : may need to move this into intersection if-statement, and then break ?
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

IsosurfaceVolumeMaterial::IsosurfaceVolumeMaterial(const std::string& data_fn,
																										float isoval,
																										const rgb & phong_matte, 
																										double phong_kd, 
																										double phong_ka , 
																										const rgb & phong_r , 
																										double phong_exp,
																										const Point& lower, 
																										const Point& upper,
																										double grid_stepsize,
																										bool nearest_neighbor )

:isovalue(isoval), 
 matte(phong_matte),
 Kd(phong_kd),
 Ka(phong_ka),
 reflective(phong_reflective),
 Exponent(phong_exp),
 lower(lower), 
 upper(upper), 
 grid_stepsize(grid_stepsize),
 nearest(nearest_neighbor)
{
  diag = upper-lower;
  bounds[0] = lower;
  bounds[1] = upper;

  typedef itk::ImageFileReader<ImageType> Reader;
  Reader::Pointer reader = Reader::New();
  reader->SetFileName(data_fn);
  data = reader->GetOutput();
  try
  {
    reader->Update();
  }
  catch(itk::ExceptionObject e)
  {
    std::cerr << "Error reading file " << data_fn << ": " << e << std::endl;
    exit(1);
  }

  std::cerr << "computing gradient ... " << std::endl;

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

  std::cerr << "done computing gradient and hessian." << std::endl;
 

  size1 = data->GetLargestPossibleRegion().GetSize()[0];
  size2 = data->GetLargestPossibleRegion().GetSize()[1];
  size3 = data->GetLargestPossibleRegion().GetSize()[2];

  cellsize = diag * Vector(1./(size1-1), 1./(size2-1), 1./(size3-1));

  world_stepsize = cellsize.length()/pow(3, 1./3.) * grid_stepsize;
  std::cerr << "world step size: " << world_stepsize << std::endl;
}
