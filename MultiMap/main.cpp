#include <iostream>
#include "multimap.h"
using namespace std;

int main()
{
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);
   freopen("multimap.in", "r", stdin);
   freopen("multimap.out", "w", stdout);
   MultiMap my_multi_map;
   string input, input1, input2;
   while (cin >> input)
   {
       cin >> input1;
       if (input == "put")
       {
           cin >> input2;
           my_multi_map.multi_map_put(input1, input2);
       }
       else if (input == "delete")
       {
           cin >> input2;
           my_multi_map.multi_map_delete(input1, input2);
       }
       else if (input == "deleteall")
           my_multi_map.multi_map_deleteall(input1);
       else if (input == "get")
           cout << my_multi_map.multi_map_get(input1) << endl;
   }
   return 0;
}