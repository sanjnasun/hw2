#ifndef CLOTHING_H
#define CLOTHING_H
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "product.h"


using namespace std;

//how come when you inhert from product, it doesn't require the .includ file?

class Clothing : public Product
{
    public:
    
    Clothing(const std::string category, const std::string name, double price, int qty,
         const std::string size, std::string brand);

    std::set<std::string> keywords() const; 
    std::string displayString() const;
    void dump(std::ostream& os) const;
    

    ~Clothing();

    private:
    std::string size_;
    std::string brand_;

};

#endif