#ifndef CALC2_HPP

#include <cstdio>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include<stdexcept>
#include <cstring>
#include <typeinfo>
#include <map>
#include "General.hpp"
#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

namespace Calculator{
    class Function;
    class Variable;
    typedef map<string,Variable*> Vars;
    extern int stringBufferSize;
    extern string stringBufferSizeStr;
    extern vector<string> unusableNames;

    extern string varNameConStr;
    extern char* varNameCon;
    void BuildVarNameCon();
    extern string funcNameConStr;
    extern char* funcNameCon;
    void BuildFuncNameCon();
    extern string ftr1str;
    extern char* ftr1;
    extern string ftr2str;
    extern char* ftr2;
    void BuildFTRStr();
    extern string fstd1str;
    extern char* fstd1;
    extern string fstd2str;
    extern char* fstd2;
    void BuildSFTDStr();
    class Types{
    public:
        enum Type {
            Null=0,
            Object=1,
            MathT=2,
            Value=3,
            Number=4,
            Vector2=5,
            Variable=6,
            Operator=7,
            Expression=8,
            Bracket=9,
            Function=10,
            InternalFunction=11,
            StringFunction=12,
            DirectValue=13,
            SemiDirectValue=14};
        static char** Names;
    };

    typedef Types::Type Type;

    class ReturnCodes{
    public:
        enum Code{
            Succeed = 0,
            Fail = 1
        };
        static char** Messages;
    };

    typedef ReturnCodes::Code ReturnCode;

    bool TryInput(char* s);
    bool TryEvaluate(char* s);
    bool TryDefine(char* s);
    void PrintHelp();
    void Init();
    void InitString();
    bool GetInput();
    void Run(int stringBufferSize = 256);


    class Object;
    class Object{
    public:
        bool allocatedWithNew;
        Object(bool allocatedWithNew = true){
            this->allocatedWithNew = allocatedWithNew;
        }
        virtual bool TryDeleteAllocatedWithNew(bool force = true);
        static bool TryDeleteAllocatedWithNew(Object* o, bool force);
        virtual Type GetType();
        virtual bool IsNull();
        static bool IsNull(Object* o);
        virtual ostream& Print(const ostream& o);
        virtual ostream& Print(ostream& o);
        virtual ostream& operator<<(ostream& o);
    };

    class MathT : public Object{
    public:
        static const double pi  =  3.141592653589793238463;
        static const double e  =   2.71828182845904523536;
        MathT(bool allocatedWithNew = true) : Object(allocatedWithNew) {}
        bool IsNull();
        static int TryRead(char* s, MathT** out, bool wasOperator, Vars* vars = null);
        virtual Type GetType();
        virtual bool IsValue();
        virtual bool IsDirectValue();
        virtual bool IsNumber();
        virtual bool IsVector();
        virtual bool IsExpression();
        virtual bool IsBracket();
        virtual int OperationGrade();
        virtual bool IsFunction();
        virtual bool IsInternalFunction();
        virtual MathT* Copy();
    };
    ostream& operator<<(MathT& mt,  ostream& o);
    ostream& operator<<(ostream& o,  MathT& mt);
    ostream& operator<<(MathT& mt, const ostream& o);
    ostream& operator<<(ostream& o, const MathT& mt);

    class Value : public MathT{
    public:
        bool createdByOperator;
        Value(bool allocatedWithNew = true, bool createdByOperator = false);
        bool IsNull();
        Type GetType();
        bool IsValue();
        static int TryRead(char* s, Value** out, Vars* vars = null);
        static int TryReadLiteral(char* s, Value** out, Vars* vars = null);
        Value& GetValue();
        virtual Value* GetValuePtr();
        virtual Type GetValueType();
        bool SetValue(Value& val);
        virtual bool SetValue(Value* val);
        virtual bool HasNan();
        virtual Value& Add(Value& rhs);
        virtual Value& Subtract(Value& rhs);
        virtual Value& Multiply(Value& rhs);
        virtual Value& Divide(Value& rhs);
        virtual Value& Modulus(Value& rhs);
        virtual Value& Pow(Value& rhs);
        virtual Value& operator^(Value& rhs);
        virtual Value& operator+=(Value& rhs);
        virtual Value& operator-=(Value& rhs);
        virtual Value& operator*=(Value& rhs);
        virtual Value& operator/=(Value& rhs);
        virtual Value& operator%=(Value& rhs);
        virtual bool operator>(Value& rhs);
        virtual bool operator<(Value& rhs);
        virtual bool operator==(Value& rhs);
        virtual bool operator!=(Value& rhs);
        virtual bool operator>=(Value& rhs);
        virtual bool operator<=(Value& rhs);
        virtual bool IsGreaterThan(Value& rhs);
        virtual bool IsLesserThan(Value& rhs);
        virtual bool IsEqualTo(Value& rhs);
        virtual bool IsInequalTo(Value& rhs);
        virtual bool IsGreaterThanOrEqualTo(Value& rhs);
        virtual bool IsLesserThanOrEqualTo(Value& rhs);
        virtual bool TryDeleteCreatedByOp();
        static bool TryDeleteCreatedByOp(Value* o);
    };
    ostream& operator<<(Value& mt, ostream& o);
    ostream& operator<<(ostream& o, Value& mt);
    ostream& operator<<(Value& mt, const ostream& o);
    ostream& operator<<(ostream& o, const Value& mt);
    Value& operator+(Value& lhs, Value& rhs);
    Value& operator-(Value& lhs, Value& rhs);
    Value& operator*(Value& lhs, Value& rhs);
    Value& operator/(Value& lhs, Value& rhs);
    Value& operator%(Value& lhs, Value& rhs);

