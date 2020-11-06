The objective of this project is to diagnose whether or not a person has diabetes.
The data was taken from the National Institute of Diabetes, India.
All the patients were women with the minimum age of 21.

The following had been considered:
1. Number of pregnancies
2. Blood sugar
3. Blood presure
4. Skin thickness
5. Insuline level
6. Bodymass index
7. Diabetes Pedigree Function
8. Age

I've built the program with the help of a Neural Network. 
The app had an accuracy of 84.5% on the training-set and 81.8% on the test-set.

mainScript is the main script, which trains the network and prints the results.
If you have your own examples:
1. Create a CSV file in WordPad.
2. In Octav load the parameters with the load("weights.mat") method and then call the featureNormalize method, and then the predict method.

The data was taken from: https://www.kaggle.com/uciml/pima-indians-diabetes-database#diabetes.csv

