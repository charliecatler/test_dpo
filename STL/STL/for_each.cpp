#include<iostream>
#include<algorithm>
#include<vector>
#include<deque>

struct Person{
    std::string name;
    int age;
};

void printPerson(Person& p){
    std::cout << "Name: " << p.name << ", Age"
              << p.age << '\n';
}

bool isBoris(Person& p){
    return p.name == "Boris";
}

int main(){
    std::deque<Person> vec = {{"Anya",20},
                               {"BoriS",13},
                               {"Vladimir",40}};
    std::for_each(vec.begin()+1,vec.end(), printPerson);

    auto it = std::find_if(vec.begin(), vec.end(), isBoris);
    if(it != vec.end()){
        std::cout << it->name << '\n';
    }
    else{
        std::cout << "Not found\n";
    }

}