    class DirectValue : public Value{
    public:
        DirectValue(bool allocatedWithNew = true, bool createdByOperator = false) : Value(allocatedWithNew, createdByOperator){}
        bool IsNull();
        Type GetType();
        Type GetValueType();
        bool IsDirectValue();
    };

    class Number : public DirectValue{
    private:
        double value;
    public:
        Number(bool allocatedWithNew = true, bool createdByOperator = false) : DirectValue(allocatedWithNew, createdByOperator){}
        Type GetType();
        Number(int val, bool allocatedWithNew = true, bool createdByOperator = false);
        Number(float val, bool allocatedWithNew = true, bool createdByOperator = false);
        Number(double val, bool allocatedWithNew = true, bool createdByOperator = false);
        bool HasNan();
        bool IsNumber();
        double GetDouble();
        bool SetValue(Value* rhs);
        void SetNumber(double v);
        void SetNumber(float v);
        void SetNumber(int v);
        static int TryRead(char* s, Number** out);
        ostream& Print(ostream& o);
        Value& Add(Value& rhs);
        Value& Subtract(Value& rhs);
        Value& Multiply(Value& rhs);
        Value& Divide(Value& rhs);
        Value& Modulus(Value& rhs);
        Value& Pow(Value& rhs);
        bool IsNegative();
        MathT* Copy();
        bool IsLesserThan(Value& rhs);
        bool IsGreaterThan(Value& rhs);
    };
    class SemiDirectValue : public Value{
    public:
        bool isDirect;
        SemiDirectValue(bool allocatedWithNew = true, bool createdByOperator = false) : Value(allocatedWithNew, createdByOperator){}
        bool IsNull();
        Type GetType();
        Type GetValueType();
        bool IsDirectValue();
    };
    class Vector2 : public SemiDirectValue{
    public:
        Value* x;
        Value* y;
        ~Vector2();
        Vector2(bool allocatedWithNew = true, bool createdByOperator = false) : SemiDirectValue(allocatedWithNew, createdByOperator){}
        Type GetType();
        Vector2(Number* x0, Number* y0, bool allocatedWithNew = true, bool createdByOperator = false);
        Vector2(double x0, double y0, bool allocatedWithNew = true, bool createdByOperator = false);
        static bool TryCreate(Value* x0, Value* y0, Vector2** out);
        Number Magnitude();
        bool Normalize(bool force = false);
        Vector2 Normalized();
        bool IsVector();
        ostream& Print(ostream& o);
        Number* GetX(){return (Number*)(x->GetValuePtr());}
        Number* GetY(){return (Number*)(y->GetValuePtr());}
        double GetXDouble();
        double GetYDouble();
        bool SetX(double x1, bool force = false);
        bool SetY(double y1, bool force = false);
        Value& Add(Value& rhs);
        Value& Subtract(Value& rhs);
        Value& Multiply(Value& rhs);
        Value& Divide(Value& rhs);
        bool IsEqualTo(Value& rhs);
        static Number& Dot(Vector2& lhs, Vector2& rhs);
        static Number& Cross(Vector2& lhs, Vector2& rhs);
        static Vector2& Project(Vector2& lhs, Vector2& rhs);
        static Vector2& Reject(Vector2& lhs, Vector2& rhs);
        static Vector2& Lerp(Vector2& lhs, Vector2& rhs, Number& t);
        static Vector2& MoveToward(Vector2& lhs, Vector2& rhs, Number& maxDelta);
        static Vector2& RotateAround(Vector2& v, Number& radians, Vector2& pivot = *zero);
        static Vector2* zero;
    };
    class Variable : public Value{
    private:
        Type type;
    public:
        string name;
        Value* value;
        Variable(bool allocatedWithNew = true, bool createdByOperator = false) : Value(allocatedWithNew, createdByOperator){}
        ~Variable();
        Variable(string name, Type t, bool allocatedWithNew = true, bool createdByOperator = false);
        Variable(string name, Value* v, bool del = false, bool allocatedWithNew = true, bool createdByOperator = false);
        void SetName(char* s);
        bool SetValueType(Type t);
        Type GetType();
        Value* GetValuePtr();
        Type GetValueType();
        bool SetValue(Value* v);
        static void Init();
        ostream& Print(ostream& o);
        MathT* Copy();
        static int MayHaveVariableAtFront(char* s, char** out = null, bool isDefine = false);
        static int HasVariableAtFront(char* s, string* out = null, Vars* vars = null);
        static bool HasVariable(string s, string* out = null, Vars* vars = null);
        static int TryRead(char* s, Value** out, Vars* vars = null);
    };

