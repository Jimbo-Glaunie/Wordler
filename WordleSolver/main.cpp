#include<stdlib.h>
#include<stdio.h>

#include<iostream>
#include <fstream>
#include<vector>
#include<string>
#include<mutex>
#include<thread>


#include "Letter.h"

#define size_vec long unsigned int


std::vector<Letter*> alphabet = initialize_alphabet();

std::vector<std::string> words = {};

std::mutex mutie;

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

void threaded_elimination(char letter, int code, int position)
{
	switch(code)
	{
		//score 0 means wrong letter
		case 0:
			for(size_vec i = 0; i < words.size(); i++)
			{
				if(words[i].find(letter)!=std::string::npos)
					words[i]="zzzzz";
			}
			break;
		//score 1 means right letter wrong place
		case 1:
			for(size_vec i = 0; i < words.size(); i++)
			{
				if(words[i][position]==letter||words[i].find(letter)==std::string::npos)
					words[i]="zzzzz";	
			}
			break;
		//score 2 means right letter right place
		case 2:
			for(size_vec i = 0; i < words.size(); i++)
			{		
				if(words[i][position]!=letter)
					words[i]="zzzzz";
			}
			break;
		//score 3 means that this letter is correct elsewhere in the word and does not repeat.
		case 3:
			for(size_vec i = 0; i < words.size(); i++)
			{
				if(words[i][position]==letter)
					words[i]="zzzzz";	
			}
			break;
	}
				

}

std::string find_best_guess(std::string prev_guess, std::vector<int> score)
{	
	std::thread t0(threaded_elimination,prev_guess[0],score[0],0);
	std::thread t1(threaded_elimination,prev_guess[1],score[1],1);
	std::thread t2(threaded_elimination,prev_guess[2],score[2],2);
	std::thread t3(threaded_elimination,prev_guess[3],score[3],3);
	std::thread t4(threaded_elimination,prev_guess[4],score[4],4);
	
	t0.join();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	
	for(std::vector<std::string>::iterator w = words.begin(); w != words.end();)
	{			
		if(w->compare("zzzzz")==0)
		{
			w=words.erase(w);
		}else{
			++w;
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
		
		if(scores[0]==2&&scores[1]==2&&scores[2]==2&&scores[3]==2&&scores[4]==2)
        {
            std::cout << "You won on round #" << i+1 << " with word: " << guess << "!!!\nPlay again tommorow.\n";
			break;
        }
		
		guess = find_best_guess(guess,scores);
		
		if(words.size()==1)
		{
			std::cout << "You won on round #" << i+2 << " with word: " << guess << "!!!\nPlay again tommorow.\n";
			break;
		}
			
	}
	free_alphabet(alphabet);
	return 0;
}
