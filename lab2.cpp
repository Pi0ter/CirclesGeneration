#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <fstream>
#include <map>
#include <iterator>


#define PI 3.1415
using namespace std;

struct point {
    int x;
    double counter;
    double avg;
};

struct circle {
    double x, y;

    double distanceFrom(double x, double y) {
        double dist = sqrt(pow(x - this->x, 2) + pow(y - this->y, 2));
        return dist;
    }
    circle( double x, double y):x(x), y(y) {}

};

class Rectangle {    
    double x1 = 0, y1 = 0;
    double x2, y2;
    double area;
    int numberOfIntervals = 0;

    size_t numberOfCircles;
    double circlesRadius;
    vector<circle> circles;
    map<int, vector<circle>> mapOfCircles;

    double getX1() { return x1; };
    double getX2() { return x2; };
    double getY1() { return y1; };
    double getY2() { return y2; };   


public:
    
    bool isNeighbourWhithoutTrespassing(circle a,double x, double y) {
        if (a.distanceFrom(x, y) < circlesRadius) {
            return false;
        }
        else {
            return true;
        }
    }

    void generateCircles() {
        double minX, minY;
        double maxX, maxY;
        minX = x1 + circlesRadius / 2;
        minY = y1 + circlesRadius / 2;
        maxX = x2 - circlesRadius / 2;
        maxY = y2 - circlesRadius / 2;


        srand(time(NULL));
        for (int i = 0; i < numberOfCircles; i++) {
            bool clear=true;
            double x,y;
            int  interval;
            
            x = minX + (double)(rand()) / (double)(RAND_MAX/(maxX - minX));
            y = minY + (double)(rand()) / (double)(RAND_MAX/(maxY - minY));            
            interval = floor(x / circlesRadius);
            
            for (auto crcl : mapOfCircles[interval -1]) {                
                if (!isNeighbourWhithoutTrespassing(crcl,x,y)) {
                    clear = false;                    
                }                
            }
            for (auto crcl : mapOfCircles[interval]) {
                if (!isNeighbourWhithoutTrespassing(crcl, x, y)) {
                    clear = false;
                }
            }
            for (auto crcl : mapOfCircles[interval + 1]) {                
                if (!isNeighbourWhithoutTrespassing(crcl, x, y)) {
                    clear = false;                
                }
            }
            if (clear == true) {
                circle tempCircle(x, y);                 
                mapOfCircles[interval].push_back(tempCircle);                   
            }
            else {
                i--;
            }            
        }
    }
    Rectangle(size_t numberOfCircles, double circlesRadius) {
        this->numberOfCircles = numberOfCircles;
        this->circlesRadius = circlesRadius;
        this->area = numberOfCircles * PI * pow(circlesRadius, 2);
        y2 = sqrt(area*10)/10;
        
        x2 = y2*100;
        cout << endl << "Area of Circles" << this->area << " Dimensions of rectangle: " <<endl<< " Y= " << y2 << " X=" << x2 << endl;
        numberOfIntervals = (x2 - x1) / circlesRadius -1;
        cout << endl << "Number of intervals dividing rectangle into sections: " << numberOfIntervals << endl;

        for (int i = 0; i < numberOfIntervals; i++) {
            vector<circle> temp;
            mapOfCircles.insert(make_pair(i, temp));        
        }        
    }

    void checkCollisions() {     
        bool collison = false;     

        for (map<int, vector<circle>>::iterator jt = mapOfCircles.begin(); jt != mapOfCircles.end(); jt++) {
            collison = false;
            for (map<int, vector<circle>>::iterator it = mapOfCircles.begin(); it != mapOfCircles.end(); it++) {            
                for (auto y : jt->second) 
                    for (auto x : it->second) {
                        if (!isNeighbourWhithoutTrespassing(x, y.x, y.y) && &x == &y) {
                            collison = true;
                            cout<< endl<<"There was a collision between circles"<<endl;
                            cout << x.x << " " << x.y << " . " << y.x << " " << y.y;
                            return;
                        }
                    }     
                }
            }
        }


    

    point getPointsAvgLenFromSection(double s, double t, double x0, double deltaZ) {
        int counter = 0;
        double avg = 0;
        
        int k = floor(s / circlesRadius);
        int limit = ceil(t / circlesRadius);        

        for (k; k <= limit; k++) {
            for (auto x : mapOfCircles[k]) {
                if (abs(x.x - x0) <= deltaZ) {
                    counter++;
                    avg = avg + abs(x.x - x0);                    
                        
                }
            }
        }        
        avg = avg / counter;

        point temp;
        temp.x = x0;
        temp.counter = counter;
        temp.avg = avg;   

        return temp;
    }
        
            
    void saveToFile(int z, string fileName) {
        vector<point> fileSave;
        ofstream file(fileName);

        //double deltaZ = (x2 -x1 / 1000) * z;
        double deltaZ =  z;

        double s = x1;
        double t,x0;
        for (int i = 0; i < ceil(numberOfIntervals / deltaZ); i++) {        
            t = s + deltaZ*2;
            x0 = s + deltaZ;
            point temp = getPointsAvgLenFromSection(s, t, x0, deltaZ);            
            fileSave.push_back(temp);
            s = t;
        }
        
        file.close();
    }
        
};


int main()
{
    size_t numberOfCircles = 100000;
    double circlesRadius = 2;

    Rectangle rect(numberOfCircles, circlesRadius);
    std::clock_t start;
    cout << endl << "Generating circles..." << endl;
    start = clock();
    rect.generateCircles();
    double tim = (clock() - start) / (double)CLOCKS_PER_SEC;
    cout << endl << "Done generating 100%, it took only: " << tim << " seconds" << endl;
   
    cout << endl << "Saving to file..."<<endl;

    rect.saveToFile(5, "File_05.csv");
    cout << endl << "Saved to file for z = 5" << endl;

    rect.saveToFile(10, "File_10.csv");
    cout << endl << "Saved to file for z = 10" << endl;

    rect.saveToFile(50, "File_50.csv");
    cout << endl << "Saved to file for z = 50" << endl;

    /*cout << endl << "Checking for collisions: " << endl;
    rect.checkCollisions();*/

}

