#ifndef BOOK_H
#define BOOK_H
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "product.h"


using namespace std;

//how come when you inhert from product, it doesn't require the .includ file?

class Book : public Product
{
    public:
    
    Book(const std::string category, const std::string name, double price, int qty,
         const std::string author, std::string isbn);

    std::set<std::string> keywords() const; 
    std::string displayString() const;
    void dump(std::ostream& os) const;
    

    ~Book();

    private:
    std::string isbn_;
    std::string author_;

};

#endif