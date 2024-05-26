//
//  silly.hpp
//  p3-sillyql
//
// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#ifndef silly_hpp
#define silly_hpp

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include "TableEntry.h"
#include "xcode_redirect.hpp"
using namespace std;

// make 3 functors 
struct LessThan {
private:
   size_t colNumber;
   TableEntry compareTo;
public:
    LessThan(size_t col, const TableEntry& compare) : colNumber(col), compareTo(compare) {}
    
    bool operator()(const vector<TableEntry> &row) {
       //implement based on whichever operator you're doing
       if (row[colNumber] < compareTo) {
           return true;
       }
       else {
           return false;
       }
    }
};

struct GreaterThan {
private:
   size_t colNumber;
   TableEntry compareTo;
public:
   GreaterThan(size_t col, const TableEntry& compare) : colNumber(col), compareTo(compare) {}
   
   bool operator()(const vector<TableEntry> &row) {
       //implement based on whichever operator you're doing
       if (row[colNumber] > compareTo) {
           return true;
       }
       else {
           return false;
       }
   }
};

struct EqualTo {
private:
   size_t colNumber;
   TableEntry compareTo;
public:
   EqualTo(size_t col, const TableEntry& compare) : colNumber(col), compareTo(compare) {}

   bool operator()(const vector<TableEntry> &row) {
       //implement based on whichever operator you're doing
       if (row[colNumber] == compareTo) {
           return true;
       }
       else {
           return false;
       }
   }
};

struct Table { 
    bool quiet_mode = false;
    size_t numCol = 0;
    string junk;
    string tableName; // key for unordered map 
    // easy to write code if each table has a string variable that it knows its own name

    map<string, int> nmap; // keep track of col name/indicies
    // name as key, value as index
    
    // That's pretty much perfect. You're also gonna want unordered_map for the hash indices and map for the bst indices you will generate with the GENERATE command.
    
    string hashCol;
    string searchIndexType;
    string searchIndexColumn;
    int searchIndexColNum;
    unordered_map<TableEntry, vector<size_t>> hash;
    map<TableEntry, vector<size_t>> bst;
    
    // emplace back vs push back @5335
    vector< EntryType > columnTypes;
    vector< string > columnNames;
    // string is tablename, and should take us to whole table object
    // @4547 have vector<tableEntry> to push into 2d vecotr of data
    // vector<TableEntry> row;
    // You reserve then push back, or you could do resize then index into vector to change all indexes.

    unordered_map<string, size_t> columnIndex; // the map associating column names with column indices
    
    vector<vector<TableEntry>> data; // the 2D vector of all the data w/ row, col
    // I have an unordered_map just as you described. I also have a vector of chars associating column index with type value. Finally, there is the 2D vector of TableEntry objects where the first index is the row and the second is the column, where the data type in each column is consistent with the char vector.
    // vector<char> types; // the vector associating column indices (the index of the vector) with data types.
    
    TableEntry returnTableEntry(size_t &cTypeLoc);
    void insert(string &name);
    void print(string &name);
    void printWhere(vector<string> &print_cols, string &name);
    void printWhereHelper(TableEntry &compareValue, char &op, string &conditionalColumn, vector<string> &print_cols, string &name);
    void printRow(vector<string> &print_cols, size_t &row_index);
    void printAll(vector<string> &print_cols, string &name);
    void deleteRows(string &name);
    void generate(string &name, bool &output);
    void join(Table &table1, Table &table2, string &name, string &name2);
};

class Database {
private:
    string tableName;
    bool quiet_mode = false;
    string junk;
    unordered_map<string, Table> allTables;
public:
    void printErrorTableNotInDatabase(string &cInput, string &tName);
    void getOptions(int argc, char * argv[]);
    void read_input();
    void create();
    void remove();
    // I would first make sure that your structure for the project has an outer "high level" class. That class would contain create, remove, comment, and quit as member functions, and some member variables.
};

#endif /* silly_hpp */