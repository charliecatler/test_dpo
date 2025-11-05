#include <iostream>

#if 0 // template
template <typename T>
class myArr
{
    T* arr;
    int n;
    public:
    myArr(int n): n(n)
    {
        arr = new T[n];
    }
    ~myArr()
    {
        delete arr;
    };
    int size()
    {
        return n;
    }
    T& get(int i){
        return arr[i];
    }
};

struct Smth {
    int a; 
    double b;
    friend std::ostream& operator << (std::ostream& out, Smth /* tut doljno bit chto-to eshe */);
    out << '{' << obj.a << ':' << obj.b << '}';
    return out;
};

template <typename T>
void print(T &obj){
    T();
    std::cout << obj << "\n";
}

int main(){
    myArr <char> arr1(5);
    for (int i = 0; i < arr1.size(); ++i)
    {
        arr1.get(i) = 'A'+i;
    }
    myArr <Smth> arr2(4); // svoi tip dannih
    Smth obj;
    obj.a = 10;
    obj.b = 2.5;
    print(10);
    print(obj);
}
#endif

#if 0 // vector + iterators
#include <vector>

int main(){
    std::vector vec = {'a', 'b', 'c'};
    std::cout << vec.size() << ' ' << vec.capacity() << '\n';
    vec.push_back('d');
    std::cout << vec.size() << ' ' << vec.capacity() << '\n';
    // vec.clear();
    // std::cout << vec.size() << ' ' << vec.capacity() << '\n';
    // std::cout << vec[0] << ' ' << vec[1]  << ' ' << vec[3] << '\n';
    vec.shrink_to_fit();
    std::cout << vec.size() << ' ' << vec.capacity() << '\n';
    
    auto it1 = vec.begin();
    std::cout << *it1 << '\n';
    // ++it1;
    // it1 += 2;
    std::cout << it1[2] << '\n';
}
#endif

#if 0 //
#include <vector>

int main()
{
    std::vector<int> vec;
    int n;
    std::cout << "N = ";
    std::cin >> n;
    vec.resize(n);
    for (auto &elem : vec)
    {
        std::cin >> elem;
    }
    for (auto it = vec.rbegin(); it != vec.rend(); ++it)
    {
        std::cout << *it << ' ';
    }
}

#endif

#if 0 // list
#include <list>

int main()
{
    std::list<int> vec;
    int n;
    std::cout << "N = ";
    std::cin >> n;
    vec.resize(n);
    for (auto &elem : vec)
    {
        std::cin >> elem;
    }
    for (auto it = vec.rbegin(); it != vec.rend(); ++it)
    {
        std::cout << *it << ' ';
    }
}

#endif

#if 0
// std::pair
#include <map>
#include <string>

int main(){
    std::map <std::string, int> dict;
    int n;
    std::cin >> n;
    std::string key;
    for (int i = 0; i < n; ++i){
        std::cin >> key;
        dict[key] += 1;
    }
    for (auto elem: dict){
        std::cout << "Word " << elem.first << " count " << elem.second << std::endl;

    }
}
#endif

#if 0 // algorithm - for_each & find_if
#include <algorithm>
#include <vector>

struct Person
{
    std::string name;
    int age;
};
void printPerson(Person &p)
{
    std::cout << "name: " << p.name << ", age: " << p.age << std::endl;
}
bool isBoris(Person &p)
{
    if (p.name == "Boris")
        return true;
    else
        return false;
}

int main()
{
    std::vector<Person> vec = {{"Anya", 25},
                               {"Boris", 18},
                               {"Maomao", 19}};
    std::for_each(vec.begin(), vec.end(), printPerson);
    auto it = std::find_if(vec.begin(), vec.end(), isBoris);
    if (it != vec.end())
        std::cout << it->name << std::endl;
    else
        std::cout << "not found";
}

#endif

#if 1
#include <algorithm>
#include <iterator>
#include <vector>

bool is(int x){
    return x < 10 && x > -10;
};

int main()
{
    std::vector<int> vec;
    std::copy(std::istream_iterator<int>(std::cin),
              std::istream_iterator<int>(),
              std::back_inserter(vec));
    std::for_each(vec.begin(), vec.end(), [](int x){std::cout << x << " ";});
    //ctrl+d
    std::ofstream fout("out_copy", std::ios_base::app);
    std::copy_if(vec.begin(), vec.end(), std::ostream_iterator<int>(fout, ","));
}
#endif