/* OOQP                                                               *
 * Authors: E. Michael Gertz, Stephen J. Wright                       *
 * (C) 2001 University of Chicago. See Copyright Notification in OOQP */

#ifndef SPARSEGENMATRIX_H
#define SPARSEGENMATRIX_H

#include "OoqpVectorHandle.h"
#include "DoubleMatrix.h"
#include "SparseStorage.h"
#include "SparseGenMatrixHandle.h"

/** Represents sparse non-symmetric, possibly non-square matrices stored in
 *  row-major Harwell-Boeing format.
 *  @ingroup SparseLinearAlgebra
 */
class SparseGenMatrix : public GenMatrix {
protected:
  SparseStorageHandle mStorage;
  int size;

public:
  SparseGenMatrix( int rows, int cols, int nnz );
  SparseGenMatrix( int rows, int cols, int nnz,
		   int krowM[], int jcolM[], double M[],
		   int deleteElts=0);
  //SparseGenMatrix(const std::vector<SparseGenMatrix*> &blocks, bool diagonal); -- not needed anymore; cpetra
  
  virtual void getSize( long long& m, long long& n );
  virtual void getSize( int& m, int& n );

  /** The actual number of structural non-zero elements in this sparse
   *  matrix. This includes so-called "accidental" zeros, elements that
   *  are treated as non-zero even though their value happens to be zero.
   */  
  virtual int numberOfNonZeros();

  virtual int isKindOf( int matType );

  virtual void atPutDense( int row, int col, double * A, int lda,
			   int rowExtent, int colExtent );
  virtual void fromGetDense( int row, int col, double * A, int lda,
			     int rowExtent, int colExtent );
  virtual void ColumnScale( OoqpVector& vec );
  virtual void RowScale( OoqpVector& vec );
  virtual void SymmetricScale( OoqpVector &vec);
  virtual void scalarMult( double num);
  virtual void fromGetSpRow( int row, int col,
			     double A[], int lenA, int jcolA[], int& nnz,
			     int colExtent, int& info );
  virtual void atPutSubmatrix( int destRow, int destCol, DoubleMatrix& M,
			       int srcRow, int srcCol,
			       int rowExtent, int colExtent );
  virtual void atPutSpRow( int col, double A[], int lenA, int jcolA[],
			   int& info );

  virtual void putSparseTriple( int irow[], int len, int jcol[], double A[], 
				int& info );

  virtual void getDiagonal( OoqpVector& vec );
  virtual void setToDiagonal( OoqpVector& vec );

  virtual void mult ( double beta,  OoqpVector& y,
                      double alpha, OoqpVector& x );
  virtual void mult ( double beta,  double y[], int incy,
                      double alpha, double x[], int incx );

  virtual void transMult( double beta,   OoqpVector& y,
			  double alpha,  OoqpVector& x );
  virtual void transMult( double beta,  OoqpVector& y_in, int incy,
			  double alpha, OoqpVector& x_in, int incx );
  virtual void transMult( double beta,  double y_in[], int incy,
			  double alpha, double x_in[], int incx );

  /** C = this^T * D * this where D=diag(d) is a diagonal matrix. */
  virtual void matTransDMultMat(OoqpVector& d, SymMatrix** res);
  /** C = this^T * inv(D) * this where D=diag(d) is a diagonal matrix. */
  virtual void matTransDinvMultMat(OoqpVector& d, SymMatrix** res);

  /** C = this * this^T */
  virtual void matMultTrans(SymMatrix** res);
  
  virtual double abmaxnorm();

  virtual void writeToStream(ostream& out) const;

  /** Make the elements in this matrix symmetric. The elements of interest
   *  must be in the lower triangle, and the upper triangle must be empty.
   *  @param info zero if the operation succeeded. Otherwise, insufficient
   *  space was allocated to symmetrize the matrix.
   */
  virtual void symmetrize( int& info );

  virtual void randomize( double alpha, double beta, double * seed );

  virtual void atPutDiagonal( int idiag, OoqpVector& v );
  virtual void fromGetDiagonal( int idiag, OoqpVector& v );

  SparseStorage * getStorage() { return mStorage.ptr(); }
  SparseStorage& getStorageRef() { return *mStorage; }
  int * krowM() { return mStorage->krowM; }
  int * jcolM() { return mStorage->jcolM; }
  double * M() { return mStorage->M; }

  virtual ~SparseGenMatrix();

 protected:
  // in the case of A'*A we internally form the transpose only once
  SparseGenMatrix* m_Mt;

};

#endif
