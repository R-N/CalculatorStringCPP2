#include <iostream>
#include <string>
#include <map>
#include "Calc2.hpp"
using namespace std;


Calculator::Number twentyfour(24);


bool TryBrackets(Calculator::Expression* ex){
    bool ret = false;
    Calculator::Expression* ex1 = (Calculator::Expression*)ex->Copy();

    Calculator::Bracket* ex1b = new Calculator::Bracket();
    ex1b->members.push_back(ex1->members[0]);
    ex1b->members.push_back(ex1->members[1]);
    ex1b->members.push_back(ex1->members[2]);
    ex1b->members.push_back(ex1->members[3]);
    ex1b->members.push_back(ex1->members[4]);
    ex1b->OrderOperations();

    ex1->members.erase(ex1->members.begin(), ex1->members.begin()+5);
    ex1->members.insert(ex1->members.begin(), ex1b);


    Calculator::Number* num1 = (Calculator::Number*)ex1->GetValuePtr();
    if (twentyfour == *num1){
        cout << "B1\t" << *ex1 << endl;
        ret = true;
        //return true;
    }

    Calculator::Expression* ex2 = (Calculator::Expression*)ex->Copy();

    Calculator::Bracket* ex2b1 = new Calculator::Bracket();
    ex2b1->members.push_back(ex2->members[0]);
    ex2b1->members.push_back(ex2->members[1]);
    ex2b1->members.push_back(ex2->members[2]);

    ex2->members.erase(ex2->members.begin(), ex2->members.begin()+3);
    ex2->members.insert(ex2->members.begin(), ex2b1);


    Calculator::Bracket* ex2b2 = new Calculator::Bracket();
    ex2b2->members.push_back(ex2->members[2]);
    ex2b2->members.push_back(ex2->members[3]);
    ex2b2->members.push_back(ex2->members[4]);
    ex2->members.erase(ex2->members.begin()+2, ex2->members.begin()+5);
    ex2->members.push_back(ex2b2);

    Calculator::Number* num2 = (Calculator::Number*)ex2->GetValuePtr();
    if (twentyfour == *num2){
        cout << "B2\t" << *ex2 << endl;
        ret = true;
        //return true;
    }


    Calculator::Expression* ex3 = (Calculator::Expression*)ex->Copy();

    Calculator::Bracket* ex3b = new Calculator::Bracket();
    ex3b->members.push_back(ex3->members[2]);
    ex3b->members.push_back(ex3->members[3]);
    ex3b->members.push_back(ex3->members[4]);
    ex3b->members.push_back(ex3->members[5]);
    ex3b->members.push_back(ex3->members[6]);
    ex3b->OrderOperations();
    ex3->members.erase(ex3->members.begin()+2, ex3->members.begin()+7);
    ex3->members.push_back(ex3b);

    Calculator::Number* num3 = (Calculator::Number*)ex3->GetValuePtr();
    if (twentyfour == *num3){
        cout << "B3\t" << *ex3 << endl;
        ret = true;
        //return true;
    }

    return ret;
}

bool TryOperator(Calculator::Expression* ex, Calculator::Number** nums, int i){
    bool ret = false;
    ex->members.push_back(nums[i]);
    if (i == 3){
        Calculator::Expression* ex2 = (Calculator::Expression*)ex->Copy();
        ex2->OrderOperations();
        Calculator::Number* num = (Calculator::Number*)ex2->GetValuePtr();
        if (twentyfour == *num){
            cout << i << "\t" << *ex2 << endl;
            ret = true;
            //return true;
        }
        if(TryBrackets(ex)){
            ret = true;
            //return true;
        }
        delete ex2;
    }else{
        for (int j = 0; j < Calculator::Operator::opCount; ++j){
            Calculator::Expression* exi = (Calculator::Expression*)(ex->Copy());
            exi->members.push_back(Calculator::Operator::operators[j]);
            if(TryOperator(exi, nums, i+1)){
                ret = true;
                //return true;
            }
        }
    }
    return ret;
}


bool TryOperators(Calculator::Number** nums){
    Calculator::Expression* ex = new Calculator::Expression();
    if(TryOperator(ex, nums, 0)){
        return true;
    }
    return false;
}

int main(){
    Calculator::Run(1024);
    return 0;
}
