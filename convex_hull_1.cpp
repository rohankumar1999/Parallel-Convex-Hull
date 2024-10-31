#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <chrono>

using namespace std;

// Global variable to store the center point for sorting

// Function prototypes
vector<pair<long long, long long> > divide(vector<pair<long long, long long> > &a);
vector<pair<long long, long long> > merger(vector<pair<long long, long long> > &a, vector<pair<long long, long long> > &b);

// Determines the quadrant of a point
long long quad(const pair<long long, long long> &p)
{
    if (p.first >= 0 && p.second >= 0)
        return 1;
    if (p.first <= 0 && p.second >= 0)
        return 2;
    if (p.first <= 0 && p.second <= 0)
        return 3;
    return 4;
}

// Orientation function
long long orientation(const pair<long long, long long> &a, const pair<long long, long long> &b, const pair<long long, long long> &c)
{
    long long res = (b.second - a.second) * (c.first - b.first) - (c.second - b.second) * (b.first - a.first);
    if (res == 0)
        return 0;
    if (res > 0)
        return 1;
    return -1;
}

// Compare function for sorting points


// Brute force algorithm for small datasets
vector<pair<long long, long long> > bruteHull(vector<pair<long long, long long> > &a)
{
    set<pair<long long, long long> > s;
    for (long long i = 0; i < a.size(); i++)
    {
        for (long long j = i + 1; j < a.size(); j++)
        {
            long long x1 = a[i].first, x2 = a[j].first;
            long long y1 = a[i].second, y2 = a[j].second;
            long long a1 = y1 - y2;
            long long b1 = x2 - x1;
            long long c1 = x1 * y2 - y1 * x2;
            long long pos = 0, neg = 0;
            for (long long k = 0; k < a.size(); k++)
            {
                long long val = a1 * a[k].first + b1 * a[k].second + c1;
                if (val <= 0)
                    neg++;
                if (val >= 0)
                    pos++;
            }
            if (pos == a.size() || neg == a.size())
            {
                s.insert(a[i]);
                s.insert(a[j]);
            }
        }
    }
    vector<pair<long long, long long> > ret(s.begin(), s.end());
    // Sorting the points in the anti-clockwise order
    pair<long long, long long> mid = {0, 0};
    long long n = ret.size();
    for (long long i = 0; i < n; i++)
    {
        mid.first += ret[i].first;
        mid.second += ret[i].second;
        ret[i].first *= n;
        ret[i].second *= n;
    }
    // Define compare as a lambda function capturing mid
    auto compare = [mid](const pair<long long, long long> &p1, const pair<long long, long long> &q1) -> bool {
        pair<long long, long long> p = make_pair(p1.first - mid.first, p1.second - mid.second);
        pair<long long, long long> q = make_pair(q1.first - mid.first, q1.second - mid.second);
        long long one = quad(p);
        long long two = quad(q);
        if (one != two)
            return (one < two);
        return (p.second * q.first < q.second * p.first);
    };
    sort(ret.begin(), ret.end(), compare);
    for (long long i = 0; i < n; i++)
    {
        ret[i].first /= n;
        ret[i].second /= n;
    }
    return ret;
}


// Divide and conquer algorithm
vector<pair<long long, long long> > divide(vector<pair<long long, long long> > &a)
{
    if (a.size() <= 5)
        return bruteHull(a);
    vector<pair<long long, long long> > left(a.begin(), a.begin() + a.size() / 2);
    vector<pair<long long, long long> > right(a.begin() + a.size() / 2, a.end());
    vector<pair<long long, long long> > left_hull = divide(left);
    vector<pair<long long, long long> > right_hull = divide(right);
    return merger(left_hull, right_hull);
}

