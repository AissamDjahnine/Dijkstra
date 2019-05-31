/// @Aissam Djahnine

///Instantiation of the main Class " Generic_mapper " 
#include "Generic_mapper.hpp"		
///std::basic_fstream, std::basic_ifstream, std::basic_ofstream class templates and several typedefs 
#include <fstream>
///std::vector container 
#include <vector>
///std::basic_string class template 
#include <string>
///Algorithms that operate on ranges
#include <algorithm>
///Range iterators
#include <iterator>
///std::set and std::multiset associative containers 
#include <set>
///std::queue and std::priority_queue container adaptors 
#include <queue>

///C compatibility headers
#include <limits.h>
#include <time.h> 
#include <iomanip> 

///Exceptions Flags :
#define NONE_STATION 1
#define SAME_STATION 2
#define ERROR_READ_FILE 3
#define SAME_DEPAR_ARRIV 4

using namespace travel;

/// External Classes and functions :
 
/*** A binary predicate that takes two elements (of any typeA :<uint64_t,uint64_t > , TypeB : < std::string,unsigned int > ) 
as arguments and returns a bool.The expression Time_Order_Criteria(a,b), where Time_Order_Criteria is an object of this type 
and a and b are elements in the container shall return true if a is considered to go before b in the strict weak ordering the 
function defines.The priority_queue uses this function to maintain the elements sorted, which returns the same as applying 
the less-than operator (b<a).
***/
/// Used to sort the priority queue for the Dijkstra 
class Time_Order_Criteria {
    public:
        bool operator()(std::pair < uint64_t, uint64_t > n1, std::pair < uint64_t, uint64_t > n2) {
            return n1.second > n2.second;
        }
};
/// Used to sort the priority queue for the levenshtein function 
class Distance_Order_Criteria {
    public:
        bool operator()(std::pair < std::string, unsigned int  > n1, std::pair < std::string, unsigned int > n2) {
            return n1.second > n2.second;
        }
};

/** Levenshtein_Distance implementation to find minimum number operations to convert s1 to s2
@param s1 : first string 
@param s2 : second string 
@return minimal distance between s1 and s2 
**/
unsigned int Levenshtein_Distance(const std::string& s1, const std::string& s2){

    const std::size_t len1(s1.size()),len2(s2.size());
	
    std::vector<std::vector<unsigned int>> d (len1 + 1, std::vector<unsigned int> (len2 + 1)); 	//distance vector
    d[0][0] = 0;

    for(unsigned int i(1); i <= len1; ++i)  d[i][0] = i;
    for(unsigned int i(1); i <= len2; ++i)  d[0][i] = i;

    for(unsigned int i (1); i <= len1; ++i)
        for(unsigned int j (1); j <= len2; ++j)
            d[i][j] = std::min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) });

    return d[len1][len2];																		//return minimal distance 
}

/** isNumber implementation to check whether a string is an integer
@param s : string 
@return boolean : true if integer
**/
bool isNumber( const std::string& s){
    for ( unsigned int i(0); i<s.length(); i++)
        if( !isdigit(s[i]))
			return false;
    return true;
}

/// Network Class : Derived from Generic_mapper
class Network: public Generic_mapper { 
	/** Attributes **/
    protected:
        void read_stations(const std::string & _filename) override;																//Read Stations
        void read_connections(const std::string & _filename) override;															//Read Connections
        std::vector < std::pair <uint64_t, uint64_t> > compute_travel(uint64_t _start, uint64_t _end) override;					//Estimate Path with IDs
        std::vector < std::pair <uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end) override;		//Display Path with IDs
        std::vector<std::pair<uint64_t,uint64_t> > compute_travel(const std::string&, const std::string&) override;				//Estimate Path with names
        std::vector<std::pair<uint64_t,uint64_t>> compute_and_display_travel(const std::string&, const std::string&) override;  //Display Path with names
	/** Methods **/
    public:
		/** Class-Constructor :
		@param : _filename1 : stations csv file
				 _filename2 : connections csv file	
				 _start : start station Id
				 _end : end station Id
		**/
        Network (std::string _filename1, std::string _filename2, uint64_t _start, uint64_t _end){
				
            bool start_flag(false) ,end_flag(false); 			//Flags indicating that _start,_end ids exists
			
			/// Reading Stations,Connections files : 
            read_stations(_filename1);
            read_connections(_filename2);
			/// check whether Ids are for the same station ( Arrival & departure )
			/// Check whether Ids exist using flags
            for (auto it: this->stations_hashmap){
                if (it.first == _start)
                    start_flag = true;                          // Start Station was found
                if (it.first == _end )
                    end_flag = true;							// End Station was found
            }
            if (!(start_flag && end_flag))
                throw ( NONE_STATION );           				// throw an exception indicating that the stations were not found 
            else {
				if ( this->stations_hashmap[_start].name == this->stations_hashmap[_end].name  ){
					throw (SAME_DEPAR_ARRIV);
				}
                compute_and_display_travel(_start, _end); 		// Estimate the Path between _start & _end
            }
			
        }
		
		/** Overloaded Class-Constructor :
			@param : _filename1 : stations csv file
					 _filename2 : connections csv file	
				     _start : start station name
				     _end : end station name
		**/
        Network (std::string _filename1, std::string _filename2, std::string _start,std::string _end){
			/// Reading Stations,Connections files :
            read_stations(_filename1);
            read_connections(_filename2);
            compute_and_display_travel(_start,_end);            // Estimate the Path between _start & _end
        }
		/** Class-Destructor **/
		~Network(){}
};

