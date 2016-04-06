#include <iostream>
﻿#include <math.h>
using namespace std;

#define URL "pastebin.com/iLBmKdyH"

class Matrix
{
protected:
	int n;
	int m;
	float* data;
public:
	Matrix();
	Matrix(int m, int n);
	virtual Matrix operator+(Matrix&);
	virtual Matrix operator*(Matrix&);
	virtual Matrix operator*(float&);
	virtual Matrix operator-(Matrix&);
	virtual Matrix reverse();
	virtual Matrix transpose();
	virtual float determinant();
	virtual ostream& print(ostream& o);
	virtual istream& read(istream& o);
	virtual void set(int i, int j, float data);
	virtual float get(int i, int j);
	virtual int getN();
	virtual int getM();
	virtual bool failed();
};

Matrix::Matrix()
{
	n = 1; m = 1;
	data = new float[n*m];
	for (int i = 0; i < n*m; i++)
	{
		data[i] = 1;
	}
}

Matrix::Matrix(int m, int n)
{
	this->n = n; this->m = m;
	data = new float[n*m];
	for (int i = 0; i < n*m; i++)
	{
		data[i] = 1;
	}
}

Matrix Matrix::operator+(Matrix& a)
{
	if (failed()){ return *this; }
	if (this->n == a.getN() && this->m == a.getM())
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				this->data[i*m + j] = this->data[i*m + j] + a.get(i, j);
			}
		}
	}
	return *this;
}

Matrix Matrix::operator*(Matrix& a)
{
	if (failed()){ return *this; }
	Matrix tmp2 = Matrix(this->n, a.m);
	if (this->getM() == a.getN())
	{
		for (int i = 0; i < this->getN(); i++)
		{
			for (int j = 0; j < a.getM(); j++)
			{
				tmp2.data[i*tmp2.m + j] = 0;
				for (int k = 0; k < this->getM(); k++)
				{
					tmp2.data[i*tmp2.m + j] += this->get(i, k) * a.get(k, j);
				}
			}
		}
	}
	return tmp2;
}

Matrix Matrix::operator*(float& num)
{
	if (failed()){ return *this; }
	Matrix tmp = Matrix(this->n, this->m);
	for (int i = 0; i < n*m; i++)
	{
		tmp.data[i] = num * data[i];
	}
	return tmp;
}

Matrix Matrix::operator-(Matrix& a)
{
	if (failed()){ return *this; }
	Matrix tmp = Matrix(this->n, this->m);
	if (tmp.n == a.getN() && tmp.m == a.m)
	{
		for (int i = 0; i < n*m; i++)
		{
			tmp.data[i] = this->data[i] - a.get(i, 0);
		}
	}
	return tmp;
}

Matrix Matrix::reverse()
{
	if (failed()){ return *this; }
	if (n == m)
	{
		if (n == 1) { return *this; }

		if (n == 2)
		{
			float det = this->determinant();
			this->data[0] = data[3] / det;
			this->data[1] = -1 * data[1] / det;
			this->data[2] = -1 * data[2] / det;
			this->data[3] = data[0] / det;
			return *this;
		}

		if (n > 2)
		{
			Matrix tmp1 = Matrix(n, n);
			float det1 = this->determinant();
			for (int l = 0; l < n; l++) //строки
			{
				for (int k = 0; k < n; k++) //столбцы
				{
					Matrix tmp2 = Matrix((n - 1), (n - 1));
					for (int i = 0; i < n - 1; i++) //идёт по строкам
					{
						for (int j = 0; j < n - 1; j++) //идёт по столбцам
						{
							if (i < l && j < k)
							{
								tmp2.data[i*(n - 1) + j] = data[i*n + j];
							}
							if (i < l && j >= k)
							{
								tmp2.data[i*(n - 1) + j] = data[i*n + (j + 1)];
							}
							if (i >= l && j < k)
							{
								tmp2.data[i*(n - 1) + j] = data[(i + 1)*n + j];
							}
							if (i >= l && j >= k)
							{
								tmp2.data[i*(n - 1) + j] = data[(i + 1)*n + (j + 1)];
							}
						}

					}
					tmp1.data[l*n + k] = tmp2.determinant() * powf(-1, l + k) / fabs(det1); //элементы матрицы алгебраических дополнений
				}
			}
			*this = tmp1.transpose();
			return *this;
		}
	}
	else
	{
		cout << "Error" << endl; return *this;
	}
}


Matrix Matrix::transpose()
{
	if (failed()){ return *this; }
	Matrix tmp = Matrix(this->m, this->n);
	for (int i = 0; i < tmp.n; i++)
	{
		for (int j = 0; j < tmp.m; j++)
		{
			tmp.data[i*tmp.m + j] = this->data[j*this->m + i];
		}
	}
	return tmp;
}

float Matrix::determinant()
{
	if (failed()){ return 0; }
	//разложение по последней строке
	float det = 0;
	if (n == m)
	{
		if (n == 1)
		{
			det = data[0];
		}
		else
		{
			if (n == 2)
			{
				det = data[0] * data[3] - data[1] * data[2];
			}
			else
			{
				for (int k = 0; k < n; k++)
				{
					Matrix tmp = Matrix((n - 1), (n - 1));
					for (int i = 0; i < n - 1; i++) //идёт по строкам
					{
						for (int j = 0; j < n - 1; j++) //идёт по столбцам
						{
							if (j < k)
							{
								tmp.data[i*tmp.m + j] = data[i*m + j];
							}
							if (j >= k)
							{
								tmp.data[i*tmp.m + j] = data[i*m + (j + 1)];
							}
						}
					}
					det += powf(-1, (n*(n - 1) + k - 1))*data[n*(n - 1) + k] * tmp.determinant();
				}

			}
		}
		return det;
	}
	else {
		cout << "Error" << endl;
		return 0;
	}
}

ostream& Matrix::print(ostream& o)
{
	for (int i = 0; i < this->n; i++)
	{
		o << endl;
		for (int j = 0; j < this->getM(); j++)
		{
			o << get(i, j) << " ";
		}
	}
	cout << '\n' << endl;
	return o;
}

istream& Matrix::read(istream& o)
{
	for (int i = 0; i < this->n; i++)
	{
		for (int j = 0; j < this->getM(); j++)
		{
			o >> data[i*getM() + j];
		}
		cout << endl;
	}
	cout << '\n' << endl;
	return o;
}

void Matrix::set(int i, int j, float data)
{
	this->data[i*m + j] = data;
}

float Matrix::get(int i, int j)
{
	return data[i*m + j];
}

int Matrix::getN() { return m; }
int Matrix::getM() { return n; }


bool Matrix::failed()
{
	if (n == 0 || m == 0)
	{
		return true;
	}
	return false;
}

Matrix* get_init(int n, int m)
{
	Matrix* a = new Matrix(n, m);

	if (n == 0 || m == 0)
	{
		return NULL;
	}
	else {
		a->read(cin);
		return a;
	}
}

