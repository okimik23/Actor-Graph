#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <limits>
#include "Movie.hpp"

class Movie;

class ActorNode
{
  public: 
  	std::string name;
	int index;
	int dist;
	ActorNode* prev;
	Movie* prevMovie;

  //public:
  	ActorNode(std::string name)
	  : name(name)
	  {dist = std::numeric_limits<int>::max();}
};

#endif