/** Reading Stations method :
	@param : _filename1 : stations csv file
**/
void Network::read_stations(const std::string & _filename) {
    travel::Station S; 											// Station structure   
	
    std::fstream file(_filename);								//Open the file to read
    std::string line = "";

    if ( file.fail() ) {
		throw (ERROR_READ_FILE);								// Throw an exception indicating an error while reading the file
    } else {
        std::getline(file, line);
        while (file.good()) {
            std::getline(file, S.name, ','); 					// Read Station_name
            std::getline(file, line, ','); 						// Read Station_id
            std::getline(file, S.line_id, ','); 				// Read line_id
            std::getline(file, S.address, ','); 				// Read Station_adress
            std::getline(file, S.line_name); 					// Read line_adress
			
            uint64_t ID = std::strtoul(line.c_str(), NULL, 10);	//Convert a string to unsigned long
            this->stations_hashmap.insert( { ID, S} );			//filling the unordered_map
        }
    }
    file.close(); 												// Close the file
}
/** Reading Connections method :
	@param : _filename1 : connections csv file
**/
void Network::read_connections(const std::string & _filename) {
    std::fstream file(_filename);								// Open the file to read
    std::string line = "";
    std::string Station_begin, Station_end, Time_str;
    if (file.fail()) {
		throw (ERROR_READ_FILE);								// Throw an exception indicating an error while reading the file
    } else {
        std::getline(file, line);
        while (!file.eof()) {
            std::getline(file, Station_begin, ','); 			// station 1
            std::getline(file, Station_end, ','); 				// station 2
            std::getline(file, Time_str); 						// Transfer time between station 1 and station 2
			/// converting strings to unsigned long : 
            uint64_t station_begin_id = std::strtoul(Station_begin.c_str(), NULL, 10);
            uint64_t station_end_id = std::strtoul(Station_end.c_str(), NULL, 10);
            uint64_t time = std::strtoul(Time_str.c_str(), NULL, 10);
			
            if (station_begin_id == 0) {
                break;
            }
            this->connections_hashmap[station_begin_id][station_end_id] = time;			//filling the unordered_map 
        }

    }
    file.close();												// Close the file
}

