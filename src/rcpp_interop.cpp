
#include "rcpp_interop.h"

using namespace Rcpp;

#ifndef STRDUP
#ifdef _WIN32
#define STRDUP _strdup
#else
#define STRDUP strdup
#endif
#endif // !STRDUP


//void callGc()
//{
//	Rcpp::Function gc("gc");
//	gc();
//}

char** to_ansi_char_array(CharacterVector charVec)
{
	char** res = new char*[charVec.length()];
	for (int i = 0; i < charVec.length(); i++)
		res[i] = STRDUP(as<std::string>(charVec[i]).c_str());
	return res;
}

void free_ansi_char_array(char ** values, int arrayLength)
{
	for (int i = 0; i < arrayLength; i++)
		delete[] values[i];
	delete[] values;
}

double** as_double_ptr_array(NumericMatrix& mat)
{
	int nrow = mat.nrow();
	double** rows = new double*[nrow];
	for (int i = 0; i < nrow; i++)
	{
		rows[i] = &(mat[nrow*i]);
	}
	return rows;
}

double** to_double_ptr_array(const NumericMatrix& mat)
{
	int nrow = mat.nrow();
	int ncol = mat.ncol();
	size_t length = nrow;
	double** columns = new double*[ncol];
	for (int i = 0; i < ncol; i++)
	{
		columns[i] = new double[nrow];
		NumericMatrix::ConstColumn c = mat.column(i);
		std::copy(c.begin(), c.begin() + length, columns[i]);
	}
	return columns;
}

CharacterVector to_r_character_vector(char** names, int size, bool cleanup)
{
	CharacterVector v(size);
	for (int i = 0; i < size; i++)
		v[i] = std::string(names[i]);
	if(cleanup)
		DeleteAnsiStringArray(names, size);
	return v;
}

NumericVector to_r_numeric_vector(double* values, int length, bool cleanup)
{
	NumericVector data(length);
	// std::copy(values, values + length * sizeof(double), data);
	for (int i = 0; i < length; i++)
	{
		data[i] = values[i];
	}
	if (cleanup)
		delete values;
	return data;
}
