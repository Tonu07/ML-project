#include <iostream>
using namespace std;
int main() {
    int n;
    cout<<"Enter number of data points:";
    cin>>n;
    double x[n],y[n];
    double sumX=0,sumY=0,sumXY=0,sumX2=0;
    for(int i=0;i<n;i++){
        cout<<"Enter x and y:";
        cin>>x[i]>>y[i];
        sumX+=x[i];
        sumY+=y[i];
        sumXY+=x[i]*y[i];
        sumX2+=x[i]*x[i];
    }
    double m=(n*sumXY-sumX*sumY)/(n*sumX2-sumX*sumX);
    double c=(sumY-m*sumX)/n;
    cout<<"Linear Regression Model:y="<<m<<"x + "<<c<<endl;
    double x_new;
    cout<<"Enter x to predict y: ";
    cin>>x_new;
    cout<<"Predicted y = "<<m*x_new+c<<endl;
    return 0;
}
