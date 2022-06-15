/*Candice Murray and Karen Munyan
 * CS 106B Spring 2015
 * Problem Set 3: Recursion
 * 4/24/15
 * Sources: Lecture notes, cplusplus.com, Stanford libraries documentation, YEAH hours
 *
 * This file is a compilation of 7 different problems that are executed using recursion.
 * */

#include "recursionproblems.h"
#include <cmath>
#include <iostream>
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
#include <fstream>
#include "vector.h"
using namespace std;

#define PI 3.14159265359

/****************************************
 * Function Prototypes
 * *************************************/
int countKarelPaths(int street, int avenue);

//This function takes an inputed number as a string and converts its type to an int of the same value.
int convertStringToInteger(string exp);

//This function is a helper,completing the same goal as the original stringToInteger function, but
// allowing for extra parameters.
int convertStringToIntegerHelper(string &exp, int place, int num);

//This function takes in a string of characters and checks if the bracketing operators are properly nested.
bool isBalanced(string exp);

//This function is a helper, with the same goal as isBalanced, but allowing for an extra starterIndex parameter
bool isBalancedHelper(int starterIndex, string exp);

//This function takes in a row and column location, along with a Vector of weights, to compute the weight supported by
// each person in a human pyramid
double weightOnKnees(int row, int col, Vector<Vector<double> > &weights);

//This function draws a Sierpinsky triangle in the desired location with the desired size and order.
void drawSierpinskiTriangle(GWindow &gw, double x, double y, int size, int order);

//This function fills in shapes in an image with a certain color given as an input.
int floodFill(GBufferedImage &image, int x, int y, int color);

//This function is a helper that makes the actual color changes for floodFill and keeps track of
// the number of changes made
int floodFillHelper(int& changes, int colorToChange, GBufferedImage& image, int x, int y, int color);

// This is a helper function which completes the depth searching to obtain the values
// for the desired symbol
string grammarGenerateHelper(string &symbol, HashMap<string, Vector<Vector<string> > > &grammarMap, string output);

//This function builds a map of the grammar in the indicated file, calls the helper function to get
// the desired values and loops until it has as many times as called for.
Vector<string> grammarGenerate(istream &input, string symbol, int times);

/***************************************************
 * Functions
 * *************************************************/

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

/*In this function, the exp is the same as the user inputs in convertStringToInteger,
 * place keeps track of the place within the original number, number is our new int
 */
int convertStringToIntegerHelper(string& exp, int place = 1, int num = 0){

    if (exp == "-"){ //If all that's left is a negative, make the int negative
        return -num;
    } else if (exp == ""){ //This is the true base case
        return num;
    } else {
        if (!isdigit(exp[exp.length()-1])){ //throws an error if the string contains something besides a number
            throw "Invalid string";
        }
        int digit = exp[exp.length()-1] - '0'; //grabs the last digit of the string and converts it to an int by adding an int
        num += digit*place;
        place = place*10;
        exp = exp.erase(exp.length()-1,1); //erases the digit already added, then calls with the new string
        return convertStringToIntegerHelper(exp,place,num);
    }
}

//This function calls the helper function and that is all.
int convertStringToInteger(string exp) {
    return convertStringToIntegerHelper(exp);
}

/* This function takes in a string of characters as an input and checks if the bracketing
 * operators are properly nested. It sets the initial startingIndex to 0, then calls the
 * isBalancedHelper recursive function.
 */
bool isBalanced(string exp)
{
    int starterIndex = 0;
    return isBalancedHelper(starterIndex, exp);
}

/* This function takes in a string of characters and the starterIndex as parameters, then
 * deletes a set of characters if the character at the starting index is right before its complement
 * and updates the starting index if it is not. It then uses recursion and returns true if there is
 * the base case of an empty string and false otherwise.
 */
bool isBalancedHelper(int starterIndex, string exp)
{
    if(exp == "") //String is balanced if the argument is empty
    {
        return true;
    }
    else
    {
        char starter = exp[starterIndex];
        char complement;
        if(starter == '(') complement = ')';
        else if(starter == '{') complement = '}';
        else if(starter == '[') complement = ']';
        else if(starter == '<') complement = '>';
        else complement = '0';

        if(exp[starterIndex + 1] == complement) //If the starter is right next to its complement, the two will be removed.
        {
            exp.erase(exp.find(complement), 1);
            exp.erase(starterIndex, 1);
            starterIndex = 0; //Starter index will be reset to zero.
            return isBalancedHelper(starterIndex, exp);
        }
        else if(exp.find(complement) != -1 && complement != '0') //If the complement is there but is not directly next to the starter
        {                                                         //symbol, isBalancedHelper will look at the next symbols.
            starterIndex++;
            return isBalancedHelper(starterIndex, exp);
        }
        else
        {
            return false;
        }
      }
}

/* This function takes in row and column integers, along with a Vector of weights, to compute the weight
 * supported by each person in a human pyramid. The weight on a given person's knees is their weight
 * plus half of the weight on the knees of each of the two people directly above the original person. If
 * someone does not have two people above them, the missing people are ignored.
 *
 */
double weightOnKnees(int row, int col, Vector<Vector<double> >& weights)
{
    if(row > weights.size() - 1 || col > weights[row].size() - 1 || row < 0 || col < 0)
    {
        return 0.0;
    }
    else if(row < 1) //The weight on the knees of the top person on the pyramid is just that person's weight.
    {
        return weights[0][0];
    }
    else
    {
        double totalWeight = weights[row][col] + 0.5 * weightOnKnees(row - 1, col, weights)
                + 0.5 * weightOnKnees(row - 1, col - 1, weights);
        return totalWeight;
    }
}

