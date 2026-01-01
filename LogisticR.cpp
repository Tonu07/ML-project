#include <iostream>
#include <cmath>
using namespace std;
// Sigmoid function
double sigmoid(double t){
    return 1.0/(1.0+exp(-t));
}
int main(){
    int n;
    cout<<"Enter number of students: ";
    cin>>n;
    double x[n],y[n];  
    // x = study hour, y = pass(1) / fail(0)
    cout<<"Enter Study Hour and Result (1=Pass,0=Fail):\n";
    for (int i=0;i<n;i++){
        cin>>x[i]>>y[i];
    }
    double a=0.0;          // intercept
    double b=0.0;          // slope
    double learning_rate=0.1;
    int epochs=1000;
    // Training using Gradient Descent
    for (int iter=0;iter<epochs;iter++){
        double da=0.0;
        double db=0.0;
        for(int i=0;i<n;i++){
            double linear=a+b*x[i];   // a + bx
            double y_pred=sigmoid(linear);
            da+=(y_pred-y[i]);
            db+=(y_pred-y[i])*x[i];
        }
        da/=n;
        db/=n;
        a-=learning_rate*da;
        b-=learning_rate*db;
    }
    cout<<"\nTrained Logistic Regression Model:\n";
    cout<<"P(Pass) = 1 / (1 + e^-(a + b*x))\n";
    cout<<"a = " << a << ", b = " << b << endl;
    // Prediction
    double study_hour;
    cout<<"\nEnter study hour to predict result: ";
    cin>>study_hour;
    double probability=sigmoid(a+b*study_hour);
    int prediction= (probability>=0.5)?1:0;
    cout<<"Probability of Pass= "<<probability<<endl;
    if (prediction==1)
        cout<<"Prediction: PASS\n";
    else
        cout<<"Prediction: FAIL\n";
    return 0;
}
