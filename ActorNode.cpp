#include "ActorNode.hpp"

ActorNode::~ActorNode()
{
  if(path)
  {
  	delete path;
  }
  
  if(prev)
  {
  	delete prev; 
  }

  for(auto it = edges.begin(); it != edges.end(); ++it)
  {
    delete (*it);
  }
}
