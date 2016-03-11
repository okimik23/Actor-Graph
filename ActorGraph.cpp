/*
 * ActorGraph.cpp
 * Author: Adrian Guthals
 * Date: 2/24/2015
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */
 
 /*
  * Jae Song
  * A12042160, jys023
  * ActorGraph.cpp
  * Date: 3/10/16
  * CSE 100 HW 4
  * Description:
  * 	implementatino of the ActorGraph data structure
  *
  */

#include <fstream>
#include <sstream>
#include <queue>
#include <limits>
#include "ActorGraph.hpp"

using namespace std;

/* CONSTRUCTOR */
ActorGraph::ActorGraph(void) {}

/* DECONSTRUCTOR */
ActorGraph::~ActorGraph()
{
  //iterate through all the actors and delete actors
  for(std::unordered_map<string,ActorNode*>::iterator it = actors.begin()
  	  ; it != actors.end(); ++it)
  {
    delete (it->second);
  }

  //iterate through and delete all movies
  for(std::unordered_map<string,Movie*>::iterator it = movies.begin()
      ; it != movies.end(); ++it)
  {
    delete (it->second);
  }

  //iterate through and delete all disjoint sets
  for(std::vector<Disjoint*>::iterator it = set.begin()
      ; it != set.end(); ++it)
  {
    delete (*it);
  }
}

/*
 * Name: loadFromFile
 * Return: true if correctly loaded, false otherwise
 * Description:
 * 		it will make the graph from the data given from in_filename
 *
 */
bool ActorGraph::loadFromFile(const char* in_filename)
{
  // Initialize the file stream
  ifstream infile(in_filename);

  //set the variables
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
    
	//get the actor name, movie name, movie year
    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);

	//set the min_year
    if(movie_year < min_year)
	{
	  min_year = movie_year;
	}

	//movie_key will be name+year
	string movie_key = record[1] + record[2];
    
	//if the actor doesn't exist...
	if(actors.find(actor_name) == actors.end())
	{
	  //make new actor, index will be unique as actors come ordered
	  node = new ActorNode(actor_name);
	  node->index = index++;
	  actors.insert({actor_name, node});

	  //make new disjoint set and it will be at node->index place
	  Disjoint* disjoint = new Disjoint(node);
	  set.push_back(disjoint);
	}
	else
	{
	  node = actors[actor_name];
	}

    //if the movie doesn't exist make new movie and insert to map
	if(movies.find(movie_key) == movies.end())
	{
	  movie = new Movie(movie_title, movie_year);
	  movies.insert({movie_key, movie});
	}
	else
	{
	  movie = movies[movie_key];
	}

	//make new edge everytime with correct weight
	ActorEdge* edge = new ActorEdge(node, movie, 1 + 2015 - movie->year);

	//connect the nodes and edges
	node->edges.push_back(edge);
	movie->edges.push_back(edge);
  }
  
  if (!infile.eof())
  {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();

  return true;
}


/*
 * Name: findPath
 * Return: true if in_filename is valid, false otherwise
 * Description:
 * 		it will find the path between two actors given in in_filename
 * 		using either bfs or dijkstras falgged by use_weighted
 * 		MAINLY USED IN PATHFINDER
 *
 */
