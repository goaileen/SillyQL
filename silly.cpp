//
//  silly.cpp
//  p3-sillyql
//
// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "silly.hpp"
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include "xcode_redirect.hpp"
#include "TableEntry.h"

using namespace std;

// indicies are not gonna cause you tto throw away code,
// gonna cause you to put code inside if or else

void Database::printErrorTableNotInDatabase(string &cInput, string &tName) {
    string junk;
    cout << "Error during " << cInput << ": " << tName << " does not name a table in the database\n";
    getline(cin, junk); // clear the rest of the input line to get rid of unread text
}

TableEntry Table::returnTableEntry(size_t &cTypeLoc) {
    switch (columnTypes[cTypeLoc]) {
        case (EntryType::String): {
            string s;
            cin >> s; // read in string
            return TableEntry(s);
        }
        case (EntryType::Double): {
            double d;
            cin >> d;
            return TableEntry(d);
        }
        case (EntryType::Int): {
            int i;
            cin >> i;
            return TableEntry(i);
        }
        case (EntryType::Bool): {
            bool b;
            cin >> b;
            // cin >> std::boolalpha >> b;
            return TableEntry(b);
        }
        default:
            break;
    }
    return TableEntry(0);
}

void Database::getOptions(int argc, char * argv[]) {
    opterr = false;
    int choice;
    int index = 0;
    quiet_mode = false;
    option long_options[] = {{ "quiet", no_argument, nullptr, 'q' },
        {"help", no_argument, nullptr, 'h'},
        { nullptr, 0, nullptr, '\0' }};
 
    while ((choice = getopt_long(argc, argv, "qh", long_options, &index)) != -1) {
        switch (choice) {
            case 'q':
                quiet_mode = true;
                break;
                
            case 'h':
                cout << "helpful message about how to use the program\n";
                exit(0);
                
            default:
                cerr << "Error: unknown command line option\n";
                cerr << "The argument seen was: " << choice << endl;
                exit(1);
        } // switch
    } // while
} // get_options

void Database::create() {
    // switch case ?
    // Bear in mind that you can only switch on expressions that can be interpreted as an integer (e.g., ints, chars, and enums).
    //  The types that are allowed to be in a table is one case; the conditional operators (<, >, =) are another.
    // For CREATE specifically,  you can read in the column type as a string, and then do different things depending on the first character of that string (e.g. if type[0] == 'i', that tells us that this column will be an EntryType::int type).
//    int nCol;
    string cType;
    string cName;
    // @5891, check
    cin >> allTables[tableName].numCol;
    // @5483
//    allTables[tableName].data.reserve(allTables[tableName].numCol);
    for (size_t i = 0; i < allTables[tableName].numCol; i++) {
        cin >> cType;
        switch (cType[0]) {
            case 's': // string
                allTables[tableName].columnTypes.push_back(EntryType::String);
                // allTables[tableName].types.push_back(cType[0]);
                break;
            case 'd': // double
                allTables[tableName].columnTypes.push_back(EntryType::Double);
                // allTables[tableName].types.push_back(cType[0]);
                break;
            case 'i': // int
                allTables[tableName].columnTypes.push_back(EntryType::Int);
                // allTables[tableName].types.push_back(cType[0]);
                break;
            case 'b': // bool
                allTables[tableName].columnTypes.push_back(EntryType::Bool);
                // allTables[tableName].types.push_back(cType[0]);
                break;
            default:
                break;
        }
    }
    cout << "New table " << tableName << " with column(s) ";
    for (size_t i = 0; i < allTables[tableName].numCol; i++) {
        cin >> cName;
        cout << cName << " ";
        allTables[tableName].columnNames.push_back(cName);
        allTables[tableName].columnIndex[cName] = i;
    }
    cout << "created\n";
//          allTables[tableName].create(tableName);
    //                alltables[tn].create(tn) // create is member fxn of the table class;

    // just in insert?
    //                 You reserve then push back, or you could do resize then index into vector to change all indexes.
}

