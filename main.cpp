// Tester.cpp: Demonstrates inconsistency in the treatment of non-static
// initializers in VS2013.  Non-templated initializers have no problem,
// but templated initializers yield:
// error C2352 : 'A::create_myvec' : illegal call of non - static member function
//
#include "stdafx.h"
#include <iostream>
#include <vector>
 
template <int M>
class myvec {
public:
    std::vector<double> v;
    myvec(double iv) : v(M, iv) {}
};
 
 
class A {
public:
    myvec<1> av{ create_myvec<1>(0.1) };
    int val = init(11);
 
    int init(int i) {
        _true_value = i;
        return i;
    }
 
    template <int M> 
    myvec<M> create_myvec(double iv) {
        _true_size = M;
        return myvec<M>(iv);
    }
 
    int get_true_value() { return _true_value; }
    int get_true_size() { return _true_size; }
 
private:
    int _true_value;
    int _true_size;
};
 
class B : public A {
public:
 
    // no error calling non-static initializer that is not templated
    int valB = init(22);
 
    // In VS2023 the following causes:
    //    error C2352 : 'A::create_myvec' : illegal call of non - static member function
    //myvec<2> bv{ create_myvec<2>(0.22) };
 
};
 
 
class C : public B {
public:
    // no error calling non-static initializer in base A that is not templated
    int valC = init(33);
 
    // In VS2023 the following causes:
    //    error C2352 : 'A::create_myvec' : illegal call of non - static member function
    // myvec<3> cv{ create_myvec<3>(0.33) };
 
};
 
int main() {
    A a;
    B b;
    C c;
 
    std::cout << "a true value " << a.get_true_value() << "  a true size =" << a.get_true_size() << std::endl;
    std::cout << "b true value " << b.get_true_value() << "  b true size =" << b.get_true_size() << std::endl;
    std::cout << "c true value " << c.get_true_value() << "  c true size =" << c.get_true_size() << std::endl;
 
    // dump a.av values
    std::cout << "a.av=" << std::endl;
    for (auto entry : c.av.v)
        std::cout << entry << ", ";
    std::cout << std::endl;
 
    /*// dump c.cv values 
    std::cout << "c.cv=" << std::endl;
    for (auto entry : c.cv.v)
        std::cout << entry << ", ";
    std::cout << std::endl;
    */
}
