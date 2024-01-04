//
// Created by nk024576 on 12/14/2023.
//
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include "src/List/Graph/Graph.h"

using namespace list;
using namespace std;
struct wordIndex {
    string word;
    int index;
};

//linear search to find index of a word.
int findIndex(const vector<wordIndex> &wordIndices, const string &word) {
    for (int i = 0; i < wordIndices.size(); i++) {
        if (wordIndices[i].word == word) {
            return wordIndices[i].index;
        }
    }
    return -1;

}

//function to check if a word is valid
bool isValidWord(const string &word) {
    if (word.length() < 3 || word.length() > 5) return false;
    for (char c: word) {
        if (!isalpha(c))
            return false;
    }
    return true;
}

//function to check if two words are adjacent
bool areAdjacent(const string &word1, const string &word2) {
    if (word1.length() != word2.length()) return false;
    int count = 0;
    for (size_t i = 0; i < word1.size(); i++) {
        if (word1[i] != word2[i]) {
            if (++count > 1) {
                return false;
            }
        }
    }
    return count == 1;
}

// function to add edges between all adjacent words in the graph.
void addEdges(Graph &graph, const vector<string> &words, const vector<wordIndex> &wordIndices) {
    for (size_t i = 0; i < words.size(); i++) {
        for (size_t j = i + 1; j < words.size(); j++) {
            if (areAdjacent(words[i], words[j])) {
                int index1 = findIndex(wordIndices, words[i]);
                int index2 = findIndex(wordIndices, words[j]);
                if (index1 != -1 && index2 != -1) {
                    graph.addEdge(index1, index2);
                    graph.addEdge(index2, index1); // for undirected graph
                }
            }
        }
    }

}
//bundan da emin değilim.
void reverseVector(vector<int> &v) {

    int i = 0;
    int j = v.size() - 1;
    while (i < j) {
        int temp = v[i];
        v[i] = v[j];
        v[j] = temp;
        i++;
        j--;
    }
}

//bfs function
//This algorithm was chosen for its efficiency in
//exploring and traversing the established graph structures.
vector<int> BFS(Graph &graph, const vector<string> words, int start, int target) {
    queue<int> q;
    vector<bool> visited(graph.getVertexCount(), false);
    vector<int> prev(graph.getVertexCount(), -1);
    visited[start] = true;
    q.push(start);
    cout << "starting from bfs:" << words[start] << endl;
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        cout << "visiting:" << words[current] << endl;
        if (current == target) {
            cout << "reached target:" << words[target] << endl;
            break;
        }

        //bu kısımda patlıyor hatırlamıyorum :((((((((
        for (Edge *edge = graph.getEdges()[current].getHead(); edge = nullptr; edge = edge->getNext()) {
            int neighbor = edge->getTo();
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                prev[neighbor] = current;
                q.push(neighbor);
            }

        }
    }
    }
    //bu kısımda yanlış olabilir.
    vector<int> path;
    for(int at = target; at != -1; at = prev[at]) {
        path.push_back(at);
}
        if (path.size() == 1 && path[0] = start) {
            return {};
        }
        return path;
    }


int main() {
    ifstream file("C:\\Users\\nk024576\\CLionProjects\\graph-puzzle-game-iremkatman\\dictionary");
    string line;
    vector<string> words3, words4, words5;
    vector<wordIndex> wordIndex3, wordIndex4, wordIndex5;
    int index3 = 0, index4 = 0, index5 = 0;

    while (getline(file, line)) {
        if (!isValidWord(line)) continue;
        if (line.length() == 3) {
            words3.push_back(line);
            wordIndex3.push_back({line, index3++});
        } else if (line.length() == 4) {
            words4.push_back(line);
            wordIndex4.push_back({line, index4++});
        } else if (line.length() == 5) {
            words5.push_back(line);
            wordIndex5.push_back({line, index5++});
        }
    }

    Graph graph3(words3.size()), graph4(words4.size()), graph5(words5.size());

    // Modify addEdges function to take vector<WordIndex> instead of unordered_map
    addEdges(graph3, words3, wordIndex3);
    addEdges(graph4, words4, wordIndex4);
    addEdges(graph5, words5, wordIndex5);

    string startWord, targetWord;
    cout << "Enter the start word: ";
    cin >> startWord;
    cout << "Enter the target word: ";
    cin >> targetWord;

    if (!isValidWord(startWord) || !isValidWord(targetWord) || startWord.length() != targetWord.length()) {
        cout << "Error: Invalid start or target word." << endl;
        return 1;
    }

    vector<int> transformationSequence;
    vector<string>* selectedWords = nullptr;
    int startIdx, targetIdx;

    if (startWord.length() == 3) {
        startIdx = findIndex(wordIndex3, startWord);
        targetIdx = findIndex(wordIndex3, targetWord);
        selectedWords = &words3;
    } else if (startWord.length() == 4) {
        startIdx = findIndex(wordIndex4, startWord);
        targetIdx = findIndex(wordIndex4, targetWord);
        selectedWords = &words4;
    } else if (startWord.length() == 5) {
        startIdx = findIndex(wordIndex5, startWord);
        targetIdx = findIndex(wordIndex5, targetWord);
        selectedWords = &words5;
    }

    if (selectedWords != nullptr && startIdx != -1 && targetIdx != -1) {
        if (startWord.length() == 3) {
            transformationSequence = BFS(graph3, *selectedWords, startIdx, targetIdx);
        } else if (startWord.length() == 4) {
            transformationSequence = BFS(graph4, *selectedWords, startIdx, targetIdx);
        } else if (startWord.length() == 5) {
            transformationSequence = BFS(graph5, *selectedWords, startIdx, targetIdx);
        }
    }

    if (transformationSequence.empty()) {
        cout << "No transformation sequence found." << endl;
    } else {
        for (int nodeId : transformationSequence) {
            cout << (*selectedWords)[nodeId] << " -> ";
        }
        cout << "end" << endl;
    }

    return 0;
}