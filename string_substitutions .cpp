#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

struct Split {
   // TODO: can optimize this if needed
   //       instead of not_done, original + index/pointer?
   std::string done;
   std::string not_done;
   
   Split(std::string start): not_done(start) {
      
   }
};

struct Solution {
   Split split1;
   Split split2;
   // TODO: shared std::string done (should match for both anyways)
   //       Split should instead store original string + index to track completed (and rename it)
   
   Solution(Split one, Split two): split1(one), split2(two) {      
   
   }

   Solution(const Solution& other) : split1(other.split1), split2(other.split2) {

   }
};

void printPartial(const Solution& s);

int main(void) {
   std::string input1("Shakes*e");
   std::string input2("S*speare");

   std::vector<Solution> solution_stack; 
   // TODO: use queue instead for depth-first?
   //       no, actually stack is depth-first
   
   
   solution_stack.push_back(Solution(Split(input1), Split(input2)));
   
   bool done = false;
   
   while (done == false) {
      
      // TODO: there must be a better way to get the popped item?  use std::move maybe?
      Solution possible_solution = solution_stack[solution_stack.size() - 1];
      solution_stack.pop_back(); 
      
      printPartial(possible_solution);

      char c1 = possible_solution.split1.not_done[0];
      char c2 = possible_solution.split2.not_done[0];
      
      if ((c1 == '*') && (c2 == '*')) {
         
      }
      else if (c1 == '*') {
         // * from s1 can match up to 4 chars from s2
         int count = 0;
         int i = 0; // index does not necessarily == count because *'s could add 0 length
                    // for example, * match ab**ab is possible
         while ((count <= 4) && (possible_solution.split2.not_done.length() >= i)) {
            // TODO: repeating this work in a loop is inefficient
            Solution s(possible_solution);
            s.split1.not_done.erase(0, 1);
            s.split1.done.append(s.split2.not_done.substr(0, i));
            s.split2.done.append(s.split2.not_done.substr(0, i)); // first iteration will be 0 - that is intended because * could match nothing at all
            s.split2.not_done.erase(0, i);

            // TODO: this is copy/pasted
            bool done1 = (s.split1.not_done.length() == 0);
            bool done2 = (s.split2.not_done.length() == 0);

            if (done1 && done2) {
               // found a solution
               std::cout << "Solution found: " << std::endl;
               std::cout << s.split1.done << std::endl;
               std::cout << s.split2.done << std::endl;
               return 0;
            }
            else if (!done1 && !done2) {
               // could still be a solution, keep going with it
               solution_stack.push_back(s);
            }
            else {
               // one is done and the other is not - this solution is no good, so discard it
               // no action required - just don't put it back on the stack
               //continue; // TODO: not necessary?
            }

            if (s.split2.done[s.split2.done.length() - 1] != '*') {
               count++; // only increase count if matched char is not a * - because * can be empty string
            }
            i++; // always increase i - to move onto the next char
         }
      }
      else if (c2 == '*') {
         // * from s2 can match up to 4 chars from s1
         int count = 0;
         int i = 0; // index does not necessarily == count because *'s could add 0 length
         // for example, * match ab**ab is possible
         while ((count <= 4) && (possible_solution.split1.not_done.length() >= i)) {
            // TODO: repeating this work in a loop is inefficient
            Solution s(possible_solution);
            s.split2.not_done.erase(0, 1);
            s.split2.done.append(s.split1.not_done.substr(0, i));
            s.split1.done.append(s.split1.not_done.substr(0, i)); // first iteration will be 0 - that is intended because * could match nothing at all
            s.split1.not_done.erase(0, i);

            // TODO: this is copy/pasted
            bool done1 = (s.split1.not_done.length() == 0);
            bool done2 = (s.split2.not_done.length() == 0);

            if (done1 && done2) {
               // found a solution
               std::cout << "Solution found: " << std::endl;
               std::cout << s.split1.done << std::endl;
               std::cout << s.split2.done << std::endl;
               return 0;
            }
            else if (!done1 && !done2) {
               // could still be a solution, keep going with it
               solution_stack.push_back(s);
            }
            else {
               // one is done and the other is not - this solution is no good, so discard it
               // no action required - just don't put it back on the stack
               //continue; // TODO: not necessary?
            }

            if (s.split1.done[s.split1.done.length() - 1] != '*') {
               count++; // only increase count if matched char is not a * - because * can be empty string
            }
            i++; // always increase i - to move onto the next char
         }
      }
      else {
         // regular characters, compare them
         if (c1 == c2) {
            // they match, so advance one char
            possible_solution.split1.done.push_back(c1);
            possible_solution.split2.done.push_back(c2);
            possible_solution.split1.not_done.erase(0, 1);
            possible_solution.split2.not_done.erase(0, 1);

            bool done1 = (possible_solution.split1.not_done.length() == 0);
            bool done2 = (possible_solution.split2.not_done.length() == 0);
            if (done1 && done2) {
               // found a solution
               std::cout << "Solution found: " << std::endl;
               std::cout << possible_solution.split1.done << std::endl;
               std::cout << possible_solution.split2.done << std::endl;
               return 0;
            }
            else if (!done1 && !done2) {
               // could still be a solution, keep going with it
               solution_stack.push_back(possible_solution);
            }
            else {
               // one is done and the other is not - this solution is no good, so discard it
               // no action required - just don't put it back on the stack
               //continue; // TODO: not necessary?
            }
            
         }
         else {
            // this branch didn't work, so move on
            //continue; // TODO: not necessary?
         }
      }
      
   }

   // if this is reached, all possible solutions were exhausted
   std::cout << "No solution found" << std::endl;
   return 0;
}

void printPartial(const Solution& s) {
   std::cout << s.split1.done << ", " << s.split1.not_done << std::endl;
   std::cout << s.split2.done << ", " << s.split2.not_done << std::endl;
   std::cout << std::endl;
}