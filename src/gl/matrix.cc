/* Daniel Perry
 * originally written for cs5605 in spring 2002
 */

#include "matrix.h"
#include <math.h>
#include <cassert>
#include <iostream>

// EPSILON for checking if doubles are != 0
#define MY_EPSILON 1e-09



matrix::matrix( int init_rows , int init_cols , double init_val ) 
{
  ROWS = init_rows; 
  COLS = init_cols; 
  data = new double*[init_rows];
  for ( int r = 0 ; r < init_rows ; r++ )
  {
    data[r] = new double[init_cols];
    for ( int c = 0 ; c < init_cols ; c++ )
      data[r][c] = init_val;
  }
}

matrix::matrix( int init_rows , int init_cols , const double * matrix_values )
{
  data = new double*[init_rows];
  for (int r=0; r<init_rows ; r++)
  {
    data[r] = new double[init_cols];
    for(int c=0; c<init_cols ; c++)
      data[r][c] = matrix_values[ c + r * init_cols ];
  }

  ROWS = init_rows;
  COLS = init_cols;
}

matrix::matrix( const matrix & new_mat )
{
  ROWS = new_mat.rows(); 
  COLS = new_mat.cols(); 
  data = new double*[rows()];

  for ( int r = 0 ; r < rows() ; r++ )
  {
    data[r] = new double[cols()];
    for ( int c = 0 ; c < cols() ; c++ )
      data[r][c] = new_mat.get( r , c );
  }
}


matrix::~matrix()
{
  for(int i = 0 ; i< rows() ; i++ )
    delete [] data[i];
  delete [] data;
}


matrix matrix::operator = ( const matrix & assignee )
{
  int r,c;
  for (r=0 ; r<rows() ; r++ )
    delete [] data[r];
  delete [] data;
  
  ROWS = assignee.rows();
  COLS = assignee.cols();

  data = new double*[ROWS];
  for ( r=0 ; r<ROWS ; r++ )
  {
    data[r] = new double[COLS];
    for ( c=0 ; c<COLS ; c++ )
      set( r , c , assignee.get( r , c ) );
  }

  return (*this);
}

/*

template <class type>
matrix<type>::matrix( int rows , int cols , type init_val )
{
  ROWS = row;
}

template <class type>
matrix<type>::matrix( int init_rows , int init_cols , type init_val )
{
  ROWS = init_rows;
  COLS = init_cols;
  for ( int r = 0 ; r < init_rows ; r++ )
    for ( int c = 0 ; c < init_cols ; c++ )
      data[r][c] = init_val;
}

*/

// row range-checked, NOT column range-checked
double* matrix::operator [] ( int row )
{
  assert( row >=0 && row <= ROWS );
  return data[row];
}
// row range-checked, NOT column range-checked
double*matrix::operator [] ( int row ) const
{
  assert( row >=0 && row <= ROWS );
  return data[row];
}

// row and column range-checked
double matrix::get( int row , int col ) const
{
  assert ( row >= 0 && row < rows() );
  assert ( col >= 0 && col < cols() );

  return data[row][col];
}

void matrix::set( int row , int col , double d )
{
  assert ( row >= 0 && row < rows() );
  assert ( col >= 0 && col < cols() );
  
  data[row][col] = d;
}

int matrix::rows() const
{
  return ROWS;
}

int matrix::cols() const
{
  return COLS;
}

void matrix::resize( int new_rows , int new_cols )
{
  double ** new_data = new double*[new_rows];
  for ( int r=0 ; r < new_rows ; r++ )
  {
    new_data[r] = new double[new_cols];
    for ( int c=0 ; c < new_cols ; c++ )
      new_data[r][c] = ( (r<rows() && c<cols()) ? data[r][c] : 0 );
  }

  
  for(int i = 0 ; i< rows() ; i++ )
    delete [] data[i];
  delete [] data;

  data =  new_data;

  ROWS = new_rows;
  COLS = new_cols;
}

double matrix::det() const
{
  assert ( rows() == cols() );

  if ( rows() == 1 )
    return get( 0 , 0 );

  double determinant = 0;

  for ( int c = 0 ; c < cols() ; c++ )
  {
    determinant +=  get(0,c) * cofactor(0,c);
  }

  return determinant;
}

double  matrix::cofactor( int row , int col ) const
{
  matrix cofactor_matrix ( rows()-1 , cols()-1 );
  int nr = 0, nc = 0;
  for ( int r = 0 ; r < rows() ; r++ )
  {
    nc = 0;
    for ( int c = 0 ; c < cols() ; c++ )
    {
      if ( r != row && c != col )
      {
	cofactor_matrix.set( nr , nc , get(r,c) ); //[nr][nc] = data[r][c];
	nc++;
      }
    }
    if( r!=row )
      nr++;
  }

  return cofactor_matrix.det() * pow(-1,row+col);
}

matrix matrix::inverse() const
{
  matrix adjoint_matrix( rows() , cols() );
  for ( int r = 0 ; r < rows() ; r++ )
    for ( int c = 0 ; c < cols() ; c++ )
      adjoint_matrix.set( r , c , cofactor(r,c) );

  double determinant = det();
  // checking if a double !=0 to avoid dividing by 0 (underflow?)
  assert( !( determinant < MY_EPSILON && determinant > -MY_EPSILON ) );

  return (1/det()) * adjoint_matrix.transpose();
}

