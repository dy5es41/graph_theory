#include "SetUI.h"

SetUI::SetUI()
{
	TopicScreen();
}

// Greet the user and shows the developer name.
void SetUI::TopicScreen()
{
	system("clear");
	cout << "----------------------------------------------------------------------" << endl;
	cout << "\033[1;31mSet Calculator\033[0m\nDeveloper Name: "
		 << "\033[1;34m Andreas (akna890, 307887883)\033[0m\n";
	cout << "To know available commands, please type 'help'" << endl;
	cout << "---------------------------------------------------------------------" << endl;
}

bool SetUI::ReadFromFile(string filename, SetOfStrings *ss, StringRelation *sr, bool verbose)
{
	string line;
	ifstream infile(filename.c_str());

	//return false if the file does not exist
	if (!infile.good())
		return false;

	// Get the first line to extract set members
	getline(infile, line);
	// Remove '//' in the line
	line.erase(0, 2);

	cout << "Extracting set members from the first line: ";
	vector<string> element;
	int i = 0;
	size_t found;
	while ((found = line.find(",", i)) != std::string::npos)
	{

		element.push_back(line.substr(i, found - i));
		cout << element.at(element.size() - 1) << " ";
		i = found + 1;
	}

	element.push_back(line.substr(i));
	cout << element.at(element.size() - 1);
	cout << endl;

	// To bypass the second line
	getline(infile, line);

	//copy the element vector to ss
	for (unsigned int i = 0; i < element.size(); ++i)
	{
		ss->insertElement(element[i]);
	}

	//read the rest of the file.
	while (getline(infile, line))
	{
		line.erase(remove(line.begin(), line.end(), ' '), line.end());
		// find } as the finisher for file reading
		if (line.find("}") != string::npos)
			break;
		// fetch set and relation in the line
		cout << "Extracting the relation per line: \n";
		//catch when getFromLine fails
		if (!getFromLine(ss, sr, line))
		{
			continue;
		}
		cout << endl;
	}
	//close stringstream and set input for sr to ss
	infile.close();
	sr->setInput(ss);

	return true;
}

//line is the current line to be analysed

//extract information (weight, relation pairs) from the string passed to it
//ss is used as the base set
//sr is the string relation
bool SetUI::getFromLine(SetOfStrings *ss, StringRelation *sr, string line)
{
	vector<string> element;
	int i = 0;
	size_t found;
	while ((found = line.find("->", i)) != std::string::npos)
	{
		element.push_back(line.substr(i, found - i));
		i = found + 2;
	}

	//check if the line is valid and contains the correct symbols
	if (line.find("->") == std::string::npos ||
		line.find("\"") == std::string::npos ||
		line.find("label") == std::string::npos)
	{
		return false;
	}

	element.push_back(line.substr(i, line.find("[", i) - i));

	for (int j = 0; j < (int)element.size() - 1; j++)
	{
		cout << element.at(j) << ", ";
	}
	cout << element.at(element.size() - 1) << endl;

	//string temp = line.substr(i + 1, line.find(";", i) - i);
	string temp = line.substr(line.find("a"), line.find(";") - 1);
	//cout << "weight info: " << temp;

	//check if the element(relation pair) is in the base set
	vector<string> *base_set = ss->getSetV();

	for (auto p : element)
	{
		//if the element as not found in the base set, say so and return false
		if (std::find(base_set->begin(), base_set->end(), p) == base_set->end())
		{
			std::cout << "the element " << element[0] << ", " << element[1] << "was not found in the base set" << endl;
			return false;
		}
	}

	//from this point forwards, the element is assumed to be correct, so we insert it into sr
	std::string to_add = std::string(element[0] + std::string(",") + element[1]);
	sr->insertElement(to_add);

	//extract the weight as a string
	std::regex rgx("[0-9]");
	std::sregex_iterator current(temp.begin(), temp.end(), rgx);
	std::smatch match = *current;
	std::string str_match = match.str();

	//convert the weight from string to int (eg "9" == 9)
	int i_match = std::stoi(str_match);
	//insert weight value into the weight vector of sr
	sr->getWeightVector()->push_back(i_match);

	return true;
}

void SetUI::clear()
{
	system("CLS");
	SetUI::TopicScreen();
}

//return the index of the element in a vector, based on js
int indexOf(std::vector<string> *vec, string element)
{
	for (int i = 0; i < vec->size(); ++i)
	{
		if (element == (*vec)[i])
		{
			return i;
		}
	}
	return -1;
}