bool ActorGraph::findPath(const char* in_filename, const char* out_filename,
						  bool use_weighted)
{
  //initialize infile and outfile
  ifstream infile(in_filename);
  ofstream outfile(out_filename);

  bool have_header = false;
  
  ActorNode* start;
  ActorNode* end;
  
  while(infile)
  {
    string s;
	
	//get lines
	if(!getline(infile,s)) break;
    
	//intake and output header
	if(!have_header)
	{
	  have_header = true;
	  outfile << "(actor)--[movie#@year]-->(actor)--...\n";
	  continue;
	}

	//get the actors separated by <tabs>
	istringstream ss(s);
	vector<string> record;

	while(ss)
	{
	  string next;
	  if(!getline(ss, next, '\t')) break;

	  record.push_back(next);
	}

	//check size, should be two actors
	if(record.size() != 2)
	{
	  continue;
	}

    //have two actors' names
	string source(record[0]);
	string dest(record[1]);
	bool find;

	//check if two actors exist
	std::unordered_map<std::string, ActorNode*>::iterator it;
	it = actors.find(source);
	if(it == actors.end())
	{
	  continue;
	}
	it = actors.find(dest);
	if(it == actors.end())
	{
	  continue;
	}

	//get the two actors
	start = actors[source];
	end = actors[dest];
    
	//if we're using weighted, we will use dijkstras
	if(use_weighted)
	{
	  //use Dijkstra's method to find shorted weighted path
	  find = DijkTraverse(end,start);
	  if(find)
	  {
	    //output the correct format while traversing
	    ActorNode* trav = start;
        while(1)
		{
		  outfile << "(" << trav->name << ")" << "--";
		  outfile << "[" << trav->path->movie->name << "#@" 
		                 << trav->path->movie->year << "]" << "-->";
		  trav = trav->path->node;

		  //if we reach our destination actor, we break
		  if(trav == end)
		  {
		    outfile << "(" << trav->name << ")" << endl;
			break;
		  }
		}
	  }
	}

	//use BFS to find unweighted path
	else
	{
	  //use BFS method to find shortest unweighted path
	  find = BFSTraverse(end, start, 2016);
	  if(find)
	  {
	  	//output the ocrrect format while traversing
	    ActorNode* trav = start;
	    while(1)
	    {
	      outfile << "(" << trav->name << ")" << "--";
		  outfile << "[" << trav->path->movie->name << "#@" 
		                 << trav->path->movie->year << "]" << "-->";
		  trav = trav->path->node;

		  //if we reach our destination actor, we break
		  if(trav == end)
		  {
		    outfile << "(" << trav->name << ")" << endl;
	  	    break;
		  } 
	    }
	  }
	}
  }
  
  //check if the input file was valid
  if(!infile.eof())
  {
    cerr << "Failed to read " << in_filename << "!\n";
	return false;
  }
  infile.close();
  outfile.close();

  return true;
}


/*
 * Name:moviespan
 * Return: true if in_filename was valid, false otherwise
 * Description:
 * 		will find whether or not the two actors give by in_filename
 * 		are connected or not using either BFS or Union find, flagged by
 * 		ufind
 * 		MAINLY USED FOR ACTORCONNECTIONS
 */
bool ActorGraph::moviespan(const char* in_filename, 
						   const char* out_filename, bool ufind)
{
  //initialize stream files
  ifstream infile(in_filename);
  ofstream outfile(out_filename);

  bool have_header = false;
  
  ActorNode* start;
  ActorNode* end;

  //set timer used to measure implementation
  std::chrono::time_point<std::chrono::high_resolution_clock> begin
       = std::chrono::high_resolution_clock::now();
  
  while(infile)
  {
  	//reset ActorNodes
    start = 0;
	end = 0;
    string s;
	
	//get line, containing two actor names
	if(!getline(infile,s)) break;
    
	//intake or output header
	if(!have_header)
	{
	  have_header = true;
	  outfile << "Actor1\tActor2\tYear" << endl;
	  continue;
	}

	istringstream ss(s);
	vector<string> record;

	//push two actor names to record
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

	//we have the two actors name
	string source(record[0]);
	string dest(record[1]);

	//check if the two actors exist, if not they will be 0
	std::unordered_map<std::string, ActorNode*>::iterator it;
	it = actors.find(source);
	if(it != actors.end())
	{
	  start = it->second;
	}
	it = actors.find(dest);
	if(it != actors.end())
	{
	  end = it->second;
	}
    
	//either Union find or BFS
	if(ufind)
	{
	  //get the year from union find and print out
	  int year = UnionFind(start,end);
	  outfile << source << "\t" << dest << "\t" << year << endl;
	}
	else
	{
	  //get the year from BFS and print out
	  int year = BFSSearch(start, end);
	  outfile << source << "\t" << dest << "\t" << year << endl;
	}
  }

  //end timer
  std::chrono::time_point<std::chrono::high_resolution_clock> done 
       = std::chrono::high_resolution_clock::now();
 
  //check if the infile was valid
  if(!infile.eof())
  {
    cerr << "Failed to read " << in_filename << "!\n";
	return false;
  }

  if(ufind)
  {
    std::cout << "Union Find took ";
  }
  else
  {
    std::cout << "BFS Search took ";
  }
  
  //calculate time and print it out
  long int time = (long int)std::chrono::duration_cast<std::chrono::milliseconds>(done-begin).count();
  std::cout << time << " milliseconds" << std::endl;

  infile.close();
  outfile.close();
  return true;
}

