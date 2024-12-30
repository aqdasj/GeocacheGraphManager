//.h file:
//// Aqdas Juya , 10/10`/2024 , ajuya@pdx.edu ,  Program # 5, cs 163
#include <cstring>
#include <iostream> 
#include <string>
#include <vector> 

using namespace std;


struct vertex;
struct edgenode {
	int vertexindex;
	float distance;
	edgenode *next;
	vertex * adjacent;
};

struct vertex
{
	string gpscoordinates;
	string description;
	string extrainfo;
	edgenode*head;
};
struct gpsnode{
	string gps;
	int index;
	gpsnode * next;

};

class table {
	public: 
		table(int size = 5);
		~table();

		int addvertex(const string & gps, const string & desciption, const string & info);
		int addege(const string & current, const string & attach, float distance);
		int displayall() const;
		int displayclosest(const string & from) const;


	private:
		vertex * adjacencylist;
		gpsnode **gps_table; 
		int listsize;
		int vertexcount;

		int hash(const  string & key) const; // hashfunction
		int findvertex(const string & key) const;
		
		int findclosestedge(edgenode * head, float & mindistance) const;
		int displayedgelist(edgenode*head) const;
};
