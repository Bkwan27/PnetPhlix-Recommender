#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>

class UserDatabase;
class MovieDatabase;
class Movie;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;

};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

  private:
      const UserDatabase* users;
      const MovieDatabase* movies;

      struct MoviePointerAndRank { //have a struct that is MovieAndRank with a pointer to a movie
          MoviePointerAndRank() {
              movie_id = "";
              compatibility_score = 0;
              movie = nullptr;
          }
          Movie* movie;
          std::string movie_id;
          int compatibility_score;
      }; 
};

#endif // RECOMMENDER_INCLUDED
