#include <iostream>
using namespace std;

class stack_size_error : public length_error {
public:
    explicit stack_size_error(const std::string &msg);
};

class stack_bad_alloc : public std::bad_alloc {
public:
    explicit stack_bad_alloc(void);
};

class stack_overflow : public std::logic_error {
public:
    explicit stack_overflow(const std::string &msg);
};

class stack_empty : public std::logic_error {
public:
    explicit stack_empty(const std::string &msg);
};

stack_size_error::stack_size_error(const std::string &msg) : std::length_error(msg) {};
stack_bad_alloc::stack_bad_alloc(void) : std::bad_alloc() {};
stack_overflow::stack_overflow(const std::string &msg) : std::logic_error(msg) {};
stack_empty::stack_empty(const std::string &msg) : std::logic_error(msg) {};

class Stack {
private:
    int *stackstore;
    int stacksize;
    int SP;
public:
    Stack(int size = 100);
    ~Stack();
    void push(int value);
    int pop(void);
};

Stack::Stack(int size) {
    if (size <= 0)
        throw stack_size_error("Size must be positive!");
    try {
        stackstore = new int[size];
    } catch (std::bad_alloc ba) {
        throw stack_bad_alloc();
    }
    stacksize = size;
    SP = 0;
}

Stack::~Stack(void) {
    delete[] stackstore;
}

void Stack::push(int value) {
    if (SP == stacksize)
        throw stack_overflow("Stack overflow!");
    stackstore[SP++] = value;
}

int Stack::pop(void) {
    if (SP == 0)
        throw stack_empty("Stack is empty!");
    return stackstore[--SP];
}

int main() {
    Stack stk;

    try {
        for (int i = 0; i < 100; i++) {
            stk.push(i);
        }
        stk.push(8);
    } catch (stack_overflow &exc) {
        cout << "Stack overflow: " << exc.what() << endl;
    }

    try {
        for (int i = 0; i < 101; i++) {
            cout << stk.pop() << endl;
        }
    } catch (stack_empty &exc) {
        cout << "Stack is empty: " << exc.what() << endl;
    }

    try {
        Stack stk2(-5);
    } catch (stack_size_error &exc) {
        cout << "Stack size error: " << exc.what() << endl;
    }

    try {
        Stack stk3;
        for (int i = 0; i < 1000000000; ++i) {
            stk3.push(i); 
        }
    } catch (stack_bad_alloc &exc) {
        cout << "Memory allocation error: " << exc.what() << endl;
    }

    return 0;
}
