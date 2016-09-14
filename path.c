/*Author: Stanley Uche Godfrey
 Date: August 26,2016
*/

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include"prehead.h"
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include<assert.h>


/* Opens a directory and store both the directoryfd and
   the directory path in a structure
*/

void open_directory(char* file_path,opened_dir_struct *ods){
	int dir_fd;
	DIR *dir;
	dir=opendir(file_path);
	if(dir !=NULL){
		ods->dirfd=dirfd(dir);
		ods->dirname=file_path;
		ods->flags=0;
		
	}
	else
     		printf("Error opening %s \n ",ods->dirname);
     		
}

/*
   Opens a directory path and adds the
   dirfd and path to the opened directory structure.
*/
opened_dir_struct ** add_OpenDir(opened_dir_struct **old_ods,opened_dir_struct *ods,int *len){
	opened_dir_struct **new_ods=(opened_dir_struct**)malloc((*len+1)*sizeof(opened_dir_struct*));
	int i;
	for(i=0;i<(*len);i++){
		new_ods[i]=old_ods[i];
	}
	new_ods[*len]=ods;
	(*len)++;
	free(old_ods);

	old_ods=new_ods;
	return old_ods;
	
	
}



/*
  Finds how many characters in a string is in another 
  string begining from the first character
*/
int findMatchingChars(char *A,char *B){
	int lenA,lenB,shorterLength,matchedNum=0,i;
	assert(A!=NULL && B!=NULL);
	lenA=strlen(A);
	lenB=strlen(B);
	if(lenB<lenA){
		shorterLength=lenB;
	}
	else{
		shorterLength=lenA;
	}
	for(i=0;i<shorterLength;i++){
		if(A[i]==B[i]){
			matchedNum++;
		}
		else{
			break;
		}
	}
	return matchedNum;
}



/*
 Returns the dirfd of the opened path with highest matched number
 and relative path to the dirfd
*/
split_path  getMostMatchedPath(int matches[],opened_dir_struct **openedpaths,int length ,char *newPath){
	int highestnumber=0,i,highest_num_index;
	opened_dir_struct *ods;
	split_path  splitPath;
	for(i=0;i<length;i++){
		if(matches[i]>highestnumber){
			highestnumber=matches[i];
			highest_num_index=i;
			
		}
		
	}
	if(matches[highest_num_index]<=1){
		ods=(opened_dir_struct*)malloc(sizeof(opened_dir_struct));
		open_directory(newPath,ods);
		openedpaths=add_OpenDir(openedpaths,ods,&length);		
		splitPath.relative_path=NULL;
		splitPath.dirfd=0;
	}
	else{
		splitPath.dirfd=openedpaths[highest_num_index]->dirfd;
		splitPath.relative_path=openedpaths[highest_num_index]->dirname+highestnumber;
	}
	return splitPath;

}

/*
 Search for the best matching path in an array that matches a path argument
 Open and store the directoryfd and directory path in the array if no matching 
 path is found in the array
*/

split_path lookupDir(opened_dir_struct**ods,char* a_filepath,int length){
	int i;
	split_path sp;
	int matched_num[length];
	
	for(i=0;i<length;i++){

		matched_num[i]=findMatchingChars(ods[i]->dirname,a_filepath);
	}
	sp=getMostMatchedPath(matched_num,ods,length,a_filepath);
	

	return sp;
}

