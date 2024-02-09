#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include <string>
#include <set>
#include <vector>
#include <map>
#include <queue>

class MyDataStore : public DataStore
{
public: 
    
    // MyDataStore(const std::map<std::set<std::string>, std::set<Product*>>& newProducts,
    //             const std::set<User*>& moreUsers,
    //             const std::set<Product*>& moreProducts);

    ~MyDataStore();

    void addProduct(Product* p);
    void addUser(User* u);
    std::vector<Product*> search(std::vector<std::string>& terms, int type);
    void dump(std::ostream& ofile);
    void addToCart(const std::string& username, Product* product);
    std::vector<Product*> viewCart(const std::string& username);

    void buyCart(const std::string& username);


private:
    std::map<std::string, std::set<Product*>> newProducts_;
    std::set<User*> moreUsers_;
    std::set<Product*> moreProducts_;
    std::map<std::string, std::queue<Product*>> userCarts_;
    
};

#endif 
