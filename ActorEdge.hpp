/*
 * Jae Song
 * A12042160, jys023
 * CSE 100 HW 4
 * Date: 3/10/16
 * FileName: ActorEdge.hpp
 */
#ifndef ACTOREDGE_HPP 
#define ACTOREDGE_HPP

#include "ActorNode.hpp"
#include "Movie.hpp"

class ActorNode;
class Movie;

/*
 * Name: ActorEdge
 * Description:
 * 		mainly used for the connection between node and movie
 *
 */
class ActorEdge
{
  public:
    
	//member variables
    ActorNode* node;
	Movie* movie;
	int weight;

	/* CONSTRUCTOR   */
  	ActorEdge(ActorNode* node, Movie* movie, int weight)
	  :node(node), movie(movie), weight(weight)
	  {}

    /* DECONSTRUCTOR */
	~ActorEdge()
	{}
	
};
#endif
