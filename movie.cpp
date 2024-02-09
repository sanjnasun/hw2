#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "util.h"
//#include "product.h"
#include "movie.h"


//using namespace std;

Movie::Movie(const std::string category, const std::string name, double price, int qty,
           const std::string genre, const std::string rating)
    : Product(category, name, price, qty), genre_(genre), rating_(rating) {}


Movie::~Movie()
{

}


std::set<std::string> Movie::keywords() const {


    //std::string keywordList = author_ + " " + isbn_;

    //author (rawword)+ productName(raword) into a keyword set
    //call parsing function into author and productname separately, which returns a set
    //make two sets (empty), 
    //keywords isnow an empty set, now needs to insert a set into antoher set, need to iterate through author set
    //keywords.insert(*it)
    
    

    std::set<std::string> result1 = parseStringToWords(genre_);
    std::set<std::string> result2 = parseStringToWords(name_);


    std::set<std::string> finalResult;

    typename std::set<std::string>::iterator it;
    
    for(it = result1.begin(); it != result1.end(); ++it)
    {
        finalResult.insert(*it);
    }

    typename std::set<std::string>::iterator ita;
    
    for(ita = result2.begin(); ita != result2.end(); ++ita)
    {
        finalResult.insert(*ita);
    }


    //finalResult.insert(rating_);

    //make sure there are no repeating keywords


    

    return finalResult;




    //what is a set
    //n is iterator for set, frm set.begin -> end. 
    //log, use find function to find 
    //look into stl slides - intersection. check if they are both avilaible
    //union, all elements that are involved, not just everyone in the set. 
    //account for repeating varaible. make sure it doesn't repeat

    //get author and productname 

    //return parseStringToWords(keywordList);
}



std::string Movie::displayString() const
{
    std::string display = name_ + "\n";
    display += "Genre: " + genre_ + " Rating: " + rating_ + "\n";
    display += std::to_string(price_) + " " + std::to_string(qty_) + "left.";
    return display;
}

void Movie::dump(std::ostream& os) const
{
//idk what this even does, just copy from product ig
os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << genre_ << "\n" << rating_ << endl;
}

