/* Daniel Perry
 * originally written for cs5605 in spring 2002
 */

#ifndef _MATRIX_H
#define _MATRIX_H

#include <iostream.h>

class matrix
{
public:
  
  //matrix() { ROWS = 0; COLS = 0; data = 0 ; }
  //matrix( int init_rows , int init_cols , double init_val );
  matrix( int init_rows = 0 , int init_cols = 0 , double init_val = 0 ); 
  matrix( const matrix & new_mat );
  matrix( int init_rows , int init_cols , const double * matrix_values );
  ~matrix();

  matrix operator = ( const matrix & assignee );

  // row range-checked, NOT column range-checked
  double* operator [] ( int row ); 
  double* operator [] ( int row ) const;
  // row and column range-checked
  double get( int row , int col ) const;  
  void set( int row , int col , double d );
  int rows() const;
  int cols() const;
  void resize( int new_rows , int new_cols );

  double det() const; // determinant
  double cofactor( int row , int col ) const; // cofactor of the element at [row][col]
  matrix inverse() const;  // returns the inverse matrix
  matrix transpose() const;  // returns the transpose of the matrix

  matrix operator-() const;
  matrix operator *= ( double right );
  matrix operator += ( const matrix & right );
  matrix operator -=( const matrix & right );
  /*
  <type> eigenvalues();  // returns the eigenvalues
  const matrix * eigenvectors(); // returns the eigenvectors
  matrix augment ( const matrix & cat_mat );
  */

private:
  int ROWS, COLS;
  double **data;
};

#endif


matrix makeIdentityMatrix();
matrix makeRotateZMatrix( double ccwThetaFromX );
matrix makeRotateYMatrix( double ccwThetaFromNegX );
matrix makeRotateXMatrix( double ccwThetaFromY );
matrix makeShearXMatrix( double dy , double dz );
matrix makeShearYMatrix( double dx , double dz );
matrix makeShearZMatrix( double dx , double dy );
matrix makeTransMatrix( double x , double y , double z );
matrix makeScaleMatrix( double x , double y , double z );


matrix operator * ( const matrix & left , const matrix & right );  // matrix multiplication
matrix operator * ( const matrix & left , double right );
matrix operator * ( double left , const matrix & right );
matrix operator + ( const matrix & left , const matrix & right ); // matrix addition
matrix operator - ( const matrix & left , const matrix & right );

ostream& operator << ( ostream & os , const matrix & right); // output

