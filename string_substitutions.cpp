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
	// sample input 1: solution "Shakespeare"
	//std::string input1("Shakes*e");
   //std::string input2("S*speare");

	// sample input 2: no solution
	//std::string input1("a*baa**ba**aa");
	//std::string input2("*ca*b**a*baac");

	// START OF CHALLENGE INPUTS

	// program claims no solution... and I can't see one either
	//std::string input1("bb*aaaaa*ba**");
	//std::string input2("*baabb*b*aaaa");

	//std::string input1("dnKeeuCCyHOnobnDYMGoXDdNWhTsaoedbPifJ*ki*wWfXjIUwqItTmGqtAItoNWpDeUnNCWgZsKWbuQxKaqemXuFXDylQubuZWhMyDsXvDSwYjui*LviGAEkyQbtR*cELfxiAbbYyJRGtcsoJZppINgJGYeZKGeWLbenBEKaoCgheYwOxLeFZJPGhTFRAjNn*");
	//std::string input1("d*eeuCCyHOnobnDYMGoXDdNWhTsaoedbP*ijrwWfXjIUwqItTmGqtAItoNWpDeUnNCWgZs*WbuQxKaqemXuFXDylQubuZWhMyDsXvDSwYjuijkLviGAEkyQbtRUsncELfxiAbbYyJRG*soJZppINgJGYeZKGeWLbenBEKaoCghe*YwOxLeFZJPGhTFRAjNn");

	//std::string input1("THAkZYrkUWgcTpZ*SsNQKsEnvdUveZxssEtCEQuoMqToJjMdCatMs*v*GyMlROpiIDUZyJjhwmjxFWpEwDgRLlLsJYebMSkwxEUvoDcLPLIwHY*GvoRhgcfkdsenObSjWGNYRDJAzRzavAGRoZZ*fDXIRlJkufqHDjLMJKEjLAkRRyQqTrUaWRIndSX");
	//std::string input1("*THAkZYrkUWgcTpZSsNQKsEnvdUveZxssEtCEQuoMqToJjMdCatMsYa*nBvIFuGyMlROpiIDUZyJjh*FWpEwDgRLlLsJYebMSkw*oDcLPLIwHYbeBGvoRhgcfkdsenObSjWGNYRDJAzRzavAGRoZZvbEfDXIRlJkufqHDjLMJKEjLAkRRyQqTrU*aWRIndSX");

	//std::string input1("jEAmXdDUtthXNLbIZFeWdiQPGEvyCEeLI**EyficABUH*YiSZRREvniDexKJSjLXMYfsw*YlbTSZBlYSecorJsWidfALQYzOdrKNrJZRdrQEDoyhPMYAfTiHZIuqGtEkKqYBzxtCOJhRYfZNSYNxRWFrfahlSLvdBTebrXDgGlZEqxRIvGhN*mfhLLSExNHaHLAZI");
	//std::string input1("jEAmXdDUtthXNLbIZFeWdiQPGEvyCEeL**BUHYiSZRREvniDexKJSjLXMYfswlaYlbTSZBlYSecorJsWidfALQYzOdrKNrJZ*EDoyhPMYAfTiHZIuqGtEkKqYBzxtC*YfZNSYNxRWFrfahlSLvdBT*ebrXDgGlZEqxRIvGhNcmfhLLSExNHaHLAZI");

   std::vector<Solution> solution_stack; 
   // TODO: use queue instead for depth-first?
   //       no, actually stack is depth-first
   
   
   solution_stack.push_back(Solution(Split(input1), Split(input2)));
   
   while (true) {
      
	   if (solution_stack.size() == 0) {
		   break;
	   }

      // TODO: there must be a better way to get the popped item?  use std::move maybe?
      Solution possible_solution = solution_stack[solution_stack.size() - 1];
      solution_stack.pop_back(); 
      
      //printPartial(possible_solution);

      char c1 = possible_solution.split1.not_done[0];
      char c2 = possible_solution.split2.not_done[0];
      
      if ((c1 == '*') && (c2 == '*')) {
         // TODO: what happens if I remove a * from one, push solution, remove a * from the other, push solution?
         //       appears to work, but what if double *'s? think it's still ok?
         
         if ((possible_solution.split1.not_done.length() == 1) && (possible_solution.split2.not_done.length() == 1)) {
            // *'s were at the end, found a solution
            std::cout << "Solution found: " << std::endl;
            std::cout << possible_solution.split1.done << std::endl;
            std::cout << possible_solution.split2.done << std::endl;
            return 0;
         }

         Solution s1(possible_solution);
         Solution s2(possible_solution);

         s1.split1.not_done.erase(0, 1); // remove a * from one of these and keep going - this pushes it into two of the three cases below
         s2.split2.not_done.erase(0, 1);

         // TODO: edge case, what if this was the last character in one or both of them?

         solution_stack.push_back(s1);
         solution_stack.push_back(s2);
         /*int i = 0;
         int j = 0;
         int count1 = 0;
         int count2 = 0;
         // TODO: O(n^2) if lots of *s?

         while (((count1 <= 4) && (possible_solution.split1.not_done.length() >= i))
            && ((count2 <= 4) && (possible_solution.split2.not_done.length() >= i))) // TODO: this only works if nested loops? 
         {
            Solution s(possible_solution);
         }*/
      }
      else if (c1 == '*') {
         // * from s1 can match up to 4 chars from s2
         int i = 0; // index does not necessarily == count because *'s could add 0 length
                    // for example, * match ab**ab is possible
         while (possible_solution.split2.not_done.length() >= i) {
            // TODO: repeating this work in a loop is inefficient
            Solution s(possible_solution);
			std::string match = s.split2.not_done.substr(0, i);
            s.split1.not_done.erase(0, 1);
            s.split1.done.append(match);
            s.split2.done.append(match); // first iteration will be 0 - that is intended because * could match nothing at all
            s.split2.not_done.erase(0, i);

            // TODO: this is copy/pasted
            // TODO: edge case: one length is 0, other not_done is all ***
            //       need a .done() function, checks for not_done length == 0 or all ***
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

			int star_count = 0;
			for (const char& c : match) {
				if (c == '*') {
					star_count++;
				}
			}
			int matched_chars = match.length() - star_count;
			if (matched_chars >= 4) {
				break;
			}

            i++; // always increase i - to move onto the next char
         }
      }
      else if (c2 == '*') {
         // * from s2 can match up to 4 chars from s1
         int i = 0; // index does not necessarily == count because *'s could add 0 length
         // for example, * match ab**ab is possible

         while (possible_solution.split1.not_done.length() >= i) {
			 
			 // TODO: repeating this work in a loop is inefficient
            Solution s(possible_solution);
            s.split2.not_done.erase(0, 1);
			std::string match = s.split1.not_done.substr(0, i); // first iteration will be 0 - that is intended because * could match nothing at all
            s.split2.done.append(match);
            s.split1.done.append(match); 
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
			
			int star_count = 0;
			for (const char& c : match) {
				if (c == '*') {
					star_count++;
				}
			}
			int matched_chars = match.length() - star_count;
			if (matched_chars >= 4) {
				break;
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