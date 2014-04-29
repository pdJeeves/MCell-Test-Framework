#include "data_table.hpp"
#include "../support/average.h"
#include <fstream>
#include <iomanip>

template<>
template<>
void data_table<double>::insert_data<std::vector<std::vector<double> > >(const std::vector<std::vector<double> > & vec)
{
	for(unsigned int i = 0; i < vec.size(); ++i)
	{
		_time.push_back(vec[i][0]);

		for(unsigned int j = 1; j < vec[i].size(); ++j)
		{
			at(j-1).push_back(vec[i][j]);
		}
	}
}

template<>
template<>
void data_table<average>::insert_data<std::map<double, std::vector<average> > >(const std::map<double, std::vector<average> > & vec)
{
	for(auto i = vec.begin(); i != vec.end(); ++i)
	{
		_time.push_back(i->first);
		for(unsigned int j = 0; j < i->second.size(); ++j)
		{
			at(j).push_back(i->second[j]);
		}
	}
}

template<>
template<>
void data_table<average>::insert_data<std::map<double, std::vector<std::vector<double> > > >(const std::map<double, std::vector< std::vector<double> > > & vec)
{
	for(auto i = vec.begin(); i != vec.end(); ++i)
	{
		_time.push_back(i->first);
		for(unsigned int j = 0; j < i->second.size(); ++j)
		{
			at(j).push_back(average(i->second[j]));
		}
	}

	std::ofstream fp(_path);
	if(!fp.good())
	{
		return;
	}

	fp << "# Seconds";
	for(unsigned int i = 0; i < size(); ++i)
	{
		fp << std::setw(21) << at(i).get_name();
	}

	for(unsigned int row = 0; row < _time.size(); ++row)
	{
		fp << "\n" << std::setw(9) << _time[row] << " ";

		for(unsigned int col = 0; col < size(); ++col)
		{
			fp <<" (" << std::setw(6) << at(col)[row].mean
			   << "," << std::setw(6) << at(col)[row].std_dev
			   << "," << std::setw(4) << at(col)[row].samples << ")";
		}

		fp << std::endl;
	}

	fp.flush();
	fp.close();
}
/*

template<>
template<>
string data_table<double>::internal_comparison<equilibrium>(__op op, const vector<double> & col, const equilibrium & compare) const
{
#ifdef DEBUG
	std::cout << "\ndata_table<double>::internal_comparison<equilibrium>:\n\t";
	std::cout << "[" << col[0];
	for(int i = 0; i < 10; ++i)
	{
		std::cout << ", " << col[i] ;
	}
	std::cout << "] == (" << compare.min << ", " << compare.max << ")" << std::endl;
#endif
	average col_avg = average(col);
	double min = col_avg.mean;
	double max = col_avg.mean;
/*
	for(vector<double>::const_iterator i(col.begin()); i != col.end(); ++i)
	{
		min = min < *i? min : *i;
		max = max > *i? max : *i;
	}

	std::ostringstream stream;

	switch(op)
	{
	case __eq__:
		if(!(min >= compare.min && max <= compare.max))
		{
			stream << "The first std. deviation is bounded by (" << min << ", " << max << "), "
					  "it should be within (" << compare.min << ", " << compare.max << ").";
		}
		break;
	case __ne__:
		if(min >= compare.min && max <= compare.max)
		{
			stream << "The first std. deviation is bounded by (" << min << ", " << max << "), "
					  "it should be outside of (" << compare.min << ", " << compare.max << ").";
		}
		break;
	case __lt__:
		if(max >= compare.max)
		{
			stream << "The first std. deviation is bounded by (" << min << ", " << max << "), "
					  "it should be less than " << compare.max << ".";
		}
		break;
	case __le__:
		if(max > compare.max)
		{
			stream << "The first std. deviation is bounded by (" << min << ", " << max << "), "
					  "it should be at most " << compare.max << ".";
		}
		break;
	case __gt__:
		if(min <= compare.min)
		{
			stream << "The first std. deviation is bounded by (" << min << ", " << max << "), "
					  "it should greater than " << compare.min << ".";
		}
		break;
	case __ge__:
		if(min < compare.min)
		{
			stream << "The first std. deviation is bounded by (" << min << ", " << max << "), "
					  "it should be at least " << compare.min << ".";
		}
		break;
	}


	return stream.str();
}


template<>
template<>
string data_table<double>::internal_comparison<average>(__op op, const vector<double> & col, const average & compare) const
{
#ifdef DEBUG
	std::cout << "\ndata_table<double>::internal_comparison<equilibrium>:\n\t";
	std::cout << "[" << col[0];
	for(int i = 0; i < 10; ++i)
	{
		std::cout << ", " << col[i] ;
	}
	std::cout << "] == (" << compare.min << ", " << compare.max << ")" << std::endl;
#endif

	average my_col(col);
	int result = compare.compare(my_col);
	std::ostringstream stream;

	if((op & __lt__) && (result & __gt__))
	{
		stream << " The mean, " << my_col.to_string()
			   << ", is greater than the expected:" << compare.to_string()
			   << " (alpha = " << average::alpha/2 << ").";
	}
	else if((op & __gt__) && (result & __lt__))
	{
		stream << " The mean, " << my_col.to_string()
			   << ", is less than the expected:" << compare.to_string()
			   << " (alpha = " << average::alpha/2 << ").";
	}
	else if((op & __eq__) && !(result & __eq__))
	{
		stream << " The mean, " << my_col.to_string()
			   << ", is different from the expected:" << compare.to_string()
			   << " (alpha = " << average::alpha << ").";
	}
	else if(!(op & __eq__) && (result & __eq__))
	{
		stream << " The mean, " << my_col.to_string()
			   << ", is the same as the expected:" << compare.to_string()
			   << " (alpha = " << average::alpha << ").";
	}

	return stream.str();
}

template<>
template<>
string data_table<double>::internal_comparison<double>(__op op, const vector<double> & col, const double & d) const
{
#ifdef DEBUG
	std::cout << "\ndata_table<double>::internal_comparison<double>:\n\t";
	std::cout << "[" << col[0];
	for(int i = 0; i < 10; ++i)
	{
		std::cout << ", " << col[i] ;
	}
	std::cout << "] == " << d << std::endl;
#endif
	int differences = 0;

	vector<double>::const_iterator i(col.begin());

	std::ostringstream stream;

	for(vector<double>::const_iterator i(col.begin()); differences < 10 && i != col.end(); ++i)
	{
		switch(op)
		{
		case __eq__:
				if(*i != d)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			case __ne__:
				if(*i == d)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			case __lt__:
				if(*i >= d)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			case __le__:
				if(*i > d)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			case __gt__:
				if(*i <= d)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			case __ge__:
				if(*i < d)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			}
	}

	return stream.str();
}

template<>
template<>
string data_table<double>::internal_comparison<double>(__op op, const vector<double> & col, const vector<double> & vec) const
{
#ifdef DEBUG
	std::cout << "\ndata_table<double>::internal_comparison<vector<double> >:\n\t";
	std::cout << "[" << col[0];
	for(int i = 0; i < 10; ++i)
	{
		std::cout << ", " << col[i] ;
	}
	std::cout << "] == [" << vec[0];
	for(int i = 0; i < 10; ++i)
	{
		std::cout << ", " << vec[i] ;
	}
	std::cout << "]" << std::endl;
#endif

	int differences = 0;

	vector<double>::const_iterator i(col.begin());
	vector<double>::const_iterator j(vec.begin());

	std::ostringstream stream;

	do
	{
		switch(op)
		{
		case __eq__:
				if(*i != *j)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			case __ne__:
				if(*i == *j)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			case __lt__:
				if(*i >= *j)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			case __le__:
				if(*i > *j)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			case __gt__:
				if(*i <= *j)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			case __ge__:
				if(*i < *j)
				{
					stream << "difference\n";
					++differences;
				}
				break;
			}


		if(++i == col.end() || ++j == vec.end())
		{
			break;
		}
	} while(differences < 10);

	return stream.str();
}
*/