std::pair<int, string> SetUI::performDijkstra(std::string source, std::string target, StringRelation *sr)
{
	vector<string> *relation = sr->getSetV();
	vector<string> *set = sr->getSet1()->getSetV();
	std::string current_path;

	//the list of vectors that are going to be processed. initially this is set up to equal the setv
	vector<bool> list(set->size());	//bool
	vector<string> paths(set->size()); //the previous node that was looked at, this can make up the path, the size is the same as number of node is G
	vector<int> dist(set->size());	 //distance vector, init size is the same as the number of nodes

	for (int i = 0; i < set->size(); ++i)
	{
		dist[i] = std::numeric_limits<double>::infinity(); //represents MAX_INT (infinity)
		list[i] = 0;									   //each is set to ero meaning that it HAS NOT been processed
														   //insertElement (v , list) ; dont need this line because it has already been set to this state
	}

	// get the index of the source vector and set the distance to zero
	int index = indexOf(set, source);

	//the weight from a node to iteself is not always 0, make sure you update this to true value
	dist[index] = 0;
	paths[index] = source;

	//while there are still nodes to be processed, continue processing
	while (std::find(list.begin(), list.end(), 0) != list.end())
	{
		{

			std::cout << "the current list to process is: ";
			for (int i = 0; i < list.size(); ++i)
			{
				std::cout << list[i] << " ";
			}
			std::cout << "\n\n";

			std::cout << "the current distances are: ";
			for (int i = 0; i < dist.size(); ++i)
			{
				std::cout << dist[i] << " ";
			}
			std::cout << "\n\n";

		}
		//index of the minimum distance that is still in the processing list (&&)
		int current_index_selected_node = -1;
		for (int i = 0; i < set->size(); ++i)
		{
			int current_min_distance = std::numeric_limits<double>::infinity(); //set the current minimum distance to inf

			if (list[i] == 1) //if the node has already been processed, just ignore it
			{
				continue;
			}
			if (dist[i] < current_min_distance) //if there is a smaller distance, set the current node to that node
			{
				//use that index to select the current node
				current_index_selected_node = i;
				current_path.append((*set)[current_index_selected_node]);
			}
		}

		//if there was no index chosen, then there is NO PATH
		//see if you don't need a temp car in this statement
		if (current_index_selected_node == -1)
		{
			std::pair<int, string> temp_pair(-1, "fail");
			return temp_pair;
		}
		//remove that element from the currently searchable
		list[current_index_selected_node] = 1;
		//update the currently searched node

		//search the relation for the outing edges and put on to weight
		for (int i = 0; i < relation->size(); ++i)
		{
			//if the selected node is the same
			if ((*set)[current_index_selected_node] == std::string(1, (*relation)[i][0]))
			{
				//get the weight
				int new_distance = dist[current_index_selected_node] + (*sr->getWeightVector())[i];
				//if the new distance is less than the current distance update that distance and the paths
				int index_of = indexOf(set, std::string(1, (*relation)[i][2]));
				if (new_distance < dist[index_of])
				{

					dist[index_of] = new_distance;

					//update path history
					//index_of = node with new shortest path
					//current selected node = set(current_index_selected_node)
					//paths[index_of] = ColorText("sorry didnt know how to implement", RED);
				}
			}
		}

		std::pair<int, string> pair(dist[indexOf(set,target)],"didnt know how");
		return pair;
	}

	{
		std::cout << "********************** END RESULT *****************\n";
		std::cout << "the current list to process is: ";
		for (int i = 0; i < list.size(); ++i)
		{
			std::cout << list[i] << " ";
		}
		std::cout << "\n\n";

		std::cout << "the current distances are: ";
		for (int i = 0; i < dist.size(); ++i)
		{
			std::cout << dist[i] << " ";
		}
		std::cout << "\n\n";

	}

	{
		//PATHS:
		std::cout << "PATHS\n";
		for (int i = 0; i < set->size(); ++i)
		{
			std::cout << (*set)[i] << "\t";
			std::cout << paths[i] << "\n";
		}
	}

	//i tried for a whole day to get this working, I feel extremely stupid that I didn't get this
}

//NOTE: returns are opposite to standard, in this case 0 == error and 1 == success
int SetUI::ListMembers(SetOfStrings *model)
{

	//print setv
	std::cout << "The members of the " << ColorText("sets", BLUE) << " are:" << std::endl;
	std::cout << "=> {";
	for (auto iter = model->getSetV()->begin(); iter != model->getSetV()->end(); ++iter)
	{
		if (iter == model->getSetV()->end() - 1)
		{
			std::cout << ColorText(*iter, BLUE);
			break;
		}
		std::cout << ColorText(*iter, BLUE) << ", ";
	}
	std::cout << "}" << std::endl;

	return 1;
}

