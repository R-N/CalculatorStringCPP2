#include "Calc2.hpp"
using namespace std;

namespace Calculator{
    int stringBufferSize = 256;
    string stringBufferSizeStr = "256";
    vector<string> unusableNames;
    string varNameConStr;
    char* varNameCon;
    string funcNameConStr;
    char* funcNameCon;
    string ftr1str;
    char* ftr1;
    string ftr2str;
    char* ftr2;
    string fstd1str;
    char* fstd1;
    string fstd2str;
    char* fstd2;
    //Types
    char** Types::Names = new char*[15]{
            "Null",
            "Object",
            "MathT",
            "Value",
            "Number",
            "Vector2",
            "Variable",
            "Operator",
            "Expression",
            "Bracket",
            "Function",
            "InternalFunction",
            "StringFunction",
            "DirectValue",
            "SemiDirectValue"};

    void PrintHelp(){
        cout << "[C]  >>> " << endl;
        cout << "[C]  >>> ( ) Evaluating" << endl;
        cout << "[C]  >>> Input an expression to evaluate. For example, '2+2'" << endl;
        cout << "[C]  >>> Order of operation goes as follows (first to last) : brackets, multiplications and divisions, addition and subtraction" << endl;
        cout << "[C]  >>> Operations in the same level will be evaluated from left to right." << endl;
        cout << "[C]  >>> " << endl;
        cout << "[C]  >>> ( ) Variables" << endl;
        cout << "[C]  >>> You may define a global variable with '(variable name) = (variable value)'" << endl;
        cout << "[C]  >>> The variable name must begin with a letter." << endl;
        cout << "[C]  >>> The variable name may only contain letters, numbers and underscores." << endl;
        cout << "[C]  >>> If you wish to multiply a variable with a number, make sure to put '*' operator or at least a space." << endl;
        cout << "[C]  >>> You may define a variable with an expression, though only the result will be saved." << endl;
        cout << "[C]  >>> If you wish to save the whole expression, use function without parameter. You'd still need to type the '()'" << endl;
        cout << "[C]  >>> Type 'variables' to see the global variables defined." << endl;
        cout << "[C]  >>> If you wish to multiply a variable with a bracket, make sure you type the '*' operator. Otherwise it will be parsed as a function, and it may fail." << endl;
        cout << "[C]  >>> Expressions may contain global variables" << endl;
        cout << "[C]  >>> " << endl;
        cout << "[C]  >>> ( ) Functions" << endl;
        cout << "[C]  >>> Type 'functions' to see the defined functions" << endl;
        cout << "[C]  >>> You may define a function with '(function name)(var1, var2, ...) = (function expression)'" << endl;
        cout << "[C]  >>> Your expression may contain functions. Just type '(function name)(input_var1, input_var2, ...)'. This also applies to functions' expressions" << endl;
        cout << "[C]  >>> Of course, this may break your function if you redefine the functions it uses and that causes their return types to change." << endl;
        cout << "[C]  >>> If you wish to just copy the function into yours so that it won't be updated (affected), put '$' before the function you want to use in the expression" << endl;
        cout << "[C]  >>> If you wish to use the value of a function, you may store it into a variable first, then put it into your function." << endl;
        cout << "[C]  >>> Functions will try to use its local variables first, then global variables if it failed." << endl;
        cout << "[C]  >>> I discourage the use of local and global variables with the same name, though." << endl;
        cout << "[C]  >>> The value of a global variable in functions follows the actual current value of the global variable." << endl;
        cout << "[C]  >>> Again, this may break your function if you redefine the variable with different type." << endl;
        cout << "[C]  >>> If you wish to use a variable's current value when the function was defined, put '$' before the variable name." << endl;
        cout << "[C]  >>> " << endl;
        cout << "[C]  >>> ( ) Misc" << endl;
        cout << "[C]  >>> This program only accept * as multiplication operator, and / as division operator" << endl;
        cout << "[C]  >>> Numbers may be rounded when printed, but this doesn't affect the actual calculation." << endl;
        cout << "[C]  >>> The 'e+n' in numbers means '*10^n'. That's just how very big or very small numbers are printed." << endl;
        cout << "[C]  >>> Type 'clear' to clear." << endl;
        cout << "[C]  >>> " << endl;
        cout << "[C]  >>> ( ) Twenty Four (24)" << endl;
        cout << "[C]  >>> Type 'twentyfour' for 24 card game brute force solver." << endl;
        cout << "[C]  >>> " << endl;
    }
    char* helpStr = "help";
    char* varsStr = "variables";
    char* clsStr = "clear";
    char* funcStr = "functions";
    char* twentyFourStr = "twentyfour";
    char* exitStr = "exit";
    void InitString(){
        cout << "[C]  >>> Calculator" << endl;
        cout << "[C]  >>> Input 'help' for help" << endl;
    }

    bool GetInput(){
        char inputBuffer[stringBufferSize];
        printf("[C] <<<  ");
        char* input = gets(inputBuffer);
        if (!strcmp(input, exitStr))
            return false;
        TryInput(input);
        return true;
    }

    void Run(int bufferSize){
        stringBufferSize = bufferSize;
        stringBufferSizeStr = SSTR(stringBufferSize);
        Init();
        while(true){
            if(!GetInput()){
                return;
            }
        }
    }

    bool TryInput(char* s){
        if (!strcmp(s, helpStr)){
            PrintHelp();
            return true;
        }
        if(!strcmp(s, varsStr)){
            GlobalVariable::PrintGlobalVars();
            return true;
        }
        if(!strcmp(s, clsStr)){
            system("cls");
            InitString();
            return true;
        }
        if(!strcmp(s, funcStr)){
            Function::PrintFunctions();
            return true;
        }
        if (!strcmp(s, twentyFourStr)){
            TwentyFour::Run();
            return true;
        }
        if (TryDefine(s)){
            return true;
        }
        if (TryEvaluate(s)){
            return true;
        }
        cout << "[C]  >>> Bad Input" << endl;
        return false;
    }
    bool TryEvaluate(char* s){
        Expression* x;
        if (!Expression::TryRead(s, &x)){
            return false;
        }

        Value& v = x->GetValue();
        stringstream ss1;
        ss1 << *x;
        stringstream ss2;
        ss2 << v;
        string s1 = ss1.str();
        string s2 = ss2.str();
        if (s1 == s2){
            cout << "[C]  >>> " << s1 << endl;
        }else{
            cout << "[C]  >>> " << s1 << " == " << s2 << endl;
        }
        delete x;
        delete &v;
        return true;
    }
    bool TryDefine(char* s){
        if (IsNullOrEmpty(s)){
            return false;
        }
        if (IsNumber(s[0])){
            return false;
        }
        char lhs[stringBufferSize];
        char rhs[stringBufferSize];
        if (sscanf(s, " %[^=] = %[^\n] ", &lhs, &rhs)!= 2){
            return false;
        }
        if (StringFunction::TryDefine(lhs, rhs)){
            return true;
        }
        if (GlobalVariable::TryDefine(lhs, rhs)){
            return true;
        }

        return false;
    }


    bool inited = false;
    void Init(){
        if(inited)
            return;
        Operator::Init();
        Variable::Init();
        Function::Init();
        unusableNames.push_back("V2");
        unusableNames.push_back("V3");
        InitString();
        inited = true;
    }

    //Object
    Type Object::GetType(){
        return Types::Object;
    }
    bool Object::IsNull(){
        return GetType() == Types::Object;
    }
    bool Object::IsNull(Object* o){
        return (o == null || o->IsNull());
    }
    ostream& Object::Print(const ostream& o){
        Object::Print(const_cast<ostream&>(o));
    }
    ostream& Object::Print(ostream& o){
        cout << Types::Names[GetType()] << " Print" << endl;
        throw(runtime_error("Function not implemented"));
    }
    ostream& Object::operator<<(ostream& o){
        return Print(o);
    }
    bool Object::TryDeleteAllocatedWithNew(bool force){
        if (allocatedWithNew){
            Value* v;
            if ((v = dynamic_cast<Value*>(this)) != 0){
                if (Value::TryDeleteCreatedByOp(v)){
                    return true;
                }else if (force){
                    delete this;
                    return true;
                }
                return false;
            }
            delete this;
            return true;
        }
        return false;
    }
    bool Object::TryDeleteAllocatedWithNew(Object* o, bool force){
        if(o == null)
            return -1;
        return o->TryDeleteAllocatedWithNew(force);
    }
    //MathT
    Type MathT::GetType(){
        return Types::MathT;
    }
    bool MathT::IsNull(){
        return (GetType() == Types::MathT);
    }
    int MathT::TryRead(char* s, MathT** out, bool wasOperator, Vars* vars){
        if (IsNullOrEmpty(s))
            return 0;
        int ret;
        if (wasOperator){
            if (ret = Value::TryRead(s, (Value**)out, vars)){
                return ret;
            }
            if (ret = Operator::TryRead(s, (Operator**)out)){
                return ret;
            }
        }else{
            if (ret = Operator::TryRead(s, (Operator**)out)){
                return ret;
            }
            if (ret = Value::TryRead(s, (Value**)out, vars)){
                return ret;
            }
        }
        return 0;
    }
    bool MathT::IsNumber(){
        return false;
    }
    bool MathT::IsExpression(){
        return false;
    }
    bool MathT::IsBracket(){
        return false;
    }
    int MathT::OperationGrade(){
        return 0;
    }
    bool MathT::IsValue(){
        return false;
    }
    bool MathT::IsDirectValue(){
        return false;
    }
    bool MathT::IsVector(){
        return false;
    }
    bool MathT::IsFunction(){
        return false;
    }
    bool MathT::IsInternalFunction(){
        return false;
    }
    ostream& operator<<(MathT& mt, ostream& o){
        return mt.Print(o);
    }
    ostream& operator<<(ostream& o, MathT& mt){
        return mt.Print(o);
    }
    ostream& operator<<(MathT& mt,const ostream& o){
        return mt.Print(o);
    }
    ostream& operator<<(ostream& o,const MathT& mt){
        return const_cast<MathT&>(mt).Print(o);
    }
    MathT* MathT::Copy(){
        throw(runtime_error("Function not implemented"));
    }

