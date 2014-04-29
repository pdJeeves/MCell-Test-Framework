#ifndef _range_hpp_
#define _range_hpp_
#include "data_column.hpp"
#include "b_string.h"

class TestRange
{
private:
	double _min;
	double _max;

	b_string get_b_string(std::stringstream & ret, bool test_type)
	{
		if(test_type)
		{
			ret << ", should be between ";
		}
		else
		{
			ret << ", should be outside of ";
		}
		ret << _min << " and " << _max;
		return b_string(ret.str());
	}

public:
	TestRange(double min, double max)
	{
		_min = min;
		_max = max;
	}

	static TestRange CreateDistribution(double value, double tolerance)
	{
		return TestRange(value-tolerance, value+tolerance);
	}

	template<typename T, bool test_type>
	b_string does_contain(T s)
	{
		if((_min < s && s < _max) != test_type)
		{
			std::stringstream ret;
			return get_b_string(ret, test_type);
		}

		return b_string();
	}

	template<typename T, bool test_type>
	b_string does_contain_vec(const data_column<T> & vec)
	{
		T min, max;
		min = max = vec[0];
		for(int i = 0; i < vec.size(); ++i)
		{
			min = vec[i] < min? vec[i] : min;
			max = max < vec[i]? vec[i] : max;
		}

		bool temp = (_min < min && max < _max);
		if((_min < min && max < _max) != test_type)
		{
			std::stringstream ret;
			return get_b_string(ret, test_type);
		}

		return b_string();
	}
};




#endif

