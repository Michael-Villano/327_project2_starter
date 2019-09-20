/*
 * functionstocomplete.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: keith
 */

//============================================================================
//	TODO add necessary includes here
//============================================================================
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>

#include "./array_functions.h"
#include "./constants.h"
#include "./utilities.h"


//============================================================================
//	stuff you will need
//============================================================================
//TODO define a structure to track words and number of times they occur

//TODO add a global array of entry structs (global to this file)

//TODO add variable to keep track of next available slot in array

//TODO define all functions in header file

//TODO look in utilities.h for useful functions, particularly strip_unwanted_chars!


using namespace constants;

struct entry {
  std::string word = "";
  int nO = 0; //number of occurrences
};

entry words[MAX_WORDS]; //creates maximum size array of words and their numberof occurances
int next = 0; // holds the next available spot in array to add a word




//zero out array that tracks words and their occurrences
void clearArray() {
  while (next > 0) {
      words[next - 1].word = "";
      words[next-1].nO = 0;
      next = next - 1;
  }

}


//how many unique words are in array
int getArraySize() {
	return next; //returns the next availible spot. if one word is in, next will be 1.
}

//get data at a particular location
std::string getArrayWordAt(int i) {
	return words[i].word;
}
int getArrayWord_NumbOccur_At(int i) {
	return words[i].nO;
}

/*loop through whole file, one line at a time
 * call processLine on each line
 * returns false: myfstream is not open
 *         true: otherwise*/
bool processFile(std::fstream &myfstream) {
	if (!myfstream.is_open()) {
	    return false;
	}
	std::string line = "";
	while (!myfstream.eof()) {
	    getline (myfstream, line);
	    processLine(line);
	}
	return true;
	}

/*take 1 line and extract all the tokens from it
feed each token to processToken for recording*/
void processLine(std::string &myString) {
  using namespace std;
  stringstream ss(myString);
  string tempStr;

  while(getline(ss, tempStr, CHAR_TO_SEARCH_FOR)) {
      processToken(tempStr);
  }

}

/*Keep track of how many times each token seen*/
void processToken(std::string &token) {
  int currentSize = getArraySize();

  //check for bs words
  strip_unwanted_chars(token);
  if (token.empty()) {
      return;
  }


  for (int i = 0; i < currentSize; i++) {
      std::string upperToken = token;
      std::string upperWord = words[i].word;
      toUpper(upperToken);
      toUpper(upperWord);
      if (words[i].word == token || upperToken == upperWord) { //if the word is already in the list, increase the number of occurances and exit the method
	  words[i].nO += 1;
	  return;
      }
  }

   //otherwise add the word to the next available spot in the list and set number of occurrences to 1
  words[next].word = token;
  words[next].nO = 1;
  next++; // move the next available spot in the array up one

}

/*if you are debugging the file must be in the project parent directory
  in this case Project2 with the .project and .cProject files*/
bool openFile(std::fstream& myfile, const std::string& myFileName,
	      std::ios_base::openmode mode /* = std::ios_base::in */) {
  myfile.open(myFileName);
	if (!myfile.is_open()) {
	    return false;
	}
	return true;
}

/*iff myfile is open then close it*/
void closeFile(std::fstream& myfile) {
  if(myfile.is_open()) {
      myfile.close();
  }

}

/* serializes all content in myEntryArray to file outputfilename
 * returns  FAIL_FILE_DID_NOT_OPEN if cannot open outputfilename
 * 			FAIL_NO_ARRAY_DATA if there are 0 entries in myEntryArray
 * 			SUCCESS if all data is written and outputfilename closes OK
 * */
int writeArraytoFile(const std::string &outputfilename) {
	std::ofstream myStream;
	myStream.open(outputfilename);

	if (!myStream.is_open()) {
	    return FAIL_FILE_DID_NOT_OPEN;
	}
	else if (next == 0) {
	    return FAIL_NO_ARRAY_DATA;
	}
	else {
	    for (int i = 0; i < next; i++) {
		myStream << words[i].word << " " << words[i].nO << "\n";
	    }
	    return SUCCESS;
	}
}

/*
 * Sort myEntryArray based on so enum value.
 * You must provide a solution that handles alphabetic sorting (A-Z)
 * The presence of the enum implies a switch statement based on its value
 */
void sortArray(constants::sortOrder so) {
  if (next == 0) { // check to make sure the list isnt empty
      return;
  }

  switch(so) {
    case NONE:
      break;
    case ASCENDING:
      for(int i = 0; i < next; i++)
      	{
      		for(int j = i + 1;j < next; j++)
      		{
      		    std::string temp1 = words[i].word;
      		    std::string temp2 = words[j].word;
      		    toUpper(temp1);
      		    toUpper(temp2);
      			if(temp1 > temp2)
      			{
      				std::string tempStr = words[i].word;
      				int tempN = words[i].nO;
      				words[i].word = words[j].word;
      				words[i].nO = words[j].nO;
      				words[j].word = tempStr;
      				words[j].nO = tempN;
      			}
      		}
      	}
      break;

    case DESCENDING:
          for(int i = 0; i < next; i++)
          	{
          		for(int j = i + 1;j < next; j++)
          		{
          		    std::string temp1 = words[i].word;
          		    std::string temp2 = words[j].word;
          		    toUpper(temp1);
          		    toUpper(temp2);
          			if(temp1 < temp2)
          			{
          				std::string tempStr = words[i].word;
          				int tempN = words[i].nO;
          				words[i].word = words[j].word;
          				words[i].nO = words[j].nO;
          				words[j].word = tempStr;
          				words[j].nO = tempN;
          			}
          		}
          	}
          break;

    case NUMBER_OCCURRENCES:
      for (int i = 0; i < next; i++) {
	  for (int j = i + 1; j < next; j++) {
	      if(words[i].nO < words[j].nO) {
		  std::string tempStr = words[i].word;
		  int tempN = words[i].nO;
		  words[i].word = words[j].word;
		  words[i].nO = words[j].nO;
		  words[j].word = tempStr;
		  words[j].nO = tempN;

	      }
	  }
      }
      break;
  }
}

