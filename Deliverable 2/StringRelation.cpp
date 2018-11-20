#include "StringRelation.h"

//will be used for calling the ColourText() method in print() functions
SetOfStrings *S = new SetOfStrings();

//This helps by assigning the composition member
//s is the pointer to the input set
//and R \subseteq s \times s
void StringRelation::setInput(SetOfStrings *s)
{
	set1 = s;
}

//check if the relation provided as input is a
//valid relation i.e., R \subseteq S \times S
//Return true when valid; false otherwise
bool StringRelation::isValid()
{
	//Create the product of set1 with itself
	//out= set1 /times set1
	SetOfStrings *out = new SetOfStrings();
	out = set1->product(set1);
	//Overloaded printRelation method used for printing the relation that is
	//equal to the product set
	if (out->subset(this))
		return true;
	else
		return false;
}

//This is one of the main helper function provided as a
//tokenizer to parse relational elements and separate them into components
//This takes a relation element in the form "a,b" and returns
//a string vector containing "a" and "b"
vector<string> StringRelation::makeTokens(string next)
{
	vector<string> out;
	vector<string>::iterator p;
	char *a = new char[next.length() + 1];
	strcpy(a, next.c_str());
	char *token = std::strtok(a, ",");
	int index = 0;
	while (token != NULL)
	{
		//std::cout << "The next token is: "<< token << '\n';
		out.push_back(token);
		token = std::strtok(NULL, ",");
		index++;
	}
	return out;
}

//helper function that returns the string "a,a" when input is "a"
string StringRelation::createReflexiveElement(string s)
{
	string next, next1;
	next = s;
	next1 = next;
	next.append(",");
	next.append(next1);
	//cout<<"The reflexive element created"<<next<<endl;
	return next;
}

//helper function that creates a dual of "a,b" as "b,a"
string StringRelation::computeDual(string s)
{

	vector<string> tokens = makeTokens(s);
	vector<string>::iterator p;
	p = tokens.begin();
	string other, other1;
	while (p != tokens.end() - 1)
	{
		other = *p;
		//cout<<"Other: "<< *p << "\n";
		other1 = *(p + 1);
		//cout<<"Other1: "<<other1<<"\n";
		other1.append(",");
		other1.append(other);
		++p;
	}
	//cout<<"Other1:"<<other1<<endl;
	return other1;
}

//return "b" if s is of the form "a,b"
string StringRelation::returnSecondComponent(string s)
{
	vector<string> tokens;
	vector<string>::iterator p;
	tokens = makeTokens(s);
	p = tokens.begin();
	return *(p + 1);
}

//check if s is of the form "a,a" and return true if so
//return false otherwise
bool StringRelation::isReflexiveElement(string s)
{
	//You are required to implement this as this
	return (s[0] == s[2]) ? true : false;
}

//check if the relation is reflexive
//if for all s \in set1, (s,s) \in r then reflexive
bool StringRelation::isReflexive()
{
	//You are required to implement this
	for (auto p : *(this->set1->getSetV()))
	{
		if (std::find(this->setv.begin(), this->setv.end(), createReflexiveElement(p)) == this->setv.end())
		{
			return false;
		}
	}
	return true;
}

//check if the relation is symmetric
//if for any s1,s2 \in set1, (s1,s2) \in R implies (s2,s1) \in R
bool StringRelation::isSymmetric()
{
	//You are required to implement this
	for (auto p : setv)
	{
		if (std::find(setv.begin(), setv.end(), std::string(p[0] + std::string(",") + p[2])) == setv.end())
		{
			return false;
		}
	}

	return true;
}

//check if the relation is transitive
// for all x,y,z \in set1, (x -> y AND y -> z) implies x -> z
bool StringRelation::isTransitive()
{
	//You are required to implement this
	for (auto p : setv)
	{
		//(a,a) ignore, the elements must be unique
		if (p[0] == p[2])
		{
			continue;
		}

		for (auto i : setv)
		{
			//ignore the same one
			if (i == p)
				continue;

			//

			if (p[2] == i[0])
			{

				//current setup
				std::string uwu = std::string(p[0] + std::string(",") + i[2]);
				//std::cout << "p: " << p << endl;
				//std::cout << "i: " << i << endl;
				//std::cout << "searching for " << uwu << endl;

				if (std::find(setv.begin(), setv.end(), uwu) == setv.end())
				{
					return false;
				}
			}
		}
	}

	return true;
}

//return true if the relation is an equivalence relation
//return false otherwise
bool StringRelation::isEquivalence()
{

	//You are required to implement this
	return (this->isReflexive() && this->isSymmetric() && this->isTransitive()) ? true : false;
}

//s1 is of the form "a" while s2 is of the form "p,q"
//if a==p then return true
bool StringRelation::isFirstComponent(string s1, string s2)
{
	//You are required to implement this
	return (std::string(1, s2[0]) == s1) ? true : false;
}

//Given a member of set1, this function determine the equivalence class
//for this element s1 i.e. {s| (s,s1) \in R and R is a equivalence relation}
SetOfStrings *StringRelation::computeEquivalenceClass(string element)
{
	//You are required to implement this
	if (!isEquivalence())
	{
		return 0;
	}

	SetOfStrings *out = new SetOfStrings();

	for (auto p : setv)
	{
		if (std::string(1, p[0]) == element)
		{
			out->insertElement(std::string(1, p[2]));
		}
	}
	return out;
}

// Dijkstra algorithm implementation
// Input: source node, destination node
// Output: path length (integer)
// Note: the generated path is also stored in "path" variable
int StringRelation::computeShortest(string source, string destination)
{
	return 0;
}