void Table::insert(string &name) {
    size_t row_insert = 0;
    cin >> row_insert >> junk;
    // pushing back into data in this fxn w/ row & col so reserve data amt
    // @4570
    // @4488
    // Before you start inserting rows, call 2DVector.reserve(2DVector.size() + num_rows).
    
    cout << "Added " << row_insert << " rows to " << name << " from position " << data.size() << " to " << (data.size() + row_insert - 1) << endl;
    
    //        When you begin inserting the data into each row, call 2DVector[i].reserve(num_cols) before calling emplace_back() for each item to insert.
    // data.reserve(data.size() + row_insert);

    // To make the TableEntry and put it in the right position, you'll have to use the following format:
    //    table_data[row].emplace_back(TableEntry(variable));
    
    size_t initialDataSize = data.size();
    data.resize(data.size() + row_insert);
    //    row.reserve(numCol);
    for (size_t r = initialDataSize; r < initialDataSize + row_insert; ++r) {
        // data.push_back(row);
        for (size_t c = 0; c < numCol; c++) {
            // vid:
            // if thsi column type is a string, read in string, emplace back the string
            data[r].reserve(numCol);
            data[r].emplace_back(returnTableEntry(c));

            // Right now what I'm thinking is starting from the new inserted row iterate to the last added row and then add the indices either to the hash map or bst 

        } // col
    } // row

    if (!bst.empty()) {
        // bst.clear();
        for (size_t r = initialDataSize; r < initialDataSize + row_insert; ++r) {
        bst[data[r][columnIndex[searchIndexColumn]]].push_back(r);
        }
    }
    if (!hash.empty()) {
        // hash.clear();
        for (size_t r = initialDataSize; r < initialDataSize + row_insert; ++r) {
        hash[data[r][columnIndex[searchIndexColumn]]].push_back(r);
        }
    }

} // insert

void Table::printAll(vector<string> &print_cols, string &name) {
    size_t colIndexLocation = 0;
    if (!quiet_mode) {
            if (data.size() != 0) {
                for (size_t i = 0; i < data.size(); i++) {
                    for (size_t j = 0; j < print_cols.size(); j++) {
                        colIndexLocation = columnIndex[print_cols[j]];
                        cout << data[i][colIndexLocation] << " ";
                    }
                    cout << endl;
                }
            }
        } // quiet mode for print all
       cout << "Printed " << data.size() << " matching rows from " << name << endl;
} // printAll

void Table::printWhereHelper(TableEntry &compareValue, char &op, string &conditionalColumn, vector<string> &print_cols, string &name) {
    size_t rowsPrinted = 0;
    size_t colIndexLocation = columnIndex[conditionalColumn];

        // Handle the case where quiet_mode is true (assuming data output is suppressed)
        // You can utilize the BST or hash index here for efficient retrieval and printing
        // Example code using BST index:
        if (!bst.empty() && (conditionalColumn == searchIndexColumn)) {
            if (op == '<') {
                for (auto it = bst.begin(); it != bst.lower_bound(compareValue); ++it) {
                    for (auto& row_index : it->second) {
                        if (!quiet_mode) {
                        printRow(print_cols, row_index);
                        }
                        rowsPrinted++;
                    }
                }
            } else if (op == '>') {
                for (auto it = bst.upper_bound(compareValue); it != bst.end(); ++it) {
                    for (auto& row_index : it->second) {
                        if (!quiet_mode) {
                        printRow(print_cols, row_index);
                        }
                        rowsPrinted++;
                    }
                }
            } else if (op == '=') {
                auto it = bst.find(compareValue);
                if (it != bst.end()) {
                    for (auto& row_index : it->second) {
                        if (!quiet_mode) {
                        printRow(print_cols, row_index);
                        }
                        rowsPrinted++;
                    }
                }
            }
        } else if (!hash.empty() && (conditionalColumn == searchIndexColumn) && (op == '=')) {
            auto it = hash.find(compareValue);
            if (it != hash.end()) {
                for (auto& row_index : it->second) {
                    if (!quiet_mode) {
                    printRow(print_cols, row_index);
                    }
                    rowsPrinted++;
                }
            }
        } else {
            for (size_t i = 0; i < data.size(); ++i) {
                bool conditionSatisfied = false;
                if (op == '<') {
                    conditionSatisfied = (data[i][colIndexLocation] < compareValue);
                } else if (op == '>') {
                    conditionSatisfied = (data[i][colIndexLocation] > compareValue);
                } else if (op == '=') {
                    conditionSatisfied = (data[i][colIndexLocation] == compareValue);
                }

                if (conditionSatisfied) {
                    if (!quiet_mode) {
                    printRow(print_cols, i); // Print the row if condition is satisfied
                    }
                    rowsPrinted++;
                }
            }
        }
    cout << "Printed " << rowsPrinted << " matching rows from " << name << endl;
} // print where


