/*
 * Jae Song
 * jys023, A12042160
 * Date: 3/10/16
 * CSE 100 HW 4
 * FileName: extension.cpp
 *
 */
#include "ActorGraph.hpp"

/*
 * Description:
 * 		main executable, will take in various inputs to find 
 * 		the average shortest unweighted path of a certain actor
 *
 */
int main(int argc, char* argv[])
{
  //check if correct number of arguments
  if(argc < 4)
  {
    std::cout << "Incorrect number of arguments" << std::endl;
	std::cout << "Usage: ./extension inputfile output actorName" 
	          << std::endl;
	return -1;
  }

  ActorGraph graph;
  
  //check if loaded successfully
  bool success = graph.loadFromFile(argv[1]);
  if(!success)
  {
    return -1;
  }

  //get the actorname
  std::string name(argv[3]);
  for(int index = 4; index < argc; ++index)
  {
    name += " ";
	name += argv[index];
  }

  //check if averageDist is calculated correctly
  success = graph.averageDist(argv[2], name);
  if(!success)
  {
    return -1;
  }

  return 0;
}
