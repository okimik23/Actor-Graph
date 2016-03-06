#include "ActorGraph.hpp"

int main(int argc, char* argv[])
{
  if(argc < 5)
  {
  	std::cout << "Incorrect number of arguments." << std::endl;
	std::cout << "Usage: ./pathfinder inputFile [u,w] pairs output" 
	  << std::endl;
	return -1;
  }

  ActorGraph graph;
  bool use_weighted;

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
  
  bool success = graph.loadFromFile(argv[1], use_weighted);

  if(!success)
  {
    return -1;
  }

  success = graph.findPath(argv[3], argv[4], use_weighted);

  if(!success)
  {
    return -1;
  }

  return 0;
}
