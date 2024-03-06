#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass(int val) : data(val), ptr(new int(val)) {}
    
    // Move constructor
    MyClass(MyClass&& other) noexcept : data(other.data), ptr(std::move(other.ptr)) {
        other.data = 0;
    }
    
    // Move assignment operator
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            data = other.data;
            ptr = std::move(other.ptr);
            other.data = 0;
        }
        return *this;
    }
    
    // Deleted copy constructor
    MyClass(const MyClass& other) = delete;
    
    // Deleted copy assignment operator
    MyClass& operator=(const MyClass& other) = delete;
    
    void print() const {
        std::cout << "Data: " << data << ", Ptr Value: " << *ptr << std::endl;
    }
    
private:
    int data;
    std::unique_ptr<int> ptr;
};

int main() {
    MyClass obj1(10);
    obj1.print();

    // This will result in a compilation error due to the deleted copy constructor
    // MyClass obj2 = obj1; // Error: use of deleted function 'MyClass::MyClass(const MyClass&)'

    // This will also result in a compilation error due to the deleted copy assignment operator
    // MyClass obj3;
    // obj3 = obj1; // Error: use of deleted function 'MyClass& MyClass::operator=(const MyClass&)'

    MyClass obj4 = std::move(obj1); // Move constructor called
    obj4.print();

    MyClass obj5;
    obj5 = std::move(obj4); // Move assignment operator called
    obj5.print();

    return 0;
}
