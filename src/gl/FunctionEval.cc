#include "FunctionEval.h"

double FunctionEval( double x , double y){
  return (cos( 500 * M_PI * ( x*x + y*y) ) + 1 )/2.;
}

double FunctionEval( double x , double y, Color & clr){
  double result = FunctionEval( x , y );
  clr.setr( result );
  clr.setg( result );
  clr.setb( result );
  return result;
}