    class VariableWrapper : public Value{
    public:
        VariableWrapper(bool allocatedWithNew = true, bool createdByOperator = false) : Value(allocatedWithNew, createdByOperator){}
        string name;
        Vars* source;
        VariableWrapper(string name0, Vars* source0, bool allocatedWithNew = true, bool createdByOperator = false);
        Value* GetValuePtr();
        Type GetValueType();
        ostream& Print(ostream& o);
        void SetName(char* name0);
        static int HasVariableAtFront(char* s, Vars* vars, string* out = null);
        static bool HasVariable(string s, Vars* vars, string* out = null);
    };

    class GlobalVariable : public VariableWrapper{
    public:
        static map<string,Variable*> globalVars;
        GlobalVariable(bool allocatedWithNew = true, bool createdByOperator = false) : VariableWrapper(allocatedWithNew, createdByOperator){}
        GlobalVariable(string name0, bool allocatedWithNew = true, bool createdByOperator = false);
        static int TryRead(char* s, GlobalVariable** out);
        static void Init();
        static bool TryDefine(char* lhs, char* rhs);
        static int HasVariableAtFront(char* s, string* out = null);
        static bool HasVariable(string s, string* out = null);
        static void PrintGlobalVars();
    };

    class Operator : public MathT{
    public:
        static vector<Operator*> operators;
        static int opCount;
        static void Init();
        Operator(bool allocatedWithNew = true) : MathT(allocatedWithNew){}
        ~Operator(){cout << "op dlet" << endl;}
        Type GetType();
        int OperationGrade();
        static int TryRead(char* s, Operator** out);
        virtual Value& Operate(Value& lhs, Value& rhs);


        class Add;
        class Subtract;
        class Multiply;
        class Divide;
        class Modulus;
        class Pow;
    };
    class Operator::Add : public Operator{
    public:
        Add(bool allocatedWithNew = true) : Operator(allocatedWithNew){}
        Value& Operate(Value& lhs, Value& rhs);
        ostream& Print(ostream& o);
        MathT* Copy();
    };
    class Operator::Subtract : public Operator{
    public:
        Subtract(bool allocatedWithNew = true) : Operator(allocatedWithNew){}
        Value& Operate(Value& lhs, Value& rhs);
        ostream& Print(ostream& o);
        MathT* Copy();
    };
    class Operator::Multiply : public Operator{
    public:
        Multiply(bool allocatedWithNew = true) : Operator(allocatedWithNew){}
        Value& Operate(Value& lhs, Value& rhs);
        ostream& Print(ostream& o);
        MathT* Copy();
        int OperationGrade();
    };
    class Operator::Divide : public Operator{
    public:
        Divide(bool allocatedWithNew = true) : Operator(allocatedWithNew){}
        Value& Operate(Value& lhs, Value& rhs);
        ostream& Print(ostream& o);
        MathT* Copy();
        int OperationGrade();
    };
    class Operator::Modulus : public Operator{
    public:
        Modulus(bool allocatedWithNew = true) : Operator(allocatedWithNew){}
        Value& Operate(Value& lhs, Value& rhs);
        ostream& Print(ostream& o);
        MathT* Copy();
    };
    class Operator::Pow : public Operator{
    public:
        Pow(bool allocatedWithNew = true) : Operator(allocatedWithNew){}
        Value& Operate(Value& lhs, Value& rhs);
        ostream& Print(ostream& o);
        MathT* Copy();
        int OperationGrade();
    };

