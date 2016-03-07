#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <vector>
#include <string>
#include "ActorEdge.hpp"

class ActorEdge;

class Movie
{
  public:
  	std::string name;
	int year;
	std::vector<ActorEdge*> edges;

  	Movie(std::string name, int year)
	  : name(name), year(year)
      {}

	~Movie()
	{}
};
#endif
