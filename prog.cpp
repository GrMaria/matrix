#include <iostream>
#include <math.h>

using namespace std;

class AbstractMatrix
{
protected:
	int n;
	int m;
	float* data;
public:

	//  AbstractMatrix();
	//  AbstractMatrix(int m, int n);
	virtual AbstractMatrix& operator+(AbstractMatrix&) = 0;
	virtual AbstractMatrix& operator*(AbstractMatrix&) = 0;
	virtual AbstractMatrix& operator*(float&) = 0;
	virtual AbstractMatrix& operator-(AbstractMatrix&) = 0;
    virtual AbstractMatrix& reverse() = 0;
	virtual AbstractMatrix& transpose() = 0;
	virtual float determinant() = 0;
	virtual ostream& print(ostream& o) = 0;
	virtual istream& read(istream& o) = 0;
	virtual void set(int i, int j, float data) = 0;
	virtual float get(int i, int j) = 0;
	virtual int getN() = 0;
	virtual int getM() = 0;
	virtual bool failed() = 0;
};

class M : public AbstractMatrix
{
public:

	M()
	{
		n = 1; m = 1;
		data = new float[n*m];
		for (int i = 0; i < n*m; i++)
		{
			data[i] = rand() % 10;
		}
	}

	M(int a, int b)
	{
		n = a; m = b;
		data = new float[n*m];
		for (int i = 0; i < n*m; i++)
		{
			data[i] = rand() % 10;
		}
	}

	M(int a, int b, float* info)
	{
		n = a; m = b;
		data = new float[n*m];
		for (int i = 0; i < n*m; i++)
		{
			data[i] = info[i];
		}
	}

	M(const M& a1)
	{
		n = a1.n; m = a1.m;
		data = new float[n*m];
		for (int i = 0; i < n*m; i++)
		{
			data[i] = a1.data[i];
		}
	}

	~M()
	{
		delete[] data;
	}

	int getN() { return n; }
	int getM() { return m; }

	AbstractMatrix& operator+(AbstractMatrix& a)
	{
		M tmp = M(this->n, this->m);
		if (tmp.n == a.getN() && tmp.m == a.getM())
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < m; j++)
				{
					tmp.data[i*m + j] = this->data[i*m + j] + a.get(i, j);
					cout << tmp.data[i*m + j] << endl;
				}
			}
		}
		return M(tmp);
	}

	AbstractMatrix& operator*(AbstractMatrix& a)
	{
		M tmp1 = M(*this);
		M tmp2 = M(tmp1.getN(), a.getM());
		if (tmp1.getM() == a.getN())
		{
			for (int i = 0; i < tmp1.getN(); i++)
			{
				for (int j = 0; j < a.getM(); j++)
				{
					tmp2.data[i*tmp2.m + j] = 0;
					for (int k = 0; k < tmp1.getM(); k++)
					{
						tmp2.data[i*tmp2.m + j] += tmp1.get(i, k) * a.get(k, j);
					}
				}
			}
		}
		return M(tmp2);
	}

	AbstractMatrix& operator*(float& num)
	{
		M tmp = M(this->n, this->m);
		for (int i = 0; i < n*m; i++)
		{
			tmp.data[i] = num * data[i];
		}
		return M(tmp);
	}

	AbstractMatrix& operator-(AbstractMatrix& a)
	{
		M tmp = M(this->n, this->m);
		if (tmp.n == a.getN() && tmp.m == a.getM())
		{
			for (int i = 0; i < n*m; i++)
			{
				tmp.data[i] = this->data[i] - a.get(i, 0);
			}
		}
		return M(tmp);
	}

	AbstractMatrix& reverse()
	{
		if (n == m)
		{
			if (n == 1) {return *this;}
			if (n == 2)
			{
				M tmp0 = M(n, n);
				tmp0.data[0] = data[3] / this->determinant();
				tmp0.data[1] = -1*data[1] / this->determinant();
				tmp0.data[2] = -1*data[2] / this->determinant();
				tmp0.data[3] = data[0] / this->determinant();
				return M(tmp0);
			}
			else
			{
				M tmp1 = M(n, n);
				for (int l = 0; l < n; l++)
				{
					for (int k = 0; k < n; k++)
					{
						M tmp2 = M((n - 1), (n - 1));
						for (int i = 0; i < n - 1; i++) //идёт по строкам
						{
							for (int j = 0; j < n - 1; j++) //идёт по столбцам
							{
								if (i < l && j < k) 
								{
									tmp2.data[i*tmp2.m + j] = data[i*m + j];
								}
								if (i < l && j >= k) 
								{
									tmp2.data[i*tmp2.m + j] = data[i*m + (j+1)];
								}
								if (i >= l && j < k) 
								{
									tmp2.data[(i+1)*tmp2.m + j] = data[i*m + j];
								}
								if (i >= l && j >= k) 
								{
									tmp2.data[(i+1)*tmp2.m + j] = data[(i+1)*m + (j+1)];
								}
							}
						}
						tmp1.data[l*n + k] = tmp2.determinant() / abs(tmp1.determinant());
					}
				}
				(tmp1.transpose()).print(cout);
				return M(tmp1);
			}

		}
		else
		{cout << "Error" << endl; return M(*this);}
	}

	AbstractMatrix& transpose()
	{
		M tmp = M(this->m, this->n);
		for(int i = 0; i < tmp.n; i++)
		{
			for(int j = 0; j < tmp.m; j++)
			{
				tmp.data[i*tmp.m + j] = this->data[j*this->m + i];
			}
		}
		return M(tmp);
	}


	float determinant()
	{
		//разложение по последней строке
		float det = 0;
		if (n == m)
		{
			if (n == 1)
			{det = data[0];}
			else
			{
				if (n == 2)
				{det = data[0]*data[3] - data[1]*data[2];}
				else
				{
					for (int k = 0; k < n; k++)
					{
						M tmp = M((n - 1), (n - 1));
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
									tmp.data[i*tmp.m + j] = data[i*m + (j+1)];
								}
							}
						}
						det += powf(-1, (n*(n-1) + k-1))*data[n*(n-1) + k]*tmp.determinant();
					}

				}
			}
			return det;
		}
		else {
			cout << "Error" << endl;
			return 0;}
	}


	ostream& print(ostream& o)
	{	
		for (int i = 0; i < this->getN(); i++)
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

	istream& read(istream& o)
	{
		for (int i = 0; i < this->getN(); i++)
		{
			cout << endl;
			for (int j = 0; j < this->getM(); j++)
			{
				o >> data[i*getM() + j];
				cout << " ";
			}
		}
		cout << '\n' << endl;
		return o;
	}


	void set(int i, int j, float data)
	{
		this->data[i*m + j] = data;
	}

	float get(int i, int j)
	{
		return data[i*m + j];
	}

	bool failed()
	{
		if (n == 0 && m == 0)
		{
			return true;
		}
		else {return false;};
	}

	void operator= (AbstractMatrix& a)
	{
		this->n = a.getN(); this->m = a.getM(); 
		for (int i = 0; i < this->n; i++)
		{
			for (int j = 0; j < this->m; j++)
			{
				this->data[i*this->m + j] = a.get(i, j);
			}
		}
	}
};

AbstractMatrix* get_init()
{
	M* s = new M(1, 1);
	return s;
}