/*
Author: Ambar Noatay
Last Date Modified: 10/07/2023
Description: 
To Calculate the Electric Field at point(x,y,z) Produced by a 2D Array (MxN) of Point Charges. 
Calculation of the Electric Field is done parallelly using OpenMP Multi-Threading.

*/

#include<iostream>
#include<stdio.h>
#include<thread>
#include<math.h>
#include<vector>
#include<chrono>
#include<omp.h>
#define k 9.0

using namespace std;

int checkDecimalPlaceBefore(double num)// function to check how many decimal places are before a double variable num. For 0.08 answer will be 2.
{
    int place = 0;// we have to calculate the no of places, initially zero
    int firstPlace = num/1;// find out the digit in the first place should be 0 for a number less than 1.
    if(firstPlace!=0 || num ==0)// if number equal to 0 or if the first digit is not 0, don't perform the calculation
        return 0;
    
    while(firstPlace==0)// while the number doesn't go above 0.
    {
        firstPlace = num*10; //check new firstplace digit
        num = num*10;//multiply the number by 10
        place++;    //increment the number of places
    }
    return place;
    
}
double decimalMoveLeft(double num, int places)// Move the digits to the left by a constant number of places.
{
    while(places>0)// till we haven't exhausted the number of places repeat the "action"
    {
        num = 10*num;//the "action" - multiply the double variable by 10.
        places--;//reduce places
    }
    return num;
}
int checkDecimalPlaceAfter(double num)// function to check how many digits are there in a number before the decimal place. E.g 400.1 will have 2 digits before it becomes 4.001
{
    int places = 0;// we have to calculate the no of places, initially zero
    if(num<1)// if number is less than 1 don't perform the calculation
        return 0;
    while(num>1)// while the number doesn't go below 1.
    {
        places++;//increase no of places

        
        num = num/10;//divide the number by 10
    }
    return places -1;
}
double decimalMoveRight(double num, int places)// Move the digits to the right by a constant number of places.
{
    while(places>0)// till we haven't exhausted the number of places repeat the "action"
    {
        num = num/10;//the "action" - divide the double variable by 10.
        places--;//reduce places
    }
    return num;
}
using namespace std;
int main()
{
    
    
    int Threads;
    cout<<"Please enter the number of concurrent threads to use: ";
    cin>>Threads;//Get number of threads
    if(cin.fail()||Threads<1)//Throw error for invalid
    {
        cerr<<"Invalid Input Number of threads should be greater than 0"<<endl;
        return 1;
    }
    
    int n,m;//Values of Rows and Columns
    cout<<"Please enter the number of rows and columns in the N x M array: ";
    cin>>m;
    
    cin>>n;
    
    if (cin.fail() || n <= 0 || m <= 0)//if the type of the varible doesn't match the input value or if M or N is not a whole number, Throw an error 
    {
        cerr << "Invalid input. Both n and m should be positive integers." << endl;
        return 1; 
    }
    
    
    
    double xSpace,ySpace;
    cout<<"Please enter the x and y separation distances in meters: ";
    cin>>xSpace;
    
    cin>>ySpace;
    
    if(xSpace<0 || ySpace<0 || cin.fail()) // Throw error for invalid input values
    {
        cerr << "Invalid input. x and y separation should be >0.0 " << endl;
        return 1; 
    }
        
    double Q;
    cout<<"Please enter the common charge on the points in micro C: ";
    cin>>Q;
    
    if (cin.fail())// Throw error for invalid input values
    {
        cerr << "Invalid input for charge. Please enter a valid numeric value." << endl;
        return 1; 
    }
    
    
    
    int n1,m1;//varibles to check whether n and m are even or odd.
    n1 = n%2;
    m1 = m%2;
    int midM = (m-m1)/2;//Calculate the mid point of M
    int midN = (n-n1)/2;//Calculate the mid point of N
    double xpos,ypos,zpos;
    char ask ='Y';
    do
    { 
        cout<<"Please enter the location in space to determine the electric field (x y z) in meters: ";
        cin>>xpos;
        
        cin>>ypos;
        
        cin>>zpos;
        
        if(cin.fail())//Throw error for invalid input values
        {
            cerr << "Invalid input. Check values of point locations" << endl;
            return 1;
        }
        
        
        auto start_time = std::chrono::high_resolution_clock::now();//capture start time of calculation
        double esumx =0,esumy=0,esumz =0;// initailize sum of Ex,Ey,Ez to zero
        int j;
        //Parallel computation of Electric field using OpenMP with guided scheduling 
        //Set number of threads to Threads, each threa will have a private copy of j, use a guided scheduling for distribution of iterations among threads.
        //combine private values of esumx,esumy and esumz at the end of parallel region
        #pragma omp parallel for num_threads(Threads) private(j) schedule(guided) reduction(+ : esumx, esumy,esumz)
        for(int i =0;i<n;i++)
        {
            for(j=0;j<m;j++)
            {
                
                double x2 = xpos - ((-midN + j + (0.5*(1-n1)))*xSpace); // Calculate Vector in x direction
                double y2 = ypos - ((midM - i - (0.5*(1-m1)))*ySpace); // Calculate Vector in y direction
                double z2 = zpos; // Calculate Vector in z direction
                double magnitudeOfVector = 0.0;
                magnitudeOfVector = sqrt((x2 * x2) + (y2 * y2) + (z2 * z2));// Calculate the magnitude of the vector
                
                if (magnitudeOfVector != 0.0) 
                {
                    double inv_mag_cubed = 1.0 / (magnitudeOfVector * magnitudeOfVector * magnitudeOfVector);//Inversion of cube to avoid redundant calculation
                    esumx+= (k * Q * x2) *inv_mag_cubed;//Add value of Ex[i][j] to esumx; same for y and z
                    esumy+= (k * Q * y2) *inv_mag_cubed;
                    esumz+= (k * Q * z2) *inv_mag_cubed;
                } 
                else
                {
                    esumx+=0;
                    esumy+=0;
                    esumz+=0;
                    
                }
                
            }
            
        }

        
        
        double magnitude = sqrt((esumx*esumx)+(esumy*esumy)+(esumz*esumz)); //Magnitude of the vator = |E|
        
        //check if we can move the decimal place for any value 
        int checkPlacesBeforeX = checkDecimalPlaceBefore(esumx);
        int checkPlacesBeforeY = checkDecimalPlaceBefore(esumy);
        int checkPlacesBeforeZ = checkDecimalPlaceBefore(esumz);
        int checkPlacesBeforeM = checkDecimalPlaceBefore(magnitude);
        int checkPlacesAfterX  = checkDecimalPlaceAfter(esumx);
        int checkPlacesAfterY  = checkDecimalPlaceAfter(esumy);
        int checkPlacesAfterZ  = checkDecimalPlaceAfter(esumz);
        int checkPlacesAfterM  = checkDecimalPlaceAfter(magnitude);
        //if we can, move the decimal places 
        if(checkPlacesBeforeX)
            esumx = decimalMoveLeft(esumx,checkPlacesBeforeX);
        if(checkPlacesBeforeY)
            esumy = decimalMoveLeft(esumy,checkPlacesBeforeY);
        if(checkPlacesBeforeZ)
            esumz = decimalMoveLeft(esumz,checkPlacesBeforeZ);
        if(checkPlacesBeforeM)
            magnitude = decimalMoveLeft(magnitude,checkPlacesBeforeM);
        if(checkPlacesAfterX)
            esumx = decimalMoveRight(esumx,checkPlacesAfterX);
        if(checkPlacesAfterY)
            esumy = decimalMoveRight(esumy,checkPlacesAfterY);
        if(checkPlacesAfterZ)
            esumz = decimalMoveRight(esumz,checkPlacesAfterZ);
        if(checkPlacesAfterM)
            magnitude = decimalMoveRight(magnitude,checkPlacesAfterM);
        
        auto end_time = std::chrono::high_resolution_clock::now();//end calculation time
        //find the duration of the calculation
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        cout<<"The electric field at ("<<xpos<<", "<<ypos<<", "<<zpos<<") in V/m is"<<endl;
        // if Ex =0,Ey=0 or Ez=0 change value of 10^y to 10^0
        if(esumx==0)
            checkPlacesBeforeX = 3+checkPlacesAfterX;
        if(esumy==0)
            checkPlacesBeforeY = 3+checkPlacesAfterY;
        if(esumz==0)
            checkPlacesBeforeZ = 3+checkPlacesAfterZ;
        if(magnitude==0)
            checkPlacesBeforeM = 3+checkPlacesAfterM;
        //display the values of Electric field
        cout<<"Ex = "<<esumx<<" * 10^"<<3-checkPlacesBeforeX + checkPlacesAfterX<<endl;
        cout<<"Ey = "<<esumy<<" * 10^"<<3-checkPlacesBeforeY + checkPlacesAfterY<<endl;
        cout<<"Ez = "<<esumz<<" * 10^"<<3-checkPlacesBeforeZ + checkPlacesAfterZ<<endl;
        
        cout<<"|E| = "<<magnitude<<" * 10^"<<3-checkPlacesBeforeM + checkPlacesAfterM<<endl;
        
        
    
    
        
        cout << "The calculation took " << duration << " microsec!" << endl;//display time taken for the calculation
        cout<<"Do you want to enter a new location (Y/N)? ";
        cin>>ask;
        cout<<endl;
        
        
        
        
    }while(ask =='Y');
    
    cout<<"Bye!"<<endl;
    

    return 0;
}