/*This function draws a Sierpinski triangle in the passed window, starting at
 *coordinates x and y, of size indicated and order indicated
 */
void drawSierpinskiTriangle(GWindow& gw, double x, double y, int size, int order) {
    if (order == 0){ //base case
        return;
    }
    else if (x<0 || y<0 || size <0 || order < 0){ //checks for validness
        throw "Invalid input";
    }
    else if (order == 1){  //if the order is 1, draws one triangle
        gw.drawLine(x, y, (x+size), y);
        gw.drawLine(x, y, (x+(size/2.0)), (y+(size*sin(PI/3.0))));
        gw.drawLine((x+(size/2.0)), (y+(size*sin(PI/3.0))), (x+size), y);
        return;
    }
    else { // if the order is not one, breaks into three pieces and calls itself for
           // each new piece until order is one
        drawSierpinskiTriangle(gw, x, y, (size/2.0), order-1);
        drawSierpinskiTriangle(gw, x+(size/2.0), y, size/2.0, order-1);
        drawSierpinskiTriangle(gw, x+(size/4.0), y+((size/2.0)*sin(PI/3.0)), size/2.0, order-1);
    }
}

/* This function fills in shapes in an image with a certain color given as an input and returns
 * the number of color changes that have been made. If the shape is
 * already the color that the user wants to change it to, no colors will change and the function will
 * return 0. If the x or y value passed is negative or outside the bounds of the image, the function
 * will throw an exception.
 */
int floodFill(GBufferedImage& image, int x, int y, int color)
{
    if(!image.inBounds(x, y))
    {
        throw "Pixel not in bounds.";
    }
    else if(color == (image.getRGB(x, y)))
    {
        return 0;
    }
    else
    {
        int changes = 0;
        int colorToChange = image.getRGB(x, y); //Finds the original color of the shape to be changed.
        return floodFillHelper(changes, colorToChange, image, x, y, color);
    }
}

/* This is a helper function that has the same goal as floodFill but makes the actual changes
 * to the colors and keeps track of the number of color changes that have been made. It checks
 * if the pixel at the parameter index is in the image bounds and if it should have its color changed,
 * then changes the color and recursively checks for the pixels above, below, to the left, and to the right
 * of the original pixel. Finally, the helper function returns the number of color changes that were made.
 */
int floodFillHelper(int& changes, int colorToChange, GBufferedImage& image, int x, int y, int color)
{
    if(image.inBounds(x,y) && image.getRGB(x, y) == colorToChange)
    {
        image.setRGB(x, y, color);
        changes++;
        floodFillHelper(changes, colorToChange, image, x + 1, y, color);
        floodFillHelper(changes, colorToChange, image, x - 2, y, color);
        floodFillHelper(changes, colorToChange, image, x, y + 1, color);
        floodFillHelper(changes, colorToChange, image, x, y - 2, color);
    }
    return changes;
}

/*This helper function takes the symbol as in grammarGenerate, as well as the map created in
 * that function and an output string which will eventually return the result of the grammar.
 */
string grammarGenerateHelper(string& symbol, HashMap<string, Vector<Vector<string> > >& grammarMap, string output = ""){
    if (!grammarMap.containsKey(symbol)){  //base case. If the symbol isn't a key it must be a termal, so we add it to the output
        output += symbol += " ";
        return output;
    } else {
        Vector<Vector<string>> currentValues = grammarMap[symbol]; //grabs the values for the desired key
        Vector<string> currentRule = currentValues[randomInteger(0,currentValues.size()-1)]; //picks a random rule from the rules vector
        for (int i = 0; i<currentRule.size(); i++){ //for all elements in that rule, call self
            string nextSymbol = currentRule[i];
            output = grammarGenerateHelper(nextSymbol, grammarMap, output);
        }
    }
    return output;
}

/*This function generates a map for a given grammar, then calls the helper function times
 * times to get an output vector of the result of each helper function call.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    HashMap<string, Vector<Vector<string> > > grammarMap;
    if (symbol == ""){
        throw "invalid symbol";
    }
    string line;
    while (getline(input,line)){  //this loop reads each line of the file, breaking the line into a
                                 //key/value vector, then a rules vector, then each rule into its own vector
        Vector<string> keyList = stringSplit(line,"::=");
        Vector<string> ruleList = stringSplit(keyList[1],"|");
        Vector<string> ruleBreakdown;
        Vector<Vector<string> > allRules;
        for (int i = 0; i<ruleList.size(); i++){
            ruleBreakdown = stringSplit(ruleList[i]," ");
            allRules.add(ruleBreakdown);
        }
        if (grammarMap.containsKey(keyList[0])){  //Throws an error if there are two lines with the same key
            throw "Cannot have two rules for the same key";
        } else{  //adds the vector of rule vectors as the value for that key in the map
        grammarMap[keyList[0]] = allRules;
        }
    }
    Vector<string> v;  //this is the vector that will be returned
    if (times <= 0){
        return v;
    } else{  //for each time desired, call the helper function and add it to the output vector
    for (int i=0; i<times; i++){
       v +=grammarGenerateHelper(symbol, grammarMap);
    }
    return v;
    }
}
