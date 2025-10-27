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

    void setName(str::string Name)
    {
        this->Name = Name;
    }
};


class Counter
{
private:
    static int count;
public:
    int sum(int x, int y)
    {
        count++;
        return x+y;
    }
    int getCount()
    {
        return count;
    }
};

int Counter::count = 0;


int main()
{
    #if 0
    Person bob;
    // bob.Name = "Bob";
    // bob.age = 37;
    // bob.salary = 150000;
    bob.setName("Bob");

    bob.sayHello();

    Person anna;
    anna.Name = "Anna";
    anna.age = 37;
    anna.salary = 100000;

    bob.multSalary();
    #endif


    Counter c1, c2;
    for (int i = -5; i <= 5; i++){
        std::cout << (c1.sum(i,i)) << ' ';
    }
    c2.sum(1,2);
    std::cout << c2.getCount() << std::endl;

}