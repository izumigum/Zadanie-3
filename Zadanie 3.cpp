#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int INF = 1000000;

struct Flight {
    int distance;
    int time;
    int cost;
};

class Graph {
private:
    vector<string> cities;
    vector<vector<Flight>> adjMatrix;

    int findCityIndex(const string& city) {
        for (int i = 0; i < cities.size(); i++) {
            if (cities[i] == city) return i;
        }
        return -1;
    }

public:
    void addCity(const string& city) {
        if (findCityIndex(city) != -1) {
            cout << "Город уже существует!" << endl;
            return;
        }
        cities.push_back(city);
        int newSize = cities.size();
        adjMatrix.resize(newSize, vector<Flight>(newSize, { INF, INF, INF }));
    }

    void addFlight(const string& city1, const string& city2, int distance, int time, int cost) {
        int index1 = findCityIndex(city1);
        int index2 = findCityIndex(city2);
        if (index1 == -1 || index2 == -1) {
            cout << "Один из городов не найден!" << endl;
            return;
        }
        adjMatrix[index1][index2] = { distance, time, cost };
    }

    void inputGraph() {
        int numCities, numFlights;
        cout << "Введите количество городов: ";
        cin >> numCities;
        cin.ignore();
        for (int i = 0; i < numCities; i++) {
            string city;
            cout << "Введите название города: ";
            getline(cin, city);
            addCity(city);
        }
        cout << "Введите количество рейсов: ";
        cin >> numFlights;
        cin.ignore();
        for (int i = 0; i < numFlights; i++) {
            string city1, city2;
            int distance, time, cost;
            cout << "Введите город отправления, город прибытия, расстояние, время и стоимость перелета: ";
            getline(cin, city1, ' ');
            getline(cin, city2, ' ');
            cin >> distance >> time >> cost;
            cin.ignore();
            addFlight(city1, city2, distance, time, cost);
        }
    }

    void dijkstra(const string& startCity, vector<int>& dist, vector<int>& prev, int weightType) {
        int n = cities.size();
        dist.resize(n, INF);
        prev.resize(n, -1);
        vector<bool> visited(n, false);
        int startIndex = findCityIndex(startCity);
        if (startIndex == -1) {
            cout << "Город не найден!" << endl;
            return;
        }
        dist[startIndex] = 0;
        for (int i = 0; i < n; i++) {
            int u = -1;
            for (int j = 0; j < n; j++) {
                if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                    u = j;
                }
            }
            if (dist[u] == INF) break;
            visited[u] = true;
            for (int v = 0; v < n; v++) {
                int weight;
                switch (weightType) {
                case 0: weight = adjMatrix[u][v].time; break;
                case 1: weight = adjMatrix[u][v].cost; break;
                default: weight = INF; break;
                }
                if (weight < INF && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                }
            }
        }
    }

    void findRouteByTime(const string& startCity, const string& endCity) {
        vector<int> dist, prev;
        dijkstra(startCity, dist, prev, 0);
        int endIndex = findCityIndex(endCity);
        if (dist[endIndex] == INF) {
            cout << "Маршрут не найден" << endl;
            return;
        }
        vector<string> path;
        for (int v = endIndex; v != -1; v = prev[v]) {
            path.insert(path.begin(), cities[v]);
        }
        cout << "Маршрут по минимальному времени: ";
        for (const string& city : path) {
            cout << city << " ";
        }
        cout << "время: " << dist[endIndex] << endl;
    }

    void findRouteByCost(const string& startCity, const string& endCity) {
        vector<int> dist, prev;
        dijkstra(startCity, dist, prev, 1);
        int endIndex = findCityIndex(endCity);
        if (dist[endIndex] == INF) {
            cout << "Маршрут не найден" << endl;
            return;
        }
        vector<string> path;
        for (int v = endIndex; v != -1; v = prev[v]) {
            path.insert(path.begin(), cities[v]);
        }
        cout << "Маршрут по минимальной стоимости: ";
        for (const string& city : path) {
            cout << city << " ";
        }
        cout << "стоимость: " << dist[endIndex] << endl;
    }

    void findCitiesWithinBudget(const string& startCity, int budget) {
        vector<int> dist, prev;
        dijkstra(startCity, dist, prev, 1);
        cout << "Города, до которых можно долететь с бюджетом " << budget << ": ";
        for (int i = 0; i < cities.size(); i++) {
            if (dist[i] <= budget && i != findCityIndex(startCity)) {
                cout << cities[i] << " ";
            }
        }
        cout << endl;
    }

    void findCitiesWithinTime(const string& startCity, int time) {
        vector<int> dist, prev;
        dijkstra(startCity, dist, prev, 0);
        cout << "Города, до которых можно долететь за время " << time << ": ";
        for (int i = 0; i < cities.size(); i++) {
            if (dist[i] <= time && i != findCityIndex(startCity)) {
                cout << cities[i] << " ";
            }
        }
        cout << endl;
    }

    void checkRouteWithMaxStops(const string& startCity, const string& endCity, int maxStops) {
        vector<int> dist, prev;
        dijkstra(startCity, dist, prev, 0);
        int endIndex = findCityIndex(endCity);
        int stops = 0;
        for (int v = endIndex; v != -1; v = prev[v]) {
            stops++;
        }
        if (stops - 1 <= maxStops) {
            cout << "Можно добраться с не более чем " << maxStops << " пересадками" << endl;
        }
        else {
            cout << "Не удается добраться с " << maxStops << " пересадками" << endl;
        }
    }

    void maxDistanceWithBudget(const string& startCity, int budget) {
        vector<int> dist, prev;
        dijkstra(startCity, dist, prev, 1);
        int maxDistance = 0;
        for (int i = 0; i < cities.size(); i++) {
            if (dist[i] <= budget && adjMatrix[findCityIndex(startCity)][i].distance < INF) {
                maxDistance = max(maxDistance, adjMatrix[findCityIndex(startCity)][i].distance);
            }
        }
        cout << "Максимальное расстояние: " << maxDistance << " при бюджете " << budget << endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    Graph g;
    g.inputGraph();
    string startCity, endCity;
    cout << "Введите начальный и конечный города для поиска маршрута по времени: ";
    cin >> startCity >> endCity;
    g.findRouteByTime(startCity, endCity);

    cout << "Введите начальный и конечный города для поиска маршрута по стоимости: ";
    cin >> startCity >> endCity;
    g.findRouteByCost(startCity, endCity);

    int budget;
    cout << "Введите начальный город и бюджет для поиска городов: ";
    cin >> startCity >> budget;
    g.findCitiesWithinBudget(startCity, budget);

    int time;
    cout << "Введите начальный город и время для поиска городов: ";
    cin >> startCity >> time;
    g.findCitiesWithinTime(startCity, time);

    int maxStops;
    cout << "Введите начальный и конечный города для проверки маршрута с пересадками и максимальное количество пересадок: ";
    cin >> startCity >> endCity >> maxStops;
    g.checkRouteWithMaxStops(startCity, endCity, maxStops);

    cout << "Введите начальный город и бюджет для поиска максимального расстояния: ";
    cin >> startCity >> budget;
    g.maxDistanceWithBudget(startCity, budget);

    return 0;
}
