#ifndef __COMPARATOR_HPP__
#define __COMPARATOR_HPP__

class Comparator
{
public:
	virtual bool compare(float left, float right) = 0;
};

class LessComparator : public Comparator
{
public:
	bool compare(float left, float right){ return left < right; }
};

class MoreComparator : public Comparator
{
public:
	bool compare(float left, float right){ return left > right; }
};

class EqualComparator : public Comparator
{
public:
	bool compare(float left, float right){ return left == right; }
};

#endif //__COMPARATOR_HPP__