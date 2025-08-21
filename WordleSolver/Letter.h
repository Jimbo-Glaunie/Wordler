#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<iostream>
#pragma once


class Letter
{
	public:
		int position_scores[5] = {0,0,0,0,0};
		char letter;
		Letter(char l)
		{
			letter=l;
		}
		
		Letter(char l, int scores[5])
		{
			letter=l;
			for(long unsigned int i = 0; i < 5; i++)
			{
				position_scores[i] = scores[i];
			}
		}
	
		void increment(int i)
		{
			position_scores[i]++;
		}
		
		int score_sum()
		{
			return position_scores[0] + position_scores[1] + position_scores[2] + position_scores[3] + position_scores[4];
		}
		
		void toString()
		{
			std::cout << "Letter: "<< letter << " [" << "1st=" << position_scores[0] << ", 2nd=" << position_scores[1] << ", 3rd=" << position_scores[2] << ", 4th=" << position_scores[3] << ", 5th=" 
			<< position_scores[4] << "] Total=" << score_sum() << '\n';
		}
};


std::vector<Letter*> initialize_alphabet()
{
	std::vector<Letter*> alphabet = {};
	for(int i = 97; i < 123; i++)
	{
		alphabet.push_back(new Letter((char) i));
	}
	return alphabet;
}

void free_alphabet(std::vector<Letter*> a)
{
	for(long unsigned int i = 0; i < a.size(); i++)
	{
		delete(a[i]);
	}
}

