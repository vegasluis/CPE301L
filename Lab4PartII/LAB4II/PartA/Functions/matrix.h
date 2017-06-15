

#ifndef _MATRIX_H
#define _MATRIX_H

#include <avr/io.h>
#include <stdio.h>

int** rref(int row, int col, int** matrix_a ){
	// Return the reduced row echelon form of the matrix using
	//Gaussian Elimination.

	//function ToReducedRowEchelonForm(Matrix M) is
	//lead := 0
	int lead = 0;
	//rowCount := the number of rows in M
	//columnCount := the number of columns in M
	//for 0 ? r < rowCount do
	for(int r = 0; r < row; r++)
	{
		//if columnCount ? lead then
		//       stop
		//   end if
		if (col <= lead)
			break;
		//   i = r
		int i = r;
		//   while M[i, lead] = 0 do
		while (matrix_a[i][lead] =0)
		{
			//i = i + 1
			i++;
			//    if rowCount = i then
			if(row == i)
			{
				//       i = r
				i=r;
				//       lead = lead + 1
				lead++;
				//       if columnCount = lead then

				if (col == lead)
				break;
				//    end if
			}
			//end while
		}
		//Swap rows i and r
		//If M[r, lead] is not 0 divide row r by M[r, lead]
		if (matrix_a[r][lead] != 0)
			matrix_a[r][lead] = r / matrix_a[r][lead];

		//for 0 ? i < rowCount do
		for(int i=0; i < row; i++)
		{
			//   if i ? r do
			if(i != r)
			//    Subtract M[i, lead] multiplied by row r from row i
			matrix_a[i][lead] = i - matrix_a[i][lead] * r;
			// end if
			//end for
		}
		lead = lead + 1;
		//end for
	}
	//end function

}

// // Clears all existing arrays and deallocates storage to save memory.
void memoryallocation (int row,int** matrix_a){
	for (int i = 0; i < row; i++)
		free(matrix_a[i]);

	free(matrix_a);
}

// //Given a matrix, return the inverse of that matrix.
int** invertmatrix(int row,int col,int** matrix_a){
	int d;
	d = determinant(matrix_a,row);
	
	if (d==0){
		printf("\nInverse of Entered Matrix is not possible\n");
		return -1;
	}

	else
		matrix_a = cofactor(matrix_a,row);

	return matrix_a;
}

// /*For calculating Determinant of the Matrix */
int determinant(int a[25][25],int k){
	int s=1,det=0,b[25][25];
	int i,j,m,n,c;
	if (k==1)
	{
		return (a[0][0]);
	}
	else
	{
		det=0;
		for (c=0;c<k;c++)
		{
			m=0;
			n=0;
			for (i=0;i<k;i++)
			{
				for (j=0;j<k;j++)
				{
					b[i][j]=0;
					if (i != 0 && j != c)
					{
						b[m][n]=a[i][j];
						if (n<(k-2))
						n++;
						else
						{
							n=0;
							m++;
						}
					}
				}
			}
			det=det + s * (a[0][c] * determinant(b,k-1));
			s=-1 * s;
		}
	}
	return (det);
}

// /*Finding transpose of matrix*/
int** trans(float num[25][25],int fac[25][25],float r){
	int i,j;
	int b[25][25],inverse[25][25],d;
	
	for (i=0;i<r;i++)
	{
		for (j=0;j<r;j++)
		{
			b[i][j]=fac[j][i];
		}
	}
	d = determinant(num,r);
	for (i=0;i<r;i++)
	{
		for (j=0;j<r;j++)
		{
			inverse[i][j]=b[i][j] / d;
		}
	}
	return inverse;
}

void cofactor(float num[25][25],float f){
	int b[25][25],fac[25][25];
	int p,q,m,n,i,j;
	for (q=0;q<f;q++)
	{
		for (p=0;p<f;p++)
		{
			m=0;
			n=0;
			for (i=0;i<f;i++)
			{
				for (j=0;j<f;j++)
				{
					if (i != q && j != p)
					{
						b[m][n]=num[i][j];
						if (n<(f-2))
							n++;
						else
						{
							n=0;
							m++;
						}
					}
				}
			}
			fac[q][p]=pow(-1,q + p) * determinant(b,f-1);
		}
	}

	int **temp = trans(num,fac,f);
}



