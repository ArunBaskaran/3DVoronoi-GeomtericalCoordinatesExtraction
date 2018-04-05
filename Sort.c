#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>


FILE * file1 ;
FILE * file2 ;
FILE * file3 ;
FILE * file4 ;
FILE * file5 ;
FILE * file6 ;
FILE * file7 ;
FILE * file8 ;

const int n = 100 ;

float vertex_list[100000][3];

int edge_list[100000][2];

int face_list[100000][100];

int cell_list[100000][100];



int id_flag = -1 ;
int id;

int assign_vertex_id(float x, float y, float z)
{
	if((x>1)||(y>1)||(z>1))
		return -1 ;
	for(int i = 0 ; i <= id_flag ; i++)
	{
		if((x==vertex_list[i][0])&&(y==vertex_list[i][1])&&(z==vertex_list[i][2]))
		{
			return i;
		}
	}
	id_flag++ ;
	vertex_list[id_flag][0] = x ;
	vertex_list[id_flag][1] = y ;
	vertex_list[id_flag][2] = z ;	

	return id_flag ;
}

int check_unique_edge(int old_id, int id, int edge_flag)
{
	int status = 0 ;
	for(int i = 0 ; i <=edge_flag ; i++)
	{
		if((edge_list[i][0] == old_id)&&(edge_list[i][1]==id))
			status = -1 ;
		else if((edge_list[i][1] == old_id)&&(edge_list[i][0]==id))
			status = -1 ;
	}
	return status ;
}

