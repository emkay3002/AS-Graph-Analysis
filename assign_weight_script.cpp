/*#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <set>
#include <sstream>
using namespace std;

int main()
{
    ifstream infile("C:\\Users\\Eman Khalid\\OneDrive\\Desktop\\algo_project2\\as20000102.txt");

    // Raw dataset file
    if (!infile)
    {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    // Output file to store weighted edges
    // The output file will contain edges with weights
    // The format of each line will be: u v w
    ofstream outfile("C:\\Users\\Eman Khalid\\OneDrive\\Desktop\\algo_project2\\weighted_edges.txt");
    set<pair<int, int>> added;
    int u, v;
    string line;

    srand(time(0)); // Seed for random numbers

    // Skip comment lines
    while (getline(infile, line))
    {
        if (line[0] == '#')
        {
            continue; // Skip lines that start with '#'
        }

        stringstream ss(line);
        ss >> u >> v; // Assuming tab or space-separated

        if (u == v)
            continue; // Skip self-loops

        int w = rand() % 10 + 1; // Random weight [1, 10]

        // Avoid duplicate undirected edges
        if (added.count({u, v}) == 0 && added.count({v, u}) == 0)
        {
            outfile << u << " " << v << " " << w << "\n";
            added.insert({u, v});
        }
    }

    infile.close();
    outfile.close();
    cout << "Weighted edges written to weighted_edges.txt\n";
    return 0;
}
*/