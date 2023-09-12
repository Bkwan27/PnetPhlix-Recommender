#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include "treemm.h"


class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    ~MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
      bool loaded;
      TreeMultimap<std::string, Movie*> idData;
      TreeMultimap<std::string, Movie*> directorData;
      TreeMultimap<std::string, Movie*> actorData;
      TreeMultimap<std::string, Movie*> genreData;
      std::vector<Movie*> movieList;

      std::string lower(const std::string& lowercase) const { //make string lowercase
          std::string temp = lowercase;
          for (int i = 0; i < lowercase.size(); i++) {
              tolower(temp[i]);
          }
          return temp;
      }
};

#endif // MOVIEDATABASE_INCLUDED
