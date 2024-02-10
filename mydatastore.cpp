#include <string>
#include <set>
#include <vector>
#include <map>
#include "util.h"
#include "mydatastore.h"
using namespace std;




MyDataStore::~MyDataStore() 
{
    for (Product* p : moreProducts_) 
    {
        delete p;
    }
    // Delete all users
    for (User* u : moreUsers_) {
        delete u;
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

        std::set<Product*> intersection = newProducts_[terms[0]];

        for (size_t i = 1; i < terms.size(); ++i) {
            intersection = setIntersection(intersection, newProducts_[terms[i]]);
        }

        std::vector<Product*> sortedIntersection(intersection.begin(), intersection.end());
        std::sort(sortedIntersection.begin(), sortedIntersection.end(), [](Product* a, Product* b) {
            return a->getName() < b->getName();
        });

        results.insert(results.end(), sortedIntersection.begin(), sortedIntersection.end());
    } 
    else if (type == 1) { 
        std::set<Product*> unionSet;

        for (size_t i = 0; i < terms.size(); ++i) {
            unionSet = setUnion(unionSet, newProducts_[terms[i]]);
        }

        std::vector<Product*> sortedUnion(unionSet.begin(), unionSet.end());
        // Sorting might be needed here if order matters for the union operation

        results.insert(results.end(), sortedUnion.begin(), sortedUnion.end());
    }

    return results; 
}




void MyDataStore::dump(std::ostream& ofile) 
{
    // Dump products
    ofile << "<products>" << std::endl;
    for (Product* product : moreProducts_) {
       product->dump(ofile); 
    }
    ofile << "</products>" << std::endl;

    // Dump users
    ofile << "<users>" << std::endl;
    for (User* user : moreUsers_) {
        user->dump(ofile); 
    }
    ofile << "</users>" << std::endl;
}






void MyDataStore::addToCart(const std::string& username, Product* product) {
    if (username.empty() || product == nullptr) {
        cout << "Invalid request" << endl;
        return;
    }

    std::set<User*>::iterator it;
    bool userExists = false;

    // Check if the user exists
    for(it = moreUsers_.begin(); it != moreUsers_.end(); ++it) {
        if ((*it)->getName() == username) {
            userExists = true;
            break;
        }
    }

    if (userExists) {
        // Validate if the product is not nullptr
        if (product != nullptr) {
            // Check if the hit index is valid (quantity of the product > 0)
            if (product->getQty() > 0) {
                // Add the product to the user's cart
                if (userCarts_.find(username) != userCarts_.end()) {
                    userCarts_[username].push(product);
                } else {
                    std::queue<Product*> newProd;
                    newProd.push(product);
                    userCarts_[username] = newProd;
                }
            } else {
                cout << "Invalid request" << endl; //prod not avaiblae
            }
        } 
        else {
            cout << "Invalid request" << endl; //no product
        }
    } else {
        cout << "Invalid request" << endl; //no user
    }
}


void MyDataStore::viewCart(const std::string& username) 
{
    if (username.empty()) 
    {
        cout << "Invalid request" << endl;
        return;
    }

    std::set<User*>::iterator it;

    bool userExists = false;

    for(it = moreUsers_.begin(); it != moreUsers_.end(); ++it) 
    {
        if ((*it)->getName() == username) 
        {
            userExists = true;
            break; 
        }
    }

    if(!userExists) {
        cout << "Invalid username" << endl;
        return; 
    }

    std::queue<Product*> userCart = userCarts_[username];
    std::queue<Product*> tempCart = userCart;
    int count = 1;
    while (!tempCart.empty()) 
    {
        cout << "Item " << count << "\n" << tempCart.front()->displayString() << endl;
        tempCart.pop();
        count++;
    }

}


// void MyDataStore::buyCart(const std::string& username) 
// {
//     std::string lowercaseUsername = convToLower(username);

//     std::set<User*>::iterator userIt = std::find_if(
//         moreUsers_.begin(), moreUsers_.end(), [&](User* user) { 
//             return convToLower(user->getName()) == lowercaseUsername; });

//     // If user doesn't exist
//     if (userIt == moreUsers_.end()) {
//         cout << "Invalid username" << endl;
//         return;
//     }

//     // Balance
//     double userBalance = (*userIt)->getBalance();
//     std::queue<Product*>& cart = userCarts_[lowercaseUsername]; 

//     double totalPrice = 0.0;

//     // Calculate total price of cart
//     while (!cart.empty()) { 
//         Product* product = cart.front();
//         if (product->getQty() > 0) { 
//             totalPrice += product->getPrice();
//             product->subtractQty(1);
//             if (userBalance < totalPrice) {
//                 return;
//             }
//         }
//         cart.pop();
//     }

//     // Deduct total price from user balance
//     (*userIt)->deductAmount(totalPrice);
// }


void MyDataStore::buyCart(const std::string& username) 
{
    std::string lowercaseUsername = convToLower(username);

    std::set<User*>::iterator userIt = std::find_if(
        moreUsers_.begin(), moreUsers_.end(), [&](User* user) { 
            return convToLower(user->getName()) == lowercaseUsername; });

    // If user doesn't exist
    if (userIt == moreUsers_.end()) {
        cout << "Invalid username" << endl;
        return;
    }

    // Get the user's balance
    double userBalance = (*userIt)->getBalance();
    // Get the user's cart
    std::queue<Product*>& cart = userCarts_[lowercaseUsername]; 

    double totalPrice = 0.0;
    int purchasedItemsCount = 0;

    // Temporary queue to hold products that can be purchased
    std::queue<Product*> tempCart;

    // Calculate total price of items that can be purchased and update their quantity
    while (!cart.empty()) { 
        Product* product = cart.front();
        if (product->getQty() > 0 && (totalPrice + product->getPrice()) <= userBalance) { 
            totalPrice += product->getPrice();
            product->subtractQty(1);
            tempCart.push(product); // Add the purchased product to the temporary cart
            purchasedItemsCount++;
        }
        cart.pop();
    }

    // Transfer the purchased products back to the original cart
    while (!tempCart.empty()) {
        cart.push(tempCart.front());
        tempCart.pop();
    }

    // Deduct total price from user balance
    (*userIt)->deductAmount(totalPrice);
}


















