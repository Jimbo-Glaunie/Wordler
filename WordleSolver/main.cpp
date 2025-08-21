#include<stdlib.h>
#include<stdio.h>

#include<iostream>
#include <fstream>
#include<vector>
#include<string>

#include "Letter.h"
//#include "wordleJSON.h"

#define size_vec long unsigned int


std::vector<Letter*> alphabet = initialize_alphabet();

std::vector<std::string> words = {};

void record_word_score(std::string word)
{
	for(long unsigned int i = 0; i < word.size(); i++)
	{
		alphabet[word[i]-97]->increment(i);
	}
}

void scan_words(std::string file)
{
	std::ifstream MyReadFile(file);
	std::string myText;
	while (getline (MyReadFile, myText)) {
  		record_word_score(myText);
		words.push_back(myText);
	}
}


bool no_letters_twice(std::string word)
{
	for(long unsigned int i = 0; i < word.size(); i++)
	{
		for(long unsigned int n = i; n < word.size(); n++)
		{
			if(word[n]==word[i]&&i!=n) return false;
		}
	}
	return true;
}

int get_word_score(std::string word)
{
	int score = 0;
	for(long unsigned int i = 0; i < word.size(); i++)
	{
		score+=alphabet[word[i]-97]->position_scores[i];
	}
	if(!no_letters_twice(word))
		score-=200;
	return score;
}


std::string find_first_word()
{
	std::string best = "zzzzz";
	for(long unsigned int w = 0; w < 14855; w++)
	{
		
		if(get_word_score(best) <= get_word_score(words[w]) && no_letters_twice(words[w]))
			best = words[w];
	}
	return best;
}

std::string find_best_guess(std::string prev_guess, std::vector<int> score)
{	
	
	bool contains = false;
	char letter;
	for(size_vec l = 0; l < 5; l++)
	{
		letter = prev_guess[l];
		//score 2 means right letter right place
		if(score[l]==2)
		{
			for(std::vector<std::string>::iterator w = words.begin(); w != words.end();)
			{
				
				if(w->at(l)!=letter)
				{
					w=words.erase(w);
				}else
				{
					++w;
				}
			}
		}
		//score 1 means right letter wrong place
		else if(score[l]==1)
		{
			for(std::vector<std::string>::iterator w = words.begin(); w != words.end();)
			{
				contains = false;
				for(int i = 0; i<5; i++)
				{
					if(w->at(i)==letter)
					{
						if(i==l)
						{
							contains=false;
							break;
						}else
							contains=true;
					}
				}
				if(contains)
					++w;
				else
					w=words.erase(w);
			}	
		}
		//score 0 means wrong letter
		else if(score[l]==0)
		{
			for(std::vector<std::string>::iterator w = words.begin(); w != words.end();)
			{
				contains = false;
				for(size_vec i = 0; i < 5; i++)
				{
					if(w->at(i)==letter)
					{
						w=words.erase(w);
						contains = true;
						break;
					}	
				}
				if(!contains)
					++w;
			}
		}
		//score 3 means that this letter is correct elsewhere in the word and does not repeat.
		else if(score[l]==3)
		{
			for(std::vector<std::string>::iterator w = words.begin(); w != words.end();)
			{
				
				if(w->at(l)==letter)
				{
					w=words.erase(w);
				}else
				{
					++w;
				}
			}
		}
		else
		{
			std::cout << "Error: code not valid\n";
			break;
		}
	}
	
	
	
	

	std::string best = "zzzzz";
	for(std::vector<std::string>::iterator w = words.begin(); w < words.end();++w)
	{
		
		if(get_word_score(best) <= get_word_score(*w))
			best = *w;
	}
	return best;
}

int main()
{
	scan_words("Data/wordleDictionary.txt");
	std::string guess = find_first_word();
	std::vector<int> scores = {0,0,0,0,0};
	bool win = false;
	for(int i = 0; i < 6; i++)
	{
		std::cout << "input score for guess #" << i+1 << ':' << guess + '\n';
		
		std::cout << "#1:";
		std::cin >> scores[0];
		
		std::cout << "#2:";
		std::cin >> scores[1];
		
		std::cout << "#3:";
		std::cin >> scores[2];
		
		std::cout << "#4:";
		std::cin >> scores[3];
		
		std::cout << "#5:";
		std::cin >> scores[4];
		
		guess = find_best_guess(guess,scores);
		
		if(words.size()==1)
		{
			std::cout << "You won on round #" << i+1 << "with word: " << guess << "!!!\nPlay again tommorow.\n";
			break;
		}
			
	}
	free_alphabet(alphabet);
	return 0;
}