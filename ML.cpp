#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>  // Added this header for time()
using namespace std;
// ============================================================
// DATA STRUCTURE
// ============================================================
class MovementData {
public:
    double A,B,C,D,E,F,G,H;  // 8 question answers
    double successScore;    // Success probability
};
// NEURAL NETWORK CLASS
class MovementBrain {
private:
    // Network weights
    double inputToHidden[8][8];    // 8 inputs to 8 hidden
    double hiddenBias[8];          // Hidden biases
    double hiddenToOutput[8];      // Hidden to output
    double outputBias;             // Output bias
    // Data
    vector<MovementData>trainingData;
    vector<MovementData>testingData;
    // Activation function
    double activate(double x) {
        return 1.0 / (1.0 + exp(-x));
    }
    double activateDerivative(double x) {
        double s = activate(x);
        return s * (1.0 - s);
    } 
    // READ CSV FILE FUNCTION
    vector<MovementData>readCSVData(string filePath){
        vector<MovementData> allData;
        ifstream file(filePath);      
        if (!file.is_open()) {
            cout << "ERROR: Cannot open file: " << filePath << endl;
            cout << "Please check if file exists!" << endl;
            return allData;
        } 
        string line;
        int lineNumber = 0;   
        cout << "Reading CSV file: " << filePath << endl;    
        while (getline(file, line)) {
            lineNumber++;
               // Skip header line
            if (lineNumber == 1) continue;          
            // Skip empty lines
            if (line.empty()) continue;           
            MovementData newData;
            stringstream ss(line);
            string cell;
            int column = 0;         
            // Check separator (tab or comma)
            size_t tabCount = count(line.begin(), line.end(), '\t');
            size_t commaCount = count(line.begin(), line.end(), ',');
            char separator = (tabCount > commaCount) ? '\t' : ',';           
            while (getline(ss, cell, separator)) {
                column++;               
                // Skip timestamp column (first column)
                if (column == 1) continue;               
                try {
                    double value = stod(cell);                 
                    // Assign to appropriate variable
                    if (column == 2) newData.A = value / 5.0;
                    else if (column == 3) newData.B = value / 5.0;
                    else if (column == 4) newData.C = value / 5.0;
                    else if (column == 5) newData.D = value / 5.0;
                    else if (column == 6) newData.E = value / 5.0;
                    else if (column == 7) newData.F = value / 5.0;
                    else if (column == 8) newData.G = value / 5.0;
                    else if (column == 9) newData.H = value / 5.0;                
                }
                 catch (...) {
                    // Skip bad data
                    break;
                }
            }
            // Calculate success score
            // A-D are positive factors, E-H are negative factors
            double score = (newData.A + newData.B + newData.C + newData.D) / 4.0;
            score = score - (newData.E + newData.F + newData.G + newData.H) / 8.0;
            
            // Make sure score is between 0 and 1
            if (score < 0) score = 0;
            if (score > 1) score = 1;
            newData.successScore = score;
            
            allData.push_back(newData);
        }
        file.close();
        cout << "Loaded " << allData.size() << " records from CSV" << endl;   
        return allData;
    }
    // ============================================================
    // INITIALIZE NETWORK WEIGHTS
    // ============================================================
    void initializeBrain() {
        // Set random seed - FIXED: removed time(0)
        // srand(time(0));  // This was causing error    
        // Initialize all weights randomly
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                inputToHidden[i][j] = (rand() % 1000) / 1000.0 - 0.5;
            }
        }
        for (int i = 0; i < 8; i++) {
            hiddenBias[i] = (rand() % 1000) / 1000.0 - 0.5;
        }
        
        for (int i = 0; i < 8; i++) {
            hiddenToOutput[i] = (rand() % 1000) / 1000.0 - 0.5;
        }
        outputBias = (rand() % 1000) / 1000.0 - 0.5;
        
        cout << "Brain initialized with random weights." << endl;
    }
    // ============================================================
    // SPLIT DATA INTO TRAIN AND TEST
    // ============================================================
    void splitData(vector<MovementData> allData, double trainRatio = 0.8) {
        // Shuffle data randomly
        random_shuffle(allData.begin(), allData.end());  
        int trainSize = allData.size() * trainRatio;  
        for (int i = 0; i < allData.size(); i++) {
            if (i < trainSize) {
                trainingData.push_back(allData[i]);
            } else {
                testingData.push_back(allData[i]);
            }
        }
        cout << "Training data: " << trainingData.size() << " samples" << endl;
        cout << "Testing data: " << testingData.size() << " samples" << endl;
    }