/*
 * Name: averageDist
 * Return: true if correct actor name, false otherwise
 * Description:
 * 		it will calculate the average shortest unweighted path
 * 		from one actor to all other possible actors
 *		MAINLY USED FOR EXTENSIONS
 */
bool ActorGraph::averageDist(const char* out_filename, 
                             std::string actorName)
{
  //set stream and output header
  ofstream outfile(out_filename);
  outfile << "Actor" << endl;

  //check if the actor exists
  std::unordered_map<string, ActorNode*>::iterator it;
  it = actors.find(actorName);
  if(it == actors.end())
  {
    std::cout << "Actor, " << actorName << " does not exist in our"
              << " database, select another" << std::endl;
    return false;
  }

  ActorNode* start = it->second; 
  
  //set the calculation parameters
  int unweight_dist = 0;
  int numActors = 0;
  for(auto act_it = actors.begin(); act_it != actors.end(); ++act_it)
  {
    ActorNode* end = act_it->second;
        
	//BFS find
	bool find = BFSTraverse(end, start, 2016);
	if(!find)
	{
	  continue;
	}
	
	//output name and increase number of actors
	outfile << end->name << std::endl;
	++numActors;

    //add up the number of edges
	ActorNode* trav = start;
	while(1)
	{
      trav = trav->path->node;
	  ++unweight_dist;
	  if(trav == end)
	  {
	    break;
      } 
	}
  }

  //calculate unweighted average
  double unweight_average = (double)unweight_dist / numActors;
  
  //print out final statements, actors connected and average path
  outfile << numActors << " Actors are Connected to " 
          << actorName << std::endl;
  outfile << "The Average Shortest Unweighted Distance to "
          << actorName << " is " << unweight_average << std::endl;

  outfile.close();
  return true;
}

/*
 * Name: UnionFind
 * Return: int, year at which the two nodes are connected
 * Description:
 * 		will use union-find method to find at what year the two
 * 		actors are connected
 *
 */
int ActorGraph::UnionFind(ActorNode* start, ActorNode* end)
{
  //case where actors don't exist in graph
  if(!start || !end)
  {
    return 9999;
  }
  
  //iterate through disjoint sets and reset them
  std::vector<Disjoint*>::iterator joint_it = set.begin();
  for( ; joint_it != set.end(); ++joint_it)
  {
  	Disjoint* disjoint = *joint_it;
    disjoint->sentinel = disjoint;
	disjoint->size = 1;
  }

  //start with the lowest year
  int year = min_year;
  while(year < 2016)
  {
    //iterate through all the movies
    std::unordered_map<string,Movie*>::iterator it = movies.begin();
  	for( ; it != movies.end(); ++it)
  	{
	  //only unite them if year is correct
	  Movie* movie = it->second;
      if(movie->year == year)
	  {
	    Disjoint* disjoint = 0;

		//iterate through all actors in that movie
	    std::vector<ActorEdge*>::iterator act_it = movie->edges.begin();
		for( ; act_it != movie->edges.end(); ++act_it)
		{
		  ActorNode* actor = (*act_it)->node;

		  //union the two actors in that movie
		  if(disjoint)
		  {
		    disjoint->Union(set[actor->index]);
		  }
		  //get the representative disjoint set of that actor
		  disjoint = set[actor->index];
		}
	  }
  	}

	//get the two sets reprsenting the actors and see if they're connected
	Disjoint* first = set[start->index];
	Disjoint* second = set[end->index];
	if(first->Find()->node->index == second->Find()->node->index)
	{
	  return year;
	}

	//repeat loop with the incremented year
	year++;
  }
  return 9999;
}

