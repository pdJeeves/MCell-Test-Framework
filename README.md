MCell-Test-Framework
====================

Testing Framework for MCell

Rules for the python scripts it can run:

the script file must end with .py
the script must begin with a """ block, which contains keywords that the framework will use to determine which things to run.  Only things inside curlybrackets are considered keywords, keywords may be in quotation marks if they contain whitespace, ` ' and " are valid for this purpose.  Keywords matching [0-9]{4}/[0-9]{2}/[0-9]{2} are considered dates and can be matched with BEFORE and AFTER query kewords.

the following functions are avalible:

MCellTest(string [path of the .mdl from this folder, the .mdl extension is not needed])
MCellTest(string path, bool verbose)

both of these return a DoubleMCellTest, meaning that the data in the test is all doubles.

MCellTest(string path, int iterations)
MCellTest(string path, int iterations, bool verbose)

Both of these return an AverageMCell test, meaning that the data in the test is all averages.

Tests have the following properties:
int exit_code
int signal
string logfile [returns a path, will be empty unless verbose is true]
string errfile [returns a path]

In the case of an Average test the exit code and signal codes are ORed together. 

When either test is performed all of the output files are found and loaded into the system, and using operator[](string test_name) will return a copy of that test.  The string matching uses fuzzy logic, using the least edit distance algorithm, if the cost divided by the length of the string is less than 1 then it matches, if there are no matches an empty file is returned.  When an AverageMCell test runs it places the output of each run in a subfolder named with that run's seed number.  After running an AverageMCell test loads all of these and averages them, and outputs the new results in the base folder, so you can use them as good data easily.

MCellOutput(string full_path) and AveragedMCellOutput(string full_path) will both load a file that is not part of the given test. 

Data tables have functions before(double time), and after(double time), which select pieces of the table by time, before and after, and rate(double time) which divides all data values by their time stamp minus the given time.

using operator[](int index), or operator[](string col_title) will select a given column from from the table and return it, so after this the column will no longer support any time operations. 

The tests support * / + - wherever possible.  Doubles support all of these operations with each other, averages support all of these operations with doubles, and only support + and - with averages.  You can subtract a double from an average but not an average from a double.

Data tables and data columns support all comparison operators, when compared with another data table they will throw an error if the number of rows or columns does not equal the number in this table.  During comparisons each item is compared with the corresponding item in the other object, if they are compared with a single double or average then each item is compared with that object.  If comparison operations are contained in str() then that will produce a string of the default error message, if they are contained in an if() or an assert() they will evaluate to a bool, otherwise they will print a default error message to stdout--this will also happen if !, &&, or || is used in the expression, due to the mechanics of how python works.  The stdout of each file is given by a string in the configuration file of the framework.

Averages are made with average(mean, std_dev), or average(doubleColumn), if average(doubleTable) is called then it will return a list of averages.  Averages have the properties:
double mean
double std_dev
double variance
int    samples

Ranges are made with Range(min, max) they are used for comparisons using 'in' and 'not in', they will check if every double/average in a table or column is in a certian range, or if an individual double/average is, this has the same semantics as the above comparisons. 


