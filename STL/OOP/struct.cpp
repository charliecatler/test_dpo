#include <iostream>
#include <string>

struct Person{
private:
    std::string name = "Alexey";
    int age = 30;
    int salary = 11111;
public:
    void sayHello() const{
        std::cout << "Hello. I am " << name << '\n';
    }

    void setName(std::string name){
        this->name = name;
    }

    void increaseSalary(int value){
        salary += value;
    }

    void multSalary(){
        salary = salary * 2 + salary / 2;
    }
};

// void incSalary(Person& person, int value){
//     person.salary = person.salary * 2 + value;
// }

int main(){
    const Person bob;
    // bob.name = "Bob";
    // bob.age = 34;
    // bob.salary = 10000;
    //bob.setName("Bob");
    bob.sayHello();

    // Person anna;
    // anna.salary = 5000;

    // //incSalary(bob, anna.salary / 2);

    // bob.multSalary();

    // std::cout << "Salary: " << bob.salary << '\n';
};