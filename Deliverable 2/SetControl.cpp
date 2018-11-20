/* This is the controller part for your program.
 * In this file, students are required to complete the "control" logic.
 * The controller accepts keyboard inputs from the user through View, and manipulate data in Model.
 * Then, the result from Model is sent back to user through View.
 * Think about the design before the actual implementation.
*/
#include "SetControl.h"

//init
SetControl::SetControl()
{
	cout << "Initializing the calculator, please wait! ... \n"
		 << endl;

	//instansiate all members
	setUI = new SetUI();
	this->setModel = nullptr;
	this->relationModel = nullptr;
}

//deconstruct
SetControl::~SetControl()
{
	//dispose all members
	delete setUI;
	delete setModel;
	delete relationModel;
}

//set the model
void SetControl::setSetModel(SetOfStrings *model)
{
	this->setModel = model;
}

//set the relaction
void SetControl::setRelationModel(StringRelation *model)
{
	this->relationModel = model;
}

//get the current model
SetOfStrings *SetControl::getSetModel()
{
	return setModel;
}

//get the current relation
StringRelation *SetControl::getRelationModel()
{
	return relationModel;
}

//main run loop
bool SetControl::Run()
{
	string command;

	//invoke the main user interaction
	while (true)
	{

		//show the menu and get user command
		command = setUI->GetCommand();

		int argc = 0;

		// string tokenizer to split string by using " " as the separator.
		// stores the values in argv array while the number of argument is argc.
		vector<string> argv;
		int i = 0;
		size_t found;
		while ((found = command.find(" ", i)) != std::string::npos)
		{
			argv.push_back(command.substr(i, found - i));
			i = found + 1;
		}
		argv.push_back(command.substr(i));

		argc = argv.size();
		// if enter is pressed
		if (argc == 0)
			continue;

		//help command execution (Completed)
		if (argv.at(0).compare("help") == 0)
		{
			setUI->Help();
		}
		//ls command execution (Completed)
		else if (argv.at(0).compare("ls") == 0)
		{
			if (!(argc == 1 || argc == 2))
			{
				setUI->printError("argument");
				continue;
			}
			if (argc == 2)
			{
				string cmd(argv[1]);
				cmd = "ls " + cmd;
				system(cmd.c_str());
			}
			else
				system("ls");
		}
		//open command execution (Incompleted)
		//**complete this function first.
		else if (argv.at(0).compare("open") == 0)
		{
			if (!(argc == 2 || argc == 3))
			{
				setUI->printError("argument");
				continue;
			}
			SetOfStrings *temps = new SetOfStrings();
			StringRelation *tempr = new StringRelation();
			if (argc == 2)
			{
				setUI->ReadFromFile(argv.at(1), temps, tempr, false);
			}
			else
			{
				//if the file cannot be read
				if (!setUI->ReadFromFile(argv.at(1), temps, tempr, (argv.at(2).compare("-v") == 0) ? true : false))
				{
					setUI->printError("file"); //print an error message
					continue;
				}
			}
			this->setSetModel(temps);
			this->setRelationModel(tempr);
		}
		// exit command execution (Completed)
		else if (argv.at(0).find("exit") != string::npos)
		{
			exit(0);
		}

		//custom
		//clear command
		else if (argv.at(0).find("clear") == 0)
		{
			setUI->clear();
		}

		//completed
		else if (argv.at(0).find("list") == 0)
		{
			//file status
			if (!this->getRelationModel() || !this->getSetModel())
			{
				setUI->printError("file");
				continue;
			}
			//you dont need to error check for empty vectors! What if the graph is empty by design
			setUI->ListMembers(this->getSetModel());
			setUI->ListMembers(this->getRelationModel());
		}

		//complete
		else if (argv.at(0).find("check") == 0)
		{
			//number of arguments
			if (argc != 2)
			{
				setUI->printError("argument");
				continue;
			}

			//somehow check if the flag is corect
			vector<string> valid_flags = {"-r", "-s", "-t", "-e"};
			if (std::find(valid_flags.begin(), valid_flags.end(), argv[1]) == valid_flags.end())
			{
				setUI->printError("argument");
				continue;
			}
			//if no file opened
			if (!this->getRelationModel())
			{
				setUI->printError("file");
				continue;
			}

			//all checking for base errors done, now complete printing operation for the flag
			setUI->checkAndPrintFlag(this->getRelationModel(), argv[1]);
		}

		//almost done, just remove the last comma from the printing
		else if (argv.at(0).find("eqclass") == 0)
		{

			if (argc != 2)
			{
				setUI->printError("argument");
				continue;
			}
			//check file open
			if (!this->getRelationModel() || !this->getSetModel())
			{
				setUI->printError("no_data");
				continue;
			}
			if (!this->getRelationModel()->isEquivalence())
			{
				setUI->printError("equivalence");
				continue;
			}

			//from this point we know that the relation is an equivlance class
			setUI->printEquivalenceClass(argv[1], this->getRelationModel());
		}

		//make sure that all the error checing that you do iswithin theis block, dont make impure functions
		//you can extend this function to the reacable function, if the path is inifite distance then not reacable
		else if (argv.at(0).find("path") == 0)
		{
			//check if file open
			if (!this->getRelationModel() || !this->getSetModel())
			{
				setUI->printError("no_data");
				continue;
			}

			//check correc tamount of args
			if (argc != 3)
			{
				setUI->printError("argument");
				continue;
			}
			//check that the args are actuallyin the set
			std::vector<string> *base_model = this->getSetModel()->getSetV();
			//arg 2
			if (std::find(base_model->begin(), base_model->end(), argv[1]) == base_model->end())
			{
				setUI->printError("argument");
				continue;
			}
			//arg 3
			if (std::find(base_model->begin(), base_model->end(), argv[2]) == base_model->end())
			{
				setUI->printError("argument");
				continue;
			}

			//change this to implement the finsish node
			std::pair<int, string> dijkstra_pair = setUI->performDijkstra(argv[1], argv[2], this->getRelationModel());

			//if node was not reachable
			if (dijkstra_pair.first == -1)
			{
				setUI->printError("not_reachable");
			}
			else
			{
				setUI->printShortestPath(dijkstra_pair.first, dijkstra_pair.second);
			}
		}

		//finish shortest path first, if there is a shortest path, then you know if its reachable
		else if (argv.at(0).find("reachable") == 0)
		{
			if (argc != 3)
			{
				setUI->printError("argument");
				continue;
			}
			if (!this->getRelationModel() || !this->getSetModel())
			{ //if no file opened
				setUI->printError("no_data");
				continue;
			}

			std::pair<int, string> dijkstra_pair = setUI->performDijkstra(argv[1], argv[2], this->getRelationModel());

			if (dijkstra_pair.first == -1)
			{
				setUI->printError("not_reachable");
			}
			else
			{
				//not putting this as an error, because it is NOT an error
				std::cout << "Reachable!"
						  << "\n";
			}
		}

		// Command is invalid
		else
		{
			setUI->printError("command");
			continue;
		}
	}

	return true;
}
