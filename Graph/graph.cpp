//graph.cpp
// Aqdas Juya , 10/10`/2024 , ajuya@pdx.edu ,  Program # 5, cs 163
#include "graph.h"


table::table(int size) {
	listsize = size;
	vertexcount = 0;


	adjacencylist = new vertex[size];
	for(int i= 0; i < size; ++i) {
		adjacencylist[i].gpscoordinates = " ";
		adjacencylist[i].description = " ";
		adjacencylist[i].extrainfo = " ";
		adjacencylist[i].head = nullptr;
	}
	
	gps_table = new gpsnode *[listsize];
	for(int i = 0; i < listsize; ++i) {
		gps_table[i] = nullptr;
	}

}
table::~table() {

	for(int i = 0;i < vertexcount; ++i) {
		edgenode * current = adjacencylist[i].head;
		while(current) {
			edgenode * to_delete = current; 
			current = current -> next;
			delete to_delete;
		}
	}
	delete[] adjacencylist;

	for(int i = 0; i < listsize; ++ i) {
		gpsnode * current = gps_table[i];
		while(current) {
			gpsnode * to_delete = current;
			current = current -> next;
			delete to_delete;
		}
	}
	delete[] gps_table;

}

//hash function:
int table::hash(const string & key) const{
	if (key.empty()) return 0; // i used the stl to cheack for the key being empty or not 
	
	int count = 0;
	int length = key.size();// i used the size to see the size
	const int prime = 31;

	for(int i = 0; i < length; ++i) {
		count = (count * prime + tolower(key[i])) % listsize;
		if(count < 0) count += listsize;
	}
	return count;
}

int table::addvertex(const string & gps, const string & description, const string & info) {

	if(vertexcount >= listsize) {
		cout <<"Error: vertex list is full. cannot add more geocaches" << endl;
		return 0;
	}
	adjacencylist[vertexcount].gpscoordinates = gps;
	adjacencylist[vertexcount].description = description;
	adjacencylist[vertexcount].extrainfo = info;
	adjacencylist[vertexcount].head = nullptr;

	int index = hash(gps);
	
	gpsnode * newnode = new gpsnode;
	newnode -> gps = gps;
	newnode -> index = vertexcount;
	newnode -> next = gps_table[index];
	gps_table[index] = newnode;

	++vertexcount;
	return 1;
}
//finds vertex using the hash and pluging the key to find it 
int table::findvertex(const string &key) const {
	int index = hash(key);
	gpsnode * current = gps_table[index];

	while (current) {
		if(current -> gps == key) {
			return current -> index;
		}
		current = current -> next;
	}
	return -1;
}
// this will addege using the current attach and distance form the struct to allocate them 
int table::addege(const string & current, const string & attach, float distance){
	int fromindex = findvertex(current);
	int toindex = findvertex(attach);

	if(fromindex == -1 ){
		return 0;
	}
	if(toindex == -1) {
		return 0;
	}

	edgenode * newedge = new edgenode;
	newedge -> vertexindex = toindex;
	newedge -> distance = distance;
	newedge -> next = adjacencylist[fromindex].head;

	adjacencylist[fromindex].head= newedge;
	return 1;
}

int table::displayall() const{
	if (vertexcount == 0) {
		return 0;
	}

	for(int i = 0; i < vertexcount; ++i) {
		cout << "geocache number" << (i + 1) << endl;
		cout << " gps coordinates: " << adjacencylist[i].gpscoordinates<< endl;
		cout << "discription: " << adjacencylist[i].description << endl;
		cout << "extra info: " << adjacencylist[i].extrainfo << endl;
		cout << "connection: ";
		if(adjacencylist[i].head) {
			displayedgelist(adjacencylist[i].head);	
		} else {
			cout << "none";
		}
		cout << endl << endl;
	}
	return 1;
}
int table::displayedgelist(edgenode * head) const{
	if(!head) return 0;


	bool first = true;
	while(head) {
		if(!first) {
			cout << ", ";
		}
		cout << "Geocache" << (head -> vertexindex + 1) 
		     << "(distance: "<< head->distance << "miles)";
		first = false;
		head = head -> next;
	}
	return 1;
}
//recyrsive way to find the closestedge 
int table::findclosestedge(edgenode * head, float & mindistance) const {
	if(!head) {
		return -1;
	}
	int closestindex = findclosestedge(head -> next, mindistance);
	if(head -> distance < mindistance) {
		mindistance = head -> distance;
		return head -> vertexindex;
	}
	return closestindex;
}
//wrapper function to help to display or cheack if it should pass all the rule :
int table::displayclosest(const string & from) const{
	int freomindex = findvertex(from);
	if(freomindex == -1) {
		return -1;
	}
	edgenode * head = adjacencylist[freomindex].head;
	if(!head) {
		return -1;
	}

	float mindistance = 1e9;
	int closestindex = findclosestedge(head, mindistance);
	if(closestindex == -1) {
		cout <<"no closest geocache found.";
		return -1;
	}
	cout << "closest geocache from " << from << "\n";
	cout << "gps coordinates: " << adjacencylist[closestindex].gpscoordinates << "\n";
	cout << "discription: " << adjacencylist[closestindex].description<<"\n";
	cout << "extra info: " << adjacencylist[closestindex].extrainfo << "\n";
	cout << "distance: " << mindistance << " miles\n";
	
	
	return closestindex;
}