    //Value
    Value::Value(bool allocatedWithNew, bool createdByOperator) : MathT(allocatedWithNew){
        if (allocatedWithNew)
            this->createdByOperator = createdByOperator;
        else
            this->createdByOperator = false;
    }
    Type Value::GetType(){
        return Types::Value;
    }
    bool Value::IsNull(){
        return GetType() == Types::Value;
    }
    bool Value::IsValue(){
        return true;
    }
    int Value::TryRead(char* s, Value** out, Vars* vars){
        if (IsNullOrEmpty(s))
            return 0;
        int ret;
        if (ret = TryReadLiteral(s, out, vars)){
            return ret;
        }
        if (ret = Number::TryRead(s, (Number**)out)){
            return ret;
        }
        if (ret = Function::TryRead(s, (Function**)out)){
            return ret;
        }
        if (ret = Variable::TryRead(s, out, vars)){
            return ret;
        }
        if (ret = Bracket::TryRead(s, (Bracket**)out, vars)){
            return ret;
        }
        return 0;
    }
    int Value::TryReadLiteral(char* s, Value** out, Vars* vars){
        s = &(s[PreceedingSpaces(s)]);
        if (s[0] == '$'){
            Value* v;
            int ret = TryRead(&(s[1]), &v, vars);
            if(ret){
                *out = v->GetValuePtr();
                if(*out != v)
                    v->TryDeleteAllocatedWithNew(true);
                return ret+1;
            }
        }
        return 0;
    }
    Value& Value::GetValue(){
        return *GetValuePtr();
    }
    Value* Value::GetValuePtr(){
        if (IsNull()){
            cout << Types::Names[GetType()] << " GetValue" << endl;
            throw(runtime_error("Function not implemented."));
        }
        return (Value*)this;
    }
    Type Value::GetValueType(){
        Value* val = GetValuePtr();
        Type t = val->GetType();
            val->TryDeleteCreatedByOp();
        return t;
    }
    bool Value::SetValue(Value& v){
        throw(runtime_error("Function not implemented."));
    }
    bool Value::SetValue(Value* v){
        cout << Types::Names[GetType()] << " SetValue" << endl;
        throw(runtime_error("Function not implemented."));
    }
    bool Value::HasNan(){
        return false;
    }
    Value& Value::Add(Value& rhs){
        cout << Types::Names[GetType()] << " Add" << endl;
        throw(runtime_error("Function not implemented."));
    }
    Value& Value::Subtract(Value& rhs){
        cout << Types::Names[GetType()] << " Subtract" << endl;
        throw(runtime_error("Function not implemented."));
    }
    Value& Value::Multiply(Value& rhs){
        cout << Types::Names[GetType()] << " Multiply" << endl;
        throw(runtime_error("Function not implemented."));
    }
    Value& Value::Divide(Value& rhs){
        cout << Types::Names[GetType()] << " Divide" << endl;
        throw(runtime_error("Function not implemented."));
    }
    Value& Value::Modulus(Value& rhs){
        cout << Types::Names[GetType()] << " Modulus" << endl;
        throw(runtime_error("Function not implemented."));
    }
    Value& Value::Pow(Value& rhs){
        cout << Types::Names[GetType()] << " Add" << endl;
        throw(runtime_error("Function not implemented."));
    }
    Value& Value::operator^(Value& rhs){
        return Pow(rhs);
    }
    Value& Value::operator+=(Value& rhs){
        Value v = Add(rhs);
        SetValue(v);
        v.TryDeleteCreatedByOp();
        rhs.TryDeleteCreatedByOp();
        return *this;
    }
    Value& Value::operator-=(Value& rhs){
        Value v = Subtract(rhs);
        SetValue(v);
        v.TryDeleteCreatedByOp();
        rhs.TryDeleteCreatedByOp();
        return *this;
    }
    Value& Value::operator*=(Value& rhs){
        Value v = Multiply(rhs);
        SetValue(v);
        v.TryDeleteCreatedByOp();
        rhs.TryDeleteCreatedByOp();
        return *this;
    }
    Value& Value::operator/=(Value& rhs){
        Value v = Divide(rhs);
        SetValue(v);
        v.TryDeleteCreatedByOp();
        rhs.TryDeleteCreatedByOp();
        return *this;
    }
    Value& Value::operator%=(Value& rhs){
        Value v = Modulus(rhs);
        SetValue(v);
        v.TryDeleteCreatedByOp();
        rhs.TryDeleteCreatedByOp();
        return *this;
    }
    bool Value::operator>(Value& rhs){
        return IsGreaterThan(rhs.GetValue());
    }
    bool Value::operator<(Value& rhs){
        return IsLesserThan(rhs.GetValue());
    }
    bool Value::operator==(Value& rhs){
        return IsEqualTo(rhs.GetValue());
    }
    bool Value::operator!=(Value& rhs){
        return IsInequalTo(rhs.GetValue());
    }
    bool Value::operator>=(Value& rhs){
        return IsGreaterThanOrEqualTo(rhs.GetValue());
    }
    bool Value::operator<=(Value& rhs){
        return IsLesserThanOrEqualTo(rhs.GetValue());
    }
    bool Value::IsGreaterThan(Value& rhs){
        throw(runtime_error("Function not implemented"));
    }
    bool Value::IsLesserThan(Value& rhs){
        throw(runtime_error("Function not implemented"));
    }
    bool Value::IsEqualTo(Value& rhs){
        return (!IsLesserThan(rhs) && !IsGreaterThan(rhs) && !rhs.HasNan() && !HasNan());
    }
    bool Value::IsInequalTo(Value& rhs){
        return !IsEqualTo(rhs);
    }
    bool Value::IsGreaterThanOrEqualTo(Value& rhs){
        return !IsLesserThan(rhs);
    }
    bool Value::IsLesserThanOrEqualTo(Value& rhs){
        return !IsGreaterThan(rhs);
    }
    Value& operator+(Value& lhs, Value& rhs){
        Value& ret = lhs.GetValue().Add(rhs.GetValue());
        lhs.TryDeleteCreatedByOp();
        rhs.TryDeleteCreatedByOp();
        return ret;
    }
    Value& operator-(Value& lhs, Value& rhs){
        Value& ret = lhs.GetValue().Subtract(rhs.GetValue());
        lhs.TryDeleteCreatedByOp();
        rhs.TryDeleteCreatedByOp();
        return ret;
    }
    Value& operator*(Value& lhs, Value& rhs){
        Value& ret = lhs.GetValue().Multiply(rhs.GetValue());
        lhs.TryDeleteCreatedByOp();
        rhs.TryDeleteCreatedByOp();
        return ret;
    }
    Value& operator/(Value& lhs, Value& rhs){
        Value& ret = lhs.GetValue().Divide(rhs.GetValue());
        lhs.TryDeleteCreatedByOp();
        rhs.TryDeleteCreatedByOp();
        return ret;
    }
    Value& operator%(Value& lhs, Value& rhs){
        Value& ret = lhs.GetValue().Modulus(rhs.GetValue());
        lhs.TryDeleteCreatedByOp();
        rhs.TryDeleteCreatedByOp();
        return ret;
    }

    ostream& operator<<(Value& v, ostream& o){
        return v.Print(o);
    }
    ostream& operator<<(ostream& o, Value& v){
        return v.Print(o);
    }

