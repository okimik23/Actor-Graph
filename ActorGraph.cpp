/*
 * ActorGraph.cpp
 * Author: Adrian Guthals
 * Date: 2/24/2015
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <sstream>
#include <queue>
#include <limits>
#include "ActorGraph.hpp"

using namespace std;

ActorGraph::ActorGraph(void) {}


ActorGraph::~ActorGraph()
{
  for(std::unordered_map<string,ActorNode*>::iterator it = actors.begin()
  	  ; it != actors.end(); ++it)
  {
    delete (it->second);
  }
  for(std::unordered_map<string,Movie*>::iterator it = movies.begin()
      ; it != movies.end(); ++it)
  {
    delete (it->second);
  }
}


bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted)
{
  
  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;
  ActorNode* node = 0;
  Movie* movie = 0;
  ActorEdge* edge = 0;
  int index = 0;

  // keep reading lines until the end of file is reached
  while (infile)
  {
    string s;
    
    // get the next line
    if (!getline( infile, s )) break;
    
    if (!have_header)
    {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss)
    {
      string next;
      
      // get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' )) break;
      
      record.push_back( next );
    }
    
    if (record.size() != 3)
    {
      // we should have exactly 3 columns
      continue;
    }
    
    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);
	string movie_key = record[1] + record[2];
    
	if(actors.find(actor_name) == actors.end())
	{
	  node = new ActorNode(actor_name);
	  node->index = index++;
	  actors.insert({actor_name, node});
	}
	else
	{
	  node = actors[actor_name];
	}

	if(movies.find(movie_key) == movies.end())
	{
	  movie = new Movie(movie_title, movie_year);
	  movies.insert({movie_key, movie});
	}
	else
	{
	  movie = movies[movie_key];
	}
	ActorEdge* edge = new ActorEdge(node, movie, 1 + 2015 - movie->year);
	node->edges.push_back(edge);
	movie->edges.push_back(edge);
	edges.push_back(edge);
  }
  
  if (!infile.eof())
  {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();

  return true;
}

bool ActorGraph::findPath(const char* in_filename, const char* out_filename,
						  bool use_weighted)
{
  ifstream infile(in_filename);
  ofstream outfile(out_filename);

  bool have_header = false;
  
  ActorNode* start = 0;
  ActorNode* end = 0;
  
  while(infile)
  {
    string s;
	
	if(!getline(infile,s)) break;
    
	if(!have_header)
	{
	  have_header = true;
	  outfile << "(actor)--[movie#@year]-->(actor)--...\n";
	  continue;
	}

	istringstream ss(s);
	vector<string> record;

	while(ss)
	{
	  string next;
	  if(!getline(ss, next, '\t')) break;

	  record.push_back(next);
	}

	if(record.size() != 2)
	{
	  continue;
	}

	string source(record[0]);
	string dest(record[1]);
	bool find;

	start = actors[source];
	end = actors[dest];
    
	if(use_weighted)
	{
	  find = DijkTraverse(end,start);
	  if(find)
	  {
	    ActorNode* trav = start;
        while(1)
		{
		  outfile << "(" << trav->name << ")" << "--";
		  outfile << "[" << trav->path->movie->name << "#@" 
		                 << trav->path->movie->year << "]" << "-->";

		  trav = trav->path->node;
		  if(trav == end)
		  {
		    outfile << "(" << trav->name << ")" << endl;
			break;
		  }
		}
	  }
	}
	else
	{
	  find = BFSTraverse(end, start);
	  if(find)
	  {
	    ActorNode* trav = start;
	    while(1)
	    {
	      outfile << "(" << trav->name << ")" << "--";
		  outfile << "[" << trav->path->movie->name << "#@" 
		                 << trav->path->movie->year << "]" << "-->";
		  trav = trav->path->node;
		  if(trav == end)
		  {
		    outfile << "(" << trav->name << ")" << endl;
	  	    break;
		  } 
	    }
	  }
	}
  }
  
  if(!infile.eof())
  {
    cerr << "Failed to read " << in_filename << "!\n";
	return false;
  }
  infile.close();
  outfile.close();

  return true;
}

bool ActorGraph::BFSTraverse(ActorNode* start, ActorNode* end)
{
  queue<ActorNode*> explore;
  for(auto it = actors.begin(); it != actors.end(); ++it)
  {
    it->second->visit = false;
  }

  explore.push(start);
  start->visit = true;

  while(!explore.empty())
  {
  	ActorNode* next = explore.front();
	explore.pop();
		
    vector<ActorEdge*>::iterator it = next->edges.begin();
	for( ; it != next->edges.end(); ++it)
	{
	  Movie* movie = (*it)->movie;
	  
	  vector<ActorEdge*>::iterator a_it = movie->edges.begin();
	  for( ; a_it != movie->edges.end(); ++a_it)
	  {
	    ActorNode* actor = (*a_it)->node;
		if(!actor->visit)
		{
		  if(actor->index == end->index)
		  {
		    end->path = *it;
			end->prev = next;
			return true;
		  }
		  else
		  {
		    actor->visit = true;
		    actor->path = *it;
			actor->prev = next;
		    explore.push(actor);
		  }
		}
	  }
	}
  }
  return false;
}

bool ActorGraph::DijkTraverse(ActorNode* start, ActorNode* end)
{
  for(auto it = actors.begin(); it != actors.end(); ++it)
  {
  	it->second->visit = false;
	it->second->prev = 0;
	it->second->dist = std::numeric_limits<int>::max();
  }
  std::priority_queue<ActorNode*, std::vector<ActorNode*>, ActorComp> pq;

  start->dist = 0;
  pq.push(start);

  while(!pq.empty())
  {
	ActorNode* next = pq.top();
	pq.pop();
    
	if(!next->visit)
	{
	  next->visit = true;

	  std::vector<ActorEdge*>::iterator it = next->edges.begin();
	  for( ; it != next->edges.end(); ++it)
	  {
	    ActorEdge* edge = *it;
		Movie* movie = edge->movie;

		std::vector<ActorEdge*>::iterator m_it = movie->edges.begin();		
		for( ; m_it != movie->edges.end(); ++m_it)
		{
		  ActorEdge* inner_edge = *m_it;
		  ActorNode* actor = inner_edge->node;
		  int distance = inner_edge->weight + next->dist;
		  if(distance < actor->dist)
		  {
		    actor->prev = next;
			actor->path = edge;
			actor->dist = distance;
			pq.push(actor);
		  }
		}
	  }
    }
  }
  return true; 
}
