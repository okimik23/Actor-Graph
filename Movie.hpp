#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <vector>
#include "ActorNode.hpp"
#include "ActorEdge.hpp"

class ActorNode;
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