    ostream& operator<<(Value& v, const ostream& o){
        return v.Print(o);
    }
    ostream& operator<<(ostream& o,const Value& v){
        return const_cast<Value&>(v).Print(o);
    }
    bool Value::TryDeleteCreatedByOp(){
        if (allocatedWithNew && createdByOperator){
            delete this;
            return true;
        }
        return false;
    }
    bool Value::TryDeleteCreatedByOp(Value* o){
        if(o == null)
            return -1;
        return o->TryDeleteCreatedByOp();
        return false;
    }

    //DirectValue
    Type DirectValue::GetType(){
        return Types::DirectValue;
    }
    Type DirectValue::GetValueType(){
        return GetType();
    }
    bool DirectValue::IsNull(){
        return (GetType() == Types::DirectValue);
    }
    bool DirectValue::IsDirectValue(){
        return true;
    }


    //Number
    Type Number::GetType(){
        return Types::Number;
    }
    Number::Number(int val, bool allocatedWithNew, bool createdByOperator) : Number(allocatedWithNew, createdByOperator){
        value = (double)val;
    }
    Number::Number(float val, bool allocatedWithNew, bool createdByOperator) : Number(allocatedWithNew, createdByOperator){
        value = (double)val;
    }
    Number::Number(double val, bool allocatedWithNew, bool createdByOperator) : Number(allocatedWithNew, createdByOperator){
        value = val;
    }
    bool Number::HasNan(){
        return value != value;
    }
    bool Number::IsNegative(){
        return (value < 0);
    }
    bool Number::IsNumber(){
        return true;
    }
    double Number::GetDouble(){
        return value;
    }
    bool Number::SetValue(Value* rhs){
        if(!rhs->IsNumber())
            return false;
        value = ((Number*)rhs)->GetDouble();
        return true;
    }

