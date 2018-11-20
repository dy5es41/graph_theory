#ifndef _SetControl_h
#define _SetControl_h

#include "SetUI.h"
#include "SetOfStrings.h"
#include "StringRelation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <cctype>

class SetControl
{
  private:
	//members variables
	SetUI *setUI;
	SetOfStrings *setModel;
	StringRelation *relationModel;


	int isReachable(string src, string dst); // this supposed to be in the model... But I programmed here for assignment separation

  public:
	//constructor/destructor
	SetControl();
	~SetControl();

	//members functions
	//setters
	void setSetModel(SetOfStrings *model);
	void setRelationModel(StringRelation *model);


	//getters
	SetOfStrings *getSetModel();
	StringRelation *getRelationModel();

	bool Run();
};
#endif
