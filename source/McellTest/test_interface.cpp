#include "test_interface.h"

constexpr int InsertCost        = 4;
constexpr int SubstitutionCost  = 3;
constexpr int CaseChangeCost    = 1;
constexpr int TranspositionCost = 2;

static int minimum(int i, int j)
{
	return i < j? i : j;
}

static int minimum(int i, int j, int k)
{
	return minimum(minimum(i, j), k);
}

static int minimum(int i, int j, int k, int l)
{
	return minimum(minimum(i, j), minimum(k, l));
}

int least_edit_distance(const std::string & string1, const std::string & string2)
{
typedef unsigned int uint;
	unsigned short ** table;
	table = new unsigned short*[string1.size()+1];

	for(uint i = 0; i <= string1.size(); ++i)
	{
		table[i] = new unsigned short[string2.size()+1];
		table[i][0] = i*InsertCost;
	}
	for(uint i = 0; i <= string2.size(); ++i)
	{
		table[0][i] = i*InsertCost;
	}

	for(uint i = 1; i <= string1.size(); ++i)
	{
		for(uint j = 1; j <= string2.size(); ++j)
		{
			int sub_cost = 0;
			if(string1[i-1] != string2[j-1])
			{
				sub_cost = (string1[i-1] | 32) == (string2[j-1] | 32)
				?
					CaseChangeCost
				:
					SubstitutionCost
				;
			}


			table[i][j] = minimum
			(
				table[i-1][j  ] + InsertCost,
				table[i  ][j-1] + InsertCost,
				table[i-1][j-1] + sub_cost
			);

			if(sub_cost == 0)
			{
				continue;
			}

			if(i > 1 && j > 1
			&& string1[i-1] == string2[j-2]
			&& string1[i-2] == string2[j-1])
			{
				table[i][j] = minimum
				(
					table[i][j],
					table[i-2][j-2] + TranspositionCost
				);
			}
		}
	}

	int cost = table[string1.size()][string2.size()];
	for(uint i = 0; i <= string1.size(); ++i)
	{
		delete [] table[i];
	}
	delete [] table;

	return cost;
}
