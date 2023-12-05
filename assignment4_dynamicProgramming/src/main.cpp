#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include <cmath> 

using namespace std;
#define MAX_STRLEN 1000

string merge(const string &a, const string &b, const string &c){

    /*
    In order to check whether c is a valid merge of a and b, we will convert this into a path finding problem.
    String c is formed by sequentially popping the first letter of string a and string b until no letters remain.
    This means that the length of c can only equal to the sum of lengths of a and b, and that the options for the nth 
    letter of c must come from the remaining letters after i letters from a and j letters from b. ie, if we've taken 1 letter from
    a (i=1) and we are on letter 3 of c(n=3), c[3] must either be b[2] or a[1]. in other words, n=i+j.
    
    To convert this to a path finding problem, let the columns of matrix M = ith letter of a, and the rows be the jth letter of b.
    Corresponding to the insight above, we can traverse through M by either taking a letter from a, moving rightwards, or taking a letter from 
    b, moving downwards. A valid step in M is when c[i+j] == a[i] or b[j]. Using dynamic programming, we can find a valid path through M using 
    the rules described above; if a valid merge exists, we can traverse back up the path to find the sequence of pops corresponding to the merge c.
    */

    //First check is if the number have sufficient numbers to be a valid merge
    if ((int)a.length()+ (int)b.length() != (int)c.length()){
        return "*** NOT A MERGE *** ";
    }
    
    //If a merge is possible, start the algorithm to see if c is a valid merge
    //Initialize matrix of size 1001x1001, which will fit the largest possible inputs for a and b of 1000 chars each
    static int matrix[MAX_STRLEN + 1][MAX_STRLEN + 1];

    // A valid path through the matrix must start from the top left corner a[1] or b[1], and end at the bottom
    // right with a[i] or b[j]. thereore we can stop the initialization at the first instance  where a[i] or b[j] == c[i+j] when
    // iterating through just a or b. 
    
    matrix[0][0] = -1;

    //Initial comparison of elements of a and b with elements of c. (note, strings are indexed to 1)
    for(int i=0; i<(int)a.length(); i++){
        if(c[i] != a[i]){
            matrix[0][i+1] = -1;
        }
        else{
            matrix[0][i+1] = matrix[0][i]+1;
        }
   
    }

    for(int j=0; j < (int)b.length(); j++){
        if(c[j] != b[j]){
            matrix[j+1][0] = -1;
        }
        else{
            matrix[j+1][0] = matrix[j][0]+1;
        }
    }

    // Iterating through every element of m, if either a step down is valid, or a step right is valid, we will remember
    // this by adding 1 to the element corresponding to the step direction, (left if letter from a, up if letter from b). 
    // the order in which we compare c to a and b will be maintained in the traversal back up the path.
    
    //iterating through every remaining element of the matrix starting from row 1, col 1. 

    for (int i=1; i < (int)a.length()+1; i++){
        for(int j=1; j<(int)b.length()+1; j++){
            //up operation, compare to see if c[n] can be taken from b
            if(c[i+j-1] == b[j-1]){
                matrix[j][i] = matrix[j-1][i]+1;
            }
            //left operation, compare to see if c[n] can be taken from a
            else if(c[i+j-1] == a[i-1]){
                matrix[j][i] = matrix[j][i-1]+1;
            }
            //no path exists
            else{
                matrix[j][i] = -1;
            }
        }
    }

    int apos = (int)a.length();
	int bpos = (int)b.length();

    //If no path exists 
    if(matrix[bpos][apos] != apos+bpos-1){
        return "*** NOT A MERGE *** ";
    }

	string output;

	// traversing back up the path
    int n = apos + bpos-1; 

	while (apos && bpos) {
		if (matrix[bpos - 1][apos] == n-1) {
			output.push_back(b[--bpos]);
            n--;
			continue;
		}
		if (matrix[bpos][apos - 1] == n-1) {
			output.push_back(toupper(a[--apos]));
            n--;
			continue;
		}
        else{
            //this should never be triggered, but for debugging this will show where the fault happened
            cout<< apos<< ' '<< bpos << endl;
            cout<< matrix[bpos][apos] << endl;
            cout<< matrix[bpos - 1][apos]<< ' '<< matrix[bpos][apos - 1] << endl;
            break;
        }
	}

	// finish up
	while (apos) output.push_back(toupper(a[--apos]));
	while (bpos) output.push_back(b[--bpos]);

	// inverting output
	reverse(output.begin(), output.end());

	return output;
}

int main()
{
	string path;

	cout << "path to input file: ";
	cin  >> path;

	ifstream ifile;
	ofstream ofile;

	ifile.open(path);
	if (!ifile) {
		cerr << path << ": could not be opened\n";
		exit(EXIT_FAILURE);
	}

	cout << "path to output file: ";
	cin  >> path;

	ofile.open(path);
	if (!ofile) {
		cerr << path << ": could not be opened\n";
		exit(EXIT_FAILURE);
	}

	string a;
	string b;
	string c;

	while (true) {
		if (!getline(ifile, a)) break;
		if (!getline(ifile, b)) break;
		if (!getline(ifile, c)) break;

		ofile << merge(a, b, c) << '\n';
	}

	ifile.close();
	ofile.close();

	return EXIT_SUCCESS;
}
