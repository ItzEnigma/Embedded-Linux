#include <iostream>

class Calculator
{
public:
    Calculator(double first_num = 0, double second_num = 0) : _first_num(first_num),
                                                              _second_num(second_num) {}

    double Add()
    {
        return _first_num + _second_num;
    }
    double Subtract()
    {
        return _first_num - _second_num;
    }
    double Multiply()
    {
        return _first_num * _second_num;
    }
    double Divide()
    {
        return _first_num / _second_num;
    }
    static inline int myVar = 20;

private:
    double _first_num;
    double _second_num;
};

class copy_constructor
{
public:
    copy_constructor()
    {
        std::cout << "Copy Constructor \n";
        _arr = new int[10];
        for (int i = 0; i < 10; i++)
            _arr[i] = 0;
    }
    ~copy_constructor()
    {
        delete[] _arr;
    }

    copy_constructor(const copy_constructor &test)
    {
        _arr = new int[10];
        for (int i = 0; i < 10; i++)
            _arr[i] = test._arr[i];
    }

    int test(const copy_constructor &test)
    {
        return test._arr[0];
    }

private:
    int *_arr;
};

class inherited_copy_constructor : copy_constructor
{
public:
    inherited_copy_constructor()
    {
        // Copy Constructor will call the super (parent) constructor and if not found ..
        // will call the default constructor
        std::cout << "Inherited Copy Constructor \n";
    }
};

int main()
{
    Calculator myVariable(15, 234.21);
    std::cout << myVariable.Add() << " | " << myVariable.Subtract() << " | " << myVariable.Divide() << '\n';
    std::cout << myVariable.myVar << std::endl;
    copy_constructor cp1;
    copy_constructor cp2(cp1);
    inherited_copy_constructor icp1;

    return 0;
}