int main(int argc, char * argv[])
{

for(int i = 0 ; i < 100000 ; i++)
{
	for(int j = 0 ; j < 100 ; j++)
	{
		face_list[i][j] = INT_MAX ;
		cell_list[i][j] = -1 ;
	}
}


	char filename[1024] ;
	sprintf(filename, "%s", argv[1]);
	file1 = fopen(filename, "r");
	if ( file1 == NULL )
    {
		//perror( "file not opened\n" );
		return EXIT_FAILURE;
    }
	file2 = fopen("vertex_list.txt", "w");
	if ( file2 == NULL )
    {
		//perror( "file not opened\n" );
		return EXIT_FAILURE;
    }
	file3 = fopen("edge_list.txt", "w");
	if ( file3 == NULL )
    {
		//perror( "file not opened\n" );
		return EXIT_FAILURE;
    }

	file8 = fopen("NewGenerator3D.fe", "w");
	if ( file8 == NULL )
    {
		//perror( "file not opened\n" );
		return EXIT_FAILURE;
    }

    int i = 0;
    float x, y, z ;
    
    //float x_old , y_old, z_old, x_new, y_new, z_new ;
    //int edge_counter ;
    //float vertices[n][3] ;
    //int id;
    int old_id ;
    //char temp_char ;
    int edge_flag = -1 ;
    int face_flag = -1 ;
    while(!feof(file1))
    {

		char temp[100];
		fgets(temp, 100, file1);
		if(feof(file1))
		{
			break;
		}
	
		if((temp[0]=='#')||(temp[0]==' ')||(temp[0]=='@')||(temp[0]=='$'))
		{
			//x_old = y_old = z_old = 0 ;
			//fscanf(file1, "%c", &temp_char);
			//sscanf( temp, " %f %f %f", &x, &y, &z);
			old_id = -1 ;
			temp[0]='\0';
			continue ;
		}
		//fflush(file1);
		sscanf( temp, " %f %f %f", &x, &y, &z);
		
		//fscanf(file1, " %f %f %f", &x, &y, &z);
		//printf("%f %f %f\n", x, y, z);
		if(feof(file1))
		{
			break;
		}

			if(x<0.000000)
				x=0.0;
			if(y<0.000000)
				y=0.0;
			if(z<0.000000)
				z=0.0;

		id = assign_vertex_id(x,y,z);
		if(id==-1)
			continue;
		if(old_id!=-1)
		{
			int status = check_unique_edge(old_id, id, edge_flag);
			if(status==-1)
			{
				i++;
				old_id = id ;
				continue;
			}
			edge_flag++ ;
			fprintf(file3, "%d\t%d\t%d", edge_flag+1, old_id+1, id+1);
			edge_list[edge_flag][0] = old_id ;
			edge_list[edge_flag][1] = id ;
			if((vertex_list[old_id][0]==0.0)&&(vertex_list[id][0]==0.0))
				fprintf(file3, "\t\t\tconstraint 1");
			else if((vertex_list[old_id][0]==1.0)&&(vertex_list[id][0]==1.0))
				fprintf(file3, "\t\t\tconstraint 2");
			else if((vertex_list[old_id][1]==0.0)&&(vertex_list[id][1]==0.0))
				fprintf(file3, "\t\t\tconstraint 3");
			else if((vertex_list[old_id][1]==1.0)&&(vertex_list[id][0]==1.0))
				fprintf(file3, "\t\t\tconstraint 4");
			else if((vertex_list[old_id][2]==0.0)&&(vertex_list[id][2]==0.0))
				fprintf(file3, "\t\t\tconstraint 5");
			else if((vertex_list[old_id][2]==1.0)&&(vertex_list[id][0]==1.0))
				fprintf(file3, "\t\t\tconstraint 6");
			fprintf(file3, "\n");
			//fflush(file3);
		}
		i++;
		old_id = id ;
							
	}
	
	//printf("edge_flag is %d\n", edge_flag);
	
	for(int i = 0 ; i <= id_flag ; i++)
	{
		if(vertex_list[i][0]<=0.0)
			vertex_list[i][0]=0.0000 ;
		if(vertex_list[i][1]<=0.0)
			vertex_list[i][1]=0.0 ;
		if(vertex_list[i][2]<=0)
			vertex_list[i][2]=0.0 ;
		fprintf(file2, "%d\t%f\t%f\t%f", i+1, vertex_list[i][0], vertex_list[i][1], vertex_list[i][2]);
		if(((vertex_list[i][0]==0.0)||(vertex_list[i][0]==1.0))&&((vertex_list[i][1]==0.0)||(vertex_list[i][1]==1.0))&&((vertex_list[i][2]==0.0)||(vertex_list[i][2]==1.0)))
			fprintf(file2, "\t\t\tfixed");
		else if(vertex_list[i][0]==0.0)
			fprintf(file2, "\t\t\tconstraint 1");
		else if(vertex_list[i][0]==1.0)
			fprintf(file2, "\t\t\tconstraint 2");
		else if(vertex_list[i][1]==0.0)
			fprintf(file2, "\t\t\tconstraint 3");
		else if(vertex_list[i][1]==1.0)
			fprintf(file2, "\t\t\tconstraint 4");
		else if(vertex_list[i][2]==0.0)
			fprintf(file2, "\t\t\tconstraint 5");
		else if(vertex_list[i][2]==1.0)
			fprintf(file2, "\t\t\tconstraint 6");
		fprintf(file2, "\n");
		//fflush(file2);
	}

	fprintf(file8, "vertices\n");
	for(int i = 0 ; i <= id_flag ; i++)
	{
		if(vertex_list[i][0]<=0.0)
			vertex_list[i][0]=0.0000 ;
		if(vertex_list[i][1]<=0.0)
			vertex_list[i][1]=0.0 ;
		if(vertex_list[i][2]<=0)
			vertex_list[i][2]=0.0 ;
		fprintf(file8, "%d\t%f\t%f\t%f", i+1, vertex_list[i][0], vertex_list[i][1], vertex_list[i][2]);
		if(((vertex_list[i][0]==0.0)||(vertex_list[i][0]==1.0))&&((vertex_list[i][1]==0.0)||(vertex_list[i][1]==1.0))&&((vertex_list[i][2]==0.0)||(vertex_list[i][2]==1.0)))
			fprintf(file8, "\t\t\tfixed");
		else if(vertex_list[i][0]==0.0)
			fprintf(file8, "\t\t\tconstraint 1");
		else if(vertex_list[i][0]==1.0)
			fprintf(file8, "\t\t\tconstraint 2");
		else if(vertex_list[i][1]==0.0)
			fprintf(file8, "\t\t\tconstraint 3");
		else if(vertex_list[i][1]==1.0)
			fprintf(file8, "\t\t\tconstraint 4");
		else if(vertex_list[i][2]==0.0)
			fprintf(file8, "\t\t\tconstraint 5");
		else if(vertex_list[i][2]==1.0)
			fprintf(file8, "\t\t\tconstraint 6");
		fprintf(file8, "\n");
		//fflush(file8);
	}

	fprintf(file8, "edges\n");
	for(int i = 0 ; i <=edge_flag ; i++)
	{
			
			fprintf(file8, "%d\t%d\t%d", i+1, edge_list[i][0]+1, edge_list[i][1]+1);
			//edge_list[edge_flag][0] = old_id ;
			//edge_list[edge_flag][1] = id ;
			if((vertex_list[edge_list[i][0]][0]==0.0)&&(vertex_list[edge_list[i][1]][0]==0.0))
				fprintf(file8, "\t\t\tconstraint 1");
			else if((vertex_list[edge_list[i][0]][0]==1.0)&&(vertex_list[edge_list[i][1]][0]==1.0))
				fprintf(file8, "\t\t\tconstraint 2");
			else if((vertex_list[edge_list[i][0]][1]==0.0)&&(vertex_list[edge_list[i][1]][1]==0.0))
				fprintf(file8, "\t\t\tconstraint 3");
			else if((vertex_list[edge_list[i][0]][1]==1.0)&&(vertex_list[edge_list[i][1]][0]==1.0))
				fprintf(file8, "\t\t\tconstraint 4");
			else if((vertex_list[edge_list[i][0]][2]==0.0)&&(vertex_list[edge_list[i][1]][2]==0.0))
				fprintf(file8, "\t\t\tconstraint 5");
			else if((vertex_list[edge_list[i][0]][2]==1.0)&&(vertex_list[edge_list[i][1]][0]==1.0))
				fprintf(file8, "\t\t\tconstraint 6");
			fprintf(file8, "\n");
			fflush(file8);

	}
	
	//printf("%d\n", i);
		
	fclose(file1);
	fclose(file2);
	fclose(file3);




	file1 = fopen(filename, "r");
	if ( file1 == NULL )
    	{
		//perror( "file not opened\n" );
		return EXIT_FAILURE;
    	}

	file4 = fopen("face_list.txt", "w");
	if ( file4 == NULL )
    	{
		//perror( "file not opened\n" );
		return EXIT_FAILURE;
    	}
    	
	file5 = fopen("cell_list.txt", "w");
	if ( file5 == NULL )
    	{
		//perror( "file not opened\n" );
		return EXIT_FAILURE;
    	}


	int first_id = -1 ;
	int last_id = -1 ;
	int cell_end = -1 ;
	int cell_count = -1 ;
	int edge_count = -1 ;
	int face_count = -1 ;
	while(!feof(file1))
    	{

		char temp[100];
		fgets(temp, 100, file1);
		if(feof(file1))
		{
			break;
		}
	
		//if((temp[0]!='@')||(temp[0]!='$'))
		//{
		//	continue;
		//}
	
		if((temp[0]=='@'))
		{
			if(cell_end==-1)
			{
				face_count = -1 ;
				cell_count++;
				fprintf(file5,"%d\t", cell_count);
			}
			cell_end = 0 ;
			face_flag++ ;
			
			fprintf(file5, "%d\t", face_flag+1);	
			cell_list[cell_count][++face_count] = face_flag ;			
			//fflush(file1);
			memmove(temp, temp+1, strlen(temp));
			sscanf( temp, " %f %f %f", &x, &y, &z);
			
			//printf("%f %f %f\n", x, y, z);
		
			if(feof(file1))
			{
				break;
			}
			if(x<0.000000)
				x=0.0;
			if(y<0.000000)
				y=0.0;
			if(z<0.000000)
				z=0.0;
			id = assign_vertex_id(x,y,z);
			
			if(last_id!=-1)
			{
				for(int i = 0; i<=edge_flag ; i++)
				{
					if((edge_list[i][0]==last_id)&&(edge_list[i][1]==first_id))
					{
						fprintf(file4, "\t%d ", i+1);
						face_list[face_flag-1][++edge_count] = i+1 ;
					}
					if((edge_list[i][1]==last_id)&&(edge_list[i][0]==first_id))
					{
						fprintf(file4, "\t%d ", -(i+1));
						face_list[face_flag-1][++edge_count] = -(i+1) ;
					}
				}
			}
			
			first_id = old_id = id ;
			if(face_flag==0)
				fprintf(file4, "%d", face_flag+1);
			else if(face_flag > 0)
				fprintf(file4, "\n%d", face_flag+1);
			
			edge_count=-1;
		}
				
		if(temp[0]=='$')
		{
			//fflush(file1);
			memmove(temp, temp+1, strlen(temp));
			sscanf( temp, " %f %f %f", &x, &y, &z);
			//printf("%f %f %f\n", x, y, z);
			if(feof(file1))
			{
				break;
			}
			if(x<0.000000)
				x=0.0;
			if(y<0.000000)
				y=0.0;
			if(z<0.000000)
				z=0.0;
			//printf("%f %f %f\n", x, y, z);
			id = assign_vertex_id(x,y,z);
			for(int i = 0; i<=edge_flag ; i++)
			{
				if((edge_list[i][0]==old_id)&&(edge_list[i][1]==id))
				{
					fprintf(file4, "\t%d", i+1);
					face_list[face_flag][++edge_count] = i+1 ;
				}
				else if((edge_list[i][1]==old_id)&&(edge_list[i][0]==id))
				{
					fprintf(file4, "\t%d", -(i+1));
					face_list[face_flag][++edge_count] = -(i+1) ;
				}
					
			}
			last_id=old_id=id;			
		}
		
		if((temp[0]=='#')&&(cell_end==0))
		{
			fprintf(file5, "\n");
			if(last_id!=-1)
			{
				for(int i = 0; i<=edge_flag ; i++)
				{
					if((edge_list[i][0]==last_id)&&(edge_list[i][1]==first_id))
					{
						fprintf(file4, "\t%d ", i+1);
						face_list[face_flag][++edge_count] = i+1 ;
					}
					if((edge_list[i][1]==last_id)&&(edge_list[i][0]==first_id))
					{
						fprintf(file4, "\t%d ", -(i+1));
						face_list[face_flag][++edge_count] = -(i+1) ;
					}
				}
			}
			cell_end=-1;
			last_id=-1;
		}	
							
	}
	
	//Upto edges, everything works fine. Redundancy arise while composing face_list.  

	fclose(file1);
	fclose(file4);
	fclose(file5);

	

	for(int i = 1 ; i <=face_flag ; i++)
	{
		int match = 0 ;
		int original_id = 0 ;
		for(int j = 0 ; j < i ; j++)
		{
			int temp_match = 1;
			for(int k = 0 ; k < 100 ; k++)
			{
				int found = 0 ;
				for(int l = 0 ; l < 100 ; l++)
				{
					//if((face_list[i][k]==-1)||(face_list[j][l]==-1))
					//	break ;
					if((face_list[i][k]==face_list[j][l])||(face_list[i][k]==-face_list[j][l]))
						found = 1 ;
				}
				if(found==0)
				{
					temp_match = 0 ;
					break;
				}	
			}
			if(temp_match==1)
			{
				printf("Match found for %d\n", i);
				match = 1 ;
				original_id = j ;
				break ;
			}
		}
		if(match==1)
		{
			//j is the original id. Modify the cell_list with this number. 
			for(int m = i ; m <=face_flag ; m++)    //CHANGED < to <=
			{
				for(int n = 0 ; n < 100 ; n++)
				{
					face_list[m][n] = face_list[m+1][n] ; 
				}
			}
			face_flag-- ;

			for(int m = 0 ; m <= cell_count ; m++)
			{
				for(int n = 0 ; n < 100 ; n++)
				{
					if(cell_list[m][n] == i)
						cell_list[m][n] = original_id ;
					if(cell_list[m][n] > i)
						cell_list[m][n]--; 					
				}
			}
		}
	}

	fprintf(file8, "faces\n");
	file6 = fopen("face_list_sorted.txt", "w");
	if ( file6 == NULL )
    	{
		//perror( "file not opened\n" );
		return EXIT_FAILURE;
    	}

	for(int i = 0 ; i<=face_flag ; i++)
	{
		fprintf(file6, "%d\t", i+1);
		fprintf(file8, "%d\t", i+1);
		for(int j = 0 ; j<100 ; j++)
		{
			if((face_list[i][j]==INT_MAX))
			{	
				break;
			}
			if(face_list[i][j] >=0)
				{	fprintf(file6, "%d\t", face_list[i][j]);
					fprintf(file8, "%d\t", face_list[i][j]);	}
			if(face_list[i][j] < 0)
				{fprintf(file6, "%d\t", face_list[i][j]);
				fprintf(file8, "%d\t", face_list[i][j]);}
		}
		fprintf(file6, "\n");
		fprintf(file8, "\n");
	}
	fclose(file6);	

	fprintf(file8, "bodies\n");
	file7 = fopen("cell_list_sorted.txt", "w");
	if ( file7 == NULL )
    	{
		//perror( "file not opened\n" );
		return EXIT_FAILURE;
    	}

	for(int i = 0 ; i<=cell_count ; i++)
	{
		fprintf(file7, "%d\t", i+1);
		fprintf(file8, "%d\t", i+1);
		for(int j = 0 ; j<100 ; j++)
		{
			if((cell_list[i][j]==-1)&&(cell_list[i][j+1]==-1)&&(cell_list[i][j+2]==-1))
				break;
			if(cell_list[i][j]>=0)
				{fprintf(file7, "%d\t", cell_list[i][j]+1);
				fprintf(file8, "%d\t", cell_list[i][j]+1);}
			if(cell_list[i][j]<0)
				{fprintf(file7, "%d\t", cell_list[i][j]-1);
				fprintf(file8, "%d\t", cell_list[i][j]-1);}
		}
		fprintf(file7, "\n");
		fprintf(file8, "\n");
	}
	fclose(file7);
	fclose(file8);



	
	return EXIT_SUCCESS;
	
	
}

