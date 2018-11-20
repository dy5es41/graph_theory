#include <iostream>
#include "SetOfStrings.h"
#include "StringRelation.h"

// Your task for Deliverable 1 is to complete SetOfStrings.cpp and StringRelation.cpp
// For marking, we will bring our own main.cpp (with different test cases) to test your code
// DO NOT SUBMIT main.cpp file for Deliverable 1

//CUSTOM REMOVE AT END
#define _DEBUG 1
#undef _DEBUG
//


#ifdef _DEBUG
#   define ASSERT(condition, message) \
do { \
if (! (condition)) { \
std::cerr << "\033[1;31m" << message << "('" #condition "') failed in " << __FILE__ \
<< " line " << __LINE__ << ".\033[0m" << std::endl; \
} \
else { \
std::cout << "\033[1;32m" << message << "('" #condition "') passed.\033[0m" << std::endl; \
} \
} while (false)
#else
// basically do nothing
#   define ASSERT(condition, message) do { } while (false)
#endif
//Test case for both negative values
//This test case goes beyond single fault assumption and detects the error.
void testInsert()
{
    SetOfStrings *s = new SetOfStrings();
    s->insertElement("aaa");
    s->insertElement("bbb");
    s->insertElement("aaa");
    ASSERT(s->size() == 2, "Inserting a duplicate element");
}

void testSubset()   
{
    SetOfStrings *s = new SetOfStrings();
    SetOfStrings *s1 = new SetOfStrings();
    s->insertElement("1");
    s->insertElement("2");
    s->insertElement("3");
    s1->insertElement("1");
    s1->insertElement("3");
    s1->insertElement("2");
    ASSERT(s->subset(s1)==true, "Subset is valid");
}

void testProduct()
{
    SetOfStrings *s = new SetOfStrings();
    SetOfStrings *s1 = new SetOfStrings();
    SetOfStrings *out= new SetOfStrings();
    s->insertElement("1");
    s->insertElement("2");
    s->insertElement("3");
    s1->insertElement("1");
    s1->insertElement("2");
    out = s->product(s1);
    cout<<"The product set is: \n";
    out->print();
    ASSERT(out->size()==6, "Product is correct");
}

void testIsReflexive()
{
    SetOfStrings *s1 = new SetOfStrings();
    s1->insertElement("0");
    s1->insertElement("1");
    s1->insertElement("2");
    s1->insertElement("3");
    //cout<<"The first set is: ";
    //s1->print();
    //make s1 the private member
    StringRelation *r = new StringRelation();
    r->setInput(s1);
    r->insertElement("0,0");
    r->insertElement("0,1");
    r->insertElement("1,0");
    r->insertElement("1,1");
    r->insertElement("2,2");
    r->insertElement("3,3");
    r->insertElement("1,3");
    r->insertElement("3,1");
    
    //cout<<"The product set is: \n";
    //out->print();
    ASSERT(r->isReflexive()==true, "The relation is reflexive indeed");
}

void testIsSymmetric()
{

    //cout<<"The first set is: ";
    //s1->print();
    //make s1 the private member
    StringRelation *r = new StringRelation();
    r->insertElement("0,0");
    r->insertElement("0,1");
    r->insertElement("1,0");
    r->insertElement("1,1");
    r->insertElement("2,2");
    r->insertElement("3,3");
    r->insertElement("1,3");
    r->insertElement("3,1");
    
    //cout<<"The product set is: \n";
    //out->print();
    ASSERT(r->isSymmetric()==true, "The relation is symmetric indeed");
}

void testIsTransitive() {
    
    //cout<<"The first set is: ";
    //s1->print();
    //make s1 the private member
    StringRelation *r = new StringRelation();
    r->insertElement("1,2");
    r->insertElement("2,3");
    r->insertElement("1,3");
    
    //cout<<"The product set is: \n";
    //out->print();
    ASSERT(r->isTransitive()==true, "The relation is transitive indeed");
}

void runAllTests()
{
    //testMember();
    testInsert();
    testSubset();
    testProduct();
    testIsReflexive();
    testIsSymmetric();
    testIsTransitive();
    //testRemove();
}

int main(int argc, const char * argv[])
{
#ifdef _DEBUG
    //test before use. this only runs in debug mode
    runAllTests();
    
#else
    SetOfStrings *S = new SetOfStrings();
    //create the set over which the relation will be created
    SetOfStrings *s1 = new SetOfStrings();
    s1->insertElement("0");
    s1->insertElement("1");
    s1->insertElement("2");
    s1->insertElement("3");
    cout<<"The first set is: ";
    s1->print();
    //make s1 the private member
    StringRelation *r = new StringRelation();
    r->setInput(s1);
    r->insertElement("0,0");
    r->insertElement("0,1");
    r->insertElement("1,0");
    r->insertElement("1,1");
    r->insertElement("2,2");
    r->insertElement("3,3");
    r->insertElement("1,3");
    r->insertElement("3,1");
    cout<<"The relation created is \n";
    
    if(r->isValid()){
        r->printRelation();
        cout<<"This is a valid relation \n";
        cout<<"Checking reflexivity \n";
        if(r->isReflexive()){
            cout<<S->ColourText("This is a reflexive relation ", BLUE) <<"\n";
        }
        else cout<<S->ColourText("This relation is not reflexive", RED)<< "\n";
        cout<<"Checking symmetry \n";
        if(r->isSymmetric()){
            cout<<S->ColourText("This is a symmetric relation", BLUE)<< "\n";
        }
        else cout<<S->ColourText("This relation is not symmetric", RED)<< "\n";
        cout<<"Checking transitivity \n";
        if(r->isTransitive()){
            cout<<S->ColourText("This is a transitive relation", BLUE)<< "\n";
        }
        else cout<<S->ColourText("This relation is not transitive", RED) << "\n";
        if(r->isEquivalence()){
            r->printEquivalenceClasses();
        }
        else cout<<S->ColourText("This is not an equivalence relation", RED)<< "\n";
    }
    else cout<<"You entered an invalid relation \n";
    
    return 0;
#endif
    
}
