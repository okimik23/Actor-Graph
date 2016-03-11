/*
 * Jae Song
 * jys023, A12042160
 * Date: 3/10/16
 * CSE 100 HW 4
 * FileName: Movie.hpp
 *
 */
#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <vector>
#include <string>
#include "ActorEdge.hpp"

class ActorEdge;

/*
 * Name: Movie
 * Description:
 * 		main connection between two actors.
 * 		Implements an edge between ActorNodes
 *
 */
class Movie
{
  public:

    //member variables
  	std::string name;
	int year;
	std::vector<ActorEdge*> edges;

    /* CONSTRUCTOR     */
  	Movie(std::string name, int year)
	  : name(name), year(year)
      {}

    /* DECONSTRUCTOR   */
	~Movie()
	{}
};
#endif