void Table::printRow(vector<string> &print_cols, size_t &row_index) {
for (size_t j = 0; j < print_cols.size(); ++j) {
        size_t colIndexLocation = columnIndex[print_cols[j]];
        cout << data[row_index][colIndexLocation] << " ";
    }
    cout << endl;
} // print row

void Table::printWhere(vector<string> &print_cols, string &name){
    string conditionalColumn, conditionalValue;
    char op;

    cin >> conditionalColumn >> op; // we read in conditional value in return table entry
    TableEntry compareValue = returnTableEntry(columnIndex[conditionalColumn]);

    printWhereHelper(compareValue, op, conditionalColumn, print_cols, name);
} // printWhere

void Table::print(string &name) {
    vector<string> col_to_print;
    size_t print_cols;
    string colName;
    string printType;

    cin >> print_cols;

    for (size_t i = 0; i < print_cols; i++) {
        cin >> colName;
        if (find(columnNames.begin(), columnNames.end(), colName) == columnNames.end()) {
            cout << "Error during PRINT: " << colName << " does not name a column in " << name << endl;
            getline(cin, junk);
            break;
        }
        else {
            col_to_print.push_back(colName);
        }
    } // for columns to print ,, check valid

    if (!quiet_mode) {
        for (size_t k = 0; k < col_to_print.size(); k++) {
            cout << col_to_print[k] << " ";
        }
        cout << endl;
    } // if not quiet mode, print out column names to print

    cin >> printType;

    if (printType == "ALL") {
        printAll(col_to_print, name);
    } // print all
    else if (printType == "WHERE") {
        printWhere(col_to_print, name);
    } // print where

} // print

void Table::deleteRows(string &name) {
    char op;
    string conditionalColumn;
    cin >> junk >> conditionalColumn >> op; // we read conditional value in return table entry

    if (find(columnNames.begin(), columnNames.end(), conditionalColumn) == columnNames.end()) {
        cout << "Error during DELETE: " << conditionalColumn << " does not name a column in " << name << endl;
        getline(cin, junk);
    }

    size_t dataInitialSize = data.size();
    size_t colIndexLocation = columnIndex[conditionalColumn];
    TableEntry compareValue = returnTableEntry(colIndexLocation);

    if (op == '<') {
        data.erase(remove_if(data.begin(), data.end(), LessThan(colIndexLocation, compareValue)), data.end());  
    }
    else if (op == '>') {
        data.erase(remove_if(data.begin(), data.end(), GreaterThan(colIndexLocation, compareValue)), data.end());  
    }
    else if (op == '=') {
        // error here... why?
        data.erase(remove_if(data.begin(), data.end(), EqualTo(colIndexLocation, compareValue)), data.end());  
    }

// Do you need to completely re-generate the index? You do for DELETE because the rows get shifted around. 
    // re gen indexes here 

    if (!hash.empty() || !bst.empty()) {
        bool output = false;
        generate(name, output);
    }

    size_t rows_deleted = dataInitialSize - data.size();

    cout << "Deleted " << rows_deleted << " rows from " << name << endl;
} // delete
// check about fixing hash / bst index  
//     take row frm column index and then put that into column types for the entry type
//     4 way split w type to read
//     3 way split in functor
    
//     once you delete, discard the old index and regenerate a new index of the same type (hash or bst).
    
//     I just made a generate helper function. So like the normal generate function, but with a colName and a tableName as parameters.
//     It's more like you want to regenerate your index after each delete. So that's why you want to do like a generate helper function. Also, I think you can keep your erase_if stuff; you just need to regenerate your index.