// // matrix multiplication . Checks if the matricies supplied can be
// //multiplied and then multiplies them. Also checks if any value is a scalar,
// //and if a scalar is detected as an input , scalar / vector multiplication
// //is performed . Stores the result in a newly initialized matrix
// //local to the function.
int** mmul(int arow,int acol,int** matrix_a,int brow,int bcol, int** matrix_b ){
	int sum = 0;
	int multiply[arow][bcol];
	
	if (acol != brow){
		printf("Matrices with entered orders can't be multiplied with each other.\n");
		return -1;
	}
	else
	{
		for (int c = 0; c < arow; c++) {
			for (int d = 0; d < bcol; d++) {
				for (int k = 0; k < brow; k++)
				sum = sum + matrix_a[c][k]*matrix_b[k][d];
				multiply[c][d] = sum;
				sum = 0;
			}
		}
	}
	return multiply;
}

// // prints the matrix
void printmatrix(int row,int col,int** matrix_a){
	for (int i = 0; i < row; i++)
		for (int j = 0 ; j < col; j++)
			printf("%d\t", matrix_a[i][j]);
	printf("\n");
}

// // store a matrix in a file and transfer to host.
void storeandtransfer(char* filename, int row, int col, int** matrix_a_){
	//Note: Transfer how?
	int num_;
	int row_;
	int col_;
	FILE *fp;
	fp = fopen(filename, "w");
	fprintf(fp, "%d %d\n", &row_, &col_);	//row and column included in the file
	int matrix_a_1 [row_][col_];
	for (int i = 0; i < row_; i++)
		for (int j = 0 ; j < col_; j++){
			num_ = matrix_a_1[i][j];
			fprintf(fp, "%d ", &num_);
	}
	int fclose(FILE *fp);
}

int** matrixtranspose(int row, int col, int** matrix_a ){
// 	// Return the tranposed matrix, using the same matrix as the input.
// 	//You may consider using an array pointer for this.
	int matrix_b[row][col];
	for(int i=0; i<row; ++i)
		for(int j=0; j<col; ++j)
			matrix_b[j][i] = matrix_a[i][j];
	return matrix_b;
}

// // Using a user input file, load a matrix.
int** loadmatrixfromfile(char* filename){
	int num;
	int row;
	int col;
	FILE *fp;
	fp = fopen(filename, "r");
	fscanf(fp, "%d %d\n", &row, &col);	//row and column included in the file
	int matrix_a [row][col];
	for (int i = 0; i < row; i++)
	for (int j = 0 ; j < col; j++){
		fscanf(fp, "%d ", &num);
		matrix_a[i][j] = num;
	}
	int fclose(FILE *fp);
	return matrix_a;
}

// // Creates an array based on user input and the c rand function
int** randfillmatrix (int low_num, int hi_num ,int row,int col){
	srand(time(NULL));
	int matrix_a [row][col];
	for (int i = 0; i < row; i++)
	for (int j = 0 ; j < col; j++)
	matrix_a[i][j] = (rand() % (hi_num - low_num)) + low_num;
	return matrix_a;
}

// //Matrix Addition. Given two matracies, subtract and return a new matrix
int** madd (int row,int col,int** matrix_a,int** matrix_b){
	int result[row][col];
	for (int i = 0; i < row; i++)
	for (int j = 0 ; j < col; j++)
	result[i][j] = matrix_a[i][j] + matrix_b[i][j];
	return result;
}

// //Same as above but with subtraction.
int** msub(int row,int col,int** matrix_a,int** matrix_b){
	int result[row][col];
	for (int i = 0; i < row; i++)
	for (int j = 0 ; j < col; j++)
	result[i][j] = matrix_a[i][j] - matrix_b[i][j];
	return result;
}

// //Scalar + Matrix (elementbyelement) addition.
int** elementadd(int row,int col,int** matrix_a,int element){
	int result[row][col];
	for (int i = 0; i < row; i++)
	for (int j = 0 ; j < col; j++)
	result[i][j] = element + matrix_a[i][j];
	return result;
}

// //Scalar - Matrix, element by element sub.
int** elementsub(int row,int col,int** matrix_a,int element ){
	int result[row][col];
	for (int i = 0; i < row; i++)
	for (int j = 0 ; j < col; j++)
	result[i][j] = element - matrix_a[i][j];
	return result;
}

// //Same as above but with division
int** elementdivide(int row,int col,int** matrix_a,int element ){
	int result[row][col];
	for (int i = 0; i < row; i++)
	for (int j = 0 ; j < col; j++)
	result[i][j] =  matrix_a[i][j] / element;		// order might be reversed
	return result;
}

// //Same as above but with exponentiation
int** elementpower (int row,int col,int** matrix_a,int element){
	int result[row][col];
	for (int i = 0; i < row; i++)
	for (int j = 0 ; j < col; j++)
	result[i][j] =  matrix_a[i][j]**element;
	return result;
}

#endif