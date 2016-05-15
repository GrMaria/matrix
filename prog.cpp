#include <iostream>
using namespace std;

#define URL "pastebin.com/iLBmKdyH"

class Matrix
{
protected:
	int n;
	int m;
	float* data;
public:
	/*
	Matrix();
	Matrix(int m, int n);
	Matrix(const Matrix&);
	~Matrix();
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
	*/

	//m - ñòîëáûстолбыstolby
	//n - ñòðîêèстрокиstroki



	Matrix()
	{
		n = 1; m = 1;
		data = new float[n*m];
		for (int k = 0; k < n*m; k++)
		{
			data[k] = 1;
		}
	}

	Matrix(int m, int n)
	{
		this->n = n; this->m = m;
		data = new float[n*m];
		for (int k = 0; k < n*m; k++)
		{
			data[k] = 1;
		}
	}


	Matrix(const Matrix& a)
	{
		n = a.n; m = a.m;
		data = new float[n*m];
		for (int k = 0; k < n*m; k++)
		{
			data[k] = a.data[k];
		}
	}

	~Matrix()
	{
		delete data;
	}

	
	Matrix operator= (const Matrix& a)
	{
		if (failed()){ return Matrix(*this); }
		delete data;
		n = a.n; m = a.m;
		data = new float[n * m];
		for (int k = 0; k < n*m; k++)
		{
			data[k] = a.data[k];
		}
		return Matrix(*this);
	}

	Matrix operator+ (Matrix& a)
	{
		if (failed()){ return Matrix(*this); }
		if (n == a.n && m == a.m)
		{
			Matrix tmp = Matrix(m, n);
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < n; j++)
				{
					tmp.set(i, j, this->get(i, j) + a.get(i, j));
				}
			}
			return Matrix(tmp);
		}
		else{ return (*this); }
	}

	Matrix operator- (Matrix& a)
	{
		if (failed()){ return Matrix(*this); }
		if (n == a.n && m == a.m)
		{
			Matrix tmp = Matrix(m, n);
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < n; j++)
				{
					tmp.set(i, j, this->get(i, j) - a.get(i, j));
				}
			}
			return Matrix(tmp);
		}
		else{ return (*this); }
	}

	Matrix operator* (float& c)
	{
		if (failed()){ return Matrix(*this); }
		Matrix tmp = Matrix(m, n);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				tmp.set(i, j, this->get(i, j)*c);
			}
		}
		return Matrix(tmp);
	}

	Matrix operator* (Matrix& a)
	{
		if (failed()){ return Matrix(*this); }
		if (m == a.n)
		{
			Matrix tmp = Matrix(a.m, n);
			for (int i = 0; i < tmp.m; i++)
			{
				for (int j = 0; j < tmp.n; j++)
				{
					tmp.set(i, j, 0);
					for (int k = 0; k < this->m; k++)
					{
						tmp.data[j*tmp.m + i] += this->get(k, j) * a.get(i, k);
					}
				}
			}
			return Matrix(tmp);
		}
		else{ return (*this); }
	}

	Matrix reverse()
	{
		if (failed()){ return Matrix(*this); }
		if (n == m)
		{
			if (n == 1) { return Matrix(*this); }

			if (n == 2)
			{
				float det = this->determinant();
				if (det == 0) { return Matrix(*this); }
				float tmp = data[0];
				this->data[0] = data[3] / det;
				this->data[1] = -1 * data[1] / det;
				this->data[2] = -1 * data[2] / det;
				this->data[3] = tmp / det;
				return Matrix(*this);
			}

			if (n > 2)
			{
				Matrix tmp1 = Matrix(n, n);
				float det1 = this->determinant();
				if (det1 == 0) { return Matrix(*this); }
				for (int l = 0; l < n; l++) //ñòðîêè
				{
					for (int k = 0; k < n; k++) //ñòîëáöû
					{
						Matrix tmp2 = Matrix((n - 1), (n - 1));
						for (int j = 0; j < n - 1; j++) //èä¸ò ïî ñòðîêàì
						{
							for (int i = 0; i < n - 1; i++) //èä¸ò ïî ñòîëáöàì
							{
								if (j < l && i < k)
								{
									tmp2.set(j, i, get(j, i));
								}
								if (j < l && i >= k)
								{
									tmp2.set(j, i, get(j, i + 1));
								}
								if (j >= l && i < k)
								{
									tmp2.set(j, i, get(j + 1, i));
								}
								if (j >= l && i >= k)
								{
									tmp2.set(j, i, get(j + 1, i + 1));
								}
							}

						}
						tmp1.set(k, l, tmp2.determinant() * powf(-1, l + k) / det1);//ýëåìåíòû ìàòðèöû àëãåáðàè÷åñêèõ äîïîëíåíèé
					}
				}

				//tmp1.transpose();
				return Matrix(tmp1.transpose());
			}
		}
		else
		{
			cout << "Error" << endl; return Matrix(*this);
		}
	}

	Matrix transpose()
	{
		Matrix tmp = Matrix(n, m);
		for (int i = 0; i < tmp.m; i++)
		{
			for (int j = 0; j < tmp.n; j++)
			{
				tmp.set(i, j, this->get(j, i));
			}
		}
		return Matrix(tmp);
	}

	float determinant()
	{
		if (failed()){ return 0; }
		//ðàçëîæåíèå ïî ïîñëåäíåé ñòðîêå
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
						for (int j = 0; j < n - 1; j++) //èä¸ò ïî ñòðîêàì
						{
							for (int i = 0; i < n - 1; i++) //èä¸ò ïî ñòîëáöàì
							{
								if (j < k)
								{
									tmp.set(j, i, this->get(j, i));
								}
								if (j >= k)
								{
									tmp.set(j, i, this->get(j + 1, i));
								}
							}
						}
						det += powf(-1, (n - 1 + k))*this->get(k, n - 1) * tmp.determinant();
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

	ostream& print(ostream& o)
	{
		for (int j = 0; j < this->n; j++)
		{
			o << endl;
			for (int i = 0; i < this->m; i++)
			{
				o << get(i, j) << " ";
			}
		}
		cout << '\n' << endl;
		return o;
	}

	istream& read(istream& is)
	{
		delete data;
		int N; int M;

		cout << "n: "; is >> N; n = N; cout << endl;
		cout << "m: "; is >> M; m = M; cout << endl;
		data = new float[n*m];

		for (int i = 0; i < this->m*this->n; i++)
		{
			is >> data[i];
		}
		cout << '\n' << endl;
		return is;
	}

	void set(int i, int j, float data) { this->data[j*m + i] = data; }

	float get(int i, int j) { return data[j*m + i]; }

	int getN() { return n; }

	int getM() { return m; }

	bool failed()
	{
		if (n == 0 || m == 0)
		{
			return true;
		}
		return false;
	}
};




Matrix* get_init(int n, int m)
{
	Matrix* a = new Matrix(n, m);
	return a;
}
/*
int main()
{
Matrix a = Matrix(3, 3);
a.read(cin);
a.print(cout);
a = a.reverse();
a.print(cout);
system("pause");
return 0;
}

*/