void Table::generate(string &name, bool &output) {
    // @4995
    // Make sure only one is not empty at a time.
    // @4580
    // check if each one empty, it isn't and we want to change it, have to clear it first
    
    if (find(columnNames.begin(), columnNames.end(), searchIndexColumn) == columnNames.end()) {
        cout << "Error during GENERATE: " << searchIndexColumn << " does not name a column in " << name << endl;
        getline(cin, junk);
    } 

    if (!bst.empty()) {
        bst.clear();
    }
    if (!hash.empty()) {
        hash.clear();
    }

// a hash table that allows a row in the table to be found rapidly given a particular value in the column <colname>
    if (searchIndexType == "hash") {
        hashCol = searchIndexColumn;
        for (size_t i = 0; i < data.size(); i++) {
            hash[data[i][columnIndex[searchIndexColumn]]].push_back(i);
        } // for
        // make it so this doesnt print when calling from another fxn
        if (output) {
        cout << "Created hash index for table " << name << " on column " << searchIndexColumn << ", with " << hash.size() << " distinct keys" << endl;
        }
    } // if hash
//  a binary search tree that allows rows in the table to be found rapidly given a particular value in the column <colname>
    else if (searchIndexType == "bst") {
        for (size_t i = 0; i < data.size(); i++) {
            bst[data[i][columnIndex[searchIndexColumn]]].push_back(i);
        } // for
        if (output) {
        cout << "Created bst index for table " << name << " on column " << searchIndexColumn << ", with " << bst.size() << " distinct keys" << endl;
        }
    } // if bst
} // generate

void Table::join(Table &table1, Table &table2, string &name, string &name2) {
    string colName1, colName2;
    int table_num;
    vector<string> col_to_print;
    vector<int> col_table;
    size_t print_cols;
    string current_name = name; // remove later
    int rowsPrinted = 0;

    cin >> junk >> colName1 >> junk >> colName2 >> junk >> junk >> print_cols;
    for (size_t i = 0; i < print_cols; i++) {
        string print_colname;
        cin >> print_colname >> table_num;

        if (table_num == 1) {
            if (find(table1.columnNames.begin(), table1.columnNames.end(), print_colname) == table1.columnNames.end()) {
            cout << "Error during JOIN: " << print_colname << " does not name a column in " << name << endl;
            getline(cin, print_colname);
            return;
            } 
        } // if table is 1
        else if (table_num == 2) {
            if (find(table2.columnNames.begin(), table2.columnNames.end(), print_colname) == table2.columnNames.end()) {
                cout << "Error during JOIN: " << print_colname << " does not name a column in " << name2 << endl;
                getline(cin, print_colname);
                return;
            } // if didnt find colname  in table 2
        } // if table is 2
        col_to_print.push_back(print_colname);
        col_table.push_back(table_num);
    } // for

// If you currently do not have a useful hash map generated when JOIN is called (we can check), then we should generate a hash map for this function (temporary because after the function is called, this hash map goes away rather than being stored as a member variable). This is what I did at least, I'm not 100% sure if this is the only way to do it.

// Anonymous Comp 1 year ago
// I believe we check No index just on the 2nd table or if there is a table if it’s on the right column we want to use. Also, yea when we create a temp it’ll just be on the 2nd table

Table temp_2 = table2;

if ((table2.hash.empty() && table2.bst.empty()) || (table2.hash.empty() && !table2.bst.empty()) || (!table2.hash.empty() && table2.hashCol != colName2)) {
        // Yes, you have to call generate to make a usable hash/bst. You should have a general generate function that you can call both from the command line and the inside of join or any other function that needs it. You will need to generate a hash/bst specifically for use in join.
        bool output = false;
        temp_2.generate(name2, output); // check
    }

// Iterate through the first table <tablename1> from beginning to end.

if (!quiet_mode) {
    for (size_t c = 0; c < col_to_print.size(); c++)
    {
        cout << col_to_print[c] << " ";
    }
    cout << endl;
}
// Iterate through the first table <tablename1> from beginning to end.
for (size_t i = 0; i < data.size(); i++) {
// For each row’s respective <colname1> value in <tablename1>, find matching <colname2> values in <tablename2>, if any exist.
auto it = temp_2.hash.find(data[i][columnIndex[colName1]]);

    // if match in values found
    if (it != temp_2.hash.end()) {
        // For each match found, print the column values in the matching rows in the order specified by the JOIN command
        // The matching rows from the second table must be selected in the order of insertion into that table.
        for (size_t k_index = 0; k_index < it->second.size(); k_index++) {
            size_t k = it->second[k_index]; // Get the index of the matching row
            if (!quiet_mode) {
                for (size_t j = 0; j < col_to_print.size(); j++) {
                    if (col_table[j] == 1) { // meaning column is from table 1
                        cout << table1.data[i][table1.columnIndex[col_to_print[j]]] << " ";
                    } else if (col_table[j] == 2) {
                        cout << temp_2.data[k][temp_2.columnIndex[col_to_print[j]]] << " ";
                    }
                }
                cout << endl;
            }
            rowsPrinted++;
} 
} // if match in values

// If no rows in the second table match a row in the first table, that row is ignored from the join.
} // for table1 num rows

// Printed 3 rows from joining pets to 281class
cout << "Printed " << rowsPrinted << " rows from joining " << name << " to " << name2 << endl;

} // join

