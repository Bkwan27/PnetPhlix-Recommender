#include "UserDatabase.h"
#include "User.h"
#include "treemm.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

UserDatabase::UserDatabase()
{
    loaded = false;
}

UserDatabase::~UserDatabase() {
    vector<User*>::iterator i = users.begin(); //destroys allocated users
    for (; i != users.end();) {
        delete* i;
        i = users.erase(i);
    }
}

bool UserDatabase::load(const string& filename)
{
    if (loaded) return false;
    else loaded = false;
    ifstream file;
    file.open(filename);
    while (!file.eof()) {
        string name;
        string email;
        string history;
        getline(file, name);
        getline(file, email);
        getline(file, history);
        int watch_size = 0;
        for (int i = 0; i < history.size(); i++) { //converts history to an int of watch size
            watch_size *= 10;
            watch_size += history[i] - '0';
        } 
        vector<string> movies;
        string movie;
        while (watch_size > 0) { //pushes all movies watched into a vector
            getline(file, movie);
            movies.push_back(movie);
            watch_size--;
        }
        User* add = new User(name, email, movies);
        database.insert(email, add); //inserts user into database with email as key
        users.push_back(add);
        file.ignore(10000, '\n');

        if (file.eof())
            loaded = true;
    }
    return loaded;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<string, User*>::Iterator i = database.find(email);
    if (i.is_valid())
        return i.get_value();
    else
        return nullptr;
}
