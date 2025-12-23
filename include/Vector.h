// Vector.h
// 注意：在模板继承中，若要访问基类（父类）中的成员，需使用 this-> 标志以避免父类（基类）中继承的静态数据成员和成员函数
// 否则会出现错误信息：(if you use `-fpermissive', G++ will accept your code, but allowing the use of an undeclared name is deprecated)
#ifndef VECTOR_H
#define VECTOR_H

#include "Vec.h"

template <typename T> class Vector : public VECTOR<T>
{
public:
	Vector(int size=0, const T *x=NULL); // 构造函数，创建指定大小的向量，可选择初始化数据

	void Input(istream &in); // 从输入流读取向量数据
	void Output(ostream &out) const; // 将向量输出到输出流
	template <typename TYPE> friend Vector<TYPE> operator+(const Vector<TYPE> &v1, const Vector<TYPE> &v2) throw(double); // 向量加法运算符
	template <typename TYPE> friend Vector<TYPE> operator-(const Vector<TYPE> &v1, const Vector<TYPE> &v2) throw(double); // 向量减法运算符
	template <typename U>
	friend Vector<U> operator*(const U &x, const Vector<U> &v); // 标量与向量相乘（标量在前）
	template <typename U>
	friend Vector<U> operator*(const Vector<U> &v, const U &x); // 向量与标量相乘（标量在后）
	Vector<T> & operator+=(const Vector<T> &v) throw(double); // 向量加法赋值运算符
	Vector<T> & operator-=(const Vector<T> &v) throw(double); // 向量减法赋值运算符
	Vector<T> & operator*=(const T &x); // 向量与标量乘法赋值运算符
	T dot(const Vector<T> &v) const throw(double); // 计算点积
	Vector<T> reverse() const; // 反转向量元素顺序
	Vector<T> subvector(int start, int length) const throw(double); // 提取子向量
	Vector<T> pad(int padding, T value) const; // 在向量两端添加填充
};

template <typename T>
Vector<T>::Vector(int size, const T *x) : VECTOR<T>(size, x)		// 构造函数
{
}

template <typename T>
void Vector<T>::Output(ostream &out) const
{
	if(this->num==0)
		out << "()";
	else
	{
		out << "(" << this->p[0];
		for(int i=1; i<this->num; i++)
			out << ", " << this->p[i];
		out << ")";
	}
}

template <typename T>
void Vector<T>::Input(istream &in)	// 该函数自动扩展数组的功能
{
	const int M=1024, N = 1;//1024;	// N取最小值1是为了简化，实际使用时取1024
	Vector<T> temp;
	char str[M], ch;				// ch初始化为一个非')'字符即可
	T buffer[N];
	int i, j, k;

	in.getline(str, M, '(');		// 跳过第'('之前的所有字符
	while(true)					// 跳过第一个括号之后的空白字符
	{
		ch = in.peek();			// 偷看下一个字符是否为空白字符
		if(ch==' ' || ch=='\t' || ch=='\n' || ch=='\r')
			in.get(ch);			// 如果是空白字符，则跳过并继续读取；
		else
			break;				// 直到遇到非空白字符，退出while循环
	}

	if(in.peek()==')')				// 偷看下一个字符是否是')'
	{
		this->resize(0);			// 将当前向量维度设置为 0 维
		return;						// 退出函数，表示当前向量为 0 维向量
	}

	for(k=0; ch!=')'; k++)
	{
		for(i=0; i<N && ch!=')'; i++)
			in >> buffer[i] >> ch;
		temp.resize(k*N+i);			// 调用resize函数实现"动态扩展数组"的功能
		for(j=0; j<i; j++)
			temp[k*N+j] = buffer[j];// 完成数据的动态扩展
	}
	*this = temp;					// 完成赋值操作（深拷贝）
}

template <typename T>
Vector<T> operator+(const Vector<T> &v1, const Vector<T> &v2) throw(double)
{
	if(v1.getsize() != v2.getsize())
		throw -1.0;					// 如果维度不一致"一刀两断"，"矛盾无法调和"，"加法运算"无法处理
	Vector<T> result(v1);
	for(int i=v1.getsize()-1; i>=0; i--)
		result[i] += v2[i];		// 完成数据的动态扩展
	return result;
}

template <typename T>
Vector<T> operator-(const Vector<T> &v1, const Vector<T> &v2) throw(double)
{
	if(v1.getsize() != v2.getsize())
		throw -1.0;					// 如果维度不一致"一刀两断"，"矛盾无法调和"，"加法运算"无法处理
	Vector<T> result(v1);
	for(int i=v1.getsize()-1; i>=0; i--)
		result[i] -= v2[i];		// 完成数据的动态扩展
	return result;
}

template <typename T>
Vector<T> operator*(const T &x, const Vector<T> &v)
{
	Vector<T> result(v);
	for(int i = v.getsize() - 1; i >= 0; i--)
		result[i] = x * v[i];
	return result;
}

template <typename T>
Vector<T> operator*(const Vector<T> &v, const T &x)
{
	return x * v;
}

template <typename T>
Vector<T> & Vector<T>::operator+=(const Vector<T> &v) throw(double)
{
	return *this = *this + v;
}

template <typename T>
Vector<T> & Vector<T>::operator-=(const Vector<T> &v) throw(double)
{
	return *this = *this - v;
}

template <typename T>
Vector<T> & Vector<T>::operator*=(const T &x)
{
	return *this = x * (*this);
}

template <typename T>
T Vector<T>::dot(const Vector<T> &v) const throw(double) {
    if (this->num != v.num) {
        throw -1.0; // Dimension mismatch
    }
    T result = 0;
    for (int i = 0; i < this->num; ++i) {
        result += this->p[i] * v.p[i];
    }
    return result;
}

template <typename T>
Vector<T> Vector<T>::reverse() const {
    Vector<T> result(this->num);
    for (int i = 0; i < this->num; ++i) {
        result[i] = this->p[this->num - 1 - i];
    }
    return result;
}

template <typename T>
Vector<T> Vector<T>::subvector(int start, int length) const throw(double) {
    if (start < 0 || start + length > this->num || length < 0) {
        throw -1.0; // Invalid range
    }
    Vector<T> result(length);
    for (int i = 0; i < length; ++i) {
        result[i] = this->p[start + i];
    }
    return result;
}

template <typename T>
Vector<T> Vector<T>::pad(int padding, T value) const {
    if (padding < 0) {
        throw -1.0; // Invalid padding value
    }
    Vector<T> result(this->num + 2 * padding);
    for (int i = 0; i < padding; ++i) {
        result[i] = value;
        result[result.getsize() - 1 - i] = value;
    }
    for (int i = 0; i < this->num; ++i) {
        result[padding + i] = this->p[i];
    }
    return result;
}

#endif