#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <limits>
#include <vector>
#include "Movie.hpp"
#include "ActorEdge.hpp"

class Movie;
class ActorEdge;

class ActorNode
{
  public: 
  	std::string name;
	int index;
	int dist;
	bool visit;
	ActorEdge* path;
	ActorNode* prev;
	std::vector<ActorEdge*> edges;

  	ActorNode(std::string name)
	  : name(name)
	  {}

	~ActorNode();

	bool operator<(const ActorNode& other)
	{
	  if(dist == other.dist)
	  {
        return index > other.index;
	  }
	  return dist > other.dist;
	}
};

class ActorComp
{
  public:
    bool operator()(ActorNode*& lhs, ActorNode*& rhs) const
	{
	  return (*lhs) < (*rhs);
	}
};
#endif
