//test
#include "package.hxx"
#include <iostream>
int main() {
    std::cout<<"Hello World!"<<std::endl;

    //Package tests

    //Czy po usunięciu paczki nowa dostaje najniższe możliwe ID
    Package* package1 = new Package();
    Package package2;
    std::cout<<package1->get_id()<<std::endl;
    std::cout<<package2.get_id()<<std::endl;
    delete package1;
    Package package3;
    std::cout<<package3.get_id()<<std::endl;

    //Move constructor
    Package package4;
    std::cout<<package4.get_id()<<std::endl;
    Package package5(std::move(package4));
    std::cout<<package5.get_id()<<std::endl;

    //Assignment operation
    Package package6;
    std::cout<<package6.get_id()<<std::endl;
    Package package7 = std::move(package6);
    std::cout<<package7.get_id()<<std::endl;
    // NIBY DZIAŁA

}