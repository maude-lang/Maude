/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

*/

//
//	Class to solve Diophantine systems associated with AC and ACU matching.
//
//	Based on:
//	  Steven Eker,
//	  "Single Elementary Associative-Commutative Matching",
//	  Journal of Automated Reasoning, pp35-51, 28(1), 2002.
//
//	Given an n-component vector of positive integers R and an m-component vector
//	of positive integers C a solution is an n*m matrix M of natural numbers such
//	that
//			R * M = C.
//	The intuition is that M_{i,j} is the multiplicity of the jth constant assigned
//	to the ith variable in an single elementary AC or ACU matching problem.
//	For AC we are only interested in solutions which all rows (possibly all but
//	one in the case of extension variables) of M have a non-zero sum.
//
//	We solve a slightly more general problem where minimum and maximum
//	values may be specified for the sum of each row of M.
//	The algorithm used here is optimized for moderately large problem instances.
//	We do not combine repeated entries in R or make use of failure information
//	since these approaches to reducing the search space only pay for their overheads
//	for very large problem instances and would considerably complicate the code.
//
//	Rows are created by the member function
//		insertRow(int coeff, int minSize, int maxSize)
//	when "coeff" is the component of R and "minSize" and "maxSize" are the
//	minimum and maximum values for the sum of the corresponding row of M.
//
//	Columns are created by the member function
//		insertColumn(int value)
//	where "value" is the component of C.
//
//	To generate the first and successive solutions the member function solve() is
//	called; this returns true if a solution was generated and false if no more
//	solutions exist. If true was returned the components of M can be extracted
//	using the member function solution(int row, int column).
//
//	Note that is a error to add more rows or columns after the first call to
//	solve(), or to try to examine a non-existent solution.
//
//	The basic approach is to sort R into descending order and solve one row
//	at a time, backtracking whenever a dead end is detected. To solve a row
//	we consider C as a multiset and compute a sub-multiset that we could
//	potentially use for the current row. We then systematically try selections
//	from this sub-multiset starting with the smallest ones.
//
//	The bottom level of the implementation uses two different algorithms for
//	solving a row depending on whether the system as a whole is "simple" or
//	"complex". A system is "simple" iff at least one element of R is 1 and
//	has a maximum allowable sum greater or equal that the largest column value.
//	Otherwise the system is "complex".
//	Since we sort R into descending order, simple systems
//	have the property that any natural number can be expressed as a natural number
//	linear combination of any final segment of the sorted R. Thus we can rule
//	out one cause of failure for partial solutions. For complex systems we keep
//	a "solubility vector" which allows us to detect this kind of failure early
//	and prune the useless branches from the search.
//

#ifndef _diophantineSystem_hh_
#define _diophantineSystem_hh_

class DiophantineSystem
{
public:
  enum SpecialValues
    {
      INSOLUBLE = -1
    };

  DiophantineSystem(int estNrRows = 0, int estNrColumns = 0);
  
  void insertRow(int coeff, int minSize, int maxSize);
  void insertColumn(int value);
  bool solve();
  int solution(int row, int column) const;
  int rowCount() const;
  int columnCount() const;
  
private:
  struct Select
    {
      int base;		// base value for element of M (0 for simple systems)
      int extra;	// extra value representing current state of solution
      int maxExtra;	// maximum for extra
    };
  
  //
  //	In a complex system, for each row with coefficient R_i and for each possible
  //	column value V we compute and store the minimum and maximum K such that
  //	V - K*R_i can be expressed as a natural number linear combination over
  //	R_j for j > i, respecting the maximum allowable sums but not the minimum
  //	allowable sums (since some other column may make up the minimum).
  //	If no such (natural number) K exists we store min = max = INSOLUBLE.
  // 
  struct Soluble
    {
      int min;	// minimum assignment to row for given column value
      int max;	// maximum assignment to row for given column value
    };

  //
  //	Structure for each row. We have a pair of member functions to handle
  //	making a selection from a multiset, both normally and in the presence
  //	of solubility constraints on the non-selected part.
  //
  struct Row
    {
      bool multisetSelect(Vector<int>& bag, bool findFirst);
      bool multisetComplex(Vector<int>& bag,
			   Vector<Soluble>& soluble,
			   bool findFirst);

      int name;			// original position of row
      int coeff;	       	// coefficient
      int minSize;	       	// minimum acceptable sum
      int minProduct;	       	// coeff * minSize
      int minLeave;	       	// minimum sum that must be left for
      				// remaining rows
      int maxSize;	       	// maximum acceptable sum
      int maxProduct;	       	// coeff * maxSize
      int maxLeave;	       	// maximum sum that may be left for
			       	// remaining rows
      int currentSize;		// current size of selection from multiset
      int currentMaxSize;      	// maximum size of selection from multiset
      Vector<Select> selection;	// vector of values selected for this row
      Vector<Soluble> soluble;	// solubility vector (complex systems only)
    };
  
  static bool rowLt(const Row& i, const Row& j);
  
  bool precompute();
  void buildSolubilityVectors();
  bool viable(int rowNr) const;
  bool solveRowSimple(int rowNr, bool findFirst);
  void solveLastRowSimple();
  bool solveSimple(bool findFirst);
  bool solveRowComplex(int rowNr, bool findFirst);
  void solveLastRowComplex();
  bool solveComplex(bool findFirst);

  Vector<Row> rows;
  Vector<int> columns;
  Vector<int> rowPermute;
  int columnSum;
  int maxColumnValue;
  bool closed;		// system is closed once we start solving
  bool complex;
  bool failed;		// set when failure detected
};

inline int
DiophantineSystem::solution(int r, int c) const
{
  Assert(closed, "solve() not called");
  Assert(!failed, "non-existent solution");
  const Select& s = rows[rowPermute[r]].selection[c];
  return s.base + s.extra;
}

inline int
DiophantineSystem::rowCount() const
{
  return rows.length();
}

inline int
DiophantineSystem::columnCount() const
{
  return columns.length();
}

#endif
