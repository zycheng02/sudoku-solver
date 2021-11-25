/* Program to assist in the challenge of solving sudoku puzzles.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2021, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Zhiyuan Cheng 1271641[Enter your full name and student number here before submission]
   Dated: 15/09/2021    [Enter the date that you "signed" the declaration]

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* these #defines provided as part of the initial skeleton */

#define NDIM 3		/* sudoku dimension, size of each inner square */
#define NDIG (NDIM*NDIM)
			/* total number of values in each row */
#define NGRP 3		/* number of sets each cell is a member of */
#define NSET (NGRP*NDIG)
			/* total number of sets in the sudoku */
#define NCLL (NDIG*NDIG)
			/* total number of cells in the sudoku */

#define ERROR	(-1)	/* error return value from some functions */

/* these global constant arrays provided as part of the initial skeleton,
   you may use them in your code but must not alter them in any way,
   regard them as being completely fixed. They describe the relationships
   between the cells in the sudoku and provide a basis for all of the
   sudoku processing loops */

/* there are 27 different different sets of elements that need to be
   checked against each other, this array converts set numbers to cells,
   that's why its called s2c */
int s2c[NSET][NDIM*NDIM] = {
	/* the first group of nine sets describe the sudoku's rows */
	{  0,  1,  2,  3,  4,  5,  6,  7,  8 },
	{  9, 10, 11, 12, 13, 14, 15, 16, 17 },
	{ 18, 19, 20, 21, 22, 23, 24, 25, 26 },
	{ 27, 28, 29, 30, 31, 32, 33, 34, 35 },
	{ 36, 37, 38, 39, 40, 41, 42, 43, 44 },
	{ 45, 46, 47, 48, 49, 50, 51, 52, 53 },
	{ 54, 55, 56, 57, 58, 59, 60, 61, 62 },
	{ 63, 64, 65, 66, 67, 68, 69, 70, 71 },
	{ 72, 73, 74, 75, 76, 77, 78, 79, 80 },
	/* the second group of nine sets describes the sudoku's columns */
	{  0,  9, 18, 27, 36, 45, 54, 63, 72 },
	{  1, 10, 19, 28, 37, 46, 55, 64, 73 },
	{  2, 11, 20, 29, 38, 47, 56, 65, 74 },
	{  3, 12, 21, 30, 39, 48, 57, 66, 75 },
	{  4, 13, 22, 31, 40, 49, 58, 67, 76 },
	{  5, 14, 23, 32, 41, 50, 59, 68, 77 },
	{  6, 15, 24, 33, 42, 51, 60, 69, 78 },
	{  7, 16, 25, 34, 43, 52, 61, 70, 79 },
	{  8, 17, 26, 35, 44, 53, 62, 71, 80 },
	/* the last group of nine sets describes the inner squares */
	{  0,  1,  2,  9, 10, 11, 18, 19, 20 },
	{  3,  4,  5, 12, 13, 14, 21, 22, 23 },
	{  6,  7,  8, 15, 16, 17, 24, 25, 26 },
	{ 27, 28, 29, 36, 37, 38, 45, 46, 47 },
	{ 30, 31, 32, 39, 40, 41, 48, 49, 50 },
	{ 33, 34, 35, 42, 43, 44, 51, 52, 53 },
	{ 54, 55, 56, 63, 64, 65, 72, 73, 74 },
	{ 57, 58, 59, 66, 67, 68, 75, 76, 77 },
	{ 60, 61, 62, 69, 70, 71, 78, 79, 80 },
};


/* there are 81 cells in a dimension-3 sudoku, and each cell is a
   member of three sets, this array gets filled by the function 
   fill_c2s(), based on the defined contents of the array s2c[][] */
int c2s[NCLL][NGRP];

void
fill_c2s() {
	int s=0, d=0, c;
	for ( ; s<NSET; s++) {
		/* record the first set number each cell is part of */
		for (c=0; c<NDIM*NDIM; c++) {
			c2s[s2c[s][c]][d] = s;
		}
		if ((s+1)%(NGRP*NDIM) == 0) {
			d++;
		}
	}
#if 0
	/* this code available here if you want to see the array
	   cs2[][] that gets created, just change that 0 two lines back
	   to a 1 and recompile */
	for (c=0; c<NCLL; c++) {
		printf("cell %2d: sets ", c);
		for (s=0; s<NGRP; s++) {
			printf("%3d", c2s[c][s]);
		}
		printf("\n");
	}
	printf("\n");
#endif
	return;
}

/* find the row number a cell is in, counting from 1
*/
int
rownum(int c) {
	return 1 + (c/(NDIM*NDIM));
}

/* find the column number a cell is in, counting from 1
*/
int
colnum(int c) {
	return 1 + (c%(NDIM*NDIM));
}

/* find the minor square number a cell is in, counting from 1
*/
int
sqrnum(int c) {
	return 1 + 3*(c/NSET) + (c/NDIM)%NDIM;
}

