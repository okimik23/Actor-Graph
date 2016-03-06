#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <vector>
#include "ActorNode.hpp"

class ActorNode;

class Movie
{
  public:
  	std::string name;
	int year;
	int weight;
	int dist;
	bool done;
	Movie* prev;
	ActorNode* prevActor;
	std::vector<ActorNode*> cast;

  //public:
  	Movie(std::string name, int year)
	  : name(name), year(year)
	  {weight = 1 + (2015 - year); done = false;}

	bool operator<(const Movie& other)
	{
	  if(dist == other.dist)
	  {
	    return name > other.name;
	  }
	  
	  return dist > other.dist; 
	}
};

class MovieComp
{
  public:
    bool operator()(Movie*& lhs, Movie*& rhs) const
    {
      return (*lhs) < (*rhs);
  	}
};

#endif