matrix matrix::transpose() const
{
  matrix trans_mat( cols() , rows() );
  for ( int r = 0 ; r < trans_mat.rows() ; r++ )
    for ( int c = 0 ; c < trans_mat.cols() ; c++ )
      trans_mat.set( r , c , get(c,r) );

  return trans_mat;
}


matrix matrix::operator-() const
{
  return (*this) * -1;
}

matrix matrix::operator *=( double right )
{  
  for ( int r = 0 ; r < rows() ; r++ )
    for ( int c = 0 ; c < cols() ; c++ )
      (*this)[r][c] *= right;
  return (*this);
}

matrix matrix::operator +=( const matrix & right )
{
  assert( rows() == right.rows() );
  assert( cols() == right.cols() );

  for( int r=0 ; r < rows() ; r++ )
    for ( int c=0 ; c < cols() ; c++ )
      (*this)[r][c] += right[r][c];

  return (*this);
}

matrix matrix::operator -=( const matrix & right )
{
  return (*this)+=(-right);
}
// NON-MEMBER FUNCTIONS

matrix operator * ( const matrix & left, const matrix & right ) 
{
  assert( left.cols() == right.rows() );
  matrix product = matrix( left.rows() , right.cols() );
  int r,c,i;
  for( r = 0 ; r < product.rows() ; r++ )
    for ( c = 0 ; c < product.cols() ; c++ ) 
    {
      // product initialized so that: product[r][c] = 0;
      for ( i = 0 ; i < left.cols() ; i++ )
	product[r][c] += left[r][i]*right[i][c];
    }
  
  return product;
}

matrix operator * ( const matrix & left , double right )
{
  matrix product( left );
  product *= right;
  return product;
}

matrix operator * ( double left , const matrix & right)
{
  return (right * left);
}

matrix operator + ( const matrix & left, const matrix & right)
{
  matrix sum( left );
  sum+=right;
  return sum;
}

matrix operator - ( const matrix & left , const matrix & right )
{
  return left + (-right);
}

std::ostream & operator << (std::ostream & os , const matrix & right)
{
  os <<right.rows()<<" X "<<right.cols()<<std::endl;
  os <<'['<<std::endl;
  for ( int r = 0 ; r < right.rows() ; r++ )
  {
    os << "[ ";
    for ( int c = 0 ; c < right.cols() ; c++ )
      os << right[r][c] << ' ';
    os << "], " << std::endl;
  }
  os << ']' << std::endl;

  return os;
}


matrix makeIdentityMatrix()
{
  double mat_data[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
  return matrix( 4 , 4 , mat_data );
}

matrix makeRotateZMatrix( double ccwThetaFromX )
{
  double cosTheta = cos( ccwThetaFromX ), sinTheta = sin( ccwThetaFromX ), mat_data[] = { cosTheta,-sinTheta,0,0, sinTheta,cosTheta,0,0, 0,0,1,0, 0,0,0,1 };
  return matrix( 4 , 4 , mat_data ) ;
}
matrix makeRotateYMatrix( double ccwThetaFromNegX )
{
  double cosTheta = cos( ccwThetaFromNegX ), sinTheta = sin( ccwThetaFromNegX ), mat_data[] = { cosTheta,0,sinTheta,0, 0,1,0,0, -sinTheta,0,cosTheta,0, 0,0,0,1 };
  return matrix( 4 , 4 , mat_data ) ;
}
matrix makeRotateXMatrix( double ccwThetaFromY )
{
  double cosTheta = cos( ccwThetaFromY ), sinTheta = sin( ccwThetaFromY ), mat_data[] = { 1,0,0,0, 0,cosTheta,-sinTheta,0, 0,sinTheta,cosTheta,0, 0,0,0,1 };
  return matrix( 4 , 4 , mat_data ) ;
}

matrix makeShearXMatrix( double dy , double dz )
{
  double mat_data[] = { 1,dy,dz,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
  return matrix( 4 , 4 , mat_data );
}
matrix makeShearYMatrix( double dx , double dz )
{
  double mat_data[] = { 1,0,0,0, dx,1,dz,0, 0,0,1,0, 0,0,0,1 };
  return matrix( 4 , 4 , mat_data );
}
matrix makeShearZMatrix( double dx , double dy )
{
  double mat_data[] = { 1,0,0,0, 0,1,0,0, dx,dy,1,0, 0,0,0,1 };
  return matrix( 4 , 4 , mat_data );
}

matrix makeTransMatrix( double x , double y , double z )
{
  double mat_data[] = { 1,0,0,x, 0,1,0,y, 0,0,1,z, 0,0,0,1 };
  return matrix( 4 , 4 , mat_data );
}

matrix  makeScaleMatrix( double x , double y , double z )
{
  double mat_data[] = { x,0,0,0, 0,y,0,0, 0,0,z,0, 0,0,0,1 };
  return matrix( 4 , 4 , mat_data );
}

