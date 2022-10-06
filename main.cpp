/*
Open readme.txt file for instructions to run the program
*/
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

// prototypes for tax calculation and to read existing file
void taxCalculation(string, double *, string *, int);
void readCsv(string fileName, vector<string> &, vector<double> &);
int inputvalidation(int);
// Declare tax as constant variable
const double ITEM_TAX = 0.0825;

int main()
{
  int arrSize, choice;
  bool mainLoop = true;
  while (mainLoop == true)
  {
   // Ask user for choice between importing file or entering their own data. This is stored in variable choice
    cout<< "*****************************************"; 
    cout << "\n*Would you like to enter your own set of*\n*items or import from CSV?              *\n";
    cout << "*1. Import from CSV                     *\n*2. Enter my own                        *\n*3. Quit the program                    *";
    cout << "\n*****************************************";
    cout << "\nEnter: ";
      cin >> choice;
    cout << "\n";

    switch (choice)
    {
      // If the user selects option 2 then ask the user for the file name they want to import. All the data in the file is stored in vector string itemName or vector double itemPrice. 
    case 1:
    {
      // create two vectors to store values from readCsv
      // using vectors as size from the input will be unknown
      vector<string> itemName;
      vector<double> itemPrice;
      string csvInputName;

      cout << "Enter name of csv you would like to import\n";
      cout << "Enter: ";
      cin >> csvInputName;
      cout << "\n";
      
      // use readCsv to get input for the vectors
      readCsv(csvInputName, itemName, itemPrice);

      // passing the vectors as a pointer to the first value of the vector so the 
      // items can be accessed like a normal array with pointers
      taxCalculation("results.csv", &itemPrice.front(), &itemName.front(), itemName.size());
      break;
    }

    case 2:
    // Get the number of items the user wants; stored in arrSize. Then get the item name and price; stored in string itemname and double array itemPrice. Use for loop so that user only enters the specified amount. 
    {
      cout << "\nHow many items are your going to enter?: ";
      arrSize = inputvalidation(arrSize);
      string itemName[arrSize];
      double itemPrice[arrSize];

      for (int i = 0; i < arrSize; i++)
      {
        cout << "Item #" << i + 1 << ": ";
        cin >> itemName[i];
        cout << "Price for item " << i + 1 << ": $";
        while (!(cin >> itemPrice[i]) || itemPrice[i] < 0)
        {
          cout << "Enter a positive number: ";
          cin.clear();
          cin.ignore(100, '\n');
        }
      }
      // Get the get subtotal, tax and total value. 
      // Then convert all the information to CSV file.
      taxCalculation("results.csv", itemPrice, itemName, arrSize);
      break;
    }
    // if user enters 3 exit the program
    case 3:
    {
      mainLoop = false;
      break;
    }
    // Make sure the program dosen't accept any number <1 or >3.
    default:
    {
      cout << "Invalid Option. Please Choose Option 1 2 or 3.\n";
      cin.clear();
      cin.ignore(100, '\n');
      break;
    }
    }
  }
  return 0;
}

void taxCalculation(string fileName, double *itemPrice, string *itemName, int arrSize)
{
  /*
    This function will take pointers to two arrays one full of strings and one full of
    doubles along with an int for array size and a string for the filename. It will then
    take the two array pointers and add tax then write both of the arrays and total to a
    csv file.
  */
  double subVal, taxVal, finalVal;

  // Loop through the arrays to add tax to the prices and calculate final value
  for (int i = 0; i < arrSize; i++)
  {
    subVal += *(itemPrice + i);
    taxVal = subVal * ITEM_TAX;
    finalVal = subVal + taxVal;
  }

  cout << endl;

  ofstream file(fileName);

  // Write the header for the csv file
  file << "Item,Price,Subtotal,Tax,Total" << endl;


  // loop through arrays to write each entry to the csv file 
  // adding a comma between each entry
  for (int i = 0; i < arrSize; i++)
  {
    file << *(itemName + i);
    file << ',';
    file << "$" << fixed << setprecision(2) << *(itemPrice + i);
    file << ",,";
    file << "\n";
  }
  // set the prescision of the output so it outputs prices to two decimial places
  file << fixed << setprecision(2);
  // writing the final valules to the file along with the blank spaces with extra commas
  file <<  ",," << '$' << subVal;
  file << ',' << '$' << taxVal;
  file << ',' << '$' << finalVal;

  // close the file for cleanup as it is no longer needed
  file.close();
  cout << "Tax has been successfully added and the list has been written  to results.csv\n";
  cout << "Please open this file in Microsoft Excel or Google Sheets to   view.\n\n";
}

int inputvalidation(int arrSize)
{
  /*
    This function will get input make sure its a positive number and return the input it
    collected
  */
  while (!(cin >> arrSize) || arrSize < 0)
  {
    cout << "Enter a positive number: ";
    cin.clear();
    cin.ignore(100, '\n');
  }
  return arrSize;
}

void readCsv(string fileName, vector<string> &itemName, vector<double> &itemPrice)
{
  /*
    Input a filename, two vectors by reference, one string vector for filenames and one
    double vector for item prices. This function will then open the provided file and loop
    through the lines seperating each item delimited by a comma and storing them in there
    respected vectors.
  */

  // create string for temp line storage
  string line;
  // create double for temp storage for string to double conversion
  double temp;

  ifstream myfile(fileName);

  // check if file was opened and error out if it was not able to be opened
  if (!myfile)
  {
    cout << "Couldn't open the file or path!" << endl;
    exit(1);
  }

  // loop for each line in opened file
  while (getline(myfile, line))
  {
    // store each line in a temp stringstream
    stringstream stream(line);

    int i = 0;

    // parse each line in the temp stringstream by ,
    while (getline(stream, line, ','))
    {
      // checking if a header for item and price is present in input csv
      // and breaking out of loop to not include it in the vectors
      if (line == "Item" || line == "Price")
      {
        break;
      }
      // check which array to put the current line in with i
      else if (i == 0)
      {
        itemName.emplace_back(line);
      }
      else
      {
        // convert the string to double and store it in temp for the price array
        temp = stod(line);
        itemPrice.emplace_back(temp);
      }
      i++;
    }
  }
}