#include "ActorGraph.hpp"

int main(int argc, char* argv[])
{
  if(argc < 4)
  {
    std::cout << "Incorrect number of arguments" << std::endl;
	std::cout << "Usage: ./actorconnections inputfile pairs output" 
	          << "[bfs,ufind(default)]" << std::endl;
	return -1;
  }

  std::string algor = "ufind";
  bool ufind = false;

  if(argc == 5)
  {
  	algor = std::string(argv[4]);  
  }

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

  bool success = graph.loadFromFile(argv[1], false);
  if(!success)
  {
  	return -1; 
  }

  success = graph.moviespan(argv[2], argv[3], ufind);
  {
    if(!success)
	{
	  return -1;
	}
  }

  return 0;
}
