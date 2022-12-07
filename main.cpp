#include "data.cpp"
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <chrono>
#include <vector>

// use a vector of objects to store all data and find what is similar
// then use what is similar to make the map
// have similarity ratio (average of genre rating and year difference)
// connect if similarity reaches a certain threshold

using namespace chrono;
using namespace std;

priority_queue<Info, vector<Info>, Compare>
compareSimilarity(string title, string genre, string year, string pop, vector<Info> &v, map<string, vector<pair<string, Info>>> &graph) {
    priority_queue<Info, vector<Info>, Compare> compare;
    vector<Info> comparable;
    for (int i = 0; i < v.size(); i++) {
        if (v[i].genre == genre) { // we find the genre
            // make similarity ratio
            double popDiff = abs(stod(pop) - stod(v[i].pop));
            int yearDiff = 2 * abs(stoi(year) - stoi(v[i].year));
            v[i].rating = yearDiff + popDiff; // we need more comparables
            compare.push(v[i]); // push into priority queue
            comparable.push_back(v[i]);

            // what if we make map here...

        }
    }

    v = comparable;
    return compare;
}

void BFS(map<string, vector<pair<string, Info>>> &graph, string title) {
    string source = title;
    set<string> visited;
    queue<string> q;
    visited.insert(source);
    q.push(source);
    int qCount = 0;
    cout << "Your BFS movie recommendations: " << endl;
    while(!q.empty() && qCount <= 30) {
        string u = q.front();
        if (qCount != 0) {
            cout << u << endl;
        }

        q.pop();
        qCount++;
        vector<pair<string, Info>> neighbors = graph[u];
        for (auto v: neighbors) {
            if (visited.count(v.first) == 0) {
                visited.insert(v.first);
                q.push(v.first);

            }
        }
    }
}

void DFS(map<string, vector<pair<string, Info>>> &graph, string title){
    string source = title;
    set<string> visited;
    stack<string> s;
    visited.insert(source);
    s.push(source);
    int sCount = 0;
    cout << "Your DFS movie recommendations: " << endl;
    while (!s.empty() && sCount <= 30) {
        string u = s.top();
        if (sCount != 0) {
            cout << u << endl;
        }

        s.pop();
        sCount++;
        vector<pair<string,Info>> neighbors = graph[u];
        for (auto v: neighbors) {
            if (visited.count(v.first) == 0) {
                visited.insert(v.first);
                s.push(v.first);
            }
        }
    }
}

int main() {
    string movieTitle, movieGenre, movieYear, moviePop, search;
    Info movie;
    cout << "Enter a movie title:" << endl;
    getline(cin, movieTitle);
    cout << "BFS or DFS?" << endl;
    cin >> search;

    //cin >> movieTitle;

    fstream file;
    file.open("movie_data.csv", ios::in);
    string line = "";
    string title;
    string genre;
    string year;
    string temp;
    string pop;
    vector<Info> v;
    stack<char> delim;
    //map<char,int>::iterator it;
    map<string, vector<pair<string, Info>>> graph; // <title, vector<<title, info>>>
    file.clear();
    while (getline(file, line)) {

        // may have to use a stack to ensure that each comma is the proper delimiter
        // ; we'd need to go through the line and

        if (line.find("null") != string::npos) { // we find null
            getline(file, temp);
        }

        else {

            stringstream file(line);
            getline(file, title, '[');
            getline(file, genre, ']');
            getline(file, temp, ',');
            getline(file, year, ',');
            getline(file, pop, ',');
            getline(file, temp, ',');


            if (genre.length() == 0) {
                title = title.substr(0, title.length() - 1);
            }

            else {
                title = title.substr(0, title.length() - 2);
            }

            //cout << title << endl;

            // make this a function later on (put in data.cpp)

            Info movies;
            movies.title = title;
            movies.genre = genre;
            movies.year = year;
            movies.pop = pop;

            if (genre.length() != 0 && year.length() != 0) {
                v.push_back(movies);
            }
        }

        if (title == movieTitle) { // we find the movie entered
            movieGenre = genre;
            movieYear = year;
            moviePop = pop;
            movie.title = movieTitle;
            movie.genre = movieGenre;
            movie.year = movieYear;
            movie.pop = moviePop;
        }

        /*for (int i = 0; i < v.size(); i++) {
          cout << v[i].title << endl;
        }*/
    }

    // graph[movieTitle].push_back(make_pair())
    auto similar = compareSimilarity(movieTitle, movieGenre, movieYear, moviePop, v, graph);
    int count = 0;
    // connect the graph together
    while (!similar.empty() && count <= 50) {
        Info temp = similar.top();
        similar.pop();

        if (temp.rating <= 100) {
            graph[movieTitle].push_back(make_pair(temp.title, temp)); // attach to root (root -> temp)
            graph[temp.title].push_back(make_pair(movieTitle, movie)); //  temp -> root
            count++;
        }

        for (int i = 0; i < v.size(); i++) {
            if (temp.rating == v[i].rating && temp.title != v[i].title) {
                graph[temp.title].push_back(make_pair(v[i].title, v[i])); // edge between temp -> v[i]
                graph[v[i].title].push_back(make_pair(temp.title, temp)); // edge between v[i] -> temp
                count++;


            }

            if (temp.rating + 1 == v[i].rating) {
                graph[temp.title].push_back(make_pair(v[i].title, v[i])); // edge between temp -> v[i]
                graph[v[i].title].push_back(make_pair(temp.title, temp)); // edge between v[i] -> temp
                count++;

            }

            if (temp.rating - 1 == v[i].rating) {
                graph[temp.title].push_back(make_pair(v[i].title, v[i])); // edge between temp -> v[i]
                graph[v[i].title].push_back(make_pair(temp.title, temp)); // edge between v[i] -> temp
                count++;
            }
        }
    }

    auto it = graph.find(movieTitle);
    if (it != graph.end()) {
        if (search == "BFS") {
            auto start = high_resolution_clock::now();
            BFS(graph, movieTitle);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout << duration.count() << " microseconds" << endl;
        }

        else if (search == "DFS") {
            auto start = high_resolution_clock::now();
            DFS(graph, movieTitle);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout << duration.count() << " microseconds" << endl;
        }

        else {
            cout << "Not a valid search!";
        }
    }

    else {
        cout << "Not a valid movie title!";
    }
}