/** Compute Travel Method : estimate the shortest path between a start & end stations using the Dijkstra algorithm
		@param : _start : start station Id
				 _end : end station Id
**/
std::vector < std::pair < uint64_t, uint64_t > > Network::compute_travel(uint64_t _start, uint64_t _end) {
	
    /// Optimal Path Between _Start and _end
    std::vector < std::pair < uint64_t, uint64_t > > Path; 							
    /// Open Nodes : Nodes visited while looking for path
    std::set < uint64_t > open_nodes;												
    /// Priority_queure : Nodes to be visited ( Sorted with the Criteria : Time_Order_Criteria 
    std::priority_queue < std::pair < uint64_t, uint64_t > , std::vector < std::pair < uint64_t, uint64_t > > , Time_Order_Criteria > prio_queue;
	/// unordered_map : - Minimal transfer time map, Predecessor Node map
	std::unordered_map < uint64_t, uint64_t > dict_time_min;
    std::unordered_map < uint64_t, uint64_t > dict_predecessor;
	/// Iterators for the connections_hashmap : 
    std::unordered_map < uint64_t, std::unordered_map < uint64_t, uint64_t >> ::iterator it;
    std::unordered_map < uint64_t, uint64_t > ::iterator it2;
	
	std::pair < uint64_t, uint64_t > Actuel_Node; 				//The Node we're exploring
	uint64_t actuel_time(0), Node(0),new_time(0),time_min(0);
	/// Start Node : 
    prio_queue.push({_start,0});								// Adding the start node to priority_queue of nodes
	dict_time_min[_start] = 0;									// Adding the distance associated to start node to the distance hashmap 
	
    while ( !prio_queue.empty() ) {
        Actuel_Node= prio_queue.top();							// we take the first element
        prio_queue.pop();										// we delete the element we took
		
        Node = Actuel_Node.first;								// Node : Ids
        actuel_time = Actuel_Node.second;						// time associated with the node
		if ( open_nodes.find(Node) != open_nodes.end() )
			continue;
        open_nodes.insert(Node);								// add the node to the opens nodes
        if (Node == _end) {										// we found the solution 
            break;
        }
        // for each child of node :
		it = this->connections_hashmap.find( Node );
		for (it2 = it -> second.begin(); it2 != it -> second.end(); it2++) {
			new_time = actuel_time + it2 -> second;				//update time
			time_min = ULONG_MAX;
			if ( dict_time_min.find(it2-> first)!= dict_time_min.end() )
				time_min = dict_time_min[it2 -> first];
			if ( new_time < time_min) {							// if the new time is less than the old time
				dict_time_min[it2 -> first] = new_time;			// Adding the the distance associated to node to the distance hashmap
				dict_predecessor[it2 -> first] = Node;			// Adding the associated parent of Node to Parent hashmap
				prio_queue.push({ it2 -> first, new_time });	// Adding the Child Nodes to the priority queue
            }
        }
	}
	
	if ( dict_time_min.find(_end)!= dict_time_min.end() )
		actuel_time = dict_time_min[_end];
    if (actuel_time != 0) {
        Node = _end;
        while ( Node != _start ){									
            Path.push_back(std::make_pair( Node, dict_time_min[Node]));		// add the the actuel node to the path		
            Node = dict_predecessor[ Node ];								// update the child node with its parent
        }
        Path.push_back(std::make_pair(_start, 0));							// add the start node to the path
        std::reverse(Path.begin(), Path.end());								// reverse the path ( start to end )
    }
    return Path;
}
/** Compute and Display Travel Method : estimate and display the shortest path between a start & end stations using the Dijkstra algorithm
		@param : _start : start station Id
				 _end : end station Id
**/
std::vector < std::pair < uint64_t, uint64_t > > Network::compute_and_display_travel(uint64_t _start, uint64_t _end) {
    std::vector < std::pair < uint64_t, uint64_t >> Path = compute_travel(_start, _end);
    uint64_t Time_current(0);			//time of actual station 	
    uint64_t Time_next(0);				//time of next station
    uint64_t time_temp(0);				//time between station 
    Station Current;					// actual station
    Station Next;						// next station
	bool same_line_flag(true);
	
    if (_end == _start)
        throw (SAME_STATION);				// Throw an exception indication that ids are identical 
    else {
        std::cout << " Best way from " << stations_hashmap[_start] << " to " << stations_hashmap[_end] << " is :" << std::endl;
        for ( unsigned int i(0); i < Path.size() - 1; i++) {
            Current = stations_hashmap[Path.at(i).first];
            Next = stations_hashmap[Path.at(i + 1).first];
			
            Time_current = Path.at(i).second;
            Time_next = Path.at(i + 1).second;
			
            if (Time_current != Time_next) {           // While we're changing stations ( except between departure and arrival )     
			
                if (Current.line_id != Next.line_id && Next.name == Current.name) { //making a correspondence at a station ( Walking )
				
                    if (i != 0) {
                        std::cout << " to " << Next.name << " ( " << time_temp << " Seconds )" << std::endl;
                        time_temp = 0;
                    }
                    std::cout << " Walk to " << Next.name << " , Line " << Next.line_id << " ( " << Time_next - Time_current << " Seconds ) " << std::endl;
                    same_line_flag = true;															// flag indicating that we take a new line
                }
                if (Current.line_id == Next.line_id && Current.name != Next.name) { 				// stations in the same line  
                    time_temp += this->connections_hashmap[Path.at(i).first][Path.at(i + 1).first]; 		// estimate time between two stations
                }
                if (Current.name != Next.name && same_line_flag) { 									// Changing actual line 
                    std::cout << " take line (" << Next.line_id << ")  " << Next.line_name << std::endl;
                    std::cout << " from " << Current.name;
                    same_line_flag = false;															// flag indicating the change of line
                }
            }
            if ( Current.name == stations_hashmap[_end].name || Next.name == stations_hashmap[_end].name) { // Arriving at the end station
                i = Path.size() - 1;
                std::cout << " to " << Next.name << "( " << time_temp << " Seconds )" << std::endl;
            }
        }
        std::cout << std::endl << " You have reached your destination in " << Path.back().second << " Seconds "; //Estimate path time 
    }
    return Path;
}