//make sure to make all the error checking form this file to SetControl.cpp
int SetUI::ListMembers(StringRelation *model)
{
	//here we are printing the relation and the weights

	//relation print
	std::cout << "The members of the " << ColorText("relations", YELLOW) << " are:" << std::endl;
	std::cout << "=> {";
	for (auto iter = model->getSetV()->begin(); iter != model->getSetV()->end(); ++iter)
	{
		if (iter == model->getSetV()->end() - 1)
		{
			std::cout << "(" << ColorText(*iter, YELLOW) << ")";
			break;
		}
		std::cout << "(" << ColorText(*iter, YELLOW) << ")"
				  << ", ";
	}
	std::cout << "}" << std::endl;

	std::cout << "The members of the " << ColorText("weights", GREEN) << " are:" << std::endl;
	std::cout << "=> {";
	for (auto iter = model->getWeightVector()->begin(); iter != model->getWeightVector()->end(); ++iter)
	{
		if (iter == model->getWeightVector()->end() - 1)
		{
			std::cout << ColorText(std::to_string(*iter), GREEN);
			break;
		}
		std::cout << ColorText(std::to_string(*iter), GREEN) << ", ";
	}
	std::cout << "}" << std::endl;

	return 1;
}

void SetUI::checkAndPrintFlag(StringRelation *model, std::string flag)
{
	bool isProperty = false;
	if (flag == "-r")
	{
		isProperty = model->isReflexive() ? true : false;
		if (isProperty)
		{
			std::cout << ColorText("It is Reflexive\n", GREEN);
		}
		else
		{
			std::cout << ColorText("It is not Reflexive\n", RED);
		}
	}
	else if (flag == "-s")
	{
		isProperty = model->isSymmetric() ? true : false;
		if (isProperty)
		{
			std::cout << ColorText("It is Symmertric\n", GREEN);
		}
		else
		{
			std::cout << ColorText("It is not Symmetric\n", RED);
		}
	}
	else if (flag == "-t")
	{
		isProperty = model->isTransitive() ? true : false;
		if (isProperty)
		{
			std::cout << ColorText("It is Transitive\n", GREEN);
		}
		else
		{
			std::cout << ColorText("It is not Transitive\n", RED);
		}
	}
	else if (flag == "-e")
	{
		isProperty = model->isEquivalence() ? true : false;
		if (isProperty)
		{
			std::cout << ColorText("It is an Equivalence relation\n", GREEN);
		}
		else
		{
			std::cout << ColorText("It is not an Equivalence relation\n", RED);
		}
	}
}

//oh god fix this function
void SetUI::printEquivalenceClass(string member, SetOfStrings *ss)
{
	//create the equivalence class
	std::cout << "=> [" << ColorText(member, BLUE) << "]"
			  << " = "
			  << "{";

	//number of matching elements
	int i = 0;
	for (auto p : *ss->getSetV())
	{
		if (std::string(1, p[0]) == member)
		{
			++i;
		}
	}

	//loop over the vector, makeing sure to print for the last case without a comma
	int e = 0;
	for (auto p : *ss->getSetV())
	{
		//last match no comma
		if ((e == i - 1) && std::string(1, p[0]) == member)
		{
			std::cout << ColorText(std::string(1, p[2]), BLUE);
			break;
		}

		//normal match
		if (std::string(1, p[0]) == member)
		{
			std::cout << ColorText(std::string(1, p[2]), BLUE) << ", ";
			++e;
		}
	}

	std::cout << "}" << std::endl;
}

void SetUI::printError(string reason)
{
	// error message if the command cannot be understood
	if (reason.compare("command") == 0)
	{
		cout << "Command cannot be understood. Please enter help to see the available commands\n";
	}
	// error message if the command argument is incorrect
	else if (reason.compare("argument") == 0)
	{
		cout << ColorText("Incorrect command arguments!\n", RED);
		cout << ColorText("Please type help to know about the command arguments\n", RED);
	}
	else if (reason.compare("file") == 0)
	{
		std::cout << "File could not be read";
	}
	else if (reason.compare("reggex") == 0)
	{
		std::cout << "No reggex match found, are you sure the format of the text file is correct?\n";
	}
	else if (reason.compare("size0") == 0)
	{
		std::cout << "Empty vector, did you populate it?\n";
	}
	else if (reason.compare("no_data") == 0)
	{
		std::cout << ColorText("Operation could be initialized because there is no graph to compute\n", RED);
		std::cout << ColorText("Please load a graph first by using 'open' command which will read a file\n", RED);
	}
	else if (reason.compare("flag") == 0)
	{
		std::cout << ColorText("Invalid flag\n", RED);
	}
	else if (reason.compare("equivalence") == 0)
	{
		std::cout << ColorText("this is not an equivalence class\n", RED);
	}
	else if (reason.compare("not_reachable") == 0)
	{
		std::cout << ColorText("this node is not reachable\n", RED);
	}
}