    class Expression : public Value{
    public:
        Expression(bool allocatedWithNew = true, bool createdByOperator = false) : Value(allocatedWithNew, createdByOperator){}
        Type GetType();
        vector<MathT*> members;
        ~Expression();
        bool IsExpression();
        virtual int TryRead(char* s, Vars* vars = null);
        static int TryRead(char* s, Expression** out, Vars* vars = null);
        Value* GetValuePtr();
        ostream& Print( ostream& o);
        MathT* Copy();
        void Copy(Expression* x);
        void OrderOperations();
        void OrderOperations(int grade);
    };

    ostream& operator<<(ostream& o, Expression& x);
    ostream& operator<<(Expression& x, ostream& o);

    class Bracket : public Expression{
    public:
        Bracket(bool allocatedWithNew = true, bool createdByOperator = false) : Expression(allocatedWithNew, createdByOperator){}
        Type GetType();
        bool IsBracket();
        static int TryRead(char* s, Bracket** out, Vars* vars = null);
        ostream& Print(ostream& o);
        MathT* Copy();
    };
    typedef map<string,Function*> FunctionTemplates;
    class Function : public Value{
    protected:
        Function(bool allocatedWithNew = true, bool createdByOperator = false) : Value(allocatedWithNew, createdByOperator){}
    public :
        ~Function();
        bool IsNull();
        bool IsFunction();
        Type GetType();
        string name;
        static FunctionTemplates templates;
        Vars variables;
        bool TryInputVariables(char* s);
        static bool TryDefine(char* s);
        static bool TryDefine(char* name, Function* var);
        static int TryRead(char* s, Function** out);
        virtual ostream& Print(ostream& o);
        virtual Function* CopyF();
        static void Init();
        static int MayHaveFunctionAtFront(char* s, char** out = null);
        static int HasFunctionAtFront(char* s, string* out = null);
        static bool HasFunction(string s, string* out = null);
        int HasVariableAtFront(char* s, string* out = null);
        bool HasVariable(string s, string* out = null);
        static void PrintFunctions();
        MathT* Copy();
    };


    class InternalFunction : public Function{
    public:
        InternalFunction(bool allocatedWithNew = true, bool createdByOperator = false) : Function(allocatedWithNew, createdByOperator){}
        bool IsInternalFunction();
        Type GetType();
        class Sqrt;
        class Log;
        class ConsV2;
    };
    class InternalFunction::ConsV2 : public InternalFunction{
    public :
        static char* funcName;
        ConsV2(bool allocatedWithNew = true, bool createdByOperator = false);
        Value* GetValuePtr();
        Function* CopyF();
        Type GetValueType();
    };
    class InternalFunction::Sqrt : public InternalFunction{
    public :
        static char* funcName;
        Sqrt(bool allocatedWithNew = true, bool createdByOperator = false);
        Value* GetValuePtr();
        Function* CopyF();
        Type GetValueType();
    };
    class InternalFunction::Log : public InternalFunction{
    public :
        static char* funcName;
        Log(bool allocatedWithNew = true, bool createdByOperator = false);
        Value* GetValuePtr();
        Function* CopyF();
        Type GetValueType();
    };
    class StringFunction : public Function{
    protected:
        StringFunction(bool allocatedWithNew = true, bool createdByOperator = false) : Function(allocatedWithNew, createdByOperator){}
    public :
        char* fString = null;
        Type valueType;
        Type GetType();
        ~StringFunction();
        Value* GetValuePtr();
        Type GetValueType();
        Function* CopyF();
        int TryReadVar(char* s, VariableWrapper** out);
        static bool TryDefine(char* lhs, char* rhs);
    };
    bool Ask(string question);
    bool IsNullOrEmpty(char* s);
    int PreceedingSpaces(char* s);
    bool IsNumber(char c);
    bool HasNumber(char* c);
    bool IsUppercaseAlphabet(char c);
    bool IsLowercaseAlphabet(char c);
    bool IsAlphabet(char c);
    bool ContainsAtFront(char* s, const char* ss);
    int ArgumentLength(char* s, bool total = false);
    bool IsUnusable(string s);

    namespace TwentyFour{
        extern Number twentyFour;
        bool TryBrackets(Expression* ex);
        bool TryOperator(Expression* ex, Number** nums, int i);
        bool TryOperators(Number** nums);
        void Solve(int* nums0);
        void Solve(Number** nums);
        void Read(char* s);
        void Run();
    }
}

#define CALC2_HPP
#endif
