#include "MovieDatabase.h"
#include "Movie.h"
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

MovieDatabase::MovieDatabase()
{
    loaded = false;
}

MovieDatabase::~MovieDatabase() {
    vector<Movie*>::iterator i = movieList.begin();
    for (; i != movieList.end();) {
        delete* i;
        i = movieList.erase(i);
    }
}

bool MovieDatabase::load(const string& filename)
{
    if (loaded) return false;
    else loaded = false;
    ifstream file;
    file.open(filename);
    while (!file.eof()) { //run to end of file
        string id;
        getline(file, id);
        string movie;
        getline(file, movie);
        string releaseYear;
        getline(file, releaseYear);
        string directorList;
        getline(file, directorList);
        string director;
        string actorList;
        getline(file, actorList);
        string genreList;
        getline(file, genreList);
        vector<string> directors;
        for (int i = 0; i < directorList.size(); i++) { //parse string for directors and add to a vector
            if (directorList[i] == ',') {
                directors.push_back(director);
                director = "";
            }
            else
                director += directorList[i];
        }
        directors.push_back(director);
        string actor;
        vector<string> actors;
        for (int i = 0; i < actorList.size(); i++) { //parse string for actors and add to a vector
            if (actorList[i] == ',') {
                actors.push_back(actor);
                actor = "";
            }
            else
                actor += actorList[i];
        }
        actors.push_back(actor);
        string genre;
        vector<string> genres;
        for (int i = 0; i < genreList.size(); i++) { //parse string for genres and add to a vector
            if (genreList[i] == ',') {
                genres.push_back(genre);
                genre = "";
            }
            else
                genre += genreList[i];
        }
        genres.push_back(genre);
        string rating;
        getline(file, rating);
        float rat = std::stof(rating); //convert rating to a float
        Movie* add = new Movie(id, movie, releaseYear, directors, actors, genres, rat);
        movieList.push_back(add);
        idData.insert(lower(id), add);
        //for each director, actor, and genre map it to the movie that they're in
        for (int i = 0; i < directors.size(); i++) {
            directorData.insert(lower(directors[i]), add);
        }
        for (int i = 0; i < actors.size(); i++) {
            actorData.insert(lower(actors[i]), add);
        }
        for (int i = 0; i < genres.size(); i++) {
            genreData.insert(lower(genres[i]), add);
        }
        file.ignore(10000, '\n');

        if (file.eof())
            loaded = true;
    }
    return loaded;  
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<string, Movie*>::Iterator i = idData.find(lower(id));
    if (i.is_valid())
        return i.get_value();
    else
        return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    TreeMultimap<string, Movie*>::Iterator i = directorData.find(lower(director));
    vector<Movie*> movies;
    while (i.is_valid()) {
        movies.push_back(i.get_value());
        i.advance();
    }
    return movies;  
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    TreeMultimap<string, Movie*>::Iterator i = actorData.find(lower(actor));
    vector<Movie*> movies;
    while (i.is_valid()) {
        movies.push_back(i.get_value());
        i.advance();
    }
    return movies;   
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    TreeMultimap<string, Movie*>::Iterator i = genreData.find(lower(genre));
    vector<Movie*> movies;
    while (i.is_valid()) {
        movies.push_back(i.get_value());
        i.advance();
    }
    return movies;   
}