/*
 * Name: BFSSearch
 * return: int, year at which the two actors become connected
 * Description:
 * 		it will use BFS to find at which the two actors become 
 * 		connected
 *
 */
int ActorGraph::BFSSearch(ActorNode* start, ActorNode* end)
{
  //if the two actors don't exist in the data return 9999
  if(!start || !end)
  {
    return 9999;
  }

  //check if two nodes are connected after every year
  bool find = false;
  int year = min_year;
  while(year < 2016)
  {
    //use BFS to get year and if it is found, return that year
 	find = BFSTraverse(start, end, year);
	if(find)
	{
      return year;	  
	}
	year++;
  }
  return 9999; 
}

/*
 * Name: BFSTraverse
 * Return: true if path is found, false otherwise
 * Description:
 * 		it will use the BFS algorithm to see whether or not
 * 		the two actors are connected
 *
 */
bool ActorGraph::BFSTraverse(ActorNode* start, ActorNode* end, int year)
{
  //if actors don't exist in the database, then paths don't exist
  if(!start || !end)
  {
    return false;
  }

  //if the two actors are same then paths don't exist
  if(start->index == end->index)
  {
    return false;
  }

  //set up before BFS, reset nodes and set up queue
  queue<ActorNode*> explore;
  for(auto it = actors.begin(); it != actors.end(); ++it)
  {
    it->second->visit = false;
  }
  explore.push(start);
  start->visit = true;

  while(!explore.empty())
  {
  	//get the next actor
  	ActorNode* next = explore.front();
	explore.pop();
		
	//go through all the edges of the Actor that was popped
    vector<ActorEdge*>::iterator it = next->edges.begin();
	for( ; it != next->edges.end(); ++it)
	{
	  Movie* movie = (*it)->movie;

	  //if the movie year is wrong, skip. Mainly for part 2
	  if(movie->year > year)
	  {
	    continue;
	  }
	  
	  //get all the actors connected via movie
	  vector<ActorEdge*>::iterator a_it = movie->edges.begin();
	  for( ; a_it != movie->edges.end(); ++a_it)
	  {
	    ActorNode* actor = (*a_it)->node;

		//if not visited, to prevent cycle
		if(!actor->visit)
		{
		  //either you found it or you add it to queue
		  if(actor->index == end->index)
		  {
		    //set the paths and nodes to traverse back
		    end->path = *it;
			end->prev = next;
			return true;
		  }
		  else
		  {
		    //set the paths and nodes to traverse back
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

/*
 * Name: DijkTraverse
 * Return: true if a path is found, false otherwise
 * Description:
 * 		it will find the shortest possible weighted path
 * 		between the two actors
 *
 */
bool ActorGraph::DijkTraverse(ActorNode* start, ActorNode* end)
{
  //if two actors don't exist
  if(!start || !end)
  {
    return false;
  }

  //intialize priority queue and reset all actors
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
    //get next actor
	ActorNode* next = pq.top();
	pq.pop();
    
	//only visit ones that haven't been popped
	if(!next->visit)
	{
	  next->visit = true;

	  //go through all movies of that actor
	  std::vector<ActorEdge*>::iterator it = next->edges.begin();
	  for( ; it != next->edges.end(); ++it)
	  {
	    ActorEdge* edge = *it;
		Movie* movie = edge->movie;
		
		//go through all actors connected via movie
		std::vector<ActorEdge*>::iterator m_it = movie->edges.begin();		
		for( ; m_it != movie->edges.end(); ++m_it)
		{
		  ActorEdge* inner_edge = *m_it;
		  ActorNode* actor = inner_edge->node;

		  //check the distance of the nodes 
		  int distance = inner_edge->weight + next->dist;
		  if(distance < actor->dist)
		  {
		    //set the paths and nodes for traverse and change dist so far
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
