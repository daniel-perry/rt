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

// std
#include <cmath>
#include <fstream>
#include <cstdlib>
using namespace std;

void CurvatureVolumeMaterial::preprocess(){}

template<class HessianType, class GradientType >
HessianType outer_product( const GradientType & v1, const GradientType & v2 )
{
  HessianType m;
  for( size_t r=0; r<v1.Size(); ++r )
  {
    for( size_t c=0; c<v2.Size(); ++c )
    {
      m[r][c] = v1[r] * v2[c];
    }
  }
  return m;
}

void CurvatureVolumeMaterial::shade( rgb & result, const RenderContext & context, const ray & r, HitRecord & hit, int depth, double attenuation) const{

  // find texit:
  double t_enter = hit.t, t_exit=t_enter;
  vector3d safety_offset(1e-6,1e-6,1e-6);
  ray r_inside( r.eval( hit.t ) + safety_offset , r.direction() );
  HitRecord hit_inside;
  hit_inside.t = MYMAX;
  
  hit.prim->intersect( hit_inside , context , r_inside );
  if( hit_inside.t < MYMAX ){ // hit other side
    t_exit = t_enter+hit_inside.t;
  }

  double it = ceil( t_enter / grid_stepsize);
  //double t = it * grid_stepsize;
  double t = t_enter;

  Color accum_color(0,0,0);
  double accum_opacity = 0;  
  float max_intensity = 0;

  while( t < t_exit ){
    Point P = r.eval( t );
    P = P - lower;
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

    double value = 0.f;
    if(nearest) // nearest neighbor
    {
      value = v000;
    }
    else // linear interpolation
    {
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

    //if( 750 <= value && value <= 900 && opacity > 0 )
    //if( 1150 <= value && value <= 1250 && opacity > 0 )
    if( 110 <= value && value <= 120 && opacity > 0 )
    {
      // get the gradient and hessian for this point:
      for(size_t loc=0; loc<3; ++loc)
        ind[loc] = lower_int[loc];
      GradientType g = gradient->GetPixel(ind);
      HessianType H = hessian->GetPixel(ind);
      
      float g_mag = g.GetNorm();
      GradientType n = -g / g_mag;
      MatrixType P;
      P.SetIdentity();
      P = P - outer_product<MatrixType,GradientType>( n, n );
      MatrixType G = (P*-1) * H.PostMultiply(P);
      //MatrixType G = P * H.PostMultiply(P);
      G /= g_mag;

      // convert vector3d to itk::CovariantVector
      GradientType v;
      Vector v_data = r.direction();
      for(size_t vi=0; vi<v.Size(); ++vi)
        v[vi] = v_data[vi];
      
      float num = v * ( G * v );
      float den = v * ( P * v );

      float k_v = num / (den + itk::NumericTraits<float>::min());

      float T = 10.f; // TODO: find good value...

      float boundary = std::sqrt( T * k_v * (2-T*k_v) );

      float intersection_cosine = fabs(dot(v_data, hit.normal));

      if( intersection_cosine <= boundary )
      {
        color = Color(1.f,1.f,1.f); // white..
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
                                         bool nearest_neighbor
                                         )
:cmap(cmap_fn), 
 lower(lower), 
 upper(upper), 
 grid_stepsize(grid_stepsize),
 maxopacity(maxopacity),
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
  data = reader->GetOutput();


  std::cerr << "computing gradient and hessian... " << std::endl;

  GradientFilter::Pointer gradientFilter = GradientFilter::New();
  gradientFilter->SetSigma(1.0);
  gradientFilter->SetInput( data );
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


  /*
  Nrrd *nin;
  nin = nrrdNew(); // empty container
  if (nrrdLoad(nin, data_fn.c_tr(), NULL)) 
  {
    err = biffGetDone(NRRD);
    std::cerr << "CurvatureVolumeMaterial: trouble reading " << data_fn << ": " <<  err << std::endl;
    free(err);
    exit(1);
  }
  int nx = nin->axis[0].size;
  int ny = nin->axis[1].size;
  int nz = nin->axis[2].size;
  std::cerr << "Finished reading " << data_fn << ": " << nx << "," << ny << "," <<nz << std::endl;
  */
  

  /*
  ifstream hdr(headername.c_str());
  string volumename;
  hdr >> volumename;
  int nx, ny, nz;
  hdr >> nx >> ny >> nz;
  short offsetvalue;
  hdr >> offsetvalue;
  string fileendian;
  hdr >> fileendian;
  if(!hdr){
    cerr << "Error reading header: " << headername << '\n';
    exit(1);
  }
  if(fileendian != "BigEndian" && fileendian != "LittleEndian"){
    cerr << "Bad data endian: " << fileendian << " in " << headername << "\n";
    exit(1);
  }
  cerr << "Reading " << volumename << ": " << nx << 'x' << ny << 'x' << nz << '\n';
  
  //SETUP ARRAY TO HOLD DATA VALS AS SHORT's
  //data.resize(nx, ny, nz);
  size1 = nx;
  size2 = ny;
  size3 = nz;
  data = new short**[size1];
  for(int i=0;i<size1;i++){
    data[i] = new short*[size2];
  }
  short * ptr = new short[nx*ny*nz]; // need the data to be contiguous
  for( int i=0;i<size1;i++){
    for(int j=0;j<size2;j++){
      data[i][j] = &ptr[i*size2*size3+j*size3]; 
    }
  }
  */

  cellsize = diag * Vector(1./(size1-1), 1./(size2-1), 1./(size3-1));

  /*
  // get the folder of the header.. data volume is relative to that..
  size_t pos = headername.find_last_of('/');
  std::string location = headername.substr(0,pos+1);
  volumename = location + volumename;
  // now read in the volume
  ifstream in(volumename.c_str()); 
  in.read(reinterpret_cast<char*>(&data[0][0][0]), nx*ny*nz*sizeof(short));

  if(!in){
    cerr << "Error reading data: " << volumename << '\n';
    exit(1);
  }
  std::cerr << "read " << volumename << std::endl;
  */

  world_stepsize = cellsize.length()/pow(3, 1./3.) * grid_stepsize;
  cmap.rescale(world_stepsize);

  /*
  short tmp = 0x1234;
  string machineendian;
  if(*reinterpret_cast<char*>(&tmp) == 0x12)
    machineendian = "BigEndian";
  else
    machineendian = "LittleEndian";

  if(machineendian != fileendian){
    for(int i=0;i<nz;i++){
      for(int j=0;j<ny;j++){
        for(int k=0;k<nx;k++){
          swap(data[k][j][i]);
          data[k][j][i] += offsetvalue;
        }
      }
    }
  } else {
    for(int i=0;i<nz;i++){
      for(int j=0;j<ny;j++){
        for(int k=0;k<nx;k++){
          data[k][j][i] += offsetvalue;
        }
      }
    }
  }
  */
}