/** Overloaded Compute Travel Method : estimate the shortest path between a start & end stations using the Dijkstra algorithm
		@param : _start : start station name
				 _end : end station name
**/
std::vector<std::pair<uint64_t,uint64_t>> Network::compute_travel(const std::string& _start, const std::string& _end){
    std::vector<std::pair<uint64_t,uint64_t>> path;

    uint64_t _start_id(0),_end_id(0);

    for(auto it: this->stations_hashmap ){
        if( it.second.name ==_start )
            _start_id = it.first;
        if( it.second.name ==_end )
            _start_id = it.first;
    }

    return compute_travel(_start_id , _end_id);
}
/** Overloaded Compute and Display Travel Method : estimate and display the shortest path between a start & end stations using the Dijkstra algorithm
		@param : _start : start station name
				 _end : end station name
**/
std::vector<std::pair<uint64_t,uint64_t>> Network::compute_and_display_travel(const std::string& _start, const std::string& _end){
    std::priority_queue < std::pair < std::string, unsigned int > , std::vector < std::pair < std::string, unsigned int> > , Distance_Order_Criteria > distance_start;
    std::priority_queue < std::pair < std::string, unsigned int > , std::vector < std::pair < std::string, unsigned int > > , Distance_Order_Criteria > distance_end;

    uint64_t _start_id(0),_end_id(0);
	// Calculate Levenshtein_Distances for each station 
    for (auto it: this->stations_hashmap) {
        distance_start.push( { it.second.name, Levenshtein_Distance( it.second.name , _start) } ) ;
        distance_end.push( { it.second.name, Levenshtein_Distance( it.second.name , _end) } );
    }
	/// take the minimum distance ( taking the top element since we're using the criteria )
    std::string departure = distance_start.top().first;
    std::string destination   = distance_end.top().first;
	/// Correction of stations name : 
		if( distance_start.top().second !=0 || distance_end.top().second !=0 ) 
			std::cout<<" You mean : "<<departure<<" To "<<destination<<std::endl;
	/// check wether it's the same station
    if ( departure== destination)
        throw (SAME_STATION);			///we throw an exception
	/// finding ids for the Departure and arrival stations found 
    for(auto it: this->stations_hashmap ){
        if( it.second.name == departure ){
            _start_id = it.first;	
        }
        if( it.second.name == destination ){
            _end_id = it.first;
        }
	}
			
    return compute_and_display_travel(_start_id,_end_id);
}

/// MAIN PROGRAM /// 
int main( int argc , char *argv[]) {
	/// Variables to estimate execution time: 
	clock_t start(0), end(0);
    try {
		
        std::cout << "#*#*# Paris - Métro Mapper Application  #*#*# \n" << std::endl;
        if( argc!= 5 ){
            std::cerr<<"Usage : Stations.csv Connections.csv Id_Start Id_End "<<std::endl; 	//Error Message indicating the right use of the program
        } else {
            std::string s = argv[1];  /// stations file_name
            std::string c = argv[2];  /// connections file_name
			
            /// check weather arguments are : Ids or strings :
			start = clock();                               						// Capture a start clock to estimate execution time   
			
            if( isNumber(argv[3]) && isNumber(argv[4]) ) {
				/// Convert String argument to unsigned long (uint64_t)
                uint64_t id1 = std::strtoul(argv[3], NULL, 10); 	 
                uint64_t id2 = std::strtoul(argv[4], NULL, 10);
				/// Call the Network Constructor ( with Ids ) 
                Network r4(s,c,id1,id2);
				
            } else
				/// Call the Network Constructor ( with String ) 
                Network r4(s,c,argv[3],argv[4]);
				
			end = clock();														// Capture an end clock to estimate execution time   
        }
			double time_taken = double(end - start) / double(CLOCKS_PER_SEC);  	// Calculate execution time between two clocks   
			
			std::cout <<"|| Elapsed time : " << time_taken << std::setprecision(8)<<" Seconds "<<std::endl; 
	
    } catch ( int const& index){												// Catching exceptions thrown  
        switch ( index ){
            case NONE_STATION :													// Catching None_stations exception
                std::cerr<<"Stations Ids were not found , Please try with valid Stations Ids "<<std::endl;
                break;
            case SAME_STATION :													// Catching Same_stations exception
                std::cerr<<" You entered Ids for the same station , no path was calculated "<<std::endl;
                break;
			case SAME_DEPAR_ARRIV :
				std::cerr<<" You entered Ids for the same station( Departure,Arrival ) , no path was calculated "<<std::endl;
				break;
			case ERROR_READ_FILE :  											// Catching None_stations exception
				std::cerr<<" Error while reading the files ( check the directory, if they're open ... )"<<std::endl;
				break;
        }
    } 
    return 0;
}

	