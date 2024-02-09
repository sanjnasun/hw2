
#include <string>
#include <set>
#include <vector>
#include <map>
#include "util.h"
#include "mydatastore.h"
using namespace std;



MyDataStore::~MyDataStore() 
{
    for (Product* p : moreProducts_) {
        delete p;
    }

    // Delete all users
    for (User* u : moreUsers_) {
        delete u;
    }

    for (std::map<std::string, std::set<Product*>>::iterator it = newProducts_.begin(); it != newProducts_.end(); ++it) {
        std::set<Product*>& productSet = it->second;
        for (std::set<Product*>::iterator setIt = productSet.begin(); setIt != productSet.end(); ++setIt) {
            delete *setIt;
        }
    }

    for (std::map<std::string, std::queue<Product*>>::iterator it = userCarts_.begin(); it != userCarts_.end(); ++it) {
        std::queue<Product*>& cart = it->second;
        while (!cart.empty()) {
            delete cart.front();
            cart.pop();
        }
    }
}

void MyDataStore::addProduct(Product* p) 
{
    //add a keyword pair, keyword function, connect that with the string of keywords, 
    //then based on that keyword, add the product to the map

    //populate the newproduct set

    // Check if newProducts_ is empty

    //adding it to more products
    moreProducts_.insert(p);

    //keyword
    std::set<std::string> keywords = p->keywords();

    //add each keyword 
    for (std::set<std::string>::iterator it = keywords.begin(); it != keywords.end(); ++it) {
        const std::string& keyword = *it;

        //make sure if it's already there
        std::map<std::string, std::set<Product*>>::iterator mapIt = newProducts_.find(keyword);
        if (mapIt == newProducts_.end()) {
            //not there, add
            std::set<Product*> productsSet;
            productsSet.insert(p);
            newProducts_[keyword] = productsSet;
        } else {
            //add it to the map
            mapIt->second.insert(p);
        }
    }
    
}


//adds a user to the user set, just insert
void MyDataStore::addUser(User* u) 
{
    moreUsers_.insert(u);
}

/**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) 
{

  std::vector<Product*> results;

    if (type == 0) { 
        if (terms.empty()) 
        return results; 

       
        std::set<Product*> intersection = newProducts_[terms[0]]; //set of producst equal to the area of terms
        
         
       for (size_t i = 1; i < terms.size(); ++i) {
            std::set<Product*> termProducts = newProducts_[terms[i]];
            std::set<Product*> temp;
            
            for (Product* product : termProducts) {
                if (intersection.find(product) != intersection.end()) {
                    temp.insert(product);
                }
            }
            intersection = temp;
        }
        results.insert(results.end(), intersection.begin(), intersection.end());
    } 
    
    else if (type == 1) {
    std::set<Product*> unionSet; 

    for (const std::string& term : terms) {
        if (newProducts_.find(term) != newProducts_.end()) {
            for (Product* product : newProducts_[term]) {
                unionSet.insert(product);
            }
        }
    }

    results.insert(results.end(), unionSet.begin(), unionSet.end());
}

    return results; 


}











void MyDataStore::dump(std::ostream& ofile) 
{
    // Dump users
    std::set<User*>::iterator userIt;
    for (userIt = moreUsers_.begin(); userIt != moreUsers_.end(); ++userIt) {
       
        User* user = *userIt;
        ofile << "User: " << user->getName() << std::endl; 
    }

    // Dump products
    std::set<Product*>::iterator productIt;
    for (productIt = moreProducts_.begin(); productIt != moreProducts_.end(); ++productIt) {
        Product* product = *productIt;
        ofile << "Product: " << product->getName() << std::endl;
    }
}




void MyDataStore::addToCart(const std::string& username, Product* product) {
    if (username.empty() || product == nullptr) {
        cout << "Invalid request" << endl;
        return;
    }
    // Add the product to the user's cart
    userCarts_[username].push(product);
}


std::vector<Product*> MyDataStore::viewCart(const std::string& username) {
    
        std::queue<Product*> userCart = userCarts_[username];
         std::queue<Product*> tempCart = userCart;
        while (!tempCart.empty()) {
        cout << tempCart.front()->displayString() << endl;
        tempCart.pop();
    }
}


void MyDataStore::buyCart(const std::string& username) {
    std::string lowercaseUsername = convToLower(username);

    //find hte user withint eh moreUser set
    std::set<User*>::iterator userIt = std::find_if(
        moreUsers_.begin(), moreUsers_.end(), [&](User* user) { 
            
            return convToLower(user->getName()) == lowercaseUsername; });

   //checking fi they exist or not
    if (userIt == moreUsers_.end()) {
        cout << "Invalid username" << endl;
        return;
    }

    //find balance
    double userBalance = (*userIt)->getBalance();
    std::queue<Product*>& cart = userCarts_[lowercaseUsername]; //cart

    while (!cart.empty()) {
        Product* product = cart.front();
        cart.pop();
        
        if (product->getQty() > 0) { 
            if (userBalance >= product->getPrice()) {
                (*userIt)->deductAmount(product->getPrice());
                product->subtractQty(1);
            } else {
                cart.push(product);
            }
        } 
    }
}
