//#include <mex.h>
#include <octave/oct.h>
#include <octave/Cell.h>
//#include <sys/cdefs.h>
#include <stdint.h>        
//#include <sys/stat.h>
//#include <net/if.h> 
//#include <fcntl.h>      // fcntl()
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include <sys/uio.h>    // readv(), writev(), preadv(), pwritev()
#include <errno.h>
#include <termios.h>
//#include <termio.h>
//#include <signal.h>
//#include <sys/time.h>
//#include <time.h>
//#include <unistd.h>     // lockf() pread(), pwrite() access() euidaccess() eaccess() faccessat ()
#include <string.h>
//#include <syslog.h>
//#include <getopt.h>
//#include <stdbool.h>

//------------------------------------------------------------
DEFUN_DLD (read_dlm_esc, args, nargout, "Read dlm Escaped")
// function [d] = read_dlm_escaped ( fname = "data/stream.txt", delim=" ", r0=0, c0=0, esc_char='"')
// Read a delimited file, similar to dlmread(), but also allow for delimiters
// to be escaped, like quotes in a CSV file.
//
// In the current implementation consecutive delimiters are treated as a single delimiter
//------------------------------------------------------------
{
   //static octave_value_list retval;
   static octave_value retval;

   //
   // Process arguments
   //
   int nargin = args.length();

   std::string fname = "data/stream.txt";
   if (nargin >0) {
      fname = args(0).char_matrix_value().row_as_string(0);
   } 

   std::string delim = ",";
   if (nargin >1) {
      delim = args(1).char_matrix_value().row_as_string(0);
   } 
   octave_stdout << "read_dlm_esc: delim = " << delim << "\n";

   int r0 = 0;
   charMatrix ch_fname;
   if (nargin >2) {
      r0 = args(2).int_value();
   } 

   int c0 = 0;
   if (nargin >3) {
      c0 = args(3).int_value();
   }

   charMatrix ch_esc;
   std::string esc_char = "\"";
   if (nargin >4) {
      esc_char = args(4).char_matrix_value().row_as_string(0);
   } 
   octave_stdout << "read_dlm_esc: esc_char = " << esc_char << "\n";


   //
   // Open the File
   //
   FILE* fid = NULL;
   char* fret = NULL;
   octave_stdout << "read_dlm_esc: attempting to open \"" << fname << "\"\n";
   fid = fopen(fname.c_str(), "r");
   if (NULL == fid) {
      octave_stdout << "Unable to find " << fname << "\n";
      return octave_value(0);
   }

   //
   // Skip any header lines
   //
   size_t nskip =0;
   char line[512] = "";
   for (nskip = 0; nskip <r0; nskip++) {
      fret = fgets(line, sizeof(line), fid);
      if(feof(fid) || NULL == fret) {
         //break;
         return octave_value(0);
      }
   }
   octave_stdout << "read_dlm_esc: skipped r0 lines = " << nskip << "\n";

   c0 += 1;
   Cell d = Cell();
   size_t row =0;
   int escaped = 0;
   std::string c_prev = delim;
   std::string meta_esc = "\\";
   std::string str = "";
   int col = 0;
   size_t n=0;
   char c;
   while ( !feof(fid) ) {
      fret = fgets(line, sizeof(line), fid);
      if(feof(fid) || NULL == fret) break;

      //octave_stdout << line;                                      // DEBUG
      col=0;
      str = "";
      for (n = 0; n<strlen(line); n++) {
         c = line[n];
         if ( c == esc_char[0] ) {
            if (c_prev == meta_esc) {
               str = str.substr(0, str.size()-1);
               str += c;
            } else {
               escaped = !escaped;
            }
         } else if ( (c == delim.c_str()[0] || (strlen(line)-1)==n) && !escaped) {//TODO: verify that this works if the last field in a line is using esc_char
            if (c_prev != delim) {
               //printf('row = %d, col = %d, str = "%s"\n', row, col, str);
               d.insert(octave_value(str, '"'), octave_idx_type(row), octave_idx_type(col));
               col += 1;
               //octave_stdout << "found str = " << str << "\n";                       // DEBUG
               str = std::string("");
            }
         } else {
            str += c;
         }
         c_prev = c;
      } // for n

      row += 1;
      c_prev = delim;

   } // while( !feof() )
   octave_stdout << "read_dlm_esc: read rows = " << row << "\n";

   //retval(0) = octave_value_list(d);
   //retval = octave_value_list(d);
   retval = d;
   /*
   octave_idx_type ni;
   //for (ni =0; ni < d.numel(); ni++) 
   for (ni = d.numel()-1; ni>=0; ni--)
   {
      retval(ni) = d.elem(ni);
      //octave_stdout << d.elem(ni).cellstr_value() << "\n";
      //octave_stdout << d.elem(ni).string_value() << "\n";
   }
   */
   return retval;
}

// vim: ts=3 sw=3
