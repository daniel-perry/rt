
#include "FilterFactory.h"
#include "Filters.h"
#include <string>
using namespace std;

// This could be improved massively...

Filter1D* FilterFactory::findFilter1D(const std::string& filterspec)
{
  if(filterspec == "Box")
    return new Filter1DWrapper<BoxKernel1D>;
  else if(filterspec == "Triangle")
    return new Filter1DWrapper<TriangleKernel1D>;
  else if(filterspec == "LanczosSinc1")
    return new Filter1DWrapper<LanczosWindow1D<SincKernel1D> >(LanczosWindow1D<SincKernel1D>(1.0));
  else if(filterspec == "LanczosSinc2")
    return new Filter1DWrapper<LanczosWindow1D<SincKernel1D> >(LanczosWindow1D<SincKernel1D>(2.0));
  else if(filterspec == "LanczosSinc3")
    return new Filter1DWrapper<LanczosWindow1D<SincKernel1D> >(LanczosWindow1D<SincKernel1D>(3.0));
  else if(filterspec == "TruncatedGaussian2")
    return new Filter1DWrapper<BoxWindow1D<GaussianKernel1D> >(BoxWindow1D<GaussianKernel1D>(2.0));
  else if(filterspec == "TruncatedGaussian3")
    return new Filter1DWrapper<BoxWindow1D<GaussianKernel1D> >(BoxWindow1D<GaussianKernel1D>(3.0));
  else
    return 0;
}

Filter2D* FilterFactory::findFilter2D(const std::string& fspec)
{
  if(fspec[0] == 'r'){
    string filterspec = fspec.substr(1, fspec.length()-1);
    if(filterspec == "Box")
      return new Filter2DWrapper<RadialKernel2D<BoxKernel1D> >;
    else if(filterspec == "Triangle")
      return new Filter2DWrapper<RadialKernel2D<TriangleKernel1D> >;
    else if(filterspec == "LanczosSinc1")
      return new Filter2DWrapper<RadialKernel2D<LanczosWindow1D<SincKernel1D> > >(LanczosWindow1D<SincKernel1D>(1.0));
    else if(filterspec == "LanczosSinc2")
      return new Filter2DWrapper<RadialKernel2D<LanczosWindow1D<SincKernel1D> > >(LanczosWindow1D<SincKernel1D>(2.0));
    else if(filterspec == "LanczosSinc3")
      return new Filter2DWrapper<RadialKernel2D<LanczosWindow1D<SincKernel1D> > >(LanczosWindow1D<SincKernel1D>(3.0));
    else if(filterspec == "TruncatedGaussian2")
      return new Filter2DWrapper<RadialKernel2D<BoxWindow1D<GaussianKernel1D> > >(BoxWindow1D<GaussianKernel1D>(2.0));
    else if(filterspec == "TruncatedGaussian3")
      return new Filter2DWrapper<RadialKernel2D<BoxWindow1D<GaussianKernel1D> > >(BoxWindow1D<GaussianKernel1D>(3.0));
    else
      return 0;
  } else {
    const string& filterspec = fspec;
    if(filterspec == "Box")
      return new Filter2DWrapper<SeparableKernel2D<BoxKernel1D> >;
    else if(filterspec == "Triangle")
      return new Filter2DWrapper<SeparableKernel2D<TriangleKernel1D> >;
    else if(filterspec == "LanczosSinc1")
      return new Filter2DWrapper<SeparableKernel2D<LanczosWindow1D<SincKernel1D> > >(LanczosWindow1D<SincKernel1D>(1.0));
    else if(filterspec == "LanczosSinc2")
      return new Filter2DWrapper<SeparableKernel2D<LanczosWindow1D<SincKernel1D> > >(LanczosWindow1D<SincKernel1D>(2.0));
    else if(filterspec == "LanczosSinc3")
      return new Filter2DWrapper<SeparableKernel2D<LanczosWindow1D<SincKernel1D> > >(LanczosWindow1D<SincKernel1D>(3.0));
    else if(filterspec == "TruncatedGaussian2")
      return new Filter2DWrapper<SeparableKernel2D<BoxWindow1D<GaussianKernel1D> > >(BoxWindow1D<GaussianKernel1D>(2.0));
    else if(filterspec == "TruncatedGaussian3")
      return new Filter2DWrapper<SeparableKernel2D<BoxWindow1D<GaussianKernel1D> > >(BoxWindow1D<GaussianKernel1D>(3.0));
    else
      return 0;
  }
}
