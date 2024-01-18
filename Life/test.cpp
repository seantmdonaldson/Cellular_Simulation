#include <fstream>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <iomanip>

using namespace std;


int main(){
    
    srand(time(NULL));

    int counter = 0;
    int chance = 0;
    for (int i = 0; i < 10; i++)
    {
        
        int a = rand()%25;
        cout << a;
        cout << "\n";
    }
    
    return 0;
}