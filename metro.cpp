#include <bits/stdc++.h>
#include <fstream>
#define ll long long
#define pb push_back
#define fi first
#define se second
#define mp make_pair
using namespace std;

map<string, ll> M;
char color[200][200] = {'\0'};

class comparedis
{
public:
    bool operator()(pair<ll, ll> &p, pair<ll, ll> &q)
    {
        return (p.se > q.se); // For min heap use > sign
    }
};

vector<pair<ll, ll>> v[100010]; // Adjacency matrix
ll N;                             // N is the number of vertices, M is edges
string station[200];
map<string, string> tourm;

void recharge()
{
    fstream f;
    ll amt, ini, cid, fin, x;
    ll c_id, amount;
    f.open("paisa.txt", ios::in | ios::out);
    if (!f)
    {
        cout << "Not Found\n"
             << endl;
        return;
    }
    f.seekg(0);
    cout << endl;
    cout << "Enter Card Id\n";
    cin >> c_id;
    cout << "Enter Amount\n";
    cin >> amount;
    f.clear();
    while (!f.eof())
    {
        ini = f.tellg();
        f.ignore();
        f >> cid;
        f >> amt;
        fin = f.tellg();
        if (cid == c_id)
        {
            x = amt + amount;
            f.seekg(ini);
            f << endl << cid << endl << x;
            cout << "Recharge Details\n";
            cout << "\nCard Id: " << cid << endl;
            cout << "Initial Balance: " << amt << endl;
            cout << "Recharge Amount: " << amount << endl;
            cout << "Total Balance: " << x << endl;
            break;
        }
    }
    f.close();
}

void gettour()
{
    ifstream fin;
    string s1, s2;
    fin.open("tourplace.txt", ios::in);
    if (!fin)
    {
        cout << "Not Found\n";
        return;
    }
    fin.seekg(0);
    fin.clear();
    while (getline(fin, s1) && getline(fin, s2))
    {
        tourm[s1] = s2;
    }
    fin.close();
}

// Given below code will print the path
void disp(ll src, ll dest, ll par[])
{
    ll i, x, y, cn = 0, ci = 0;
    stack<ll> st;
    st.push(dest);
    i = dest;
    while (par[i] != -1)
    {
        i = par[i];
        st.push(i);
    }
    char col = '\0';
    while (!st.empty())
    {
        x = st.top();
        st.pop();
        if (!st.empty())
            y = st.top();
        cout << station[x] << " ";
        cn++;
        if (col == '\0')
            col = color[x][y];
        else if (col != '\0' && col != color[x][y])
        {
            char c = color[x][y];
            ci++;
            if (c == 'b')
                cout << "\t\tChange to blue line";
            else if (c == 'y')
                cout << "\t\tChange to yellow line";
            else if (c == 'o')
                cout << "\t\tChange to orange line";
            else if (c == 'g')
                cout << "\t\tChange to green line";
            else if (c == 'r')
                cout << "\t\tChange to red line";
            else if (c == 'v')
                cout << "\t\tChange to Violet line";
            col = c;
        }
        cout << endl;
    }
    cout << endl
         << "No of stations =" << cn << endl;
    cout << "No of interchange stations =" << ci - 1 << endl;
    cout << endl;
}

// To find the shortest path
void bfs(ll src, ll dest)
{
    bool vis[100010] = {false};
    ll par[100010];
    for (ll i = 0; i < N; i++)
        par[i] = -1;
    queue<ll> q;
    q.push(src);
    vis[src] = true;
    while (!q.empty())
    {
        ll x = q.front();
        q.pop();
        ll vsz = v[x].size();
        for (ll i = 0; i < vsz; i++)
        {
            ll y = v[x][i].fi;
            if (!vis[y])
            {
                par[y] = x;
                vis[y] = true;
                q.push(y);
            }
        }
        v[x].clear();
    }
    disp(src, dest, par);
}

// To find the most economical path
void dijkstra(ll src, ll dest)
{
    bool vis[100010] = {false};
    ll dist[100010], par[100010];
    for (ll i = 0; i < N; i++)
    {
        dist[i] = LLONG_MAX;
        par[i] = -1;
    }
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, comparedis> pq;
    pq.push(mp(src, 0));
    dist[src] = 0;
    par[src] = -1;
    vis[src] = true;
    while (!pq.empty())
    {
        pair<ll, ll> k = pq.top();
        pq.pop();
        ll x = k.fi;
        ll vsz = v[x].size();
        for (ll i = 0; i < vsz; i++)
        {
            ll y = v[x][i].fi;
            ll w = v[x][i].se;
            if (dist[x] + w < dist[y])
            {
                par[y] = x;
                dist[y] = dist[x] + w;
            }
            if (!vis[y])
            {
                vis[y] = true;
                pq.push(mp(y, dist[y]));
            }
        }
        v[x].clear();
    }
    disp(src, dest, par);
}

void consmap() // To assign values to metro stations
{
    ifstream fin;
    string s;
    fin.open("list.txt", ios::in);
    ll l = 0;
    fin.seekg(0);
    fin.clear();
    while (getline(fin, s))
    {
        M[s] = l;
        station[l] = s;
        l++;
    }
    N = l;
    fin.close();
}

void addedge(const string &s, ll t)
{
    ll sz = s.size();
    for (ll i = 0; i < sz - 1; i++)
    {
        string temp = "";
        while (i < sz - 1 && s[i] != ' ')
        {
            temp += s[i];
            i++;
        }
        temp += s[i];
        if (i < sz - 1 && s[i + 1] != ' ')
        {
            i++;
            temp += s[i];
        }
        ll k = M[temp];
        if (i < sz - 1 && s[i + 1] != ' ')
        {
            i++;
            temp = s[i];
            i++;
            temp += s[i];
            k = M[temp];
        }
        v[t].pb(mp(k, 1));
    }
}

int main()
{
    gettour();
    consmap();
    ifstream fin;
    string s;
    fin.open("metro.txt", ios::in);
    if (!fin)
    {
        cout << "Not Found\n";
        return 0;
    }
    fin.seekg(0);
    fin.clear();
    while (getline(fin, s))
    {
        if (s == "BL" || s == "YL" || s == "OL" || s == "GL" || s == "RL" || s == "VL" || s == "BW")
        {
            getline(fin, s);
            ll k = M[s];
            addedge(s, k);
            while (getline(fin, s) && !s.empty())
            {
                addedge(s, k);
            }
        }
    }
    while (1)
    {
        ll choice, cid, x, y;
        cout << "\n***** Metro Rail Route Finder *****\n";
        cout << "1. Find Shortest Path\n";
        cout << "2. Find Most Economical Path\n";
        cout << "3. Recharge Metro Card\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "Enter Source and Destination Stations: ";
            cin >> x >> y;
            cout << "Shortest Path: ";
            bfs(x, y);
            break;
        case 2:
            cout << "Enter Source and Destination Stations: ";
            cin >> x >> y;
            cout << "Most Economical Path: ";
            dijkstra(x, y);
            break;
        case 3:
            recharge();
            break;
        case 4:
            exit(0);
            break;
        default:
            cout << "Invalid choice! Please enter a valid option." << endl;
            break;
        }
    }
    return 0;
}
