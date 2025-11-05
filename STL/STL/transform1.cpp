#include<iostream>
#include<algorithm>
#include<vector>
#include<deque>

struct Person{
    std::string name;
    int age;
};

std::string printPerson(Person& p){
    std::string str = "Name: " + p.name + ", Age " 
                        + std::to_string(p.age) + '\n';
    return str;
}

bool isBoris(Person& p){
    return p.name == "Boris";
}

int main(){
    std::deque<Person> vec = {{"Anya",20},
                               {"BoriS",13},
                               {"Vladimir",40}};

    std::vector<std::string> str_vec;
    std::transform(vec.begin(), vec.end(),
                    std::back_inserter(str_vec),
                    printPerson);
    for(auto str: str_vec){
        std::cout << str;
    }

    // std::for_each(vec.begin()+1,vec.end(), printPerson);

    // auto it = std::find_if(vec.begin(), vec.end(), isBoris);
    // if(it != vec.end()){
    //     std::cout << it->name << '\n';
    // }
    // else{
    //     std::cout << "Not found\n";
    // }

}