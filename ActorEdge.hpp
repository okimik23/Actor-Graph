#ifndef ACTOREDGE_HPP 
#define ACTOREDGE_HPP

#include "ActorNode.hpp"
#include "Movie.hpp"

class ActorNode;
class Movie;

class ActorEdge
{
  public:
    ActorNode* node;
	Movie* movie;
	int weight;

  	ActorEdge(ActorNode* node, Movie* movie, int weight)
	  :node(node), movie(movie), weight(weight)
	  {}

	ActorEdge(ActorNode* node)
	  :node(node)
 	  {}

	~ActorEdge()
	{}
	
};
#endif
