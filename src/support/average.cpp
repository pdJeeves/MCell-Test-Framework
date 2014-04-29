#include <boost/math/distributions/students_t.hpp>
#include <iostream>
#include <cstdio>

#include "average.h"
#include "operators.h"

equilibrium::equilibrium()
{
	min = max = 0.0;
}

equilibrium::equilibrium(int mi, int ma)
{
	min = mi;
	max = ma;
	if(max < min)
	{
		min += max;
	}
}

constexpr double alpha = 0.05;

average::average()
{
	mean = 0; std_dev = 0; samples = 0;
}

average::average(double avg, double sd, int count)
{
	mean = avg;
	std_dev = sd;
	samples = count;
}

average::average(const vector<double> & in)
{
	mean = 0;
	std_dev = 0;
	samples = in.size();

	for(auto i(in.begin()); i != in.end(); ++i)
	{
		mean += *i;
	}

	mean /= samples;

	for(auto i(in.begin()); i != in.end(); ++i)
	{
		double t = mean - *i;
		std_dev += t*t;
	}

	std_dev = sqrt(std_dev/samples);
}

average::average(const vector<average> & in)
{
	mean    = 0;
	std_dev = 0;
	samples = 0;

	for(unsigned int i = 0; i < in.size(); ++i)
	{
		mean    += in[i].mean;
		std_dev += (in[i].samples - 1)*in[i].std_dev*in[i].std_dev;
		samples += in[i].samples;
	}

	mean /= samples;
	std_dev = sqrt(std_dev/samples);
}

int average::compare(double cmp) const
{
	return compare(samples - 1,  (mean - cmp) * sqrt(samples - 1) / std_dev);
}

int average::compare(const average & cmp) const
{
	double v;
	double t_stat = mean - cmp.mean;

		double sq0 = std_dev * std_dev;
		double sq1 = cmp.std_dev * cmp.std_dev;
		double var0 = sq0 / samples;
		double var1 = sq1 / samples;

		if(cmp.samples == samples)
		{
			v = cmp.samples + samples - 2;
			t_stat /= sqrt(var0 + var1);
			double sp = sqrt((samples - 1)*sq0 + (cmp.samples - 1)*sq1);
			t_stat /= sp * sqrt(1.0/samples + 1.0/cmp.samples);
		}
		else
		{
			v = var0 + var1;
			v *= v;
			double t0 = (var0*var0)/(samples - 1);
			double t1 = (var1*var1)/(cmp.samples - 1);
			v /= t0 + t1;
			t_stat /= sqrt(var0 + var1);
		}

	return compare(v, t_stat);
}

int average::compare(double v, double t_stat) const
{
	int outs = 0;

	if(v > 0)
	{
	boost::math::students_t dist(v);
	if(cdf(complement(dist, fabs(t_stat))) < (alpha/2))
	{
		outs |= __eq__;
	}

   if(cdf(dist, t_stat) > alpha)
   {
	   outs |= __lt__;
   }
   else if(cdf(complement(dist, t_stat)) > alpha)
   {
	   outs |= __gt__;
   }

   return outs;
}
	return -1;
}

average average::operator+(const average & a) const
{
	return average((mean * samples + a.mean * a.samples) / (samples + a.samples) ,
			      sqrt(std_dev*std_dev + a.std_dev * a.std_dev), samples+a.samples);
}

average average::operator-(const average & a) const
{
	return average((mean * samples - a.mean * a.samples) / (samples - a.samples) ,
			       sqrt(std_dev*std_dev - a.std_dev * a.std_dev), samples-a.samples);
}

average average::operator*(const average & a) const
{
	return average(mean*a.mean, std_dev*a.std_dev, samples*a.samples);
}

average average::operator/(const average & a) const
{
	return average(mean/a.mean, std_dev/a.std_dev, samples/a.samples);
}

average average::operator+(const double & a) const
{
	return average(mean + a/(samples+1),
			      sqrt((std_dev * std_dev * samples + (a - (mean + a/(samples+1))))/(samples+1)),
			       samples + 1);
}

average average::operator-(const double & a) const
{
	return average(mean - a/(samples-1),
			       sqrt((std_dev * std_dev * samples - (a - (mean - a/(samples-1))))/(samples-1)),
			       samples - 1);
}

average operator-(double d, const average & a)
{
	return average(d - a.mean, a.std_dev, a.samples);
}

average average::operator*(const double & a) const
{
	return average(mean * a, std_dev * a, samples);
}

average average::operator/(const double & a) const
{
	return average(mean / a, std_dev / a, samples);
}

average operator/(double d, const average & a)
{
	return average(d / a.mean, d / a.std_dev, a.samples);
}






string average::to_string() const
{
	char BUFFER[64];
	if(samples == 1000)
	{
		sprintf(BUFFER, "(%g, %g)", mean, std_dev);
	}
	else
	{
		sprintf(BUFFER, "(%g, %g, %d)", mean, std_dev, samples);
	}

	return std::string(BUFFER);
}

