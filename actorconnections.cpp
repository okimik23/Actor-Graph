/*
 * Jae Song
 * A12042160, jys023
 * Date: 3/10/16
 * CSE 100 HW 4
 * FileName: actorconnections.cpp
 *
 */
#include "ActorGraph.hpp"

/*
 * Description:
 * 		main executable, will take various arguments to see
 * 		at what year will the two actos be connected
 *
 */
int main(int argc, char* argv[])
{
  //check if number of arguments is correct
  if(argc < 4)
  {
    std::cout << "Incorrect number of arguments" << std::endl;
	std::cout << "Usage: ./actorconnections inputfile pairs output" 
	          << "[bfs,ufind(default)]" << std::endl;
	return -1;
  }

  std::string algor = "ufind";
  bool ufind = false;

  //check if optional argument is used
  if(argc == 5)
  {
  	algor = std::string(argv[4]);  
  }

  //set the ufind or bfs flag
  if(!algor.compare("ufind"))
  {
    ufind = true;
  }
  else if(!algor.compare("bfs"))
  {
  	ufind = false;
  }
  else
  {
  	std::cout << "Incorrect Fourth Input: [bfs,ufind(default)]" 
	          << std::endl;
	return -1;
  }

  ActorGraph graph;

  //load and check if loaded succesfully
  bool success = graph.loadFromFile(argv[1]);
  if(!success)
  {
  	return -1; 
  }

  //find and check if found successfully
  success = graph.moviespan(argv[2], argv[3], ufind);
  if(!success)
  {
    return -1;
  }
  
  return 0;
}
