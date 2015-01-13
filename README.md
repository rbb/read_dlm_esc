# read_dlm_esc
Quickly read character delimited files (ie CSV, etc) into a cell array in octave.

This oct creates an Octave function to read a delimited file, similar to dlmread(),
but allowing for delimiters to be escaped, like quotes in a CSV file.

In the current implementation consecutive delimiters are treated as a single delimiter

read_dlm_esc() Vs dlmread():
-------------------------
   - read_dlm_esc() is much faster than dlmread(), but it is slower than csv2cell() from the io package
   - read_dlm_esc() is probably not as robust as dlmread()
   - read_dlm_esc() returns a cell array, rather than a matrix
   - Because read_dlm_esc() returns a cell array, strings can be loaded from a CSV file
   - read_dlm_esc(), as the name implies will escape delimiters. 
   - read_dlm_esc() can handle a variable number of fields in a line, which csv2cell() will not do.
     
Example
-------

     a = read_dlm_esc("file.csv", ","0, 0, '"')

used on a file like:

     "Item", "a,b", "decimal"
     1,2,3
     2,3,4

will return:

     a = 
     {
       [1,1] = Item
       [2,1] =  1
       [3,1] =  2
       [1,2] = a,b
       [2,2] =  2
       [3,2] =  3
       [1,3] = decimal
       [2,3] =  3
       [3,3] =  4
     }

TODO
----
   - Implement the starting column parameter
