#include "ActorGraph.hpp"

int main(int argc, char* argv[])
{
  if(argc < 5)
  {
    std::cout << "Incorrect number of arguments" << std::endl;
	std::cout << "Usage: ./actorconnections inputfile pairs output" 
	          << "[bfs,ufind(default)]" << std::endl;
	return -1;
  }

  ActorGraph graph;

  bool success = graph.loadFromFile(argv[1], false)
  if(!success)
  {
  	return -1; 
  }
  return 0;
}
