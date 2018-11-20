#include "SetOfStrings.h"

using namespace std;

bool SetOfStrings::isEmpty()
{
    return (setv.empty()) ? true : false;
}

int SetOfStrings::size()
{
    return (int)setv.size();
}

bool SetOfStrings::isMember(string s)
{
    p = setv.begin();
    while (p != setv.end())
    {
        if (*p == s)
            return true;
        ++p;
    }
    return false;
}

void SetOfStrings::insertElement(string s)
{
    if (!isMember(s))
    {
        setv.push_back(s);
    }
}

void SetOfStrings::removeElement(string s)
{
    //You have implemented this in the lab
    while (p != setv.end())
    {
        if (*p == s)
        {
            setv.erase(p);
            break;
        }
    }
}

SetOfStrings *SetOfStrings::setUnion(SetOfStrings *s)
{
    //You have implemented this in the lab
    SetOfStrings *out = new SetOfStrings();

    for (auto p : this->setv)
    {
        out->insertElement(p);
    }

    for (auto i : s->setv)
    {
        out->insertElement(i);
    }

    return out;
}

bool SetOfStrings::isEqual(SetOfStrings *s)
{
    //You have implemented this in the lab
    if (this->setv.size() != s->setv.size())
    {
        return false;
    }

    for (int i = 0; i < s->size(); ++i)
    {
        if (this->setv[i] != s->setv[i])
        {
            return false;
        }
    }

    return true;
}

string SetOfStrings::returnElement(int index)
{
    if (index < (int)setv.size() && index >= 0)
        return setv.at(index);
    else
        return "\n";
}

SetOfStrings *SetOfStrings::product(SetOfStrings *s)
{
    //You are required to implement this as this
    SetOfStrings *out = new SetOfStrings();

    //if any of the vectors are null sets return empty
    if (this->setv.size() == 0 || s->setv.size() == 0)
    {
        return out;
    }

    for (auto p : this->setv)
    {
        for (auto i : s->setv)
        {
            //creates element in format (p,i)
            out->insertElement(std::string(p + std::string(",") + i));
        }
    }

    return out;
}

bool SetOfStrings::subset(SetOfStrings *s)
{
    //You are required to implement this
    for (auto p : s->setv)
    {
        if (std::find(this->setv.begin(), this->setv.end(), p) == this->setv.end())
        {
            return false;
        }
    }

    return true;
}

vector<string> *SetOfStrings::getSetV()
{
    return &this->setv;
}