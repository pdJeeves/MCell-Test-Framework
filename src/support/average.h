#ifndef _average_h_
#define _average_h_
#include "../data_tables/data_table.hpp"
#include <string>
#include <vector>

using std::vector;
using std::string;

class equilibrium
{
public:
	equilibrium();
	equilibrium(int min, int max);
	double min;
	double max;

	template<class T>
	bool does_contain(T t);
	template<class T>
	bool does_contain_column(T t);
	template<class T>
	bool does_contain_table(const data_table<T> & t);
	template<class T>
	bool doesnt_contain(T t)
	{
		return !does_contain(t);
	}

	template<bool T, typename S>
	expression get_expression(S s)
	{
		return expression(bind(&equilibrium::does_contain, *this, T, s),
			              bind(&equilibrium::does_contain, *this, T, s));
	}

	template<bool T, typename S>
	expression get_column_expression(S s)
	{
		return expression(bind(&equilibrium::does_contain_column, *this, T, s),
			              bind(&equilibrium::does_contain_column, *this, T, s));
	}

	template<bool T, typename S>
	expression get_table_expression(S s)
	{
		return expression(bind(&equilibrium::does_contain_table, *this, T, s),
				          bind(&equilibrium::does_contain_table, *this, T, s));
	}
};

class average
{
public:
	average();
	average(double avg, double std_dev, int count = 1000);
	average(const vector<double> &);
	average(const vector<average> &);
	template<typename T>
	average(const data_table<T> & vec)
	{
		mean = 0;
		std_dev = 0;
		samples = 0;

		auto temp = data_column<average>("", "", "");

		for(int i = 0; i < vec.size(); ++i)
		{
			temp.push_back(average(vec[i]));
		}

		average t2 = average(temp);

		mean    = t2.mean;
		std_dev = t2.std_dev;
		samples = t2.samples;
	}

	double mean;
	double std_dev;
	int samples;

	int compare(double ) const;
	int compare(const average & ) const;
	int compare(const equilibrium & ) const;

	int compare(double v, double t_stat) const;
	average operator+(const average & a) const;
	average operator-(const average & a) const;
	average operator*(const average & a) const;
	average operator/(const average & a) const;

	average operator+(const double & a) const;
	average operator-(const double & a) const;
	average operator*(const double & a) const;
	average operator/(const double & a) const;

	string to_string() const;
};

inline
average operator+(double d, const average & a)
{
	return a + d;
}

inline
average operator*(double d, const average & a)
{
	return a * d;
}

extern
average operator/(double d, const average & a);
extern
average operator-(double d, const average & a);


#endif