    Value& Number::Add(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Number:{
                Number* num = new Number(value + ((Number&)rhs).GetDouble(), true, true);
                return *num;
            }
        }
        cout << Types::Names[GetValueType()] << " Add" << endl;
        throw(runtime_error("Function not defined"));
    }
    Value& Number::Subtract(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Number:{
                Number* num = new Number(value - ((Number&)rhs).GetDouble(), true, true);
                return *num;
            }
        }
        cout << Types::Names[GetValueType()] << " Subtract" << endl;
        throw(runtime_error("Function not defined"));
    }
    Value& Number::Multiply(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Number:{
                Number* num = new Number(value * ((Number&)rhs).GetDouble(), true, true);
                return *num;
            }
            case Types::Vector2:{
                Vector2& v0 = (Vector2&)rhs;
                Vector2* v = new Vector2(value * v0.GetXDouble(), value * v0.GetYDouble(), true, true);
                return *v;
            }
        }
        cout << Types::Names[GetValueType()] << " Multiply " << Types::Names[GetType()] << endl;
        throw(runtime_error("Function not defined"));
    }
    Value& Number::Divide(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Number:{
                Number* num = new Number(value / ((Number&)rhs).GetDouble(), true, true);
                return *num;
            }
        }
        cout << Types::Names[GetValueType()] << " Divide" << endl;
        throw(runtime_error("Function not defined"));
    }
    Value& Number::Modulus(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Number:{
                Number* num = new Number(fmod(value, ((Number&)rhs).GetDouble()), true, true);
                return *num;
            }
        }
        cout << Types::Names[GetValueType()] << " Modulus " << Types::Names[rhs.GetValueType()]<< endl;
        throw(runtime_error("Function not defined"));
    }
    Value& Number::Pow(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Number:{
                Number* num = new Number(pow(value, ((Number&)rhs).GetDouble()), true, true);
                return *num;
            }
        }
        cout << Types::Names[GetValueType()] << " Pow" << endl;
        throw(runtime_error("Function not defined"));
    }
    bool Number::IsGreaterThan(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Number:{
                return (value > ((Number&)rhs).GetDouble());
            }
        }
        cout << Types::Names[GetValueType()] << " IsLesserThan" << endl;
        throw(runtime_error("Function not defined"));
    }
    bool Number::IsLesserThan(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Number:{
                return (value < ((Number&)rhs).GetDouble());
            }
        }
        cout << Types::Names[GetValueType()] << " IsLesserThan" << endl;
        throw(runtime_error("Function not defined"));
    }
    void Number::SetNumber(int val){
        value = (double)val;
    }
    void Number::SetNumber(float val){
        value = (double)val;
    }
    void Number::SetNumber(double val){
        value = val;
    }
    MathT* Number::Copy(){
        return new Number(value, true);
    }
    int Number::TryRead(char* s, Number** out){
        if (IsNullOrEmpty(s)){
            return 0;
        }
        double lf;
        int n;
        if (sscanf(s, " %lf%n", &lf, &n) != 1){// || !n || n > stringBufferSize){
            return 0;
        }
        *out = new Number(lf, true);
        return n;
    }
    ostream& Number::Print(ostream& o){
        return o << value;
    }

    bool IsUnusable(string name){
        return (std::find(unusableNames.begin(), unusableNames.end(),name)!=unusableNames.end());
    }
    //SemiDirectValue
    Type SemiDirectValue::GetType(){
        return Types::SemiDirectValue;
    }
    Type SemiDirectValue::GetValueType(){
        return GetType();
    }
    bool SemiDirectValue::IsNull(){
        return (GetType() == Types::SemiDirectValue);
    }
    bool SemiDirectValue::IsDirectValue(){
        return isDirect;
    }
    //Vector2
    Vector2::~Vector2(){
        delete x;
        delete y;
    }
    double Vector2::GetXDouble(){
        Number* num = GetX();
        double d = num->GetDouble();
        if (!x->IsDirectValue())
            delete num;
        return d;
    }
    double Vector2::GetYDouble(){
        Number* num = GetY();
        double d = num->GetDouble();
        if (!y->IsDirectValue())
            delete num;
        return d;
    }
    Vector2* Vector2::zero = new Vector2(0.0, 0.0, true, false);
    Type Vector2::GetType(){
        return Types::Vector2;
    }
    Vector2::Vector2(Number* x0, Number* y0, bool allocatedWithNew, bool createdByOperator) : Vector2(allocatedWithNew, createdByOperator){
        x = (Number*)(x0->Copy());
        y = (Number*)(y0->Copy());
    }
    Vector2::Vector2(double x0, double y0, bool allocatedWithNew, bool createdByOperator) : Vector2(allocatedWithNew, createdByOperator){
        x = new Number(x0, true, false);
        y = new Number(y0, true, false);
    }
    Number Vector2::Magnitude(){
        double x1 = GetXDouble();
        double y1 = GetYDouble();
        return Number(sqrt(x1*x1 + y1*y1));
    }
    bool Vector2::Normalize(bool force){
        if (!isDirect && !force)
            return false;
        double x1 = GetXDouble();
        double y1 = GetYDouble();
        double mag = sqrt(x1*x1 + y1*y1);
        SetX(x1/mag, force);
        SetY(y1/mag, force);
    }
    bool Vector2::SetX(double x1, bool force){
        if(x->IsDirectValue()){
            GetX()->SetNumber(x1);
            return true;
        }else if (force){
            delete x;
            x = (Value*)new Number(x1, true, false);
            return true;
        }
        return false;
    }
    bool Vector2::SetY(double y1, bool force){
        if(y->IsDirectValue()){
            GetX()->SetNumber(y1);
            return true;
        }else if (force){
            delete y;
            y = (Value*)new Number(y1, true, false);
            return true;
        }
        return false;
    }

    Vector2 Vector2::Normalized(){
        double x1 = GetXDouble();
        double y1 = GetYDouble();
        double mag = sqrt(x1*x1 + y1*y1);
        return Vector2(x1/mag, y1/mag);
    }
    bool Vector2::IsVector(){
        return 2;
    }

    bool Vector2::TryCreate(Value* x0, Value* y0, Vector2** out){
        Value* x1 = x0->GetValuePtr();
        Value* y1 = y0->GetValuePtr();
        if (x1->GetValueType() != Types::Number || y1->GetType() != Types::Number){
            if (x1->createdByOperator)
                delete x1;
            if (y1->createdByOperator)
                delete y1;
            return false;
        }
        Vector2* ret = new Vector2(true, false);
        if (x1->createdByOperator || y1->createdByOperator){
            ret->isDirect = false;
            if (x1->createdByOperator){
                ret->x = (Value*)(x0->Copy());
                delete x1;
            }else{
                ret->x = (Value*)(x1->Copy());
            }
            if (y1->createdByOperator){
                ret->y = (Value*)(y0->Copy());
                delete y1;
            }else{
                ret->y = (Value*)(y1->Copy());
            }
        }else{
            ret->x = (Value*)(x1->Copy());
            ret->y = (Value*)(y1->Copy());
            ret->isDirect = true;
        }
        *out = ret;
        return true;
    }

    /*int Vector2::TryRead(char* s, Vector2** out, Vars* vars){//TODO
        int ps = PreceedingSpaces(s);
        s = &(s[ps]);
        char temp[stringBufferSize];
        char* s2 = &(temp[0]);
        int n;
        if (sscanf(s, " V2 ( %n%[^\n]", &n, &temp) != 1 || !n || n > stringBufferSize){
            //cout << " V2TR 1 ";
            return 0;
        }
        int n2 = ArgumentLength(temp);
        if (!n2){
            cout << " V2TR 2 ";
            return 0;
        }
        char t = s2[n2];
        s2[n2] = '\0';
        Expression* x;
        if (!Expression::TryRead(s2, &x, vars)){
            cout << " V2TR 3 ";
            return 0;
        }
        s2[n2] = t;
        s2 = &(s2[n2+1]);
        int n3 = ArgumentLength(s2);
        if (!n3){
            cout << " V2TR 4 ";
            delete x;
            return 0;
        }
        t = s2[n3];
        s2[n3] = '\0';
        Expression* x2;
        if (!Expression::TryRead(s2, &x2, vars)){
            cout << " V2TR 5 '" << s2 << "' " << endl;
            delete x;
            return 0;
        }
        s2[n3] = t;

        if (!TryCreate(x, x2, out)){
            cout << "V2TR 6";
            delete x;
            delete x2;
            return 0;
        }

        delete x;
        delete x2;

        return (ps + n + n2 + n3 +2);
    }*/
    ostream& Vector2::Print(ostream& o){
        o << '(' << GetXDouble() << ", " << GetYDouble() << ')';
    }
    int ArgumentLength(char* s, bool total){
        int b = 0;
        int n = 0;
        while(s[0] != '\0'){
            char c = s[0];
            if (c == '(')
                ++b;
            if (c == ')')
                --b;
            if (b == -1 || (!total && b == 0 && c == ',')){
                break;
            }
            ++n;
            s = &(s[1]);
        }
        if (b > 0)
            return 0;
        return n;
    }
    bool Vector2::IsEqualTo(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Vector2:{
                Vector2& v = (Vector2&)rhs;
                return (GetXDouble() == v.GetXDouble() && GetYDouble() == v.GetYDouble());
            }
        }
        cout << Types::Names[GetValueType()] << " IsLesserThan" << endl;
        throw(runtime_error("Function not defined"));
    }
    Value& Vector2::Add(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Vector2:{
                Vector2& v = (Vector2&)rhs;
                return *(new Vector2(GetXDouble() + v.GetXDouble(), GetYDouble() + v.GetYDouble(), true, true));
            }
        }
        cout << Types::Names[GetValueType()] << " Add" << endl;
        throw(runtime_error("Function not defined"));
    }
    Value& Vector2::Subtract(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Vector2:{
                Vector2& v = (Vector2&)rhs;
                return *new Vector2(GetXDouble() - v.GetXDouble(), GetYDouble() - v.GetYDouble(), true, true);
            }
        }
        cout << Types::Names[GetValueType()] << " Subtract" << endl;
        throw(runtime_error("Function not defined"));
    }
    Value& Vector2::Multiply(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Number:{
                double num = ((Number&)rhs).GetDouble();
                Vector2* v = new Vector2(GetXDouble() * num, GetYDouble() * num, true, true);
                return *v;
            }
        }
        cout << Types::Names[GetValueType()] << " Multiply " << Types::Names[GetType()] << endl;
        throw(runtime_error("Function not defined"));
    }
    Value& Vector2::Divide(Value& rhs){
        switch(rhs.GetValueType()){
            case Types::Number:{
                double num = ((Number&)rhs).GetDouble();
                Vector2* v = new Vector2(GetXDouble() / num, GetYDouble() / num, true, true);
                return *v;
            }
        }
        cout << Types::Names[GetValueType()] << " Divide" << endl;
        throw(runtime_error("Function not defined"));
    }
    Number& Dot(Vector2& lhs, Vector2& rhs){
        return *new Number(lhs.GetXDouble() * rhs.GetXDouble() + lhs.GetYDouble() * rhs.GetYDouble(), true, true);
    }
    Number& Cross(Vector2& lhs, Vector2& rhs){
        return *new Number(lhs.GetXDouble() * rhs.GetYDouble() - lhs.GetYDouble() * rhs.GetXDouble(), true, true);
    }
    //Variable
    void BuildVarNameCon(){
        varNameConStr = " %" + stringBufferSizeStr + "[a-zA-Z0-9_]%n %" + stringBufferSizeStr + "s ";
        varNameCon = const_cast<char*>(varNameConStr.c_str());
    }
    int Variable::MayHaveVariableAtFront(char* s, char** out, bool isDefine){
        int n;
        char temp2[stringBufferSize];
        int ret = 0;
        char f;
        if (out == null){
            char temp[stringBufferSize];
            ret = sscanf(s, varNameCon, &temp, &n, &temp2);
        }else{
            ret = sscanf(s, varNameCon, out, &n, &temp2);
        }
        if (n > stringBufferSize){
            return false;
        }
        f = s[PreceedingSpaces(s)];
        if (isDefine){
            if (IsAlphabet(f) &&(ret == 1 || (ret == 2 &&
                (IsNullOrEmpty(temp2) ||
                 temp2[PreceedingSpaces(temp2)] == '=')))){
                return n;
            }
        }else{
            if (IsAlphabet(f) &&(ret == 1 || (ret == 2 &&
                (IsNullOrEmpty(temp2) ||
                 temp2[PreceedingSpaces(temp2)] != '=')))){
                return n;
            }
        }
        //cout << "VMHVAF 1 " << IsAlphabet(f) << " " << ret << " '" << s << "' '" << f << "' '" << temp2 << "' " << n << endl;
        return false;
    }
    Type Variable::GetType(){
        return Types::Variable;
    }
    Variable::Variable(string n, Type t, bool allocatedWithNew, bool createdByOperator) : Variable(allocatedWithNew, createdByOperator){
        name = n;
        type = t;
    }
    Variable::Variable(string n, Value* v, bool del, bool allocatedWithNew, bool createdByOperator) : Variable(allocatedWithNew, createdByOperator){
        name = n;
        type = v->GetValueType();
        value = (Value*)(v->Copy());
        if (del)
            delete v;
    }

    Variable::~Variable(){
        if(value != null)
            delete value;
    }
    Type Variable::GetValueType(){
        return type;
    }
    bool Variable::SetValueType(Type t){
        if (value->IsNull() || value->GetType() == t){
            type = t;
            return true;
        }
        return false;
    }

    Value* Variable::GetValuePtr(){
        if (Object::IsNull(value))
            throw(runtime_error("Value is null"));
        return value->GetValuePtr();
    }
    void Variable::SetName(char* name0){
        name = string(name0);
    }
    bool Variable::SetValue(Value* v){
        if (v->GetValueType() != type){
            cout << "VSV " << Types::Names[v->GetType()] << " != " << Types::Names[type];
            return false;
        }
        value = (Value*)(v->Copy());
        return true;
    }


    ostream& Variable::Print(ostream& o){
        o << name;
    }

    MathT* Variable::Copy(){
        return new Variable(name, value->GetValuePtr(), false, true, false);
    }
    int Variable::TryRead(char* s, Value** out, Vars* vars){
        char temp[stringBufferSize];
        int n;
        if (!(n = Variable::MayHaveVariableAtFront(s, (char**)&temp))){
            //cout << "VTR 1" << endl;
            return false;
        }
        string name = string(temp);
        if (vars != null){
            if (vars->count(name)){
                *out = (Value*)new VariableWrapper(name, vars, true, false);
                return n;
            }
        }
        if(GlobalVariable::globalVars.count(name)){
            *out = (Value*)new GlobalVariable(name, true, false);
            return n;
        }
        cout << "VTR 2" << endl;
        return 0;
    }
    int Variable::HasVariableAtFront(char* s, string* out, Vars* vars){
        if(vars != null){
            int ret = VariableWrapper::HasVariableAtFront(s, vars, out);
            if(ret)
                return ret;
        }
        return VariableWrapper::HasVariableAtFront(s, &GlobalVariable::globalVars, out);
    }
    bool Variable::HasVariable(string s, string* out, Vars* vars){
        if(vars != null){
            int ret = VariableWrapper::HasVariable(s, vars, out);
            if(ret)
                return ret;
        }
        return VariableWrapper::HasVariable(s, &GlobalVariable::globalVars, out);
    }
    //VariableWrapper
    Value* VariableWrapper::GetValuePtr(){
        return (*source)[name]->GetValuePtr();
    }
    Type VariableWrapper::GetValueType(){
        return (*source)[name]->GetValueType();
    }
    ostream& VariableWrapper::Print(ostream& o){
        (*source)[name]->Print(o);
    }
    void VariableWrapper::SetName(char* name0){
        name = string(name0);
    }
    VariableWrapper::VariableWrapper(string name0, Vars* source0, bool allocatedWithNew, bool createdByOperator) : VariableWrapper(allocatedWithNew, createdByOperator){
        if(source0 == null){
            throw(runtime_error("No source!"));
        }
        if (!source0->count(name0)){
            throw (runtime_error("Variable doesn't exist"));
        }
        source = source0;
        name = name0;
    }
    int VariableWrapper::HasVariableAtFront(char* s, Vars* vars, string* out){
        char temp[stringBufferSize];
        int n;
        if (!(n = Variable::MayHaveVariableAtFront(s, (char**)&temp))){
            cout << "VWHVA 1" << endl;
            return false;
        }
        if(VariableWrapper::HasVariable(string(temp), vars, out)){
            return n;
        }
        return false;
    }
    bool VariableWrapper::HasVariable(string s, Vars* vars, string* out){
        int ret = vars->count(s);
        if (out != null)
            *out = s;
        return ret;
    }
    void Variable::Init(){
        BuildVarNameCon();
        GlobalVariable::Init();
    }

    //GlobalVariable
    GlobalVariable::GlobalVariable(string name0, bool allocatedWithNew, bool createdByOperator) : GlobalVariable(allocatedWithNew, createdByOperator){
        if(!GlobalVariable::globalVars.count(name0)){
            throw (runtime_error("Variable doesn't exist"));
        }
        source = &GlobalVariable::globalVars;
        name = name0;
    }
    int GlobalVariable::HasVariableAtFront(char* s, string* out){
        return VariableWrapper::HasVariableAtFront(s, &GlobalVariable::globalVars, out);
    }
    bool GlobalVariable::HasVariable(string s, string* out){
        return VariableWrapper::HasVariable(s, &GlobalVariable::globalVars, out);
    }
    Vars GlobalVariable::globalVars = Vars();

    void GlobalVariable::Init(){
        GlobalVariable::globalVars["pi"] = new Variable("pi", new Number(MathT::pi, true), true);
        GlobalVariable::globalVars["e"] = new Variable("e", new Number(MathT::e, true), true);
    }
    void GlobalVariable::PrintGlobalVars(){
        cout << "[C]  >>> Global Variables" << endl;
        Vars::iterator it, end;
        for (it = globalVars.begin(), end = globalVars.end(); it != end; ++it){
            Variable* v = it->second;
            cout << "[C]  >>> " << v->name << " = " << v->GetValue() << endl;
        }
    }
    bool GlobalVariable::TryDefine(char* lhs, char* rhs){
        char name1[stringBufferSize];
        if(!Variable::MayHaveVariableAtFront(lhs, (char**)(&name1), true)){
            return false;
        }
        char* name2 = &(name1[0]);
        while(name2[0] != '\0'){
            if (name2[0] == ' '){
                if (!IsNullOrEmpty(name2)){
                    return false;
                }else{
                    name2[0] = '\0';
                    break;
                }
            }
            name2 = &(name2[1]);
        }
        Value* val;
        if (!Value::TryRead(rhs, &val)){
            return false;
        }
        string vname = string(name1);
        if (IsUnusable(vname)){
            cout << "[C]  >>> The name '" << vname << "' is unusable." << endl;
            return true;
        }
        if (Function::templates.count(vname)){
            if (Function::templates[vname]->IsInternalFunction()){
                cout << "[C]  >>> The name '" << vname << "' is already used as an Internal Function name." << endl;
                delete val;
                return true;
            }else{
                cout << "[C]  >>> The name '" << vname << "' is already used as a User-defined Function name." << endl;
                if (!Ask("[C]  >>> Continue anyway? This may break things. (y/n) : ")){
                    delete val;
                    return true;
                }
            }
        }
        if (globalVars.count(vname)){
            cout << "[C]  >>> Variable named '" << vname << "' already exists." << endl;
            cout << "[C]  >>> Its value is " << globalVars[vname]->GetValue() << endl;
            if (!Ask("[C]  >>> Replace? (y/n) : ")){
                delete val;
                return true;
            }
        }
        Type t = val->GetType();
        if (val->IsDirectValue()){
            Variable* var = new Variable(vname, val, true);
            GlobalVariable::globalVars[vname] = var;
            cout << "[C]  >>> " << vname << " = " << var->GetValue() << endl;
            return true;
        }else if(val->IsExpression()){
            Variable* var = new Variable(vname, val->GetValuePtr(), true);
            val->TryDeleteAllocatedWithNew(true);
            GlobalVariable::globalVars[vname] = var;
            cout << "[C]  >>> " << vname << " = " << var->GetValue() << endl;
            return true;
        }
        delete val;
        return false;
    }

    bool Ask(string question){
        cout << question;
        string s;
        cin >> s;
        cin.ignore(cin.rdbuf()->in_avail()+1, '\n');
        if (s == "n" || s == "N"){
            return false;
        }
        if (s == "y" || s == "Y"){
            return true;
        }
        cout << "[C]  >>> Invalid input." << endl;
    }

    //Operator
    vector<Operator*> Operator::operators;
    int Operator::opCount;
    void Operator::Init(){
        operators.push_back(new Add());
        operators.push_back(new Subtract());
        operators.push_back(new Multiply());
        operators.push_back(new Divide());
        operators.push_back(new Modulus());
        //operators.push_back(new Pow());
        opCount = operators.size();
    }
    Type Operator::GetType(){
        return Types::Operator;
    }
    int Operator::OperationGrade(){
        return 1;
    }
    int Operator::TryRead(char* s, Operator** out){
        if (IsNullOrEmpty(s))
            return 0;

        int ps = PreceedingSpaces(s);
        if (ps > 0)
            s = &(s[ps]);
        switch(s[0]){
            case '+':{
                *out = new Operator::Add(true);
                return ps + 1;
            }
            case '-':{
                *out = new Operator::Subtract(true);
                return ps + 1;
            }
            case '*':{
                *out = new Operator::Multiply(true);
                return ps + 1;
            }
            case '/':{
                *out = new Operator::Divide(true);
                return ps + 1;
            }
            case '%':{
                *out = new Operator::Modulus(true);
                return ps + 1;
            }
            case '^':{
                *out = new Operator::Pow(true);
                return ps + 1;
            }
        }
        return 0;
    }
    Value& Operator::Operate(Value& lhs, Value& rhs){
        cout << Types::Names[GetType()] << " Operate" << endl;
        throw(runtime_error("Function not implemented"));
    }
    //Add
    Value& Operator::Add::Operate(Value& lhs, Value& rhs){
        return lhs + rhs;
    }
    ostream& Operator::Add::Print(ostream& o){
        return o << " + ";
    }
    MathT* Operator::Add::Copy(){
        return new Add(true);
    }
    //Subtract
    Value& Operator::Subtract::Operate(Value& lhs, Value& rhs){
        return lhs - rhs;
    }
    ostream& Operator::Subtract::Print(ostream& o){
        return o << " - ";
    }
    MathT* Operator::Subtract::Copy(){
        return new Subtract(true);
    }
    //Multiply
    int Operator::Multiply::OperationGrade(){
        return 2;
    }
    Value& Operator::Multiply::Operate(Value& lhs, Value& rhs){
        return lhs*rhs;
    }
    ostream& Operator::Multiply::Print(ostream& o){
        return o << " * ";
    }
    MathT* Operator::Multiply::Copy(){
        return new Multiply(true);
    }
    //Divide
    int Operator::Divide::OperationGrade(){
        return 2;
    }
    Value& Operator::Divide::Operate(Value& lhs, Value& rhs){
        return lhs/rhs;
    }
    ostream& Operator::Divide::Print(ostream& o){
        return o << " / ";
    }
    MathT* Operator::Divide::Copy(){
        return new Divide(true);
    }
    //Modulus
    Value& Operator::Modulus::Operate(Value& lhs, Value& rhs){
        Value* vptr = lhs.GetValuePtr();
        Value& ret = vptr->Modulus(rhs);
        vptr->TryDeleteCreatedByOp();
       // lhs.TryDeleteCreatedByOp(); MEMLLEAK MEMORY LEAK
        //rhs.TryDeleteCreatedByOp();
        return ret;
    }
    ostream& Operator::Modulus::Print(ostream& o){
        return o << " % ";
    }
    MathT* Operator::Modulus::Copy(){
        return new Modulus(true);
    }
    //Pow
    int Operator::Pow::OperationGrade(){
        return 3;
    }
    Value& Operator::Pow::Operate(Value& lhs, Value& rhs){
        return lhs.Pow(rhs);
    }
    ostream& Operator::Pow::Print(ostream& o){
        return o << "^";
    }
    MathT* Operator::Pow::Copy(){
        return new Pow(true);
    }
    //Expression
    Type Expression::GetType(){
        return Types::Expression;
    }
    bool Expression::IsExpression(){
        return true;
    }
    Expression::~Expression(){
        for (std::vector<MathT*>::iterator iterator = members.begin(), end = members.end(); iterator != end; ++iterator) {
            delete *iterator;
        }
    }

    int Expression::TryRead(char* s, Vars* vars){
        if (IsNullOrEmpty(s)){
            cout << endl << "ETR 1" << endl;
            return 0;
        }
        s = &(s[PreceedingSpaces(s)]);
        string tempS = "";
        if (!Calculator::IsNumber(s[0]) && s[0] != '(' && s[0] != '$' && !Variable::HasVariableAtFront(s, &tempS, vars) && !IsUnusable(tempS) && !Function::HasFunctionAtFront(s, &tempS) && IsUnusable(tempS)){
            cout << endl << "ETR 2" << endl;
            return 0;
        }
        int n;
        int nt = 0;
        bool wasOperator = true;
        int d;
        MathT* t;
        int asd  = 0;
        while(s[0] != '\0'){
            ++asd;
            if (n = MathT::TryRead(s, &t, wasOperator, vars)){
                if (wasOperator){
                    if (t->OperationGrade()){
                        cout << endl << "ETR 3" << endl;
                        return 0;
                    }
                    wasOperator = false;
                }else{
                    if (t->OperationGrade()){
                        wasOperator=true;
                    }else{
                        if (t->IsNumber()){
                            Number* val = ((Number*)t);
                            if (val->IsNegative()){
                                members.push_back(new Operator::Subtract(true));
                                val->SetNumber(-val->GetDouble());
                            }
                        }else{
                            members.push_back(new Operator::Multiply(true));
                        }
                        wasOperator = false;
                    }
                }
            }else if (IsNullOrEmpty(s)){
                break;
            }else{
                cout << "ETR 4 '" << s << "' ";
                return 0;
            }
            members.push_back(t);
            t = null;
            s=&(s[n]);
            nt += n;
        }
        if (wasOperator){
            cout << "ETR 5";
            return 0;
        }
        int size = members.size();
        if (size == 1 || size == 3){
            return nt;
        }
        if ((size-1)%2!=0){
            cout << "ETR 6";
            return 0;
        }
        OrderOperations();
        return nt;
    }

    void Expression::OrderOperations(){
        for(int i = 3; i > 1; --i){
            OrderOperations(i);
        }
    }

    void Expression::OrderOperations(int grade){
        std::vector<MathT*>::iterator iterator = members.begin();
        std::vector<MathT*>::iterator lhsi = iterator;
        MathT* lhs = *iterator;
        MathT* mtx2;
        ++iterator;
        int i = 1;
        for (std::vector<MathT*>::iterator end = members.end(); iterator != end;) {

            MathT* op = *iterator;
            ++iterator;
            ++i;
            if (iterator == end){
                throw(runtime_error("Bad expression 0"));
            }
            if (op->OperationGrade() == grade){
                MathT* rhs = *iterator;
                Expression* x2 = new Expression(true);
                x2->members.push_back(lhs);
                x2->members.push_back(op);
                x2->members.push_back(rhs);
                std::vector<MathT*>::iterator rhsi = iterator;
                while(iterator != end){
                    ++iterator;
                    if (iterator == end)
                        break;
                    op = *iterator;
                    if (op->OperationGrade() != grade)
                        break;
                    ++iterator;
                    if (iterator == end)
                        throw (runtime_error("Bad expression 3"));
                    x2->members.push_back(op);
                    rhsi = iterator;
                    x2->members.push_back(*iterator);
                }
                mtx2 = x2;
                iterator = members.erase(lhsi, ++rhsi);
                iterator = members.insert(iterator, mtx2);
                lhs = mtx2;
                std::vector<MathT*>::iterator itercpy = iterator;
                lhsi = iterator;
                ++iterator;
                --i;
                end = members.end();
            }else{
                lhsi = iterator;
                lhs = *iterator;
                ++iterator;
                ++i;
            }
            if (iterator == end)
                break;
        }
    }
    int Expression::TryRead(char* s, Expression** out, Vars* vars){
        if (IsNullOrEmpty(s)){
            cout << endl << "ETRA 1" << endl;
            return 0;
        }
        int ps = PreceedingSpaces(s);
        if (ps > 0)
            s = &(s[ps]);
        string tempS = "";
        if (!HasNumber(s) && s[0] != '(' && s[0] != '$' && !Variable::HasVariableAtFront(s, &tempS, vars) && !IsUnusable(tempS) && !Function::HasFunctionAtFront(s) && !IsUnusable(tempS)){
            cout << "ETRA 2" << endl;
            return 0;
        }
        Expression* x = new Expression(true);
        int n;
        if (!(n = x->TryRead(s, vars))){
            cout << endl << "ETRA 3 '" << s << "' " << endl;
            return 0;
        }
        *out = x;
        return n+ps;
    }

    Value* Expression::GetValuePtr(){
        int count = members.size();
        if (count == 0)
            throw(runtime_error("Expression is empty"));
        if (count < 3){
            MathT* f = members.front();
            if (f->IsValue())
                return ((Value*)f)->GetValuePtr();
            throw(runtime_error("Bad Expression 1"));
        }
        std::vector<MathT*>::iterator iterator = members.begin();
        Value* lhs = ((Value*)(*iterator));
        ++iterator;
        for (std::vector<MathT*>::iterator end = members.end(); iterator != end;) {
            Operator* op = (Operator*)(*iterator);
            ++iterator;
            if (iterator == end)
                throw(runtime_error("Bad expression 2"));
            Value* rhs = (Value*)(*iterator);
            ++iterator;
            lhs = &op->Operate(*lhs, *rhs);
        }
        lhs->createdByOperator = false;
        return lhs;
    }



    ostream& Expression::Print(ostream& o){
        for (std::vector<MathT*>::iterator iterator = members.begin(), end = members.end(); iterator != end; ++iterator) {
            o << **iterator;
        }
        return o;
    }
    ostream& operator<<(ostream& o, Expression& x){
        return x.Print(o);
    }
    ostream& operator<<(Expression& x,ostream& o){
        return x.Print(o);
    }
    MathT* Expression::Copy(){
        Expression* x = new Expression(true, false);
        x->Copy(this);
        return x;
    }
    void Expression::Copy(Expression* x){
        vector<MathT*>::iterator it, end;
        for (it = x->members.begin(), end = x->members.end(); it != end; ++it){
            members.push_back((*it)->Copy());
        }
    }

    //Bracket
    Type Bracket::GetType(){
        return Types::Bracket;
    }
    bool Bracket::IsBracket(){
        return true;
    }
    ostream& Bracket::Print(ostream& o){
        o << '(';
        Expression::Print(o);
        return o << ')';
    }
    int Bracket::TryRead(char* s, Bracket** out, Vars* vars){
        if (IsNullOrEmpty(s)){
            cout << "BTR 1";
            return 0;
        }
        int ps = PreceedingSpaces(s);
        if (ps > 0)
            s = &(s[ps]);
        if (s[0] != '('){
            cout << "BTR 2";
            return 0;
        }
        char* s2 = &(s[1]);
        s = s2;
        int n;
        if (!(n = ArgumentLength(s))){
            cout << "BTR 4";
            return false;
        }
        char s3[n+1];
        strncpy(s3, s2, n);
        char c = s3[n];
        s3[n] = '\0';
        Bracket *br = new Bracket();
        if (((Expression*)br)->TryRead(&(s3[0]), vars) == 0){
            s3[n] = c;
            delete br;
            cout << "BTR 5";
            return false;
        }
        s3[n] = c;
        *out = br;

        return ps + n + 2;
    }
    MathT* Bracket::Copy(){
        Bracket* b = new Bracket(true, false);
        b->Expression::Copy(this);
        return b;
    }
    //Function
    FunctionTemplates Function::templates = FunctionTemplates();
    bool Function::IsNull(){
        return GetType() == Types::Function;
    }
    void BuildFuncNameCon(){
        funcNameConStr = " %" + stringBufferSizeStr + "[a-zA-Z0-9_] (%n";
        funcNameCon = const_cast<char*>(funcNameConStr.c_str());
    }
    int Function::MayHaveFunctionAtFront(char* s, char** out){
        int n;
        int ret = 0;
        char f;
        if (out == null){
            char temp[stringBufferSize];
            ret = sscanf(s, funcNameCon, &temp, &n);
        }else{
            ret = sscanf(s, funcNameCon, out, &n);
        }
        if (n > stringBufferSize){
            return false;
        }
        f = s[PreceedingSpaces(s)];
        if (IsAlphabet(f) && ret == 1){
            return n;
        }
        return false;
    }
    int Function::HasFunctionAtFront(char* s, string* out){
        char temp[stringBufferSize];
        int n;
        if (!(n = MayHaveFunctionAtFront(s, (char**)&temp))){
            return false;
        }
        if(Function::HasFunction(string(temp), out)){
            return n;
        }
        return false;
    }
    bool Function::HasFunction(string s, string* out){
        int ret = Function::templates.count(s);
        if (out != null)
            *out = s;
        return ret;
    }
    Function::~Function(){
        Vars::iterator it, end;
        for (it = variables.begin(), end = variables.end(); it != end; it++){
            delete it->second;
        }
    }
    bool Function::IsFunction(){
        return true;
    }
    void Function::Init(){
        BuildFuncNameCon();
        BuildFTRStr();
        BuildSFTDStr();
        templates[InternalFunction::ConsV2::funcName] = new InternalFunction::ConsV2(true);
        templates[InternalFunction::Sqrt::funcName] = new InternalFunction::Sqrt(true);
        templates[InternalFunction::Log::funcName] = new InternalFunction::Log(true);
    }
    bool Function::TryInputVariables(char* s){
        Vars::iterator it, end;
        char* s0 = s;
        char* s1 = s0;
        int n = 0;
        bool hasComma = false;
        for(it = variables.begin(), end = variables.end(); it != end; ++it){

            hasComma = false;
            if (s[0] == '\0'){
                if (s1 == s){
                    cout << "FTIV 1";
                    return false;
                }
            }else{
                if (!(n = ArgumentLength(s))){
                    cout << "FTIV 6";
                }else{
                    s = &(s[n]);
                    if (s[n] == ','){
                        hasComma = true;
                        s[n] = '\0';
                    }
                }
            }
            Value* v;
            if (!Value::TryRead(s1, &v)){
                if (hasComma)
                    s[0] = ',';
                cout << "FTIV 2 " << s1 << endl;
                return false;
            }
            if (hasComma)
                s[0] = ',';
            if (!it->second->SetValue(v)){
                cout << "FTIV 3";
                return false;
            }
            Object::TryDeleteAllocatedWithNew(v, true);
            s = &(s[1]);
            s1 = s;
        }
        if (hasComma){
            cout << "FTIV 4 " << s1 << endl;
            return false;
        }

        return true;
    }
    ostream& Function::Print(ostream& o){
        o << name << "(";
        Vars::iterator it, end;
        bool first = true;
        for(it = variables.begin(), end = variables.end(); it != end; ++it){
            if (first)
                first = false;
            else
                o << ", ";
            o << it->second->GetValue();
        }
        o << ")";
        return o;
    }
    void BuildFTRStr(){
        ftr1str = " %" + stringBufferSizeStr + "[^(] (%n";
        ftr1 = const_cast<char*>(ftr1str.c_str());
        ftr2str = " %" + stringBufferSizeStr + "[a-zA-Z0-9_] %" + stringBufferSizeStr + "s ";
        ftr2 = const_cast<char*>(ftr1str.c_str());
    }
    int Function::TryRead(char* s, Function** out){
        char name[stringBufferSize];
        char temp[stringBufferSize];
        char vars[stringBufferSize];
        int n;
        if (sscanf(s, ftr1, &temp, &n) != 1 || !n || n>stringBufferSize){
            //cout << "FTR 1 " << n << " " << s << "." << endl;
            return 0;
        }

        if (sscanf(temp, ftr2, &name, &vars) > 1){
            if (!IsNullOrEmpty(vars)){
                cout << "FTR 2 '" << name << "' '" << vars << "'" << endl;
                return 0;
            }
        }
        string name1 = string(name);
        if (!templates.count(name1)){
            cout << "FTR 3 '" << name << "' '" << vars << "' '" << s << "' " << n << endl;
            return 0;
        }
        char* s0 = s;
        s = &(s[n]);
        char* s1 = s;
        int b = 1;
        int n0 = n;
        if (!(n = ArgumentLength(s, true))){
            cout << "FTR 4";
            return 0;
        }
        s = &(s[n]);
        n+=n0;
        char t = s[0];
        s[0] = '\0';
        int len = n-n0;
        strncpy(vars, s1,sizeof(char) * len);
        vars[len] = '\0';
        s[0] = t;
        Function* f = templates[name1]->CopyF();
        if (!f->TryInputVariables(vars)){
            f->TryDeleteAllocatedWithNew(true);
            cout << "FTR 5";
            return 0;
        }
        *out = f;

        return n+1;
    }
    Type Function::GetType(){
        return Types::Function;
    }
    Function* Function::CopyF(){
        throw (runtime_error("Function not implemented"));
    }
    MathT* Function::Copy(){
        return CopyF();
    }
    void Function::PrintFunctions(){
        cout << "  <<< Defined functions " << endl;
        FunctionTemplates::iterator it = templates.begin(), end = templates.end();
        for(;it != end; ++it){
            Function* f = it->second;
            cout << "  <<< " << Types::Names[f->GetValueType()] << " " << f->name << " (";
            Vars::iterator it2 = f->variables.begin(), end2 = f->variables.end();
            bool first = true;
            for (; it2 != end2; ++it2){
                if (first)
                    first = false;
                else
                    cout << ", ";
                cout << Types::Names[it2->second->GetValueType()] << " " << it2->first;
            }
            cout << ")" << endl;
        }
    }
    int Function::HasVariableAtFront(char* s, string* out){
        return VariableWrapper::HasVariableAtFront(s, &variables, out);
    }
    bool Function::HasVariable(string s, string* out){
        return VariableWrapper::HasVariable(s, &variables, out);
    }
    //InternalFunction
    Type InternalFunction::GetType(){
        return Types::InternalFunction;
    }
    bool InternalFunction::IsInternalFunction(){
        return true;
    }
    //ConsV2
    char* InternalFunction::ConsV2::funcName = "V2";
    InternalFunction::ConsV2::ConsV2(bool allocatedWithNew, bool createdByOperator) : InternalFunction(allocatedWithNew, createdByOperator){
        name = funcName;
        variables["x"] = new Variable("x", Types::Number, true, false);
        variables["y"] = new Variable("y", Types::Number, true, false);
    }
    Value* InternalFunction::ConsV2::GetValuePtr(){
        Vector2* v2 = new Vector2((Number*)(variables["x"]->GetValuePtr()), (Number*)(variables["y"]->GetValuePtr()));
        return v2;
    }
    Function* InternalFunction::ConsV2::CopyF(){
        return new ConsV2(true);
    }
    Type InternalFunction::ConsV2::GetValueType(){
        return Types::Vector2;
    }
    //Sqrt
    char* InternalFunction::Sqrt::funcName = "Sqrt";
    InternalFunction::Sqrt::Sqrt(bool allocatedWithNew, bool createdByOperator) : InternalFunction(allocatedWithNew, createdByOperator){
        name = funcName;
        variables["x"] = new Variable("x", Types::Number, true, false);
    }
    Value* InternalFunction::Sqrt::GetValuePtr(){
        Number* num = new Number(
                      sqrt(
                           ((Number*)(variables["x"]->GetValuePtr()))->GetDouble()
                           )
                      );
        return num;
    }
    Function* InternalFunction::Sqrt::CopyF(){
        return new Sqrt(true);
    }
    Type InternalFunction::Sqrt::GetValueType(){
        return Types::Number;
    }
    //Log
    char* InternalFunction::Log::funcName = "Log";
    InternalFunction::Log::Log(bool allocatedWithNew, bool createdByOperator) : InternalFunction(allocatedWithNew, createdByOperator){
        name = funcName;
        variables["base"] = new Variable("base", Types::Number, true, false);
        variables["x"] = new Variable("x", Types::Number, true, false);
    }
    Value* InternalFunction::Log::GetValuePtr(){
        Number* num = new Number(
                      log(
                           ((Number*)(variables["x"]->GetValuePtr()))->GetDouble()
                           )
                        /
                        log(
                           ((Number*)(variables["base"]->GetValuePtr()))->GetDouble()
                           )
                      );
        return num;
    }
    Function* InternalFunction::Log::CopyF(){
        return new Log(true);
    }
    Type InternalFunction::Log::GetValueType(){
        return Types::Number;
    }
    //StringFunction
    Type StringFunction::GetType(){
        return Types::StringFunction;
    }
    Type StringFunction::GetValueType(){
        return valueType;
    }
    StringFunction::~StringFunction(){
        delete fString;
    }
    Value* StringFunction::GetValuePtr(){
        Expression* x;
        if (!Expression::TryRead(fString, &x, &variables)){
            cout << "Failed to read expression '" << fString << "'" << endl;
            throw(runtime_error("Failed to read expression " + string(fString)));
            return 0;
        }
        Value* val = x->GetValuePtr();
        if(!val->createdByOperator){
            val = (Value*)val->Copy();
        }
        Object::TryDeleteAllocatedWithNew(x, true);
        return val;
    }
    Function* StringFunction::CopyF(){
        StringFunction* strF = new StringFunction(true);
        strF->fString = new char[strlen(fString) + 1];
        strcpy(strF->fString, fString);
        strF->valueType = valueType;
        strF->name = name;
        Vars::iterator it = variables.begin(), end = variables.end();
        for(;it != end; ++it){
            strF->variables[it->first] = (Variable*)it->second->Copy();
        }

        return strF;
    }
    void BuildSFTDStr(){
        fstd1str = " ( %" + stringBufferSizeStr + "[^)] )%n ";
        fstd1 = const_cast<char*>(fstd1str.c_str());
        fstd2str = " %" + stringBufferSizeStr + "[a-zA-Z0-9_] %" + stringBufferSizeStr + "[,)]%n ";
    }
    bool StringFunction::TryDefine(char* lhs, char* rhs){
        char name0[stringBufferSize];
        int n2;
        if (!(n2 = MayHaveFunctionAtFront(lhs, (char**)&name0) || n2 > stringBufferSize)){
            cout << "SFTD 2" << endl;
            return false;
        }
        string name = string(name0);
        if (IsUnusable(name)){
            cout << "[C]  >>> The name '" << name << "' is unusable." << endl;
            return true;
        }
        if (Function::templates.count(name)){
            if(Function::templates[name]->IsInternalFunction()){
                cout << "  <<< The name '" << name << "' is already used as an Internal Function name." << endl;
                return true;
            }else{
                cout << "  <<< Function '" << name << "' is already defined." << endl;
                if (!Ask("Replace? (y/n) : ")){
                    return true;
                }
            }
        }
        if (GlobalVariable::globalVars.count(name)){
            cout << "  <<< The name '" << name << "' is already used as an Internal Function name." << endl;
            if (!Ask("  <<< Continue anyway? This may break things. (y/n) : ")){
                return true;
            }
        }
        char* vars0 = &(lhs[n2-1]);
        char vars1[stringBufferSize];
        int n6;
        if (sscanf(vars0, fstd1, &vars1, &n6) != 1 || !n6 || n6 > stringBufferSize){
            //cout << "SFTD 11 " << vars0 << endl;
            return false;
        }
        int vars1len = strlen(vars1);
        vars1[vars1len] = ')';
        vars1[vars1len+1] = '\0';
        char* vars = &(vars1[0]);
        StringFunction* sf = new StringFunction(true, false);

        char input[stringBufferSize];
        char* varInput = 0;
        while(true){
            cout << "  <<< Please enter an example of how you would use this function." << endl;
            cout << " >>>  ";
            gets(input);
            char name0b[stringBufferSize];
            int n3;
            if (!(n3 = MayHaveFunctionAtFront(input, (char**)&name0b))){
                cout << "  <<< Bad input" << endl;
                continue;
            }
            string nameb = string(name0b);
            if (name != nameb){
                cout << "  <<< Bad input '" << name << "' '" << nameb << "'" << endl;
                continue;
            }
            varInput = &(input[n3]);
            break;
        }
        char temp[stringBufferSize];
        char* s1 = varInput;
        char* ss = s1;
        char t = '\0';
        char temp2[stringBufferSize];
        while(vars[0] != '\0'){
            int n4;

            if (sscanf(vars, fstd2, &temp, &temp2, &n4) != 2 || !n4 || n4 > stringBufferSize){
                break;
            }
            string varName = string(temp);
            vars = &(vars[n4]);

            int b = 0;

            int n7;
            if (!(n7 = ArgumentLength(ss))){
                t = '\0';
                break;
            }else{
                t = ss[n7];
                ss[n7] = '\0';
                ss = &(ss[n7]);
            }



            Value* v;
            if (!Value::TryRead(s1, &v)){
                ss[0] = t;
                cout << "SFTD 4" << endl;
                return false;
            }
            sf->variables[varName] = new Variable(varName, v, true, true, false);
            ss[0] = t;
            ss = &(ss[1]);
            s1 = ss;
            if (vars[0] == ')')
                break;
        }
        if (t != ')'){
            Object::TryDeleteAllocatedWithNew(sf, true);
            if (Ask("[C]  >>> Bad Input. Retry? (y/n) : ")){
                cout << "[C]  >>> Please redefine the function." << endl;
                cout << "[C] <<<  ";
                gets(temp);
                return Calculator::TryDefine(temp);
            }
            cout << "SFTD 5" << endl;
            return false;
        }
        int rhsLen = strlen(rhs);
        sf->fString = new char[rhsLen + 1];
        strcpy(sf->fString, rhs);
        sf->name = name;
        Value* val;
        try{
            val = sf->GetValuePtr();
            if (val == null)
                throw(runtime_error("Failed to get value"));
            cout << "[C]  >>> Output = " << *val << endl;
            if (Ask("[C]  >>> Redefine? (y/n) : ")){
                cout << "[C]  >>> Please redefine the function." << endl;
                cout << "[C] <<<  ";
                gets(temp);
                return Calculator::TryDefine(temp);
            }
        }catch(exception ex){
            cout << "[C]  >>> Bad Input : " << ex.what() << endl;
            if (Ask("[C]  >>> Retry? (y/n) : ")){
                cout << "[C]  >>> Please redefine the function." << endl;
                cout << "[C] <<<  ";
                gets(temp);
                return Calculator::TryDefine(temp);
            }
            cout << "SFTD 6" << endl;
            return false;
        }
        sf->valueType = val->GetType();
        Function::templates[name] = sf;

        return true;
    }
    //Misc
    int PreceedingSpaces(char* s){
        int i = 0;
        while(s[0] == ' '){
            ++i;
            s = &s[1];
        }
        return i;
    }
    bool IsNumber(char c){
        return (c >= '0' && c <= '9');
    }
    bool IsUppercaseAlphabet(char c){
        return (c >= 'A' && c <= 'Z');
    }
    bool IsLowercaseAlphabet(char c){
        return (c >= 'a' && c <= 'z');
    }
    bool IsAlphabet(char c){
        return (IsUppercaseAlphabet(c) || IsLowercaseAlphabet(c));
    }
    bool HasNumber(char* s){
        return (IsNumber(s[0]) || (s[0] == '-' && IsNumber(s[1])));
    }
    bool IsNullOrEmpty(char* s){
        return (s==null || s[0] == '\0' || s[PreceedingSpaces(s)] == '\0');
    }
    namespace TwentyFour{
        Number twentyFour(24);

        bool TryBrackets(Expression* ex){
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
            if (twentyFour == *num1){
                cout << "[24]  >>> " << *ex1 << endl;
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
            if (twentyFour == *num2){
                cout << "[24]  >>> " << *ex2 << endl;
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
            if (twentyFour == *num3){
                cout << "[24]  >>> " << *ex3 << endl;
                ret = true;
                //return true;
            }

            return ret;
        }

        bool TryOperator(Expression* ex, Number** nums, int i){
            bool ret = false;
            ex->members.push_back(nums[i]);
            if (i == 3){
                Calculator::Expression* ex2 = (Calculator::Expression*)ex->Copy();
                ex2->OrderOperations();
                Calculator::Number* num = (Calculator::Number*)ex2->GetValuePtr();
                if (twentyFour == *num){
                    cout << "[24]  >>> " << *ex2 << endl;
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

        bool TryOperators(Number** nums){
            Calculator::Expression* ex = new Calculator::Expression();
            if(TryOperator(ex, nums, 0)){
                return true;
            }
            return false;
        }
        void Solve(Number** nums){
            sort(nums, nums+4);
            bool suc = false;
            do{
                if (TryOperators(nums)){
                    suc = true;
                    //break;
                }
            }while(next_permutation(nums, nums+4));
            if(!suc){
                cout << "Failed" << endl;
            }
        }
        void Solve(int* nums0){
            Calculator::Number* nums[4];
            nums[0] = new Calculator::Number(nums0[0]);
            nums[1] = new Calculator::Number(nums0[1]);
            nums[2] = new Calculator::Number(nums0[2]);
            nums[3] = new Calculator::Number(nums0[3]);
            Solve(nums);
        }
        void Read(char* s){
            int nums0[4];
            if(sscanf(s, " %d %d %d %d",
               &(nums0[0]), &(nums0[1]),
               &(nums0[2]), &(nums0[3])) == 4){
               Solve(nums0);
               return;
            }
            cout << "Invalid input." << endl;
        }
        void Run(){
            char buff[stringBufferSize];
            cout << "[24]  >>> TwentyFour Brute Force" << endl;
            cout << "[24]  >>> Type 'exit' to exit." << endl;
            while(1){
                cout << "[24]  >>> Insert the four numbers, separated by a space." << endl;
                cout << "[24] <<<  ";
                gets(buff);
                if (!strcmp(buff, exitStr))
                    break;
                Read(buff);
            }

        }
    }
}
