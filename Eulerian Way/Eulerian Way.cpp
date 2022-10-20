#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <stack>

/*============================================================================================================*/

int numOfVertexes;

std::vector <std::vector <int> > adjacencyMatrix;

std::stack <int> resultCycle;

/*============================================================================================================*/

std::vector <std::vector <int> > inputAdjacencyMatrixFromFile();

std::vector <bool> checkingForConnectivity(int vertex);

bool returnConnectivity(std::vector <bool> visitedVertexes);

void writeToOutputFile(int flag);

void findEulerianCycle(std::vector <std::vector <int> > adjacencyMatrix, std::stack <int> &resultCycle);

void dfs(int vertex, std::vector <std::vector <int> >& adjacencyMatrix, std::stack <int>& resultCycle);

/*============================================================================================================*/

int main()
{
    adjacencyMatrix = inputAdjacencyMatrixFromFile();
    std::vector<bool> visitedVertexes = checkingForConnectivity(0);
    if (returnConnectivity(visitedVertexes))
    {
        findEulerianCycle(adjacencyMatrix, resultCycle);
            writeToOutputFile(1);
    }
    else
        writeToOutputFile(0);
}

std::vector <std::vector <int> > inputAdjacencyMatrixFromFile()
{
    std::ifstream reader("input.txt");
    if (reader.is_open())
    {
        reader >> numOfVertexes;
        std::vector < std::vector <int > > adjacencyMatrix(numOfVertexes, std::vector <int>(numOfVertexes));
        for (int i = 0; i < numOfVertexes; ++i)
            for (int j = 0; j < numOfVertexes; ++j)
                if (!reader.eof())
                    reader >> adjacencyMatrix[i][j];
                else
                    break;
        reader.close();
        return adjacencyMatrix;
    }
    else
    {
        reader.close();
        std::cout << "File Alert!";
        std::exit(0);
    }
}

std::vector<bool> checkingForConnectivity(int vertex)
{
    std::queue <int> lookingVertexes;
    lookingVertexes.push(vertex);
    std::vector<bool> visitedVertexes(numOfVertexes, false);
    visitedVertexes[vertex] = true;
    while (!lookingVertexes.empty())
    {
        int currentVertex = lookingVertexes.front();
        lookingVertexes.pop();
        visitedVertexes[currentVertex] = true;
        for (size_t i = 0; i < adjacencyMatrix[vertex].size(); ++i)
            if (!visitedVertexes[i] && adjacencyMatrix[currentVertex][i])
                lookingVertexes.push(i);
    }
    return visitedVertexes;
}

bool returnConnectivity(std::vector<bool> visitedVertexes)
{
    bool result = all_of(visitedVertexes.begin(), visitedVertexes.end(), [](bool value) {return value; });
    return result;
}

void writeToOutputFile(int flag)
{
    switch (flag)
    {
        case 0:
        {
            std::ofstream writer;
            writer.open("output.txt");
            if (writer.is_open())
            {
                writer << "The graph is not connected!\n";
                writer.close();
            }
            else
            {
                writer.close();
                std::cout << "File Alert!\n";
            }
        }
        case 1:
        {
            std::ofstream writer;
            writer.open("output.txt");
            if (writer.is_open())
            {
                writer << "Graph is Eulerian. The path through the graph has the form:\n";
                int size = resultCycle.size();
                for (int i = 0; i < size; ++i)
                {
                    writer << resultCycle.top() << " ";
                    resultCycle.pop();
                }
                writer.close();
            }
            else
            {
                writer.close();
                std::cout << "File Alert!\n";
            }
        }
        case 2:
        {
            std::ofstream writer;
            writer.open("output.txt");
            if (writer.is_open())
            {
                writer << "There is no Eulerian cycle in the graph\n";
                writer.close();
            }
            else
            {
                writer.close();
                std::cout << "File Alert!\n";
            }
        }
    }
}

void findEulerianCycle(std::vector <std::vector <int> > adjacencyMatrix, std::stack <int>& resultCycle)
{
    std::vector <int> degrees(numOfVertexes, 0);

    for (int i = 0; i < numOfVertexes; ++i)
        for (int j = 0; j < numOfVertexes; ++j)
            if (adjacencyMatrix[i][j])
                ++degrees[i];

    int countNotEvenVertexes = 0, startVertex = 0;

    for (int i = 0; i < numOfVertexes; ++i)
        if (degrees[i] & 1)
        {
            ++countNotEvenVertexes;
            if (countNotEvenVertexes > 2)
            {
                writeToOutputFile(2);
                exit(1);
            }
            startVertex = i;
        }

    dfs(startVertex, adjacencyMatrix, resultCycle);
}

void dfs(int startVertex, std::vector <std::vector <int> >& adjacencyMatrix, std::stack <int>& resultCycle)
{
    for (int i = 0; i < numOfVertexes; ++i)
        if (adjacencyMatrix[startVertex][i])
        {
            adjacencyMatrix[startVertex][i] = 0;
            adjacencyMatrix[i][startVertex] = 0;
            dfs(i, adjacencyMatrix, resultCycle);
        }
    
    resultCycle.push(startVertex);
}