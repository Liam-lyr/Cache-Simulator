#include <iostream>
#include "base.h"

using namespace std;

void PrintProperties()
{
    cout << "===============================================" << endl
         << "===============Cache Properties================" << endl;

    cout << "Cache size: " << i_cache_size << "KB" << endl;
    cout << "Cache line size: " << i_cache_line_size << "B" << endl;
    cout << "Cache set number: " << i_cache_set_num << endl;
    cout << "Cache line number: " << i_cache_line_num << endl;
    
    cout << "Type of association: " << t_assoc << endl;
    cout << "Type of replacement: " << t_replace << endl;
    cout << "Type of write policy: " << t_write << endl;


    cout << "Cache line size in bits: " << bit_cache_line _size << endl;


}