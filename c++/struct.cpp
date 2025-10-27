#include <iostream>
#include <string>

struct Person
{
    std::string Name;
    int age;
    int salary;

    void sayHello()
    {
        std::cout << "Hi, I'm " << Name << std::endl;
    };
    // int increaseSalary(int value){
    //     salary += value;
    //     return salary;
    // };
    int multSalary()
    {
        salary = salary * 2 + salary / 2;
        return salary;
    };
};

int main()
{
    Person bob;
    bob.Name = "Bob";
    bob.age = 37;
    bob.salary = 150000;

    bob.sayHello();

    Person anna;
    anna.Name = "Anna";
    anna.age = 37;
    anna.salary = 100000;

    bob.multSalary();
}