void Database::read_input() {
    string command;
    string tableName2;
    do {
        cout << "% ";
        cin >> command; // a string
        // do a bunch of stuff to process that command
        // Check for cin failure
        if (cin.fail()) {
            cout << "Error: Reading from cin has failed\n";
        } // cin.fail
        if (command[0] == 'Q') {
            // need to delete all tables too
            if (!allTables.empty()) {
            auto it = allTables.begin();
            while (it != allTables.end()) {
                 auto eraseIt = it++;
                 allTables.erase(eraseIt);
            }
            }
            cout << "Thanks for being silly!\n";
            break; 
        } // quit
        else if (command[0] == '#') {
            getline(cin, command);
        } // comment
        else if (command[0] == 'C') {
            cin >> tableName;
            if (allTables.find(tableName) != allTables.end()) {
                cout << "Error during " << command << ": Cannot create already existing table " << tableName << endl;
                getline(cin, junk); //  clear the rest of the input line to get rid of unread text
                break;
                } // if table name alrdy exists = error!
            create();
        } // create
        else if (command[0] == 'R') {
            cin >> tableName;
            if (allTables.find(tableName) == allTables.end()) {
                printErrorTableNotInDatabase(command, tableName);
                break;
            } // if table name not in database = error!
            // use erase?
            // With the remove function, is it enough to do something like use the .erase() function on the hashmap
            // For the remove command, if we use .erase(tablename), does it automatically clear ups the data as well? Or do we need to call clear() afterward?
            allTables.erase(tableName);
            cout << "Table " << tableName << " removed\n";
        } // remove
        else if (command[0] == 'I') {
            cin >> junk >> tableName;
            if (allTables.find(tableName) == allTables.end()) {
                printErrorTableNotInDatabase(command, tableName);
                break;
            } // if table name not in database = error!
            allTables[tableName].insert(tableName);
        } // insert
        else if (command[0] == 'P') {
             cin >> junk >> tableName;
             // print where, all
             if (allTables.find(tableName) == allTables.end()) {
                printErrorTableNotInDatabase(command, tableName);
                break;
            } // if table name not in database = error!
            allTables[tableName].print(tableName);
            // print stuff
        } // print
        else if (command[0] == 'D') {
            cin >> junk >> tableName;
            if (allTables.find(tableName) == allTables.end()) {
                printErrorTableNotInDatabase(command, tableName);
                break;
            } // if table name not in database = error!
            allTables[tableName].deleteRows(tableName);
        } // delete
        else if (command[0] == 'G') {
            cin >> junk >> tableName;
            if (allTables.find(tableName) == allTables.end()) {
                printErrorTableNotInDatabase(command, tableName);
                break;
            } // if table name not in database = error!
                
    string colName;
    bool output = true;
    cin >> allTables[tableName].searchIndexType >> junk >> junk >> allTables[tableName].searchIndexColumn;
            allTables[tableName].generate(tableName, output);
        } // generate
        else if (command[0] == 'J') {
            cin >> tableName >> junk >> tableName2;
            if (allTables.find(tableName) == allTables.end()) {
                printErrorTableNotInDatabase(command, tableName);
                break;
            }
            Table table1 = allTables[tableName];
            Table table2 = allTables[tableName2];
            allTables[tableName].join(table1, table2, tableName, tableName2);
        } // join
        else {
            cout << "Error: unknown command line option\n";
            getline(cin, command);
            break;
        }
    } while (command != "QUIT"); // while not telling us to quit
} // read_input

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);

    cin >> std::boolalpha;  // add these two lines
    cout << std::boolalpha; // add these two lines
    
    Database data;
    data.getOptions(argc, argv);
    data.read_input();
    
    return 0;
}