string SetUI::GetCommand()
{
	string commandInput;
	cout << ">> ";

	getline(cin, commandInput);
	return commandInput;
}

//prints the help message (Completed)
void SetUI::Help()
{
	cout << "Currently available commands:\n";
	cout << "Anything inside <> bracket is an optional argument, while [] bracket contains a must argument.\n\n";
	cout << left << setw(45) << ColorText("	help", RED) << left << setw(50) << "Show help." << endl
		 << endl;
	cout << left << setw(45) << ColorText("	ls <path> ", RED) << left << setw(50) << "Identical to the ls shell command. If a path is given, then" << endl;
	cout << left << setw(41) << "" << left << setw(50) << "the program returns all the files in that path." << endl
		 << endl;
	cout << left << setw(45) << ColorText("	clear ", RED) << left << setw(50) << "Identical to the clear shell command. Clear the screen." << endl
		 << endl;
	cout << left << setw(45) << ColorText("	open [file_path] <-v>", RED) << left << setw(50) << "Open a file. If the file does not exist, then program returns" << endl;
	cout << left << setw(41) << "" << left << setw(50) << "an error message. If -v (verbose) option is enabled (e.g., open a.txt -v)," << endl;
	cout << left << setw(41) << "" << left << setw(50) << "then each line in the file is printed on console while reading." << endl
		 << endl;
	cout << left << setw(45) << ColorText("	list", RED) << left << setw(50) << "List all the members of set and relation." << endl
		 << endl;
	cout << left << setw(45) << ColorText("	check [OPTION] ", RED) << left << setw(50) << "Check the relation. The option has to be provided. Options are:" << endl;
	cout << left << setw(41) << "" << left << setw(50) << "-r (reflexive)" << endl;
	cout << left << setw(41) << "" << left << setw(50) << "-s (symmetric)" << endl;
	cout << left << setw(41) << "" << left << setw(50) << "-t (transtivie)" << endl;
	cout << left << setw(41) << "" << left << setw(50) << "-e (equivalence relation)" << endl
		 << endl;
	cout << left << setw(45) << ColorText("	eqclass [VALUE]", RED) << left << setw(50) << "Show equivalence class of the given [VALUE = Set member]." << endl;
	cout << left << setw(41) << "" << left << setw(50) << "If VALUE is not a member of set, then the program returns" << endl;
	cout << left << setw(41) << "" << left << setw(50) << "an error message." << endl
		 << endl;
	cout << left << setw(45) << ColorText("	reachable [VALUE1] [VALUE2]", RED) << left << setw(50) << "This command checks if two given members are connected to each other" << endl;
	cout << left << setw(41) << "" << left << setw(50) << "If any of VALUE1 and VALUE2 is not a member of the set, then it" << endl;
	cout << left << setw(41) << "" << left << setw(50) << "returns an error message." << endl
		 << endl;
	cout << left << setw(45) << ColorText("	path [VALUE1] [VALUE2]", RED) << left << setw(50) << "Apply Dijkstra algorithm to find the shortest path." << endl;
	cout << left << setw(41) << "" << left << setw(50) << "VALUE1 and VALUE2 are members of the set. VALUE1 is the starting node, and VALUE2 is the destination." << endl
		 << endl;
	cout << left << setw(45) << ColorText("	exit", RED) << left << setw(50) << "Terminate the program." << endl;
	cout << "\n\n";
}

void SetUI::printShortestPath(int distance, string path)
{
	cout << "  The shortest path distance: " << ColorText(to_string(distance), GREEN) << endl;
	cout << "  The path is: " << ColorText(path, BLUE) << endl;
}

string SetUI::ColorText(string s, COLOR color)
{
	string temp;
	switch (color)
	{
	case RED:
		temp = "\033[1;31m" + s + "\033[0m";
		break;
	case BLUE:
		temp = "\033[1;34m" + s + "\033[0m";
		break;
	case YELLOW:
		temp = "\033[1;33m" + s + "\033[0m";
		break;
	case GREEN:
		temp = "\033[1;32m" + s + "\033[0m";
		break;
	case WHITE:
		temp = "\033[1;37m" + s + "\033[0m";
		break;
	default:
		temp = "\033[0m" + s;
		break;
	}
	return temp;
}
