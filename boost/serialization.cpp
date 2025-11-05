#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <fstream>
#include <string>
#include <iostream>

// Объявление класса для сериализации
class Person
{
public:
    std::string get_name()
    {
        return this->name;
    }
    int get_age()
    {
        return this->age;
    }
    Person() = default;
    Person(std::string n, int a) : name(std::move(n)), age(a) {}

private:
    std::string name;
    int age;
    // Разрешение доступа библиотеке Boost.Serialization для доступа к приватным членам
    friend class boost::serialization::access;
    // Шаблонная функция для сериализации/десериализации
    template <class Archive>
    void serialize(Archive &ar, const unsigned int /*version*/)
    {
        ar &BOOST_SERIALIZATION_NVP(name);
        ar &BOOST_SERIALIZATION_NVP(age);
    }
};

void save_to_xml(const Person &p, const std::string &filename)
{
    std::ofstream ofs(filename); // Создаем поток файла для записи
    boost::archive::xml_oarchive oa(ofs);
    oa << BOOST_SERIALIZATION_NVP(p);
}

Person load_from_xml(const std::string &filename)
{
    Person p;                    // Пустой объект для загрузки данных
    std::ifstream ifs(filename); // Создаем поток файла для чтения
    boost::archive::xml_iarchive ia(ifs);
    ia >> BOOST_SERIALIZATION_NVP(p);
    return p;
}

int main()
{
    Person person_in("Alice", 30);

    // Сохранение в XML
    save_to_xml(person_in, "person.xml");
    std::cout << "Объект сохранен в person.xml" << std::endl;

    // Загрузка из XML
    Person person_out = load_from_xml("person.xml");
    std::cout << "Загруженный объект: " << person_out.get_name() << ", " << person_out.get_age() << std::endl;
}
