/**
 * Jae Song
 * jys023, A12042160
 * CSE 100 HW 4
 * 3/10/16
 * FileName: ActorNode.hpp
 * Description: This will be a class that implements the
 * 				ActorNode class which is a vertex representation of 
 * 				Actors.
 */

#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <limits>
#include <vector>
#include <string>
#include "ActorEdge.hpp"

class ActorEdge;

/*
 * Name: ActorNode
 * Description: Vertex representation of actor
 *
 */
class ActorNode
{
  public: 
    
	//member variables
  	std::string name;
	int index;
	int dist;
	bool visit;
	ActorEdge* path;
	ActorNode* prev;
	std::vector<ActorEdge*> edges;

    /* CONSTRUCTOR  */
  	ActorNode(std::string name)
	  : name(name)
	  {}

    /* DECONSTRUCTOR */
	~ActorNode()
	{
	  //delete all the edges
	  for(std::vector<ActorEdge*>::iterator it = edges.begin()
	  	  ; it != edges.end(); ++it)
	  {
	   	delete(*it);
	  }
	}

	/*
	 * Name: operator<
	 * Description:
	 *  	Overrides the operator <, it will be used for the priority
	 * 		queue. Prioritizes actornodes with smaller dist
	 */
	bool operator<(const ActorNode& other)
	{
	  //condition if dist is same
	  if(dist == other.dist)
	  {
        return index > other.index;
	  }
	  return dist > other.dist;
	}
};

/*
 * Name: ActorComp
 * Description:
 *  	This class will be the comparator class for the
 *  	Priority Queue of ActorNodes
 */
class ActorComp
{
  public:
    bool operator()(ActorNode*& lhs, ActorNode*& rhs) const
	{
	  return (*lhs) < (*rhs);
	}
};
#endif
