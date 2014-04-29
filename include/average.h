#ifndef _average_h_
#define _average_h_
#include <string>
#include <vector>
#include <ostream>
#include "operators.h"

#ifndef _data_table_h_
#include "data_table.hpp"
#endif

using std::vector;
using std::string;

class average
{
private:
	double mean;
	double std_dev;
	int samples;

	int compare(double v, double t_stat) const;

public:
	average();
	average(double avg, double std_dev, int count = 1000);
	average(const vector<double> &);

//non-template functions
	static data_column<average> list_from_table(const data_table<double> & it);
	static average 				create(double m, double s)
	{
		return average(m, s);
	}
	static average				create_from_col(const data_column<double> & it)
	{
		return average(it);
	}
	average operator+(const average & a) const;
	average operator-(const average & a) const;
	average operator-()                  const { return average(-mean, std_dev, samples); };
	average operator+(double a)  const;
	average operator-(double a)  const;
	average operator*(double a)  const;
	average operator/(double a)  const;

	std::string to_string() const;
	friend std::ostream& operator<< (std::ostream& stream, const average& avg);
	friend average operator/(double d, const average & a);
	friend average operator-(double d, const average & a);

//need these to interface with python
	double get_mean()    const { return mean; }
	double get_std_dev() const { return std_dev; }
	double get_variance() const { return std_dev*std_dev; }
	long get_samples() const { return samples; }
	template<typename T> average add(T t) const { return *this + t; }
	template<typename T> average sub(T t) const { return *this - t; }
	template<typename T> average rsub(T t) const { return t - *this; }
	average mul(double t) const { return *this * t; }
	average div(double t) const { return *this / t; }
	average rdiv(double t) const { return t / *this; }

	template<typename T> bool equals(T t)        const { return __eq__ & compare(t);    }
	template<typename T> bool not_equals(T t)    const { return !(__eq__ & compare(t)); }
	template<typename T> bool less_than(T t)     const { return __lt__ & compare(t);    }
	template<typename T> bool less_equal(T t)    const { return __le__ == compare(t);   }
	template<typename T> bool greater_than(T t)  const { return __gt__ & compare(t);    }
	template<typename T> bool greater_equal(T t) const { return __ge__ == compare(t);   }	

	int compare(double ) const;
	int compare(const average & ) const;

	bool operator<(double d) const 
	{ 
		return (mean + std_dev) < d;
	}
 	friend bool operator<(double d, const average & avg)
	{
		return d < (avg.mean - avg.std_dev);
	}

	bool operator<(const average & d) const 
	{ 
		return mean < d.mean;
	}

};

inline average operator+(double d, const average & a) { return a + d; }
inline average operator*(double d, const average & a) { return a * d; }

#endif

