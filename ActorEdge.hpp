#ifndef ACTOREDGE_HPP 
#define ACTOREDGE_HPP

#include <vector>
#include "ActorNode.hpp"
#include "Movie.hpp"

class ActorEdge
{
  public:
 	std::vector<Movie*> connection;

  //public:
  	ActorEdge()
	{}
};

#endif
