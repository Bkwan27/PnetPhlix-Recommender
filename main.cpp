#include "UserDatabase.h"
#include "User.h"
#include "treemm.h"
#include "MovieDatabase.h"
#include"Movie.h"
#include "Recommender.h"
#include <chrono>
#include <iostream>
#include <string>
using namespace std;
using namespace std::chrono;

//////////////////////////i/////////////////////////////////////////////////////
//
// You are free to do whatever you want with this file, since you won't
// be turning it in.  Presumably, you will make changes to help you test
// your classes.  For example, you might try to implement only some User
// member functions to start out with, and so replace our main routine with
// one that simply creates a User and verifies that the member functions you
// implemented work correctly.
//
//////////////////////////i/////////////////////////////////////////////////////


  // If your program is having trouble finding these files. replace the
  // string literals with full path names to the files.  Also, for test
  // purposes, you may want to create some small, simple user and movie
  // data files to makde debuggiing easier, so you can replace the string
  // literals with the names of those smaller files.

const string USER_DATAFILE  = "users.txt";
const string MOVIE_DATAFILE = "movies.txt";

void findMatches(const Recommender& r,
	const MovieDatabase& md,
	const string& user_email,
	int num_recommendations) {
	// get up to ten movie recommendations for the user
	vector<MovieAndRank> recommendations =
		r.recommend_movies(user_email, 20);
	if (recommendations.empty())
		cout << "We found no movies to recommend :(.\n";
	else {
		for (int i = 0; i < recommendations.size(); i++) {
			const MovieAndRank& mr = recommendations[i];
			Movie* m = md.get_movie_from_id(mr.movie_id);
			cout << i << ". " << m->get_title() << " ("
				<< m->get_release_year() << ")\n Rating: "
				<< m->get_rating() << "\n Compatibility Score: "
				<< mr.compatibility_score << "\n";
		}
	}
}

int main()
{
	/*TreeMultimap<std::string, int> tmm;
	tmm.insert("carey", 5);
	tmm.insert("carey", 6);
	tmm.insert("carey", 7);
	tmm.insert("david", 25);
	tmm.insert("david", 425);
	TreeMultimap<std::string, int>::Iterator it = tmm.find("carey");
	// prints 5, 6, and 7 in some order
	while (it.is_valid()) {
		std::cout << it.get_value() << std::endl;
		it.advance();
	}
	it = tmm.find("laura");
	if (!it.is_valid())
		std::cout << "laura is not in the multimap!\n"; */
	/*UserDatabase udb;

	if (!udb.load("test.txt"))  // In skeleton, load always return false
	{
		cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
		return 1;
	}
	for (;;)
	{
		cout << "Enter user email address (or quit): ";
		string email;
		getline(cin, email);
		if (email == "quit")
			return 0;
		User* u = udb.get_user_from_email(email);
		if (u == nullptr)
			cout << "No user in the database has that email address." << endl;
		else
			cout << "Found " << u->get_full_name() << endl;
	} */

	UserDatabase udb;
	MovieDatabase mdb;
	if (!mdb.load(MOVIE_DATAFILE))  // In skeleton, load always return false
	{
		cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
		return 1;
	}
	if (!udb.load(USER_DATAFILE))  // In skeleton, load always return false
	{
		cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
		return 1;
	}
	/*vector<Movie*> actor = mdb.get_movies_with_actor("carl lange");
	for (int i = 0; i < actor.size(); i++) {
		cout << actor[i]->get_title() << endl;
	} */
	Recommender r(udb, mdb);
	//auto start = high_resolution_clock::now();
	int recCount = 5;
	findMatches(r, mdb, "HezekF0394@aol.com", recCount);
	//auto stop = high_resolution_clock::now();

	//auto duration = duration_cast<seconds>(stop - start);
	//cout << "Time taken by function " << duration.count() << endl;
	/*vector<MovieAndRank> recommend = r.recommend_movies("BrunB@yahoo.com", recCount);
	for (int i = 0; i < recommend.size(); i++) {
		cerr << recommend[i].movie_id << endl;
		cerr << recommend[i].compatibility_score << endl;
	} */
	/*for (;;)
	{
		cout << "Enter genre: ";
		string id;
		getline(cin, id);
		vector<Movie*> director = mdb.get_movies_with_genre(id);
		for (int i = 0; i < director.size(); i++) {
			cout << director[i]->get_title() << endl;
		}
	} */
}