/* If you wish to add further #defines, put them below this comment,
   then prototypes for the functions that you add

   The only thing you should alter above this line is to complete the
   Authorship Declaration 
*/


/****************************************************************/


/****************************************************************/

/* main program controls all the action
*/
int sudoku[81];
int printyorn= 0;
int solved= 0;
void reading_n_printing();
void strategy_one();
    
int
main(int argc, char *argv[]) {
    
    
    /*
    stage 1 - reading and printing
    */
    reading_n_printing();
    
    /*
    stage 2 Grid Checking
    */
    int violations=0, sets_w_violations=0;
    fill_c2s();
    
    for (int set=0; set<27; set++){ 	// inner looping thru all sets in current group 
        int counter[10]={0};
        
        // counting occurences of each number 
        for(int i = 0; i < NDIG; i++) {       
            int digit_i= sudoku[s2c[set][i]];
            counter[digit_i]+=1;   
        }    
        
        
        int violation_1or0= 0;
        //2 or more occurences means a duplicate 
        for (int x=1; x<10; x++){
            if (counter[x]> 1){
                violations+= 1;
                violation_1or0= 1; 
                int set_no= set%9;
                const char *set_type[3];
                set_type[0] = "row";
                set_type[1] = "col";
                set_type[2] = "sqr";
                //print violation message
                printf("set %d (%s %d): %d instances of %d\n", set, set_type[(set)/9], set_no+1, counter[x], x);
                
            }      
        }
        if (violation_1or0== 1){
             sets_w_violations+= 1; //counting different sets w violations
        }
        }
    
    
    //violations summary    
    if (violations!= 0){
        printf("\n%d different sets have violations", sets_w_violations);
        printf("\n%d violations in total\n", violations);
        return 0;
    }
    
    /*
    stage 3 Implementing strategy one 
    */
    strategy_one();
    printf("\n");
    reading_n_printing();
    if (solved== 1){
    printf("\nta daa!!!");
    }
    //algorithms are fun
	/* all done, so pack up bat and ball and head home */
	return 0;
}

void reading_n_printing(){
    int cellsunknown=0;
    for (int n=0; n<81; n++){
        if (printyorn== 0) {
            scanf("%d", &sudoku[n]);  
        }
        /*printing . for all 0s and other digits in sudoku as is*/
        {
        if (sudoku[n]== 0){
            printf(". ");
            cellsunknown++;
            }                
        else{
            printf("%d ", sudoku[n]);
            }
        }
        
        /*prints separator for every 3rd row*/
        if ((n+1) % (NDIM* NDIG)== 0 && n!= 0 && n!= 80){
            printf("\n------+-------+------\n");
        }

        /*switches lines for every 9th digit*/
        else if ((n+1) % NDIG== 0 && n!= 0){
            printf("\n");
        } 
        
        /* ' | ' formatter*/
        else if ((n+1) % NDIM== 0 && n!= 0){ 
            printf("| ");
        } 
    }
    printf("\n%d cells are unknown\n", cellsunknown);
}

void strategy_one(){
    printf("\nstrategy one\n");
    int change_yorno= 0;
    for (int i=0; i<81; i++){
       if (sudoku[i]== 0){
           int set_row[9]= {0} ;
           int set_col[9]= {0} ;
           int set_sqr[9]= {0} ;

           // convert pointers to values
           for (int x=0; x<9; x++){
                set_row[x]= sudoku[ (s2c[ c2s[i][0] ])[x] ];
                set_col[x]= sudoku[ (s2c[ c2s[i][1] ])[x] ];
                set_sqr[x]= sudoku[ (s2c[ c2s[i][2] ])[x] ];
           } 
           
           
           int possible[]= {1,2,3,4,5,6,7,8,9};
           int possible_count= 0;
           for (int x=0; x<9; x++){
               for (int y=0; y<9; y++){
                    if (set_row[x]== possible[y]){
                        possible[y]= 0;
                    }
                    else if (set_col[x]== possible[y]){
                        possible[y]= 0;
                    }
                    else if (set_sqr[x]== possible[y]){
                        possible[y]= 0;
                    }
               }
           } 
           for (int times=0; times<2; times++){
           
               for (int x=0; x<9; x++){
                   if ((times== 1) && (possible_count== 1) && (possible[x]!=0)){
                       printf("row %d col %d must be %d\n", c2s[i][0]+1, c2s[i][1]-8, possible[x] );
                       sudoku[i]= possible[x];
                       change_yorno =1;
                   }
                   if (possible[x]!=0 && times==0){
                       possible_count +=1;
                   }
               }
           }   
       }
   } 
   int num_o_zeros=0;
   for (int times=0; times<2; times++){
       for (int x=0; x<81; x++){
           if(sudoku[x]== 0){
               num_o_zeros++;
           }
       }
       if (times== 1){
           if (num_o_zeros!= 0 || change_yorno==0){
           strategy_one();
           }
           else if (num_o_zeros== 0){
               printyorn= 1;
               solved=1;
               return;
           }
           else {
               printyorn= 1;
               return;
           }
       }
       
   }

}



/****************************************************************/

