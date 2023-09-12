#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    users = &user_database;
    movies = &movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* u = users->get_user_from_email(user_email);
    if (u == nullptr || movie_count <= 0) {
        return vector<MovieAndRank>();
    }
    vector<string> moviesWatched = u->get_watch_history();
    vector<Movie*> userMovies;
    vector<string> directorsWatched;
    vector<string> actorsWatched;
    vector<string> genresWatched;
    for (int i = 0; i < moviesWatched.size(); i++) { //finding all the movies the user has watched
        userMovies.push_back(movies->get_movie_from_id(moviesWatched[i]));
    }
    for (int i = 0; i < userMovies.size(); i++) { //adding all directors, actors, and genres watched by the user
        vector<string> directors;
        vector<string> actors;
        vector<string> genres;
        directors = userMovies[i]->get_directors();
        actors = userMovies[i]->get_actors();
        genres = userMovies[i]->get_genres();
        for (int j = 0; j < directors.size(); j++) {
            directorsWatched.push_back(directors[j]);
        }
        for (int j = 0; j < actors.size(); j++) {
            actorsWatched.push_back(actors[j]);
        }
        for (int j = 0; j < genres.size(); j++) {
            genresWatched.push_back(genres[j]);
        }
    }

    unordered_map<Movie*, MoviePointerAndRank> recommend;
    for (int i = 0; i < directorsWatched.size(); i++) { //getting all movies directors user has watched has made
        vector<Movie*> dmovies = movies->get_movies_with_director(directorsWatched[i]);
        for (int j = 0; j < dmovies.size(); j++) { //adding movies to map with 20 compatability score
            recommend[dmovies[j]].compatibility_score += 20;
            recommend[dmovies[j]].movie = dmovies[j];
        }
    }
    for (int i = 0; i < actorsWatched.size(); i++) { //getting all movies of the actors that the user has watched
        vector<Movie*> amovies = movies->get_movies_with_actor(actorsWatched[i]);
        for (int j = 0; j < amovies.size(); j++) { //adding movies to the map increasing them by 30 score
            recommend[amovies[j]].compatibility_score += 30;
            recommend[amovies[j]].movie = amovies[j];
        }
    }
    for (int i = 0; i < genresWatched.size(); i++) { //getting all movies of the genres that the user has watched
        vector<Movie*> gmovies = movies->get_movies_with_genre(genresWatched[i]); //adding movies to the map increasing them by 1 score
        for (int j = 0; j < gmovies.size(); j++) {
            recommend[gmovies[j]].compatibility_score += 1;
            recommend[gmovies[j]].movie = gmovies[j];
        }
    }

    for (int i = 0; i < userMovies.size(); i++) { //making sure movies that user has already watched have 0 compatibility score
        recommend[userMovies[i]].compatibility_score = 0;
    }

    vector<MoviePointerAndRank> orderedlist;
    unordered_map<Movie*, MoviePointerAndRank>::iterator i = recommend.begin();
    for (; i != recommend.end(); i++) { //including into a MovieRank vector that is going to be sorted
        i->second.movie_id = i->first->get_id();
        if (i->second.compatibility_score != 0) //make sure movies with 0 compatibility aren't added
            orderedlist.push_back(i->second);
    }

    auto compare = [this](const MoviePointerAndRank& a, const MoviePointerAndRank& b) -> bool { //comparing all Movies and their compatibility score, returning a if it is more recommended than b
        if (a.compatibility_score > b.compatibility_score) {
            return true;
        }
        else if (b.compatibility_score > a.compatibility_score) {
            return false;
        }
        if (a.movie->get_rating() > b.movie->get_rating())
            return true;
        else if (b.movie->get_rating() > a.movie->get_rating())
            return false;
        return a.movie->get_title() < b.movie->get_title();
    }; 

    sort(orderedlist.begin(), orderedlist.end(), compare); //sort the vector based on which movies have greater compatibility score

    vector<MovieAndRank> result;
    for (int i = 0; i < movie_count; i++) { //add the most compatible movie_count movies to a vector and return that vector
        if (i >= orderedlist.size())
            break;
        MovieAndRank add(orderedlist[i].movie_id, orderedlist[i].compatibility_score);
        result.push_back(add);
    }

    return result;  
}
