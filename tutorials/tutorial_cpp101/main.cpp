#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>

using namespace std;

void Test_Array()
{
	//vector<int> array={1,2,3,4};
	vector<int> array;
	vector<float> array2;
	array.resize(5,1);
	array.resize(5);
	array.resize(6);
	array.resize(8,2);

	for(int i=0;i<array.size();i++){
		cout << array[i] << " ";
	}
	cout << endl;
	
	array[1]=0;
	array.push_back(5);
	for(auto& a:array){
		cout << a << " ";
	}
	cout << endl;
    
	array.clear();
}


void Test_Grammar()
{
	// && - and; || - or; ! - not
	int a=0;
	int b=1;
	if ( !(a>-1) ) {
		cout << "a<=-1" << endl;
	} else {
		cout << "a>-1" << endl;
	}

	if ((a>=0) && (b>=0)) {
		cout << "Both a & b are non-negative." << endl;
	}
	if ((a==0) || (b>=0)) {
		cout << "Either a or b is zero." << endl;
	}


	// ++ OPERATOR
	int x = 3;
	int y, z;
   	y = x++; //returns the value before incrementing.
   	z = ++x; //returns the value after it has been incremented. 
   	cout << x << " " << y << " " << z << endl;


	// DO & WHILE LOOP
	a=0;
	b=1;
	while(a<3){
		a++;
		int c=b+(a++);
		cout << "c = " << c << ", ";
		cout << "a = " << a << endl;
	}

	do{
		a++;
	}while(a<5);


   	// SWITCH
	int d=0;
	// int d=1;
	// int d=2;
	switch(d){
	case 0:
		cout<<"case 0"<<endl;
		break;
	case 1:
		cout<<"case 1"<<endl;
		break;
	default:
		cout<<"case default"<<endl;
		break;
	}
}

void Test_Containers()
{
	// SET
	set<int> s={1,3,5,7};
	s.insert(2);
	s.insert(5);
	//s.erase(2);
	//s.clear();

	cout << "set elements: ";
	for(auto& a:s){
		cout<< a << " ";
	}
	cout << endl;

	auto result=s.find(6);
	if(result!=s.end()){
		cout<<"find "<<(*result)<<endl;
	}

	// MAP
	cout << "map elements: " << endl;
	map<int,string> m;
	m[1]="one";
	m[3]="three";
	m[2]="two";
	for(auto& iter:m){
		std::cout<<"map ele key: "<<iter.first
			<<", value: "<<iter.second<<endl;
	}	

	auto result2=m.find(1);
	if(result2!=m.end()){
		cout<<(*result2).first<<", "<<(*result2).second<<endl;
	}

	// UNORDERED_SET
	unordered_set<int> hashset;
	hashset = {1,3,5,7};
	hashset.insert(2);
	hashset.insert(5);
	//hashset.erase(3);
	cout << "hashset elements: ";
	for(auto& a: hashset){
		cout << a << " ";
	}
	cout << endl;

	auto result3=s.find(6);
	if(result!=s.end()){
		cout<<"find "<<(*result3)<<endl;
	}

	// UNORDERED_MAP
	cout << "hashmap elements: " << endl;
	unordered_map<int,string> hashmap;
	hashmap[1]="one";
	hashmap[3]="three";
	hashmap[2]="two";
	for(auto& iter:hashmap){
		std::cout<<"hashmap ele key: "<<iter.first
			<<", value: "<<iter.second<<endl;
	}
	auto result4=m.find(1);
	if(result4!=m.end()){
		cout<<(*result4).first<<", "<<(*result4).second<<endl;
	}
}

int main()
{
	//Test_Container();
	//Test_Grammar();
	Test_Containers();
}
