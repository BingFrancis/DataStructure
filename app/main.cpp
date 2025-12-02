#include <iostream>
using namespace std;


struct book
{
    string title;
    string auther;
    string subjec;
};

book booklist[10];

int main(int argc, char const *argv[])
{
    booklist[0].title = "c++_data_structure";
    booklist[0].auther = "francis_mt";
    for(auto it :booklist){
        if(it.title.size()> 0){
            cout << "title:" << it.title << " subject:" << it.subjec << " auther:" <<it.auther << endl;
        }
        
    }
    return 0;
}
