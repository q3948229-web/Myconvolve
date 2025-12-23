// Vec.h
#ifndef VEC_H
#define VEC_H
#include <iostream>
using namespace std;

template <typename T> class VECTOR			// 定义向量类模板（将所有成员函数的实现全部写在头文件中）
{
public:
	VECTOR(int size=0, const T *x=NULL);	// 1. 构造函数，包括默认的构造函数和转换构造函数
	VECTOR(const VECTOR &v);				// 2. 拷贝构造函数（实现深拷贝）
	virtual ~VECTOR();						// 3. 析构函数
	VECTOR & operator=(const VECTOR &v);	// 4. 赋值运算符（实现深拷贝）
	T & operator[](int index) const throw(int);
					// 重载下标运算符（支持取值和赋值）；下标越界时抛出异常
	int getsize() const;					// 获取向量的维度（保护成员变量）
	void resize(int size);					// 动态调整向量的维度（保留原有数据）

	virtual void Output(ostream &out) const = 0;// 输出函数，供派生类实现具体的输出逻辑
	virtual void Input(istream &in) = 0;		// 输入函数，供派生类实现具体的输入逻辑

protected:								// 受保护的成员变量和方法
	int num;								// 向量的维度
	T *p;									// 指针（占sizeof(void*)字节，约4字节，动态分配空间）
};

template <typename T>
VECTOR<T>::VECTOR(int size, const T *x)	// 1. 构造函数，包括默认的构造函数和转换构造函数
{
	num = (size>0) ? size : 0;
	p = NULL;
	if(num>0)
	{
		p = new T[num];
		for(int i=0; i<num; i++)
			p[i] = (x==NULL)? T() : x[i];
	}
}

template <typename T>
VECTOR<T>::VECTOR(const VECTOR<T> &v)	// 2. 拷贝构造函数（实现深拷贝）
{
	num = 0;
	p = NULL;
	*this = v;
}

template <typename T>
VECTOR<T>::~VECTOR()						// 3. 析构函数
{
	num = 0;
	if(p!=NULL) delete [] p;
}

template <typename T>
VECTOR<T> & VECTOR<T>::operator=(const VECTOR<T> &v)// 4. 赋值运算符（实现深拷贝）
{
	if(num!=v.num)
	{
		if(p!=NULL) delete [] p;
		p = new T[num=v.num];
	}
	for(int i=0; i<num; i++)
		p[i] = v.p[i];
	return *this;
}

template <typename T>
T & VECTOR<T>::operator[](int index) const throw(int)// 重载下标运算符（支持取值和赋值）
{
	if(p==NULL)
		throw 0;							// 抛出空指针异常
	if(index<0 || index>=num)
		throw -1;
	return p[index];
}

template <typename T>
int VECTOR<T>::getsize() const				// 获取向量的维度（保护成员变量）
{
	return num;
}

template <typename T>
void VECTOR<T>::resize(int size)			// 动态调整向量的维度（保留原有数据）
{
	if(size<0 || size==num)
		return;
	else if(size==0)
	{
		if(p!=NULL) delete [] p;
		p = NULL;
		num = 0;
	}
	else
	{
		T *temp = p;
		p = new T[size];
		for(int i=0; i<size; i++)
			p[i] = (i<num) ? temp[i] : T();	// 动态扩展原有数据
		num = size;
		delete [] temp;	
	}
}

template <typename T>
ostream & operator<<(ostream &out, const VECTOR<T> &v)	// 输出向量元素
{
	v.Output(out);
	return out;
}

template <typename T>
istream & operator>>(istream &in, VECTOR<T> &v)			// 输入向量元素
{
	v.Input(in);
	return in;
}

#endif