// Merges two convex hulls
vector<pair<long long, long long> > merger(vector<pair<long long, long long> > &a, vector<pair<long long, long long> > &b)
{
    long long n1 = a.size(), n2 = b.size();
    long long ia = 0, ib = 0;
    for (long long i = 1; i < n1; i++)
        if (a[i].first > a[ia].first)
            ia = i;
    for (long long i = 1; i < n2; i++)
        if (b[i].first < b[ib].first)
            ib = i;
    long long inda = ia, indb = ib;
    bool done = false;
    while (!done)
    {
        //cout<<"here 1"<<endl;
        done = true;
        while (orientation(b[indb], a[inda], a[(inda + 1) % n1]) >= 0){

            //cout<<"here 2"<<endl;
            inda = (inda + 1) % n1;
        }
        while (orientation(a[inda], b[indb], b[(n2 + indb - 1) % n2]) <= 0)
        {
            //cout<<"here 3"<<endl;
            indb = (n2 + indb - 1) % n2;
            done = false;
        }
    }
    long long uppera = inda, upperb = indb;
    inda = ia, indb = ib;
    done = false;
    while (!done)
    {
            //cout<<"here 6"<<endl;
        done = true;
        while (orientation(a[inda], b[indb], b[(indb + 1) % n2]) >= 0){
            //cout<<"here 4"<<endl;
            indb = (indb + 1) % n2;

        }
        while (orientation(b[indb], a[inda], a[(n1 + inda - 1) % n1]) <= 0)
        {
            //cout<<"here 5"<<endl;
            inda = (n1 + inda - 1) % n1;
            done = false;
        }
    }
    long long lowera = inda, lowerb = indb;
    vector<pair<long long, long long> > ret;
    long long ind = uppera;
    ret.push_back(a[uppera]);
    while (ind != lowera)
    {
            //cout<<"here 7"<<endl;
        ind = (ind + 1) % n1;
        ret.push_back(a[ind]);
    }
    ind = lowerb;
    ret.push_back(b[lowerb]);
    while (ind != upperb)
    {
            //cout<<"here 8"<<endl;
        ind = (ind + 1) % n2;
        ret.push_back(b[ind]);
    }
    return ret;
}

// Function to partition points into clusters
void partitionPoints(const vector<pair<long long, long long> > &points, vector<vector<pair<long long, long long> > > &clusters, long long numClusters)
{
    // Determine the minimum and maximum x-values
    long long minX = points[0].first, maxX = points[0].first;
    for (const auto &p : points)
    {
        if (p.first < minX)
            minX = p.first;
        if (p.first > maxX)
            maxX = p.first;
    }
    long long interval = (maxX - minX) / numClusters;
    if (interval == 0)
        interval = 1;
    clusters.resize(numClusters);
    // Assign points to clusters based on x-value
    for (const auto &p : points)
    {
        long long idx = (p.first - minX) / interval;
        if (idx >= numClusters)
            idx = numClusters - 1; // Ensure index is within bounds
        clusters[idx].push_back(p);
    }
}

// Main function
int main()
{
    // Number of clusters
    long long numClusters = 1;
    // Generate random points
    // //cout<<"Num threads: "<<num_threads<<endl;
    long long numPoints = 1280000;
    vector<pair<long long, long long> > points(numPoints);
    srand(time(0));
    for (long long i = 0; i < numPoints; ++i)
    {
        long long x = i;
        long long y = x * x; // y = x^2
        points[i] = std::make_pair(x, y);
    }
    sort(points.begin(), points.end());
    auto start_time = std::chrono::high_resolution_clock::now();
    // Partition points into clusters
    vector<vector<pair<long long, long long> > > clusters;
    partitionPoints(points, clusters, numClusters);
    // Compute convex hulls for each cluster in parallel
    vector<vector<pair<long long, long long> > > hulls(numClusters);
    #pragma omp parallel for
    for (long long i = 0; i < numClusters; i++)
    {
        if (clusters[i].size() > 0)
        {
            // Sort the points in the cluster
            // Compute convex hull for the cluster
            hulls[i] = divide(clusters[i]);
        }
    }
    //cout<<"here1"<<endl;
    // Merge all hulls into a final convex hull
    vector<pair<long long, long long> > finalHull = hulls[0];
    for (long long i = 1; i < numClusters; i++)
    {
        if (hulls[i].size() > 0)
            finalHull = merger(finalHull, hulls[i]);
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    // Compute the duration
    std::chrono::duration<double> elapsed = end_time - start_time;

    cout << "Total time taken: " << elapsed.count() << " seconds." << endl;
    // Output the result
    cout << "Final Convex Hull has " << finalHull.size() << " points." << endl;
    for (const auto &p : finalHull)
    {
        //cout << "(" << p.first << ", " << p.second << ")" << endl;
    }
    return 0;
}

