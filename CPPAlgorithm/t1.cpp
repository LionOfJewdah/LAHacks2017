#include <iostream>
#include <string>

using namespace std;

string OptimizeSolutionPath(string sol_path)
{
   string ret, cache;
   char prev = ' ', prev2 = ' ';
   int count = 0;

   sol_path += ' ';   // add space at the end for proper algorithm function at the end of the string
   for (string::iterator it = sol_path.begin(); it != sol_path.end(); ++it) {
      char c = *it;
      if ('i' == c) continue;
      if ('2' == c) c = prev;
      char ci = *(it+1);
      if ('i' == ci)
         c = tolower(c);
      if (prev == c && 3 > count)
         count++;
      else {
         if (3 == count) {
            cache.clear();
            count = 0;
         } else if (2 == count) {
            if (isupper(prev))
               ret = ret + (char)tolower(prev);
            else
               ret = ret + (char)toupper(prev);
            cache.clear();
            count = 0;
         } else {
            if (0 < cache.length())
               ret += cache;
            cache.clear();
            count = 0;
         }
      }
      cache += c;
      prev = c;
   }
   if (0 < cache.length())
      ret += cache;
   cache = string();
   prev = ' ';
   prev2 = ' ';
   for (string::iterator it = ret.begin(); it != ret.end(); ++it) {
      char c = *it;
      if (prev != ' '
              &&
              (toupper(prev) == toupper(c)
               &&
               ((isupper(prev) && islower(c)) || (islower(prev) && isupper(c))))
         ) 
      {
         cout << "prev=" << prev << ", c=" << c << endl;
         string::iterator cache_tail = cache.end() - 1;
         cout << "Remove1:[" << *cache_tail << "]" << endl;
         string::iterator itrem = cache.erase(cache_tail);
         cout << "Now pointing to:[" << *itrem << "]" << endl;
         /***
         if (islower(prev)) {
            cache_tail = cache.end() - 1;
            cout << "Remove2:[" << *cache_tail << "]" << endl;
            itrem = cache.erase(cache_tail);
            cout << "Now pointing to:[" << *itrem << "]" << endl;
         }
         ***/
         prev = ' ';
         cout << "Cache: [" << cache << "]" << endl;
         continue;
      } else if (prev2 != ' '
                 && toupper(prev2) == toupper(c)
                 && ((isupper(prev2) && islower(c)) 
                      || (islower(prev2) && isupper(c)))
                )
      {
         cout << "prev2=" << prev2 << ", prev=" << prev << ", c=" << c << endl;
         string layer_pairs = "UDLRFB";
         bool bopt = false;
         for (int i=0; !bopt && i<layer_pairs.size(); i+=2) {
            if (((layer_pairs[i] == toupper(c)
                  && layer_pairs[i+1] == toupper(prev)
                 ) ||
                 (layer_pairs[i] == toupper(prev)
                  && layer_pairs[i+1] == toupper(c)
                 )) 
               )
            {
               bopt = true;
            }
         }
         if (bopt) {
            string::iterator cache_tail = cache.end() - 2;
            cout << "Remove3:[" << *cache_tail << "]" << endl;
            string::iterator itrem = cache.erase(cache_tail);
            cout << "Now pointing to:[" << *itrem << "]" << endl;
            /***
            if (islower(prev2) && cache.size()>1) {
               cache_tail = cache.end() - 2;
               cout << "Remove2:[" << *cache_tail << "]" << endl;
               itrem = cache.erase(cache_tail);
               cout << "Now pointing to:[" << *itrem << "]" << endl;
            }
            ***/
            prev = ((cache.size()>0) ? cache[cache.size()-1] : ' ');
            prev2 = ((cache.size()>1) ? cache[cache.size()-2] : ' ');
            cout << "Cache: [" << cache << "]" << endl;
            continue;
         }
      }
      //if (islower(c))
      //   cache = cache + (char)toupper(c) + 'i';
      //else
         cache += c;
      prev2 = prev;
      prev = c;
      cout << "Cache: [" << cache << "]" << endl;
   }
   ret = cache;
   cache = string();
   prev = ' ';
   for (string::iterator it = ret.begin(); it != ret.end(); ++it) {
      char c = *it;
      if (isupper(c) && c == prev)
         c = '2';
      if (islower(c))
         cache = cache + (char)toupper(c) + 'i';
      else
         cache += c;
      prev = c;
   }
   ret = cache;



   // remove the extra space from the end - we don't need it anymore
   string::iterator ret_tail = ret.end() - 1;
   ret.erase(ret_tail);

   return ret;
}

int main(int argc, char *argv[])
{
   string str_in = string(argv[1]);
   string str_out = OptimizeSolutionPath(str_in);
   cout << str_out << endl;
}
