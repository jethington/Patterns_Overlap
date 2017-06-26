#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <tuple>
#include <cassert>

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

    Solution(): split1(std::string("")), split2(std::string("")) { // TODO: default constructor not needed if using boost::Optional?
        //split1 = Split(std::string("")); // should never be used...
    }

    Solution(Split one, Split two): split1(one), split2(two) {      

    }

    Solution(const Solution& other) : split1(other.split1), split2(other.split2) {

    }
};

//void printPartial(const Solution& s);
std::tuple<bool, Solution> solve(std::string input1, std::string input2);
void run_tests(void);

std::tuple<bool, Solution> solve(std::string input1, std::string input2) {
    std::vector<Solution> solution_stack;   
   
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
            return std::make_tuple(true, possible_solution);
         }

         Solution s1(possible_solution);
         Solution s2(possible_solution);

         s1.split1.not_done.erase(0, 1); // remove a * from one of these and keep going - this pushes it into two of the three cases below
         s2.split2.not_done.erase(0, 1);

         // TODO: edge case, what if this was the last character in one or both of them?

         solution_stack.push_back(s1);
         solution_stack.push_back(s2);
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
            
			bool s1empty = (s.split1.not_done.length() == 0);
            bool s2empty = (s.split2.not_done.length() == 0);

			bool done1 = true;
			bool done2 = true;
			for (const char& c : s.split1.not_done) {
				if (c != '*') {
					done1 = false;
					break;
				}
			}
			for (const char& c : s.split2.not_done) {
				if (c != '*') {
					done2 = false;
					break;
				}
			}

            if (done1 && done2) {
               // found a solution
               return std::make_tuple(true, s);
            }
			else if (s1empty != s2empty) {
				// one is out of chars, other has chars left (that are not *'s)
				// this solution is no good, so discard it
			}

            else {
               // could still be a solution, keep going with it
               solution_stack.push_back(s);
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
               return std::make_tuple(true, s);
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
               return std::make_tuple(true, possible_solution);
            }
            else if (!done1 && !done2) {
               // could still be a solution, keep going with it
               solution_stack.push_back(possible_solution);
            }
            else {
               // one is done and the other is not - this solution is no good, so discard it
               // no action required - just don't put it back on the stack
            }
            
         }
         else {
            // this branch didn't work, so move on
            // no action required - just don't put it back on the stack
         }
      }
      
   }

   // if this is reached, all possible solutions were exhausted
   return std::make_tuple(false, Solution());
}

int main(void) {
	run_tests();

	// finds no solution - but there is one
	std::string input1("dnKeeuCCyHOnobnDYMGoXDdNWhTsaoedbPifJ*ki*wWfXjIUwqItTmGqtAItoNWpDeUnNCWgZsKWbuQxKaqemXuFXDylQubuZWhMyDsXvDSwYjui*LviGAEkyQbtR*cELfxiAbbYyJRGtcsoJZppINgJGYeZKGeWLbenBEKaoCgheYwOxLeFZJPGhTFRAjNn*");
	std::string input2("d*eeuCCyHOnobnDYMGoXDdNWhTsaoedbP*ijrwWfXjIUwqItTmGqtAItoNWpDeUnNCWgZs*WbuQxKaqemXuFXDylQubuZWhMyDsXvDSwYjuijkLviGAEkyQbtRUsncELfxiAbbYyJRG*soJZppINgJGYeZKGeWLbenBEKaoCghe*YwOxLeFZJPGhTFRAjNn");

	// finds solution - check
	//std::string input1("THAkZYrkUWgcTpZ*SsNQKsEnvdUveZxssEtCEQuoMqToJjMdCatMs*v*GyMlROpiIDUZyJjhwmjxFWpEwDgRLlLsJYebMSkwxEUvoDcLPLIwHY*GvoRhgcfkdsenObSjWGNYRDJAzRzavAGRoZZ*fDXIRlJkufqHDjLMJKEjLAkRRyQqTrUaWRIndSX");
	//std::string input2("*THAkZYrkUWgcTpZSsNQKsEnvdUveZxssEtCEQuoMqToJjMdCatMsYa*nBvIFuGyMlROpiIDUZyJjh*FWpEwDgRLlLsJYebMSkw*oDcLPLIwHYbeBGvoRhgcfkdsenObSjWGNYRDJAzRzavAGRoZZvbEfDXIRlJkufqHDjLMJKEjLAkRRyQqTrU*aWRIndSX");

	// finds solution - check
	//std::string input1("jEAmXdDUtthXNLbIZFeWdiQPGEvyCEeLI**EyficABUH*YiSZRREvniDexKJSjLXMYfsw*YlbTSZBlYSecorJsWidfALQYzOdrKNrJZRdrQEDoyhPMYAfTiHZIuqGtEkKqYBzxtCOJhRYfZNSYNxRWFrfahlSLvdBTebrXDgGlZEqxRIvGhN*mfhLLSExNHaHLAZI");
	//std::string input2("jEAmXdDUtthXNLbIZFeWdiQPGEvyCEeL**BUHYiSZRREvniDexKJSjLXMYfswlaYlbTSZBlYSecorJsWidfALQYzOdrKNrJZ*EDoyhPMYAfTiHZIuqGtEkKqYBzxtC*YfZNSYNxRWFrfahlSLvdBT*ebrXDgGlZEqxRIvGhNcmfhLLSExNHaHLAZI");

    auto result = solve(input1, input2);
    bool solution_found;
    Solution s;
    std::tie(solution_found, s) = result;

    if (solution_found) {
        std::cout << "Solution found: " << std::endl;
        std::cout << s.split1.done << std::endl;
        std::cout << s.split2.done << std::endl;
    }
    else {
        std::cout << "No solution found" << std::endl;
    }
    return 0;
}

void run_tests(void) {
    std::tuple<bool, Solution> result;
    bool solution_found;
    Solution s;    

    //sample input 1: solution "Shakespeare"
	std::string s1("Shakes*e");
    std::string s2("S*speare");
    result = solve(s1, s2);
    std::tie(solution_found, s) = result;
    assert(solution_found == true);
    assert(s.split1.done == "Shakespeare");

    // sample input 2: no solution
	std::string s3("a*baa**ba**aa");
	std::string s4("*ca*b**a*baac");
    result = solve(s3, s4);
    std::tie(solution_found, s) = result;
    assert(solution_found == false);

	// START OF CHALLENGE INPUTS

	// no solution - check
    std::string s5("bb*aaaaa*ba**");
	std::string s6("*baabb*b*aaaa");
    result = solve(s5, s6);
    std::tie(solution_found, s) = result;
    assert(solution_found == false);

    // "a*b" 
	// "*"
	
	// a*b
	// c*b

	// a*b
	// a*c

	// *
	// abcd

	// *ab
	// cd*

	// abcdefgh
	// **

    // abc**
    // abc*

    // ab*cd**
    // *def
}

//void printPartial(const Solution& s) {
//    std::cout << s.split1.done << ", " << s.split1.not_done << std::endl;
//    std::cout << s.split2.done << ", " << s.split2.not_done << std::endl;
//    std::cout << std::endl;
//}
