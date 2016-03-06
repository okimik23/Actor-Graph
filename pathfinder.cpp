#include "ActorGraph.hpp"
#include "ActorNode.hpp"
#include "ActorEdge.hpp"

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
  else
  {
    use_weighted = false;
  }
  
  graph.loadFromFile(argv[1], use_weighted);
  graph.findPath(argv[3], argv[4], use_weighted);

  return 0;
}
