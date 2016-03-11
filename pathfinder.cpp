/*
 * Jae Song
 * A12042160, jys023
 * Date: 3/10/16
 * CSE 100 HW 4
 * FileName: pathfinder.cpp 
 */
#include "ActorGraph.hpp"

/*
 * Description:
 * 		main executable method, will take in various inputs
 * 		to find either BFS or Dijkstra's path of the pairs 
 *
 */
int main(int argc, char* argv[])
{
  //check if correct amount of arguments
  if(argc < 5)
  {
  	std::cout << "Incorrect number of arguments." << std::endl;
	std::cout << "Usage: ./pathfinder inputFile [u,w] pairs output" 
	  << std::endl;
	return -1;
  }

  ActorGraph graph;
  bool use_weighted;

  //check if correct usage
  if(*argv[2] == 'w')
  {
    use_weighted = true;
  }
  else if(*argv[2] == 'u')
  {
    use_weighted = false;
  }
  else
  {
    std::cout << "Incorrect Second Input: [u,w]" << std::endl;
	return -1;
  }

  //check if loaded successfully
  bool success = graph.loadFromFile(argv[1]);
  if(!success)
  {
    return -1;
  }

  //check if found successfully
  success = graph.findPath(argv[3], argv[4], use_weighted);
  if(!success)
  {
    return -1;
  }

  return 0;
}