public:
    // ============================================================
    // CONSTRUCTOR
    // ============================================================   
    MovementBrain(string filePath) {
        cout << "\n==========================================" << endl;
        cout << "   SOCIAL MOVEMENT ANALYZER" << endl;
        cout << "==========================================" << endl;      
        // Load data from CSV
        vector<MovementData> allData = readCSVData(filePath);
        
        if (allData.empty()) {
            cout << "ERROR: No data loaded. Program will exit." << endl;
            exit(1);
        }
       // Split into train and test
        splitData(allData, 0.8); 
        // Initialize network
        initializeBrain(); 
        cout << "Neural network ready!" << endl;
    }
    // ============================================================
    // MAKE PREDICTION
    // ============================================================
    double predict(double a, double b, double c, double d,
                   double e, double f, double g, double h) {
        // Normalize inputs to 0-1 range
        double inputs[8] = {a/5.0, b/5.0, c/5.0, d/5.0,
                           e/5.0, f/5.0, g/5.0, h/5.0};
        // Hidden layer calculation
        double hidden[8] = {0};
        
        for (int j = 0; j < 8; j++) {
            double sum = hiddenBias[j];
            for (int i = 0; i < 8; i++) {
                sum += inputs[i] * inputToHidden[i][j];
            }
            hidden[j] = activate(sum);
        }  
        // Output layer calculation
        double output = outputBias;
        for (int i = 0; i < 8; i++) {
            output += hidden[i] * hiddenToOutput[i];
        }  
        return activate(output);
    }
    
    // ============================================================
    // TRAIN THE NETWORK
    // ============================================================    
  void trainBrain(int cycles = 500, double learnRate = 0.1) {
        cout << "\n==========================================" << endl;
        cout << "   TRAINING STARTED" << endl;
        cout << "==========================================" << endl;
        cout << "Training cycles: " << cycles << endl;
        cout << "Learning rate: " << learnRate << endl;
        for (int cycle = 0; cycle < cycles; cycle++) {
            double totalError = 0.0;     
            for (MovementData data : trainingData) {
                double inputs[8] = {data.A, data.B, data.C, data.D,
                                   data.E, data.F, data.G, data.H};          
                // Store values for backpropagation
                double hidden[8] = {0};
                double hiddenSum[8] = {0};          
                // Forward pass - hidden layer
                for (int j = 0; j < 8; j++) {
                    double sum = hiddenBias[j];
                    for (int i = 0; i < 8; i++) {
                        sum += inputs[i] * inputToHidden[i][j];
                    }
                    hiddenSum[j] = sum;
                    hidden[j] = activate(sum);
                }        
                // Forward pass - output layer
                double outputSum = outputBias;
                for (int i = 0; i < 8; i++) {
                    outputSum += hidden[i] * hiddenToOutput[i];
                }
                double prediction = activate(outputSum);         
                // Calculate error
                double error = data.successScore - prediction;
                totalError += error * error;        
                // Backpropagation - output layer
                double outputDelta = error * activateDerivative(prediction);      
                // Update hidden to output weights
                for (int i = 0; i < 8; i++) {
                    hiddenToOutput[i] += learnRate * outputDelta * hidden[i];
                }
                outputBias += learnRate * outputDelta;     
                // Backpropagation - hidden layer
                double hiddenDelta[8] = {0};
                for (int i = 0; i < 8; i++) {
                    double hiddenError = outputDelta * hiddenToOutput[i];
                    hiddenDelta[i] = hiddenError * activateDerivative(hidden[i]);
                }
                
                // Update input to hidden weights
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        inputToHidden[i][j] += learnRate * hiddenDelta[j] * inputs[i];
                    }
                }
                
                // Update hidden biases
                for (int i = 0; i < 8; i++) {
                    hiddenBias[i] += learnRate * hiddenDelta[i];
                }
            }
            // Show progress
            if (cycle % 50 == 0) {
                cout << "Cycle " << cycle << " - Error: " << totalError << endl;
            }
        }
        
        cout << "Training completed!" << endl;
    }
    // ============================================================
    // TEST THE NETWORK
    // ============================================================ 
    void testBrain() {
        cout << "\n==========================================" << endl;
        cout << "   TESTING RESULTS" << endl;
        cout << "==========================================" << endl;        
        double totalError = 0.0;
        int correctCount = 0;       
        for (int i = 0; i < testingData.size(); i++) {
            MovementData data = testingData[i];
         double prediction = predict(data.A*5, data.B*5, data.C*5, data.D*5,
                                       data.E*5, data.F*5, data.G*5, data.H*5);      
            double actual = data.successScore;
            double error = abs(prediction - actual);
            totalError += error;     
            // Categorize results
            string actualCat = getCategory(actual);
            string predCat = getCategory(prediction);
            
            if (actualCat == predCat) {
                correctCount++;
            }       
            cout << "Test " << (i+1) << ": ";
            cout << "Actual=" << (actual*100) << "% (" << actualCat << ") ";
            cout << "Predicted=" << (prediction*100) << "% (" << predCat << ") ";
            cout << "Error=" << error << endl;
        } 
        double avgError = totalError / testingData.size();
        double accuracy = (double)correctCount / testingData.size() * 100;    
        cout << "\n==========================================" << endl;
        cout << "SUMMARY:" << endl;
        cout << "Average Error: " << avgError << endl;
        cout << "Accuracy: " << accuracy << "%" << endl;
        cout << "Correct: " << correctCount << "/" << testingData.size() << endl;
        cout << "==========================================" << endl;
    }
    string getCategory(double score) {
        if (score >= 0.8) return "Excellent";
        else if (score >= 0.6) return "Good";
        else if (score >= 0.4) return "Average";
        else if (score >= 0.2) return "Poor";
        else return "Very Poor";
    }    
    // ============================================================
    // ANALYZE FEATURE IMPORTANCE
    // ============================================================
    void analyzeFeatures() {
        cout << "\n==========================================" << endl;
        cout << "   FEATURE IMPORTANCE" << endl;
        cout << "==========================================" << endl;     
        string featureNames[8] = {"Public Support (A)", "Non-violence (B)", 
                                 "Inclusiveness (C)", "Organization (D)",
                                 "Damage (E)", "Instability (F)",
                                 "Media (G)", "Govt Response (H)"};
        
        for (int f = 0; f < 8; f++) {
            double importance = 0.0;
            for (int h = 0; h < 8; h++) {
                importance += abs(inputToHidden[f][h]);
            }
            importance = importance / 8.0;
            
            cout << featureNames[f] << ": ";
            cout << importance << " ";         
            // Visual bar
            cout << "[";
            int bars = importance * 20;
            for (int i = 0; i < bars; i++) cout << "#";
            for (int i = bars; i < 20; i++) cout << " ";
            cout << "]" << endl;
        }
    }
    // ============================================================
    // PREDICT NEW MOVEMENT
    // ============================================================   
    void predictNewMovement() {
        cout << "\n==========================================" << endl;
        cout << "   ANALYZE NEW MOVEMENT" << endl;
        cout << "==========================================" << endl;
        cout << "Enter values (1-5) for each factor:" << endl;    
        double a, b, c, d, e, f, g, h;   
        cout << "A - Public Support (1-5): ";
        cin >> a;
        cout << "B - Non-violence (1-5): ";
        cin >> b;
        cout << "C - Inclusiveness (1-5): ";
        cin >> c;
        cout << "D - Organization (1-5): ";
        cin >> d;
        cout << "E - Damage (1-5): ";
        cin >> e;
        cout << "F - Instability (1-5): ";
        cin >> f;
        cout << "G - Media (1-5): ";
        cin >> g;
        cout << "H - Govt Response (1-5): ";
        cin >> h;   
        // Validate inputs
        a = max(1.0, min(5.0, a));
        b = max(1.0, min(5.0, b));
        c = max(1.0, min(5.0, c));
        d = max(1.0, min(5.0, d));
        e = max(1.0, min(5.0, e));
        f = max(1.0, min(5.0, f));
        g = max(1.0, min(5.0, g));
        h = max(1.0, min(5.0, h));
        double success = predict(a, b, c, d, e, f, g, h);
        double percent = success * 100;
        string category = getCategory(success);   
        cout << "\n==========================================" << endl;
        cout << "   PREDICTION" << endl;
        cout << "==========================================" << endl;
        cout << "Success Probability: " << percent << "%" << endl;
        cout << "Category: " << category << endl;     
        // Recommendations
        cout << "\n==========================================" << endl;
        cout << "   RECOMMENDATIONS" << endl;
        cout << "==========================================" << endl;    
        if (a < 3) cout << "• Work on increasing public support" << endl;
        if (b < 3) cout << "• Focus on non-violent methods" << endl;
        if (c < 3) cout << "• Make movement more inclusive" << endl;
        if (d < 3) cout << "• Improve organization" << endl;
        if (e > 3) cout << "• Reduce economic damage" << endl;
        if (f > 3) cout << "• Decrease social instability" << endl;
        if (g < 3) cout << "• Increase media attention" << endl;
        if (h < 3) cout << "• Improve government relations" << endl;
       if (percent >= 70) {
            cout << "\nExcellent potential for success!" << endl;
        } else if (percent >= 50) {
            cout << "\n Good potential, needs some improvements" << endl;
        } else {
            cout << "\n Needs significant improvements" << endl;
        }
    }
};
// ============================================================
// MAIN FUNCTION - PROGRAM START
// ============================================================
int main() {
    cout << "==========================================" << endl;
    cout << "   SOCIAL MOVEMENT SUCCESS PREDICTOR" << endl;
    cout << "   Version 1.0" << endl;
    cout << "==========================================" << endl;
    
    // ============================================================
    // VERY IMPORTANT: CHANGE THIS TO YOUR CSV FILE PATH
    // ============================================================
    
    // OPTION 1: Your actual path (with space in filename)
    string csvPath = "C:\\Users\\HP\\Downloads\\Merged_Movements new.csv";
    
    // OPTION 2: If option 1 doesn't work, try this (copy CSV to program folder)
    // string csvPath = "Merged_Movements new.csv";
    
    // OPTION 3: Rename file to remove space and use this
    // string csvPath = "C:\\Users\\HP\\Downloads\\Merged_Movements_new.csv";
    
    cout << "\nTrying to open CSV file at:" << endl;
    cout << csvPath << endl;
    cout << "\nIf file not found, you need to:" << endl;
    cout << "1. Change csvPath in the code (line 418)" << endl;
    cout << "2. Or move CSV file to same folder as program" << endl;
    cout << "\nPress Enter to continue..." << endl;
    cin.ignore();
    // ============================================================
    // CREATE AND TRAIN BRAIN
    // ============================================================
    try {
        // Create brain with CSV data
        MovementBrain brain(csvPath);
        
        // Train the brain
        brain.trainBrain(300, 0.1);
        
        // Test accuracy
        brain.testBrain();
        
        // Show feature importance
        brain.analyzeFeatures();
        
        // Interactive predictions
        char choice;
        do {
            brain.predictNewMovement();
            
            cout << "\nAnalyze another movement? (y/n): ";
            cin >> choice;
            cin.ignore(); // Clear input buffer
            
        } while (choice == 'y' || choice == 'Y');
        
        cout << "\n==========================================" << endl;
        cout << "   PROGRAM FINISHED" << endl;
        cout << "==========================================" << endl;
        
    } 
    catch (...) {
        cout << "\n ERROR! Something went wrong." << endl;
        cout << "Possible issues:" << endl;
        cout << "1. CSV file not found at: " << csvPath << endl;
        cout << "2. CSV file format incorrect" << endl;
        cout << "3. File has spaces in name" << endl;
        
        cout << "\nQuick solutions:" << endl;
        cout << "1. Copy CSV file to same folder as program" << endl;
        cout << "2. Change line 418 to: string csvPath = \"Merged_Movements new.csv\";" << endl;
    }
    cout << "\nPress Enter to exit...";
    cin.get(); 
    return 0;
}
